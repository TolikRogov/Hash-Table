#include "HashTable.hpp"

int main(int argc, char* argv[]) {
	clock_t start_t = clock();

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	if (argc > 1 && !strcmp(argv[1], _REWORK_KEY)) {
		DATA_FILE_REWORK();
		return HASHTABLE_NO_ERROR;
	}

	const char* word = "1234";
	u_int32_t value = *(const u_int32_t*)word;
	u_int32_t crc = CRC32_INIT_CRC;
	printf("0x%X\n", crc32(crc, value));

	crc = CRC32_INIT_CRC;
	crc = _mm_crc32_u32(crc, value);
	crc ^= 0xFFFFFFFF;
	printf("0x%X\n", crc);
	return 0;

	Buffer buffer = {};
	Bucket_t buckets = {};

	BUFFER_CTOR(&buffer);
	BUCKETS_CTOR(&buffer, &buckets);

	BUCKETS_UPLOADER(&buffer, &buckets);
	BucketsFinder(&buffer, &buckets);

	BUCKETS_DTOR(&buckets);
	BUFFER_DTOR(&buffer);

	printf("PROGRAM TIME: %lg sec\n", (double)(clock() - start_t) / CLOCKS_PER_SEC);

	return 0;
}
