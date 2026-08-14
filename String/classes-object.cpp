#include<bits/stdc++.h>
using namespace std;

class student {
  public:

  int scores[5];
  void input () {
    for(int i = 0; i<5; i++)  {
      cin>>scores[i];
    }
  }

  int calculateTotalScore() {
    int sum = 0;
    for(int i = 0; i< 5; i++) {
      sum += scores[i];
    }
    return sum;
  }
};

int main()  {

  int n; cin>>n;

  student kristen;
  kristen.input();

  int kristenScore = kristen.calculateTotalScore();

  int cnt = 0;

      for (int i = 1; i < n; i++) {
        student student;

        student.input();

        int studentScore = student.calculateTotalScore();

        if (studentScore > kristenScore) {
            cnt++;
        }
    }
  
   cout<<cnt<<endl; 



}
