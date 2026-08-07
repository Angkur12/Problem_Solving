#include<iostream>
#include<string>
using namespace std;

int main() {
  // string str = "hey college";
  // str = "hello";
  // cout<<str;
  // string str1 = "shradha";
  // string str2 = "khapra";

  // // cout<< (str1> str2) << endl;
  // string str;
  // getline(cin,str);


  // cout<< str<< endl;

  string str = "apna college";

  for(int i = str.length() - 1; i >= 0; i--) {
    cout<<str[i];
  }

}