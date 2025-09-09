#pragma once
#include "message.hpp"
#include <string>

class Node {
public:
  int id;
  int promised_number;
  int accepted_number;
  std::string value;

  Node(int proposed_n, int accepted_n, std::string v);
  Message handlePrepare(int propoed_n);
  Message handleAccept(int proposed_n, std::string proposed_val);

  private:
    static int next_id;
};
