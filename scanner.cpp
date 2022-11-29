#include<stdio.h>
#include"Token.h"
#include<iostream>
#include<string>
#define NUM_OF_KEYWORD 32
using std::cout,std::string;
// scanner file pointer
FILE * sfp=fopen("input.txt","r");
unsigned int state;
string token_value;
unsigned int line_no=1;
string keywords[32]={"auto","double","int","struct","break","else","long","switch","case","enum","register","typedef","char","extern","return","union","const","float","short","unsigned","continue","for","signed","void",
"default","goto","sizeof","volatile","do","if","static","while"};

enum STATES{
    BEGIN,ID,NUMBER_0,NUMBER_1,NUMBER_2,NUMBER_3,
    NUMBER_4,NUMBER_5,GT,LT,ASSIGN,NOT,ADD,SUBTRACT,AND,OR
};

bool is_digit(char c){
    return c>='0' && c<='9';
}

bool is_letter(char c){
    return c>='a' && c<='z' || c>='A' && c<='Z';
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

bool is_keyword(string s){
    for(int i=0;i<NUM_OF_KEYWORD;i++)
        if(keywords[i]==s)
            return true;
    return false;
}

// go back one character
void retractFilePointer(){
    if(!feof(sfp))
        fseek(sfp,-1,SEEK_CUR);
}

Token * generateToken(string name,string value=""){
    return new Token(name,value);
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
                else if(is_start_of_id(c))
                    state=ID;
                else if(is_whitespace(c)){
                    state=BEGIN;
                    token_value="";
                }
                else if(c=='*' || c=='/')
                    return generateToken("MULOP",token_value);
                else if(c=='%')
                    return generateToken("MODOP");
                else if(c=='+')
                    state=ADD;
                else if(c=='-')
                    state=SUBTRACT;
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
                else if(c=='&')
                    state=AND;
                else if(c=='|')
                    state=OR;
                else if(c=='^')
                    return generateToken("BITOP","^");
                // punctuations
                else if(c=='{')
                    return generateToken("LCURL");
                else if(c=='}')
                    return generateToken("RCURL");
                else if(c=='(')
                    return generateToken("LPAREN");
                else if(c==')')
                    return generateToken("RPAREN");
                else if(c=='[')
                    return generateToken("LTHIRD");
                else if(c==']')
                    return generateToken("RTHIRD");
                else if(c==';')
                    return generateToken("SEMICOLON");
                else if(c==',')
                    return generateToken("COMMA");
                // invalid character
                else return NULL;
                break;
            case ID:
                if(is_digit(c) || is_letter(c) || c=='_')
                    continue;
                else{
                    retractFilePointer();
                    string tok=token_value.substr(0,token_value.length()-1);
                    if(!is_keyword(tok))
                        return new Token("ID",tok);
                    else
                        return new Token(tok);
                }
                break;
            case GT:
                if(c=='=')
                    return new Token("RELOP","GE");
                else if(c=='>')
                    return generateToken("BITOP",">>");
                else{
                    retractFilePointer();
                    return new Token("RELOP","GT");
                }
                break;
            case LT:
                if(c=='=')
                    return new Token("RELOP","LE");
                else if(c=='<')
                    return generateToken("BITOP","<<");
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
            case ADD:
                if(c=='+')
                    return generateToken("INC");
                else{
                    retractFilePointer();
                    return generateToken("ADDOP","+");
                }
                break;
            case SUBTRACT:
                if(c=='-')
                    return generateToken("DEC");
                else{
                    retractFilePointer();
                    return generateToken("ADDOP","-");
                }
                break;
            case AND:
                if(c=='&')
                    return generateToken("LOGICOP","&&");
                else{
                    retractFilePointer();
                    return generateToken("BITOP","&");
                }
                break;
            case OR:
                if(c=='|')
                    return generateToken("LOGICOP","||");
                else{
                    retractFilePointer();
                    return generateToken("BITOP","|");
                }
                break;
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
        delete t;
    };
    cout<<"lines: "<<line_no<<'\n';
    
}
