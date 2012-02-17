/*
** Author(s):
**  - Cedric GESTES <gestes@aldebaran-robotics.com>
**
** Copyright (C) 2012 Aldebaran Robotics
*/

#include <qimessaging/message.hpp>
#include <qimessaging/datastream.hpp>
#include <iostream>

unsigned int qi::Message::_id_seq = 0;

namespace qi {

  std::ostream& operator<<(std::ostream& os, const qi::Message& msg) {
    os << "message {"
       << "id=" << msg.id()
       << ", type=" << (char)msg.type() + '0'
       << ", dest=" << msg.service()
       << ", func=" << msg.function()
       << ", data_size=" << msg.data().size()
       << "}";
    return os;
  }


  Message::Message(const std::string &data)
    : _id (_id_seq++)
  {
    qi::DataStream ds(data);
    char c;
    int  i;

    ds >> c;
    _type = (MessageType)c;
    ds >> i;
    _id = i;
    ds >> _service;
    ds >> _func;
    ds >> _data;
  }

  std::string Message::str()const {
    qi::DataStream ds;

    ds << (char)_type;
    ds << (int)_id;
    ds << _service;
    ds << _func;
    ds << _data;
    return ds.str();
  }

}
