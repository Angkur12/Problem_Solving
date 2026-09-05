#include<bits/stdc++.h>
using namespace std;

int main()  {
  deque<int> deq;

  deq.push_back(1);
  deq.push_back(2);
  deq.push_back(3);
  deq.push_front(6);
  deq.push_front(7);

  for(int val: deq) {
    cout<<val<<" ";
  }

  //list => doubly linked list
  //deque => dynamic array
}