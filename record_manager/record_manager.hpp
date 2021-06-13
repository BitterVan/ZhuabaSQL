#include "../buffer_pool/buffer_pool.hpp"
using namespace std;

class RecordManager {
	private:
		BufferPool& buffer_pool;
	public:
		RecordManager() = delete;
		RecordManager(const RecordManager&);
		RecordManager(BufferPool&);
		~RecordManager();

		void insertTuple(const string&, const vector<string>&);
		void deleteTuple(const string&, const vector<Requirement>&);
		vector<Tuple> selectTuple(const string&, const vector<Requirement>&) const;
};