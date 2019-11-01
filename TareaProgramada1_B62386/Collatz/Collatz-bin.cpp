#include <iostream>
#include <bitset>
using namespace std;
int pasos=0;

int suma(unsigned int x, unsigned int y)
{
    while(y!=0)
    {
        int carry=x&y;
        x=x^y;
        y=carry<<1;
        
    }
    return x;
}

void collatz(unsigned int numero){
    unsigned int tmp=numero;
    
    while(tmp!=1){
        if((tmp&1)==0){
            tmp=tmp>>1;
            pasos++;
        }else{
            unsigned int multiply=tmp<<1;
            unsigned int add=multiply|1;
            unsigned int third=suma(add,tmp);
            tmp=third;
        }
        cout<<tmp<<endl;
    }
}

int main()
{
collatz(123);
}
