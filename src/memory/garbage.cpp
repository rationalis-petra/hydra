#include "expressions.hpp"

// the garbage collector for the language
using namespace expr;

using std::string;
using std::list;

void mark() {
  // mark all objects accessible from the root and active module
  Object::r.root->mark_node();

  for (LocalRuntime* loc : Object::r.local_runtimes) {
    loc->active_module->mark_node();
    for (auto o : loc->restarts) {
      o->mark_node();
    }
  }

  // also, mark everything accessible via a lexical context
  for (LexicalScope* s : Object::context_list) {
    for (auto o : s->map) {
      o.first->mark_node();
      o.second->mark_node();
    }
  }
  for (auto o : Object::roots.data) {
    o.first->mark_node();
  }

  t::get()->mark_node();
  nil::get()->mark_node();
}

void sweep() {
  list<Object*> new_list;
  for (Object* obj : Object::node_list) {
    if (!obj->marked) {
      delete obj;
    }
    else {
      obj->marked = false;
      new_list.push_front(obj);
    }
  }
  Object::node_list = new_list;
}

void Object::collect_garbage(LocalRuntime& r) {
  Runtime& runtime = Object::r;

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

      mark();
      sweep();

      // get the lock and tell all threads that they can continue as normal
      std::unique_lock<std::mutex> finlck(runtime.finished_mutex);
      runtime.finished.notify_all();
    }
  } 
}

