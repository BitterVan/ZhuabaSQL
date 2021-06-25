sql: utilities/attribute.o utilities/basics.o utilities/bit_stream.o utilities/block_specifier.o utilities/block.o utilities/item.o utilities/requirement.o utilities/schema.o utilities/tuple_spec.o utilities/tuple.o ui/ui.o record_manager/record_manager.o catalog_manager/catalog_manager.o index_manager/index_manager.o interpreter/interpreter.o api/api.o record_manager/record_manager.o buffer_pool/buffer_pool.o exception/exception.o
	g++ -o sql $^

utilities/basics.o : utilities/basics.cpp utilities/basics.hpp
	g++ utilities/basics.cpp -c utilities/basics.o