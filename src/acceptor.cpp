#include "acceptor.hpp"
#include "message.hpp"
#include <vector>

Acceptor::Acceptor(std::vector<Node *> nodes) : acceptors(nodes) {}

int Acceptor::getQuorum() { return acceptors.size() / 2 + 1; }

std::vector<Message> Acceptor::sendPrepare(int proposed_number) {

  std::vector<Message> promises;
  for (auto node : acceptors) {
    promises.push_back(node->handlePrepare(proposed_number));
  }
  return promises;
}

std::vector<Message> Acceptor::sendAccept(int proposed_number, std::string proposed_value) {
  
  std::vector<Message> accept_m;

  for(auto node: acceptors){
    accept_m.push_back(node->handleAccept(proposed_number, proposed_value));
  }
  return accept_m;
}

