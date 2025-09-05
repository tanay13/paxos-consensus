#include <algorithm>
#include <iostream>
#include <random>

using namespace std;

class Node {
public:
  string value;
  int proposed_number;
  Node(int proposed_n, string value) {
    value = value;
    proposed_number = proposed_n;
  }
};

class Acceptor {
public:
  vector<Node *> acceptors;
  vector<Node *> majority_nodes;

  Acceptor(vector<Node *> nodes) { acceptors = nodes; }

  void getAndSetMajorityNodes() {
    random_device rd;
    mt19937 gen(rd());
    shuffle(acceptors.begin(), acceptors.end(), gen);
    int majority_number = acceptors.size() / 2 + 1;

    vector<Node *> majority(acceptors.begin(),
                            acceptors.begin() + majority_number);

    this->majority_nodes = majority;
    return;
  }

  void validateAndFilterMajorityNodes(int proposed_number) {
    for (auto it = majority_nodes.begin(); it != majority_nodes.end();) {
      int threshold = (*it)->proposed_number;
      if (threshold > proposed_number)
        it = majority_nodes.erase(it);
      else
        it++;
    }
    return;
  }

  void sendPromise(int proposed_number) {

    this->getAndSetMajorityNodes();
    this->validateAndFilterMajorityNodes(proposed_number);

    vector<pair<int, string>> promises;

    for (auto it : majority_nodes) {
      it->proposed_number = proposed_number;
      promises.push_back({it->proposed_number, it->value});
    }
    return;
  }

  void handleAccept(int proposed_number, string value) {
    this->validateAndFilterMajorityNodes(proposed_number);
    return;
  }
};

class Proposer {
public:
  string proposed_value;
  static int proposed_number;

  Proposer(string value) { proposed_value = value; }

  pair<int, string> prepare() {
    proposed_number += 1;
    return {proposed_number, proposed_value};
  }

  string sendAccept(Acceptor *acceptor) {

    for (auto it : acceptor->majority_nodes) {
      if (it->value != "" && it->value != proposed_value) {
        proposed_value = it->value;
        break;
      }
    }

    acceptor->handleAccept(proposed_number, proposed_value);
    return proposed_value;
  }
};

int Proposer::proposed_number = 0;

class Learner {
public:
  Acceptor *acceptor;

  Learner(Acceptor *acceptor) : acceptor(acceptor) {}

  bool isMajority() {
    int total_nodes = acceptor->acceptors.size();
    int quoram = total_nodes / 2 + 1;
    return acceptor->majority_nodes.size() >= quoram;
  }

  bool setValues(string proposed_value) {

    if (!isMajority())
      return false;

    for (auto it : acceptor->majority_nodes) {
      it->value = proposed_value;
    }
    return true;
  }
};

int main() {
  cout << "PAXOS algorithm slot" << endl;

  /* Create Nodes */

  Node *n1 = new Node(0, "");
  Node *n2 = new Node(0, "");
  Node *n3 = new Node(0, "");
  Node *n4 = new Node(0, "");

  vector<Node *> nodes{n1, n2, n3, n4};

  /* Create Acceptors */

  Acceptor *acceptor = new Acceptor(nodes);

  /* Create Proposer */

  Proposer *proposer = new Proposer("Insert Alice");

  Proposer *proposer2 = new Proposer("Insert Bob");
  /* Create Learner */
  Learner *learner = new Learner(acceptor);

  /* Slot flow starts from here */
  int t = 2;
  while (t--) {
    
    pair<int, string> p;
    if(t == 1)
      p = proposer->prepare();
    else
     p = proposer2->prepare();
    acceptor->sendPromise(p.first);

    string p_val_based_on_majority = proposer->sendAccept(acceptor);

    bool isApplied = learner->setValues(p_val_based_on_majority);

    if (isApplied) {
      cout << "Successfuly applied the proposed value to the nodes" << endl;
    } else {
      cout << "Quoram not met" << endl;
    }

    for (auto it : nodes) {
      cout << it->proposed_number << " " << it->value;
      cout << endl;
    }
  }
  return 0;
}
