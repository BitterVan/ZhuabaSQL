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

const char* PrimaryExist::what() const throw() {
	return "Duplicated primary key!";
}

const char* PrimaryNull::what() const throw() {
	return "Primary key cannot be NULL!";
}

const char* NoMatchingAttribute::what() const throw() {
	return "No such attribute!";
}

const char* DuplicantUnique::what() const throw() {
	return "Duplicant unique values!";
}