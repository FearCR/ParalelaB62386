//compila con g++ ContarHTML.cc
#include <iostream>
#include <string>
#include <map>
#include <fstream>
using namespace std;
int main(int argc, char **argv)
{
    ifstream file;
    file.open("archivo.html");
    if(!file.is_open()) return 1;
    map<string, int> etiquetas;
    while(file.good())
    {
        string s;
        string word;
        getline(file, s);
        int pos = s.find_first_of('>');
        int ini = s.find_first_of('>');
        if(pos < 0) continue;
        while(s.size() > 0)
        {
            word="";
            ini = s.find_first_of('<');
            pos = s.find_first_of('>');
            if(pos < 0 )
                pos = s.size();
            if(ini>=0){
              word = s.substr(ini, (pos+1)-ini);//substr(posicionInicial, cantidad de caracteres a la derecha)
            }else{
              ini=0;
            }
            if(word != "")
                etiquetas[word]++;
            s = s.erase(0, (pos+1));//erase(posicionInicial, cantidad de caracteres a la derecha)

        }
    }
    for(map<string, int>::iterator wit = etiquetas.begin(); wit != etiquetas.end(); ++wit)
        cout << "etiqueta: " << wit->first << " | Encontro: " << wit->second << endl;
    return 0;
}
