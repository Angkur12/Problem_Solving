#include<bits/stdc++.h>
using namespace std;

int main()  {
  pair<int, pair<int,int>> p = {1, {2,3}};
  cout<<p.first<<endl;
  cout<<p.second.first<<endl;
  cout<<p.second.second<<endl;

  return 0;
}