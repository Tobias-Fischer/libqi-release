#pragma once
/*
** Author(s):
**  - Chris Kilner <ckilner@aldebaran-robotics.com>
**
** Copyright (C) 2010 Aldebaran Robotics
*/
#ifndef   __QI_MESSAGING_NODE_HPP__
#define   __QI_MESSAGING_NODE_HPP__

#include <qi/messaging/server.hpp>
#include <qi/messaging/client.hpp>

namespace qi {
  /// <summary> Node: A combination of Server and Client. </summary>
  class Node : public Server, public Client {
  public:

    /// <summary> Default constructor. </summary>
    Node();

    /// <summary> Finaliser. </summary>
    virtual ~Node();

    /// <summary> Full Constructor. </summary>
    /// <param name="nodeName"> Name of the node. </param>
    /// <param name="nodeAddress"> The node address. </param>
    /// <param name="masterAddress"> The master address. </param>
    Node(const std::string& nodeName,
      const std::string& nodeAddress,
      const std::string& masterAddress);
  };
}

#endif // __QI_MESSAGING_NODE_HPP__

