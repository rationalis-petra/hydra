#include <string>

#include <boost/asio.hpp>

#include "expressions.hpp"
#include "utils.hpp"

using namespace expr;

Object *Socket::parent;

Socket::Socket(boost::asio::ip::tcp::socket *socket)
  : isocket(socket), index(0), size(0) {
  slots[get_keyword("parent")] = parent;
  parents.insert(get_keyword("parent"));
}

void Socket::mark_node() {
  if (marked)
    return;
  Object::mark_node();
}

std::string Socket::to_string(interp::LocalRuntime &r,
                              interp::LexicalScope &s) {
  return "<socket>";
}

Char* Socket::next() {
  boost::system::error_code error;
  if (index == size) {
    size = isocket->read_some(boost::asio::buffer(buff), error);
    index = 0;
  } else {
    index++;
  }
  return new Char(buff[index]);
}

Char* Socket::peek() {
  return new Char(buff[index]);
}
