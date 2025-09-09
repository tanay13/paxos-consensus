#include "acceptor.hpp"
#include "learner.hpp"
#include "node.hpp"
#include "proposer.hpp"
#include <iostream>

const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string CYAN = "\033[36m";
const std::string RESET = "\033[0m";


int main() {
  std::cout <<BLUE<<"-------- PAXOS algorithm Demo -------- \n\n"<<RESET;

  Node *n1 = new Node(0, 0, "");
  Node *n2 = new Node(0, 0, "");
  Node *n3 = new Node(0, 0, "");
  Node *n4 = new Node(0, 0, "");
  std::vector<Node *> nodes{n1, n2, n3, n4};

  Acceptor acceptor(nodes);
  Proposer proposer1("Insert Alice", &acceptor);
  Proposer proposer2("Insert Bob", &acceptor);
  Learner learner;

  for (int round = 0; round < 2; ++round) {
    std::cout<<CYAN<<"ROUND "<<round<<" STARTED"<<RESET<<"\n\n";
    auto [num, val] = (round == 0) ? proposer1.start() : proposer2.start();

    if (num < 0) {
      std::cout <<RED<<"Quorum not met\n"<<RESET;
    } else {
      std::cout <<GREEN<<"Value accepted: " << val << "\n"<<RESET;
    }

    for (auto node : acceptor.acceptors) {
      std::cout <<YELLOW<< "Node id: " <<RESET<<node->id <<YELLOW<<" Value: " <<RESET<<node->value
           <<YELLOW<<" Promised Value: "<<RESET<< node->promised_number
           <<YELLOW<<" Accepted Value: "<<RESET<< node->accepted_number << "\n";
    }

    std::cout<<"\n";

    learner.setValues(val);
  }
  return 0;
}
