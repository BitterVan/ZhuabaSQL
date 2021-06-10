#include "block.hpp"
using namespace	std;

int max(int a, int b) {
	if (a < b) {
		return b;
	} else {
		return a;
	}
}

void Block::writeBack() const {
	ofstream write_file(specifier.schemaName());
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
		memcpy(pos, tuple_list[i].toBit().begin(), block_schema.bitLength());
		pos += block_schema.bitLength();
	}
	write_file.write(page, BLOCK_SIZE);
}

void Block::clean() {
	dirty = 0;
}

Block::Block(const Schema& src_schema, const BlockSpecifier& src_block_spec) : block_schema(src_schema), specifier(src_block_spec) {
	capacity = max(REAL_SIZE / block_schema.bitLength(), MAX_CAPACITY);
	pinned = 0;
	dirty = 0;
	char page[BLOCK_SIZE];
	ifstream init_file(src_block_spec.schemaName());
	init_file.seekg(src_block_spec.pageNumber() * BLOCK_SIZE, ios::beg);
	init_file.read(page, BLOCK_SIZE);

	char valid[MAX_CAPACITY];
	char data[REAL_SIZE];
	memcpy(valid, page, MAX_CAPACITY);
	memcpy(data, page + MAX_CAPACITY, REAL_SIZE);

	BitStream temp(block_schema.bitLength());
	for (int i = 0; i < capacity; i++) {
		if ((int)valid[i] > 0) {
			memcpy(temp.begin(), data + i*block_schema.bitLength(), block_schema.bitLength());
			tuple_list.push_back(Tuple(block_schema, temp));
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

void Block::insertTuple(const Tuple& src_tuple) {
	dirty = 1;
	tuple_list.push_back(src_tuple);
}

void Block::deleteTuple(const vector<Requirement>& src_requirements) {
	dirty = 1;
	for (auto i = tuple_list.begin(); i != tuple_list.end(); i++) {
		if (i->meetRequirement(src_requirements)) {
			i = tuple_list.erase(i) - 1;
		}
	}
}

void Block::pin() {
	pinned = 1;
}

void Block::unpin() {
	pinned = 0;
}