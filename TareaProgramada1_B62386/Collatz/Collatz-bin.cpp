#include <iostream>
#include <bitset>
using namespace std;
#define BLACK   "\033[0m"
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"
double inicio;
double final;
double tiempo;
time_t timer;

int pasos=0;
unsigned long multiply=0;
unsigned long add=0;
unsigned long third=0;

int bitwise_sum(unsigned long x, unsigned long y)
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
    unsigned long tmp=numero;
    
    while(tmp!=1){
cout<<"El numero es: "<<BLUE<<tmp<<BLACK<<endl;
        if((tmp&1)==0){
            tmp=tmp>>1;
            pasos++;
        }else{
            multiply=tmp<<1;
            add=multiply|1;
            third=bitwise_sum(add,tmp);
            tmp=third;
        }
    }
cout<<"El numero es: "<<RED<<tmp<<BLACK<<endl;
}

int main()
{
unsigned long entrada;
cout<<"Ingrese el numero al que quiere aplicar Collatz:"<<endl;
cin>>entrada;
 inicio=clock();
 collatz(entrada);
final=clock();
  tiempo=(final-inicio)/CLOCKS_PER_SEC;
cout<<"numero de pasos: "<<pasos<<endl;
cout<<"El programa tardó: "<<tiempo<<" segundos"<<endl;
}
