#include "HashTable.hpp"

hash_t crc32_table[CRC32_TABLE_SIZE] = {};

int main(int argc, char* argv[]) {
	clock_t start_t = clock();

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	if (argc < 2)
		HASHTABLE_ERROR_CHECK(HASHTABLE_COMPILE_KEYS_ERROR);

	Buffer buffer = {};
	Bucket_t buckets = {};

	if (!strcmp(argv[1], _REWORK_KEY)) {
		DATA_FILE_REWORK();
		return HASHTABLE_NO_ERROR;
	}
	if (!strcmp(argv[1], _DJB2HASH_KEY)) {
		buckets.hash_function = DJB2Hash;
		buckets.hash_func_num = HASH_DJB2;
	}
	else if (!strcmp(argv[1], _CRC32HASH_KEY)) {
		crc32HashGentable(crc32_table);
		buckets.hash_function = crc32Hash;
		buckets.hash_func_num = HASH_CRC32;
	}
	else if (!strcmp(argv[1], _CRC32_INTRINSIC_HASH_KEY)) {
		buckets.hash_function = crc32IntrinsicHash;
		buckets.hash_func_num = HASH_CRC32_INTRINSIC;
	}
	else
		HASHTABLE_ERROR_CHECK(HASHTABLE_COMPILE_KEYS_ERROR);

	BUFFER_CTOR(&buffer);
	BUCKETS_CTOR(&buffer, &buckets);

	BUCKETS_UPLOADER(&buffer, &buckets);
	BucketsFinder(&buffer, &buckets);

	BUCKETS_DTOR(&buckets);
	BUFFER_DTOR(&buffer);

	printf("PROGRAM TIME: %lg sec\n", (double)(clock() - start_t) / CLOCKS_PER_SEC);

	return HASHTABLE_NO_ERROR;
}
