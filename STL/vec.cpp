#include<bits/stdc++.h>
using namespace std;

int main()  {
  vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);

  for(int val: vec) {
    cout<<val<<" ";
  }
  cout<<endl; 

  cout<<vec.capacity()<<endl;
}