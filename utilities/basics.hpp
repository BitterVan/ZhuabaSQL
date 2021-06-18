#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <cstring>
#include <fstream>
#include <utility>
#include <list>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <sstream>
#include "../exception/zhuaba_exceptions.hpp"
using namespace std;

// Enumeration of the basic three types
enum class Type {INT, DOUBLE, STRING};

// Enumeration of condition operators
enum class Operator {EQ, NE, LT, LE, GT, GE};

const int BLOCK_SIZE = 4096;
const int MAX_CAPACITY = 1024;
const int REAL_SIZE = 3072;
const int BUFFER_CAPACITY = 2;

const string CATALOG_FILE = ".catalog";

int str2int(const string& src_string);

double str2double(const string& src_string);

string str2str(const string& src_stirng);

const int INVALID_INT = -2147483648;
const double INVALID_DOUBLE = -2147483648;
const int INVALID_TUPLE_NUMBER = -10086;
const string INVALID_STRING = ".v";
const string FILE_PREFIX = "database_file/";
const string FREE_LIST_PREFIX = "database_file/.";
const string INVALID_PRIMARY = "ghdhfdksjhf";
const char PARENT_TAG = 'p';
const char PREVIOUS_TAG = 'l';
const char NEXT_TAG = 'n';