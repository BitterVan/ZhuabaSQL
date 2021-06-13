#include "basics.hpp"
using namespace std;

class Attribute {
	private:
		Type data_type;
		int data_length;
	
	public:
		Attribute(Type, int);
		Attribute(const Attribute&);
		Attribute();
		~Attribute();
		Type attributeType() const;
		int attributeLength() const;
};