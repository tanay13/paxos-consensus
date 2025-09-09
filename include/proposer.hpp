#pragma once
#include "acceptor.hpp"
#include <string>
#include <utility>

class Proposer {
public:
  int propose_id;
  std::string proposed_value;

  Acceptor* acceptor;

  Proposer(std::string value, Acceptor* acceptor);

  std::pair<int, std::string> start();
  std::pair<int, std::string> prepare();
  bool sendAccept(int proposed_n, std::string proposed_val);

private:
  static int propose_number;
};
