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
#include <climits>
#include <chrono>
#include <unordered_set>
#include "../exception/zhuaba_exceptions.hpp"
using namespace std;

// Enumeration of the basic three types
enum class Type {INT, DOUBLE, STRING};

// Enumeration of condition operators
enum class Operator {EQ, NE, LT, LE, GT, GE};

const int BLOCK_SIZE = 8192;
const int MAX_CAPACITY = 1024;
const int REAL_SIZE = 8192-1024;
const int BUFFER_CAPACITY = 1024;

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
const string PARENT_STRING = "_parent";
const string PREVIOUS_STRING = "_previous";
const int PARENT_INT = -2147483647;
const int PREVIOUS_INT = 2147483646;
const double PARENT_DOUBLE = -2147483646;
const double PREVIOUS_DOUBLE = 2147483645;
const string MIN_STRING = "\0";
const int MIN_INT = -2147483648;
const int MIN_DOUBLE = -2147483648;
const int MIN_PAGE_NUMBER = -1;

const string ROOT_PARENT_FILE_NAME = "_root";
const int ROOT_PARENT_PAGE_NUMBER = -1;