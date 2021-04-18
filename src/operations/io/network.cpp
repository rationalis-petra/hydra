#include <iostream>
#include <list>
#include <string>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "operations/io.hpp"
#include "operations/network.hpp"
#include "utils.hpp"

using std::list;
using std::string;

using namespace expr;
using namespace interp;
using boost::asio::ip::tcp;

Object *op_mk_socket(list<Object *> arg_list, LocalRuntime &r,
                     LexicalScope &s) {
  HString* str = get_inbuilt<HString*>(arg_list.front());

  boost::asio::io_context io_context;
  tcp::resolver resolver(io_context);

  tcp::resolver::results_type endpoints = resolver.resolve(str->value, "daytime");

  tcp::socket *socket = new tcp::socket(io_context);
  boost::asio::connect(*socket, endpoints);

  return new Socket(socket);
}

Object *op_close_socket(list<Object *> arg_list, LocalRuntime &r,
                        LexicalScope &s) {
  Socket* sock = get_inbuilt<Socket*>(arg_list.front());
  sock->isocket->close();
  return nil::get();
}

Object *op_next_socket(list<Object *> arg_list, LocalRuntime &r,
                       LexicalScope &s) {
  Socket* sock = get_inbuilt<Socket*>(arg_list.front());
  return sock->next();
}

Object *op_peek_socket(list<Object *> arg_list, LocalRuntime &r,
                       LexicalScope &s) {
  Socket* sock = get_inbuilt<Socket*>(arg_list.front());
  return sock->peek();
}

Operator *op::mk_socket;

void op::initialize_network() {

  op::mk_socket =
    new InbuiltOperator("socket", "creates a new network socket",
                        op_mk_socket,
                        type::Fn::with_args({type::string_type}), true);
  Operator* in_op_peek_socket =
    new InbuiltOperator("close-socket", "closes a netowrk socket",
                        op_peek_socket,
                        type::Fn::with_args({type::socket_type}), true);
  op::peek->add(in_op_peek_socket);
  Operator* in_op_next_socket =
    new InbuiltOperator("close-socket", "closes a netowrk socket",
                        op_next_socket,
                        type::Fn::with_args({type::socket_type}), true);
  op::next->add(in_op_next_socket);

  Operator* in_op_close_socket =
    new InbuiltOperator("close-socket", "closes a netowrk socket",
                        op_close_socket,
                        type::Fn::with_args({type::socket_type}), true);
  op::close->add(in_op_close_socket);
  
}
