#include<bits/stdc++.h>
using namespace std;

int main()  {
  // pair<int, pair<int,int>> p = {1, {2,3}};
  // cout<<p.first<<endl;
  // cout<<p.second.first<<endl;
  // cout<<p.second.second<<endl;

  vector<pair<int,int>> vec = {{1,2}, {2,3}, {3,4}};
  vec.push_back({4,5});
  vec.emplace_back(4,5);
  for(auto p: vec) {
    cout<<p.first<<" "<<p.second<<endl;
  }

  return 0;
}