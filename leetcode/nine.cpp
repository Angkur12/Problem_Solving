#include<bits/stdc++.h>
using namespace std;


int main()  {
  int n;
  cin>>n;

  long long rev = 0;
  int temp = n;
  while (x > 0) {
    int mod = x % 10;
    rev = (rev*10) + mod;
    x = x/10;
  }
  if(rev == temp) cout<<"true"<<endl;
  else cout<<"false"<<endl;

}
