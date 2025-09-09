#pragma once

#include<string>

struct Message {
  int dest_node;
  std::string type;  // promised, promise_reject
  std::pair<int, std::string> balot;
};
