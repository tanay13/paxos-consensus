#include "node.hpp"
#include "message.hpp"
#include <utility>

int Node::next_id = 0;

Node::Node(int proposed_n, int accepted_n, std::string v)
    : promised_number(proposed_n), value(v), accepted_number(accepted_n) {
  id = next_id++; 
}

Message Node::handlePrepare(int proposed_n) {

  Message m{id, "PROMISE_REJECT", {promised_number, value}};

  if (proposed_n >= promised_number) {
    promised_number = proposed_n;
    m.type = "PROMISE_ACCEPT";
    m.balot.first = proposed_n;
    if (accepted_number > 0) {
      m.balot.second = value;
    }
  }
  return m;
}


Message Node::handleAccept(int proposed_n, std::string proposed_val){

  Message m{id, "NOT_ACCEPTED", {promised_number, value}};

  if(proposed_n >= promised_number){
    accepted_number = proposed_n;
    value = proposed_val;
    m.type = "ACCEPTED";
    m.balot = std::make_pair(proposed_n, proposed_val);
  }
  return m;
}
