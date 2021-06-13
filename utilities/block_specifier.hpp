#pragma once
#include "basics.hpp"
using namespace std;

class BlockSpecifier {
	private:
		string schema_name;
		int page_number;
	public:
		BlockSpecifier(const string&, int);
		BlockSpecifier(const BlockSpecifier&);
		~BlockSpecifier();
		BlockSpecifier() = delete;
		string schemaName() const;
		int pageNumber() const;
		bool operator==(const BlockSpecifier&) const;
	
	// friend size_t block_spec_hash(const BlockSpecifier&);
};

namespace std {
	template<>
		class hash<BlockSpecifier> {
			public:
			size_t operator() (const BlockSpecifier& src_spec) const {
				return hash<string>()(src_spec.schemaName()) ^ hash<int>()(src_spec.pageNumber());
			}
		};
};
// size_t block_spec_hash(const BlockSpecifier&);