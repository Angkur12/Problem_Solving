#include<bits/stdc++.h>
using namespace std;

int main()  {

  int n; cin>>n;
  vector<int> v(n);

  for(int i = 0; i<n; i++)  {
    cin>>v[i];
  }



  int m; cin>>m;
  v.erase(v.begin() + (m- 1));


  int x, y; cin>> x >> y;
  v.erase(v.begin() + (x-1), v.begin() + (y-1));

  cout<<v.size()<<endl;

  for(int k : v) {
    cout<<k << " ";
  }
}