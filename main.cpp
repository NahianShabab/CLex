#include<iostream>
#include<fstream>
#include<string>
using namespace std;

int main(){
    string s;
    ifstream fin("input.txt");
    ofstream fout("output.txt");
    while (!fout.eof())
    {
        /* code */
        getline(fin,s);
        if(s=="")
            break;
        fout<<"\""<<s<<"\",";

    }
    fout.close();
    fin.close();
    
}