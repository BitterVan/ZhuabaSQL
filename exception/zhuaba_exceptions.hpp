#pragma once
#include <exception>
using namespace std;

class SyntaxError : public exception {
	public:
		const char* what() const throw();
};

class FileError : public exception {
	public:
		const char* what() const throw();
};

class SchemaError : public exception {
	public:
		const char* what() const throw();
};

class SchemaDuplication : public exception {
	public:
		const char* what() const throw();
};

class TupleInitialFail : public exception {
	public:
		const char* what() const throw();
};

class NotValidString : public exception {
	public:
		const char* what() const throw();
};

class PrimaryExist : public exception {
	public:
		const char* what() const throw();
};

class PrimaryNull : public exception {
	public:
		const char* what() const throw();
};

class NoMatchingAttribute : public exception {
	public:
		const char* what() const throw();
};

class DuplicantUnique : public exception {
	public:
		const char* what() const throw();
};

