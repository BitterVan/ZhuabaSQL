#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../api/api.hpp"
#include "../buffer_pool/buffer_pool.hpp"
#include<iostream>
#include<string>
using namespace std;

class Interpreter{
    private:
    API *api;

    public:
    Interpreter(API *api):api(api){}
    void interpret(string s);
    string getword(string s, int pos); //pos starts from 0.
    string getline(string s, int pos); //pos starts from 0.
    string getterm(string s, int pos); //pos starts from 0.
};

// class SyntaxError{
//     public:
//     void print(){
//         cerr<<"Syntax error!"<<endl;
//     }
// };

#endif