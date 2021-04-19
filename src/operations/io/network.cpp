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

boost::asio::io_context context;

Object *op_mk_tcp_socket(list<Object *> arg_list, LocalRuntime &r,
                         LexicalScope &s) {

  tcp::resolver resolver(context);
  tcp::socket *socket;

  if (arg_list.size() < 2) {
    socket = new tcp::socket(context);
  } else {
    HString *place = get_inbuilt<HString *>(arg_list.front());
    HString *port = get_inbuilt<HString *>(arg_list.back());

    tcp::resolver::results_type endpoints =
        resolver.resolve(place->value, port->value);

    socket = new tcp::socket(context);
    boost::asio::connect(*socket, endpoints);
  }
  return new Socket(socket);
}

Object *op_close_socket(list<Object *> arg_list, LocalRuntime &r,
                        LexicalScope &s) {
  Socket *sock = get_inbuilt<Socket *>(arg_list.front());
  sock->isocket->close();
  return nil::get();
}

Object *op_next_socket(list<Object *> arg_list, LocalRuntime &r,
                       LexicalScope &s) {
  Socket *sock = get_inbuilt<Socket *>(arg_list.front());
  return sock->next();
}

Object *op_peek_socket(list<Object *> arg_list, LocalRuntime &r,
                       LexicalScope &s) {
  Socket *sock = get_inbuilt<Socket *>(arg_list.front());
  return sock->peek();
}

Object *op_write_socket(list<Object *> arg_list, LocalRuntime &r,
                        LexicalScope &s) {
  Socket *sock = get_inbuilt<Socket *>(arg_list.front());
  arg_list.pop_front();
  for (Object *obj : arg_list) {
    sock->write(obj, r, s);
  }

  return arg_list.back();
}

Object *op_mk_tcp_acceptor(list<Object *> arg_list, LocalRuntime &r,
                           LexicalScope &s) {
  tcp::acceptor *acceptor;
  Symbol *sym = get_inbuilt<Symbol *>(arg_list.front());
  Integer *num = get_inbuilt<Integer *>(arg_list.back());

  if (sym == get_keyword("v6")) {
    acceptor = new tcp::acceptor(context, tcp::endpoint(tcp::v6(), num->value));
  } else if (sym == get_keyword("v4")) {
    acceptor = new tcp::acceptor(context, tcp::endpoint(tcp::v4(), num->value));
  } else {
    return expr::nil::get();
  }

  return new Acceptor(acceptor);
}

Object *op_tcp_accept(list<Object *> arg_list, LocalRuntime &r,
                      LexicalScope &s) {
  Acceptor *acceptor = get_inbuilt<Acceptor *>(arg_list.front());
  Socket *sock = get_inbuilt<Socket *>(arg_list.back());

  acceptor->accept(sock);
  return sock;
}

Operator *op::mk_tcp_acceptor;
Operator *op::mk_tcp_socket;

GenericFn *op::accept;

void op::initialize_network() {

  op::mk_tcp_socket = new InbuiltOperator(
      "tcp-socket", "creates a new network socket", op_mk_tcp_socket,
      type::Fn::with_args_optional({}, {type::string_type, type::string_type}),
      true);
  Operator *in_op_peek_socket = new InbuiltOperator(
      "close-socket", "closes a netowrk socket", op_peek_socket,
      type::Fn::with_args({type::socket_type}), true);
  op::peek->add(in_op_peek_socket);
  Operator *in_op_next_socket = new InbuiltOperator(
      "close-socket", "closes a netowrk socket", op_next_socket,
      type::Fn::with_args({type::socket_type}), true);
  op::next->add(in_op_next_socket);

  Operator *in_op_close_socket = new InbuiltOperator(
      "close-socket", "closes a netowrk socket", op_close_socket,
      type::Fn::with_args({type::socket_type}), true);
  op::close->add(in_op_close_socket);

  Operator *in_op_write = new InbuiltOperator(
      "write-socket", "writes data to a network soket", op_write_socket,
      type::Fn::with_all({type::socket_type}, new type::Any, new type::Any),
      true);

  op::write->add(in_op_write);
      // ACCETPTOR
      op::mk_tcp_acceptor = new InbuiltOperator(
      "tcp-acceptor", "creates a new tcp acceptor", op_mk_tcp_acceptor,
      type::Fn::with_args({type::symbol_type, type::integer_type}), true);

  op::accept = new GenericFn;
  op::accept->type =
      type::Fn::with_args({type::acceptor_type, type::socket_type});

  Operator *in_op_tcp_accept = new InbuiltOperator(
      "accept (tcp)", "accepts a connection & attaches it to a socket",
      op_tcp_accept,
      type::Fn::with_args({type::acceptor_type, type::socket_type}), true);
  op::accept->add(in_op_tcp_accept);
};
