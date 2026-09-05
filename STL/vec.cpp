#include<bits/stdc++.h>
using namespace std;

int main()  {
  vector<int> vec = {1,2,3,4,5};

  vector<int>::iterator it;

  for(it = vec.begin(); it != vec.end(); it++) {
    cout<<*it<<" ";
  }

  // cout<<*(vec.end())<<endl;
}