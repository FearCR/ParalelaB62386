
#include <iostream>
using namespace std;
double inicio;
double final_;
double tiempo;
time_t timer;
int pasos=0;

// A method to multiply two numbers using Russian Peasant method

unsigned int bitwiseSum(unsigned int num1, unsigned int num2){
  unsigned int carry;
  while (num2 != 0) {
      carry = num1 & num2;
      num1 = num1 ^ num2;
      num2 = carry << 1;
   }
   return num1;
}



int main()
{
  int a;
  unsigned int num=123;
  unsigned int mul;
  unsigned int  adi;
  unsigned int fin;
 while(num!=1){
    cout<<num<<endl;
    if(num%2==1){
      mul=num<<1;
      adi=num|1;
      fin=bitwiseSum(num,adi);
      num=fin;
    }else{
      num=num>>1;
    }
    cin>>a;
  }
  cout<<num<<endl;

  cout << num << endl;
  return 0;
}
