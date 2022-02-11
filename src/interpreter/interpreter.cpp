#include "interpreter.hpp"
#include<iostream>
#include<string>
#include<cstring>
#include<vector>
#include<map>

using namespace std;

string Interpreter::getword(string s, int pos){
    char *input = (char*)s.c_str();
    char *word;
    char delim[] =" ,();";
    string str;
    int i=0;
    while(word!=NULL&&i<=pos){
        word = strsep(&input, delim);
        while(word!=NULL&&strcmp(word,"")==0) word = strsep(&input, delim);
        i++;
    }
    if(word==NULL) str = "out of range";
    else str=word;
    return str;
}

string Interpreter::getterm(string s, int pos){
    char *input = (char*)s.c_str();
    char *word;
    char delim[] ="=>< ";
    string str;
    int i=0;
    while(word!=NULL&&i<=pos){
        word = strsep(&input, delim);
        while(word!=NULL&&strcmp(word,"")==0) word = strsep(&input, delim);
        i++;
    }
    if(word==NULL) str = "out of range";
    else str=word;
    return str;
}

string Interpreter::getline(string s, int pos){
    char *input = (char*)s.c_str();
    char *word;
    char delim[] =",";
    string str;
    int i=0;
    while(word!=NULL&&i<=pos){
        word = strsep(&input, delim);
        while(word!=NULL&&strcmp(word,"")==0) word = strsep(&input, delim);
        i++;
    }
    if(word==NULL) str = "out of range";
    else str=word;
    return str;
}

void Interpreter::interpret(string s){
    string word;
    int pos=0;
    
    word=getword(s,0);
    if(word == "create"){
        if(getword(s,1) == "table" ){
            string tablename=getword(s,2);
            string primarykey;
            int primarykeyindex;
            string para=s.substr(s.find("("),s.size()-s.find("("));
            vector <Attribute> attr;
            vector<string> attr_names;
            vector<string> unique_names;
            string line = getline(para,0);
            int i=0;
            Type data_type;
            while(line!="out of range"&&getword(line,0)!="primary"){
                string name = getword(line,0);
                attr_names.push_back(name);
                string datatype = getword(line,1);
                int datalen = -1;
                string s2=getword(line,2);
                if(s2!="out of range") datalen = atoi(s2.c_str());
                bool unique = false;
                string s3=getword(line,3);
                if(s3=="unique") {
                    unique = true;
                    unique_names.push_back(name);
                }
                if (datatype == "int") {
                    data_type = Type::INT;
                    datalen = sizeof(int);
                } else if (datatype == "float") {
                    data_type = Type::DOUBLE;
                    datalen = sizeof(double);
                } else if (datatype == "char") {
                    data_type = Type::STRING;
                } else {
                    cerr << datatype << endl;
                    cerr << "My fault" << endl;
                    throw SyntaxError();
                }
                attr.push_back(Attribute(data_type,datalen));
                i++;
                line = getline(para,i);
            }
            if(getword(line,0)=="primary"){
                primarykey=getword(line,2);
                unique_names.push_back(primarykey);
            }
            api->createTable(tablename, attr_names, attr, primarykey, unique_names);
        } else if(getword(s,1) == "index"){
            string indexname=getword(s,2);
            string tablename=getword(s,4);
            string attrname =getword(s,5);
            api->createIndex(tablename,indexname,attrname);
        } else throw SyntaxError();

    } else if(word == "drop"){
        string name=getword(s,2);
        if(getword(s,1)=="table"){
            api->dropTable(name);
        } else if(getword(s,1)=="index"){
            api->dropIndex(name);
        } else throw SyntaxError();

    } else if(word == "select"){
        int i=1;
        bool flag=false;
        while(getword(s,i)!="out of range"){
            if(getword(s,i)=="from"){
                flag=true;
                break;
            }
            i++;
        }
        vector <string> attr;
        for(int j=1;j<i;j++) attr.push_back(getword(s,j));
        if(!flag) throw SyntaxError();
        string tablename=getword(s,i+1);

        if(getword(s,i+2)=="where"){
            vector <Requirement> cond;
            vector <string> term;
            string exp;
            string s_copy=s;
            int begin=s_copy.find("where")+5;
            s_copy=s_copy.substr(begin,s_copy.size()-begin);
            while(s_copy.find("and")<s_copy.size()){
                int end=s_copy.find("and");
                exp = s_copy.substr(0,end);
                s_copy=s_copy.substr(end+3,s_copy.size()-end-3);
                string t;
                if(exp.find('\'')!=exp.npos) {
                    t=exp.substr(exp.find_first_of('\''),exp.find_last_of('\'')-exp.find_first_of('\'')+1);
                } else t=getword(exp,2);
                cond.push_back(Requirement(getword(exp,0),getword(exp,1),t, api->fetchType(tablename, getword(exp, 0))));
            }
            exp=s_copy;
            string t;
            if(exp.find('\'')!=exp.npos) {
                t=exp.substr(exp.find_first_of('\''),exp.find_last_of('\'')-exp.find_first_of('\'')+1);
            } else t=getword(exp,2);
            cond.push_back(Requirement(getword(exp,0),getword(exp,1),t, api->fetchType(tablename, getword(exp, 0))));
            api->selectTuple(tablename, attr, cond);     
        } else api->selectTuple(tablename, attr, vector<Requirement>());
    } else if(word == "insert"){
        if(getword(s,1)!="into"||getword(s,3)!="values") throw SyntaxError();
        else {
            string tablename=getword(s,2);
            vector <string> tuple;
            string t=s.substr(s.find('('),s.find(')')-s.find('('));
            int i=0;
            while(getline(t,i)!="out of range"){
                string item=getline(t,i);
                if(item.find('\'')!=item.npos){
                    item=item.substr(item.find_first_of('\''),item.find_last_of('\'')-item.find_first_of('\'')+1);
                } else {
                    item=getword(item,0);
                }
                tuple.push_back(item);
                i++;
            }
            api->insertTuple(tablename, tuple);
        }
    } else if(word == "delete"){
        if(getword(s,1)!="from") throw SyntaxError();
        else {
            string tablename=getword(s,2);
            if(getword(s,3)=="where"){
                vector <Requirement> cond;
                vector <string> term;
                string exp;
                string s_copy=s;
                int begin=s_copy.find("where")+5;
                s_copy=s_copy.substr(begin,s_copy.size()-begin);
                while(s_copy.find("and")<s_copy.size()){
                    int end=s_copy.find("and");
                    exp = s_copy.substr(0,end);
                    s_copy=s_copy.substr(end+3,s_copy.size()-end-3);
                    string t;
                    if(exp.find('\'')!=exp.npos) {
                        t=exp.substr(exp.find_first_of('\''),exp.find_last_of('\'')-exp.find_first_of('\'')+1);
                    } else t=getword(exp,2);
                    cond.push_back(Requirement(getword(exp,0),getword(exp,1),t, api->fetchType(tablename, getword(exp, 0))));
                }
                exp=s_copy;
                string t;
                if(exp.find('\'')!=exp.npos) {
                    t=exp.substr(exp.find_first_of('\''),exp.find_last_of('\'')-exp.find_first_of('\'')+1);
                } else t=getword(exp,2);
                cond.push_back(Requirement(getword(exp,0),getword(exp,1),t, api->fetchType(tablename, getword(exp, 0))));
                api->deleteTuple(tablename,cond);
            } else if(getword(s,3)=="out of range") {
                api->deleteTuple(tablename, vector<Requirement>());
            }
            else throw SyntaxError();
        }
    } else {
        throw SyntaxError();
    }
}
