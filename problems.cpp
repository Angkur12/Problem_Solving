#include<iostream>
#include<iomanip>
using namespace std;

int main()  {
 string str[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

 int n; cin>>n;

 if(n>= 1 && n <= 9  ) cout <<str[n-1];
 else cout<< n << " is greater than 9" ;

}