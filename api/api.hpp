#pragma once
#include "../record_manager/record_manager.hpp"
#include "../catalog_manager/catalog_manager.hpp"
#include "../ui/ui.hpp"
#include "../index_manager/index_manager.hpp"
#include <vector>
using namespace std;

// The programming interface
// Declared by Wang Yiming
// Modified by BitteVan
class API {
	private:
		CatalogManager& catalog_manager;
		IndexManager& index_manager;
		RecordManager& record_manager;
		UI& ui;

	public:
		API(CatalogManager&, IndexManager&, RecordManager&, UI&);
		~API();

		void createTable(const string&, const vector<string>&, const vector<Attribute>&, const string&, const vector<string>&);
		void dropTable(const string&);
		void createIndex(const string&, const string&, const string&);
		void dropIndex(const string&);
		vector<Tuple> selectTuple(const string&, const vector<Requirement>&) const;
		void insertTuple(const string&, const vector<string>&);
		void deleteTuple(const string&, const vector<Requirement>&);
		Type fetchType(const string&, const string&) const;
		string fetchStatement() const;
		vector<string> fetchFile(const string&) const;
};