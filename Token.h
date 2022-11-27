#pragma once
#include<string>
using std::string;
struct Token{
    public:
        string name;
        string value;
        Token(string name,string value=""){
            this->name=name;
            this->value=value;
        }
};