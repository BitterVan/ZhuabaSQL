#include "block.hpp"
using namespace	std;

void Block::writeBack() const {
	if (!dirty) {
		return;
	}
	fstream write_file(FILE_PREFIX + specifier.schemaName());
	write_file.seekp(specifier.pageNumber() * BLOCK_SIZE);
	char page[BLOCK_SIZE];
	for (int i = 0; i < MAX_CAPACITY; i++) {
		if (i < tuple_list.size())
			page[i] = 1;
		else
			page[i] = 0;
	}

	char *pos = page + MAX_CAPACITY;
	for (int i = 0; i < tuple_list.size(); i++) {
		if (tuple_list[i]._debug_hold_illegal()) {
			tuple_list[i]._debug_show_info();
		}
		memcpy(pos, tuple_list[i].toBit().begin(), block_schema.bitLength());
		pos += block_schema.bitLength();
	}
	write_file.write(page, BLOCK_SIZE);
	write_file.close();
}

void Block::clean() {
	dirty = 0;
}

Block::Block(const Block&) = default;

Block::Block(const Schema& src_schema, const BlockSpecifier& src_block_spec) : block_schema(src_schema), specifier(src_block_spec) {
	capacity = min(REAL_SIZE / block_schema.bitLength(), MAX_CAPACITY);
	dirty = 0;
	char page[BLOCK_SIZE];
	ifstream init_file(FILE_PREFIX + src_block_spec.schemaName());
	init_file.seekg(src_block_spec.pageNumber() * BLOCK_SIZE, ios::beg);
	init_file.read(page, BLOCK_SIZE);
	init_file.close();

	char c;

	for (int i = 0; i < MAX_CAPACITY; i++) {
		c = page[i];	
		if ( c != 0 && c != 1 && c != 2) {
			for (int i = 0; i < capacity; i++) {
				page[i] = 0;
			}
			break;
		}
	}

	char valid[MAX_CAPACITY];
	char data[REAL_SIZE];
	memcpy(valid, page, MAX_CAPACITY);
	memcpy(data, page + MAX_CAPACITY, REAL_SIZE);

	BitStream temp(block_schema.bitLength());
	for (int i = 0; i < capacity; i++) {
		if ((int)valid[i] > 0) {
			memcpy(temp.begin(), data + i*block_schema.bitLength(), block_schema.bitLength());
			auto temp_tuple = Tuple(block_schema, temp);
			if (temp_tuple._debug_hold_illegal()) {
				temp_tuple._debug_show_info();
			}
			tuple_list.push_back(temp_tuple);
		} else {
			break;
		}
	}
}

Block::~Block() {
	if (dirty) {
		writeBack();
	}
}

vector<Tuple> Block::selectTuple(const vector<Requirement>& src_requirements) const {
	vector<Tuple> ret;
	for (auto i : tuple_list) {
		if (i.meetRequirement(src_requirements)) {
			ret.push_back(i);
		}
	}
	return ret;
}

int Block::insertTuple(const Tuple& src_tuple) {
	dirty = 1;
	if (src_tuple._debug_hold_illegal()) {
		src_tuple._debug_show_info();
	}
	tuple_list.push_back(src_tuple);
	return tuple_list.size() - 1;
}

vector<Tuple> Block::deleteTuple(const vector<Requirement>& src_requirements) {
	vector<Tuple> ret;
	dirty = 1;
	for (auto i = tuple_list.begin(); i != tuple_list.end(); i++) {
		if (i->meetRequirement(src_requirements)) {
			// i = tuple_list.erase(i) - 1;
			ret.push_back(*i);
			i->makeInvalid();
		}
	}
	return ret;
}


bool Block::isFull() const {
	return tuple_list.size() == capacity;
}

string Block::schemaName() const {
	return block_schema.name();
}

void Block::orderedInsert(const Tuple& src_tuple) {
	dirty = 1;
	auto pos = upper_bound(tuple_list.begin() + 2, tuple_list.end(), src_tuple);
	tuple_list.insert(pos, src_tuple);
}

bool Block::holdingLeaf() const {
	vector<string> temp;
	temp.push_back("tuple_number");
	return tuple_list.size() <= 2 || tuple_list[2].valueList(temp)[0].toString() != to_string(INVALID_TUPLE_NUMBER);
}

void Block::flowInto(Block& des_block) {
	dirty = 1;
	des_block.dirty = 1;
	// this is the tuple pointing the parent
	des_block.tuple_list.push_back(tuple_list[0]);
	// des_block.tuple_list.push_back(tuple_list[1]);
	// if current block holds leafs instead of the internal nodes;
	// this number is the begin number of the second block
	int split_pos = (des_block.tuple_list.size() - 2) / 2 + 3;
	if (holdingLeaf()) {
		vector<string> attrs;
		// insert the end of splited block's tuple space to the second of the tuple list
		attrs.push_back("key");
		attrs.push_back("file_name");
		auto temp = tuple_list[1].valueList(attrs);
		temp.emplace_back(this->specifier.pageNumber());
		temp.emplace_back(split_pos - 1);
		des_block.tuple_list.emplace_back(this->block_schema, temp);
		move(tuple_list.begin() + split_pos, tuple_list.end(), back_inserter(des_block.tuple_list));
	} else {
		move(tuple_list.begin() + split_pos, tuple_list.end(), back_inserter(des_block.tuple_list));
	}
}

BlockSpecifier Block::parentPosition() const {
	vector<string> attrs;
	attrs.push_back("file_name");
	attrs.push_back("page_number");
	auto temp = tuple_list[0].valueList(attrs);
	return BlockSpecifier(temp[0].string_val, temp[1].int_val);
}

BlockSpecifier Block::previousPosition() const {
	vector<string> attrs;
	attrs.push_back("file_name");
	attrs.push_back("page_number");
	auto temp = tuple_list[1].valueList(attrs);
	return BlockSpecifier(temp[0].string_val, temp[1].int_val);
}

BlockSpecifier Block::findPosition(const string& src_attr_name, const Tuple& src_tuple) const {
	auto temp_pos = upper_bound(tuple_list.begin() + 2, tuple_list.end(), src_tuple);
	vector<string> attrs;
	attrs.push_back("file_name");
	attrs.push_back("page_number");
	auto temp = (temp_pos-1)->valueList(attrs);
	return BlockSpecifier(temp[0].string_val, temp[1].int_val);
}

bool Block::holdRoot() const {
	return (tuple_list[0].tuple_vals.find("file_name")->second.toString() == ROOT_PARENT_FILE_NAME);
}

Tuple Block::goingUpTuple() const {
	vector<Item> temp;
	temp.push_back(tuple_list[2].tuple_vals.find("key")->second);
	temp.emplace_back(this->specifier.schemaName());
	temp.emplace_back(this->specifier.pageNumber());
	temp.emplace_back(INVALID_TUPLE_NUMBER);
	return Tuple(block_schema, temp);
}

void Block::commitDad(const BlockSpecifier& src_specifier) {
	dirty = 1;
	vector<Item> temp;
	temp.push_back(tuple_list[0].tuple_vals.find("key")->second);
	temp.emplace_back(src_specifier.schemaName());
	temp.emplace_back(src_specifier.pageNumber());
	temp.emplace_back(INVALID_TUPLE_NUMBER);
	tuple_list[0] = Tuple(block_schema, temp);
}
