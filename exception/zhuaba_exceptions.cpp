#include "zhuaba_exceptions.hpp"
using namespace std;

const char* SyntaxError::what() const throw() {
	return "Syntax Error!";
}

const char* FileError::what() const throw() {
	return "File does not exist!";
}

const char* SchemaError::what() const throw() {
	return "Schema does not exist!";
}

const char* SchemaDuplication::what() const throw() {
	return "Schema already exist!";
}

const char* TupleInitialFail::what() const throw() {
	return "Tuple length won't match!";
}

const char* NotValidString::what() const throw() {
	return "Some string have wrong format!";
}

