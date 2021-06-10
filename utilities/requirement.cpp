#include "requirement.hpp"
using namespace std;

Requirement::Requirement(const string& src_attr, const Item& src_val, Operator src_type) : attribute_name(src_attr), item(src_val), operator_type(src_type) {}

Requirement::Requirement(const Requirement&) = default;

Requirement::~Requirement() = default;