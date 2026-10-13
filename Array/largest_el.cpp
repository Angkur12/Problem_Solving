#include<bits/stdc++.h>
using namespace std;

int main()  {
  int n;
  cin>>n;
  int arr[n];
  for(int i = 0; i<n; i++)  {
    cin>>arr[i];
  }

  int lar = arr[0];

  for(int i = 0; i<n; i++)  {
    if(arr[i] > arr[0]) {
     lar = arr[i];
    }
  }
  cout<< "the largest: "<< lar;
}