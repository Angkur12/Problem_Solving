#include<bits/stdc++.h>
using namespace std;

int main()  {
  set<int>s1;
  set<int>s2 = {2,3};
  s2.insert(1);

  for(auto x : s2) cout<<x<<" ";
  cout<<endl;


  return 0;
}