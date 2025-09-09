#pragma once
#include "message.hpp"
#include "node.hpp"
#include <vector>

class Acceptor {
public:
  std::vector<Node *> acceptors;
  Acceptor(std::vector<Node *> nodes);

  int getQuorum();

  std::vector<Message> sendPrepare(int proposed_number);
  std::vector<Message> sendAccept(int proposed_number, std::string value);
  void acceptMessages(Message messages[]);

  Message handleAccept(int proposed_n, std::string proposed_val);
};
