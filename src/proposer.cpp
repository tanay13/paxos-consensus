#include "proposer.hpp"
#include "acceptor.hpp"
#include "message.hpp"
#include <utility>
#include<iostream>

int Proposer::propose_number = 1;

Proposer::Proposer(std::string value, Acceptor *acceptor)
    : proposed_value(value), acceptor(acceptor) {
  propose_id = propose_number++;
}

std::pair<int, std::string> Proposer::start() {

  std::pair<int, std::string> balot = prepare();

  if (balot.first == -1)
    return balot;

  bool isAccepted = sendAccept(propose_id, proposed_value);

  if (isAccepted)
    return {propose_id, proposed_value};

  return {-1, ""};
}

std::pair<int, std::string> Proposer::prepare() {

  std::vector<Message> promises = acceptor->sendPrepare(propose_id);

  int count_promised_accept = 0;

  for (auto promise : promises) {
    if (promise.type == "PROMISE_ACCEPT") {
      count_promised_accept++;
    }
  }

  int quorum = acceptor->getQuorum();

  if (count_promised_accept >= quorum) {
    std::string majority_value = proposed_value;
    int max_proposed_number = -1;
    for (auto promise : promises) {
      if (promise.balot.second != "" &&
          promise.balot.first > max_proposed_number) {
        majority_value = promise.balot.second;
        max_proposed_number = promise.balot.first;
      }
    }
    this->proposed_value = majority_value;
    return {propose_id, proposed_value};
  }
  return {-1, ""};
}

bool Proposer::sendAccept(int propose_n, std::string proposed_val) {

  std::vector<Message> accept_messages =
      acceptor->sendAccept(propose_n, proposed_val);

  int quorum = acceptor->getQuorum();

  int count_accepts = 0;

  for (auto message : accept_messages) {
    if (message.type == "ACCEPTED")
      count_accepts++;
  }

  return count_accepts >= quorum;
}
