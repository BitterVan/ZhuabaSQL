#include "../record_manager/record_manager.hpp"
#include "../catalog_manager/catalog_manager.hpp"
using namespace std;

class IndexManager {
	private:
		BufferPool& buffer_pool;
		RecordManager& record_manager;
		CatalogManager& catalog_manager;
		map<string, string> index_map;
		map<string, int> root_position;

	public:
		IndexManager(BufferPool&, CatalogManager&, RecordManager&);
		~IndexManager();
		bool holdIndex(const string&, const string&) const;
		void createIndex(const string&, const string&, const string&);
		void insertIndex(const Item&, const TupleSpecifier&);
};