#include "attribute.hpp"
using namespace std;

Attribute::Attribute(Type src_type, int src_length) : data_type(src_type), data_length(src_length) {}

Attribute::Attribute(const Attribute&) = default;

Attribute::~Attribute() = default;

Attribute::Attribute() = default;

Type Attribute::attributeType() const {
	return data_type;
}

int Attribute::attributeLength() const {
	return data_length;
}