#include "HashTable.hpp"

int main(int argc, char* argv[]) {
	clock_t start_t = clock();

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	if (argc > 1 && !strcmp(argv[1], _REWORK_KEY)) {
		DATA_FILE_REWORK();
		return HASHTABLE_NO_ERROR;
	}

	hash_t crc32_table[CRC32_TABLE_SIZE] = {};
	crc32_gentable(crc32_table);

	Buffer buffer = {};
	Bucket_t buckets = {.table = crc32_table};

	BUFFER_CTOR(&buffer);
	BUCKETS_CTOR(&buffer, &buckets);

	BUCKETS_UPLOADER(&buffer, &buckets);
	BucketsFinder(&buffer, &buckets);

	BUCKETS_DTOR(&buckets);
	BUFFER_DTOR(&buffer);

	printf("PROGRAM TIME: %lg sec\n", (double)(clock() - start_t) / CLOCKS_PER_SEC);

	return 0;
}
