#include<bits/stdc++.h>
using namespace std;


int main()  {
  string str;
  cin>>str;

  stringstream ss(str);
  string word;

  while (getline(ss,word, ',')) {
    cout<<word<<endl;
  }
  



}
