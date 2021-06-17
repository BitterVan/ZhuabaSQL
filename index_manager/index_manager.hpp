#include "../record_manager/record_manager.hpp"
#include "../catalog_manager/catalog_manager.hpp"
using namespace std;

class IndexManager {
	private:
		BufferPool& buffer_pool;
		RecordManager& record_manager;
		CatalogManager& catalog_manager;

	public:
		IndexManager(BufferPool&, CatalogManager&, RecordManager&);
		~IndexManager();

};