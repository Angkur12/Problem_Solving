#include<bits/stdc++.h>
using namespace std;

class human {
  public :
    int age;
    string name1;
    string name2;
    int cls;

    void display() {
      cout<<age<<endl;
      cout<<name2<<", "<<name1<<endl;
      cout<<cls<<endl;
      cout<<endl;
      cout<<age<<","<<name1<<","<<name2<<","<<cls;
    }



};

int main()  {
  human man;
  cin>>man.age;
  cin>>man.name1;
  cin>>man.name2;
  cin>>man.cls;


  man.display();
}