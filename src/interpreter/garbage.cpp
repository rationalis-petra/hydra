#include "expressions.hpp"

// the garbage collector for the language
using namespace expr;
using namespace interp;

using std::string;
using std::list;
using std::mutex;
using std::unique_lock;

void mark(STWCollector& coll) {
  // mark all objects accessible from the root and active module
  coll.r.root->mark_node();

  for (LocalRuntime* loc : coll.r.local_runtimes) {
    loc->active_module->mark_node();
    for (auto o : loc->restarts) {
      o->mark_node();
    }
  }

  // also, mark everything accessible via a lexical context
  for (LexicalScope* s : coll.context_list) {
    for (auto o : s->map) {
      o.first->mark_node();
      o.second->mark_node();
    }
  }
  for (auto o : coll.roots.data) {
    o.first->mark_node();
  }

  t::get()->mark_node();
  nil::get()->mark_node();
}

void sweep(STWCollector& coll) {
  list<Object*> new_list;
  for (Object* obj : coll.node_list) {
    if (!obj->marked) {
      delete obj;
    }
    else {
      obj->marked = false;
      new_list.push_front(obj);
    }
  }
  coll.node_list = new_list;
}


void STWCollector::collect_garbage(LocalRuntime& r) {
  Runtime& runtime = r.r;

  // counter is atomic, is all good :)
  if (counter > 10000) {
    // BEGIN COLLECTION
    std::unique_lock<std::mutex> lck(runtime.collect_mutex);
    //lck.lock();

    if (runtime.collecting) {
      // NOT COLLECTOR THREAD
      // so set our can_collect value to true, and notify the
      // collector thread
      r.can_collect = true;

      // NOW, we wait until garbage collection is finished
      std::unique_lock<std::mutex> finlck(runtime.finished_mutex);
      runtime.finished.wait(finlck);

    } else {
      // WE ARE THE COLLECTOR THREAD
      r.can_collect = true;
      bool can_collect = false;
      while (!can_collect) {
        // we check if we are able to collect
        can_collect = true;
        for (LocalRuntime* loc : runtime.local_runtimes) {
          if (!loc->can_collect) {
            can_collect = false;
          }
        }

        // if we cannot collect, wait until we are notified,
        // where we try again
        if (!can_collect) {
          runtime.collect.wait(lck);
        }
      }

      counter = 0;

      mark(*this);
      sweep(*this);

      // get the lock and tell all threads that they can continue as normal
      std::unique_lock<std::mutex> finlck(runtime.finished_mutex);
      runtime.finished.notify_all();
    }
  } 
}

void STWCollector::register_node(expr::Object *obj) {
  node_list_mutex.lock();
  node_list.push_front(obj);
  node_list_mutex.unlock();
}

void STWCollector::register_context(LexicalScope *s) {
  unique_lock<mutex> lck(context_mutex);
  context_list.push_front(s);
  lck.unlock();
}

void STWCollector::remove_context(LexicalScope *s) {
  unique_lock<mutex> lck(context_mutex);
  context_list.remove(s);
  lck.unlock();
}

void STWCollector::insert_root(Object *obj) {
  unique_lock<mutex> lck(root_mutex);
  roots.insert(obj);
  lck.unlock();
}

void STWCollector::remove_root(Object *obj) {
  unique_lock<mutex> lck(root_mutex);
  roots.remove(obj);
  lck.unlock();
}


STWCollector::STWCollector(Runtime& _r) : r(_r) {}
