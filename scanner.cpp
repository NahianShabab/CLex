#include<stdio.h>
#include"Token.h"
#include<iostream>
#include<string>
using std::cout,std::string;
// scanner file pointer
FILE * sfp=fopen("input.txt","r");
unsigned int state;
string token_value;
unsigned int line_no=1;

enum STATES{
    BEGIN,ID,ID_KEYWORD,NUMBER_0,NUMBER_1,NUMBER_2,NUMBER_3,
    NUMBER_4,NUMBER_5,GT,LT,ASSIGN,NOT
};

bool is_digit(char c){
    return c>='0' && c<='9';
}

bool is_lowercase_letter(char c){
    return c>='a' && c<='z';
}

bool is_start_of_id(char c){
    return c>='a' && c<='z' || c>='A' && c<='Z' || c=='_';
}

bool is_whitespace(char c){
    return c==' ' || c=='\t'|| c=='\n';
}
bool is_newline(char c){
    return c=='\n';
}

// go back one character
void retractFilePointer(){
    if(!feof(sfp))
        fseek(sfp,-1,SEEK_CUR);
}


Token * scan(){
    state=BEGIN;
    token_value="";
    while(1){
        char c=fgetc(sfp);
        if(feof(sfp) && state==BEGIN){
            return NULL;
        }
        else
            token_value+=c;
        if(is_newline(c))
            line_no++;
        switch (state)
        {
            case BEGIN:
                if(is_digit(c))
                    state=NUMBER_0;
                else if(is_lowercase_letter(c))
                    state=ID_KEYWORD;
                else if(is_start_of_id(c))
                    state=ID;
                else if(is_whitespace(c)){
                    state=BEGIN;
                    token_value="";
                }
                // relational operators
                else if(c=='>')
                    state=GT;
                else if(c=='<')
                    state=LT;
                else if(c=='=')
                    state=ASSIGN;
                else if(c=='!')
                    state=NOT;
                //logical operators
                //arithmetic operators
                // invalid character
                else return NULL;
                break;
            case GT:
                if(c=='=')
                    return new Token("RELOP","GE");
                else{
                    retractFilePointer();
                    return new Token("RELOP","GT");
                }
                break;
            case LT:
                if(c=='=')
                    return new Token("RELOP","LE");
                else{
                    retractFilePointer();
                    return new Token("RELOP","LT");
                }
                break;
            case ASSIGN:
                if(c=='=')
                    return new Token("RELOP","EQ");
                else{
                    retractFilePointer();
                    return new Token("ASSIGNOP");
                }
                break;
            case NOT:
                if(c=='=')
                    return new Token("RELOP","NE");
                else{
                    retractFilePointer();
                    return new Token("LOGICOP","NOT");
                }
                break;
            case NUMBER_0:
                if(is_digit(c))
                    continue;
                else if(c=='.')
                    state=NUMBER_1;
                else{
                    retractFilePointer();
                    return new Token("CONST_INT",token_value.substr(0,token_value.length()-1));
                }
                break;
            case NUMBER_1:
                if(is_digit(c))
                    state=NUMBER_2;
                // error, no digit following dot
                else
                    return NULL;
                break;
            case NUMBER_2:
                if(is_digit(c))
                    continue;
                else if(c=='e' || c=='E')
                    state=NUMBER_3;
                else{
                    retractFilePointer();
                    return new Token("CONST_FLOAT",token_value.substr(0,token_value.length()-1));
                }
                break;
            case NUMBER_3:
                if(c=='+' | c=='-')
                    state=NUMBER_4;
                else if(is_digit(c))
                    state=NUMBER_5;
                //error, 1.2e...
                else
                    return NULL;
                break;
            case NUMBER_4:
                if(is_digit(c))
                    state=NUMBER_5;
                //error, 1.2e+...
                else
                    return NULL;
                break;
            case NUMBER_5:
                if(is_digit(c))
                    continue;
                else{
                    retractFilePointer();
                    return new Token("CONST_FLOAT",token_value.substr(0,token_value.length()-1));
                }
                break;
            // case 1:
            //     if(c=='f'){
            //         return new Token("IF");
            //     }
            //     return NULL;
            // case 3:
            //     if(c=='h')
            //         state=4;
            //     else
            //         return NULL;
            //     break;
            // case 4:
            //     if(c=='e')
            //         state=5;
            //     else 
            //         return NULL;
            //     break;
            // case 5:
            //     if(c=='n'){
            //         return new Token("THEN");
            //     }
            //     return NULL; 
            // case 7:
            //     if(c=='l')
            //         state=8;
            //     else
            //         return NULL;
            //     break;
            // case 8:
            //     if(c=='s')
            //         state=9;
            //     else
            //         return NULL;
            //     break;
            // case 9:
            //     if(c=='e')
            //         return new Token("ELSE");
            //     return NULL;
            default:
                break;
        }
    }
};

int main(int argc,char * argv[]){
    Token * t;
    while ((t=scan())!=NULL)
    {
        cout<<'<'<<t->name<<','<<t->value<<'>'<<'\n';
    };
    cout<<"lines: "<<line_no<<'\n';
    
}
