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
using namespace std;

// Enumeration of the basic three types
enum class Type {INT, DOUBLE, STRING};

// Enumeration of condition operators
enum class Operator {EQ, NE, LT, LE, GT, GE};

const int BLOCK_SIZE = 4096;
const int MAX_CAPACITY = 1024;
const int REAL_SIZE = 3072;
const int BUFFER_CAPACITY = 128;

const string CATALOG_FILE = ".catalog";

int str2int(const string& src_string);

double str2double(const string& src_string);

