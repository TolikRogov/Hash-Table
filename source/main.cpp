#include "HashTable.hpp"

int main(int argc, char* argv[]) {

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	if (argc < 2)
		HASHTABLE_ERROR_CHECK(HASHTABLE_COMPILE_KEYS_ERROR);

	if (!strcmp(argv[1], _REWORK_KEY)) {
		DATA_FILE_REWORK();
		return HASHTABLE_NO_ERROR;
	}

	Buffer buffer = {};
	Bucket_t buckets = {};

	CHOOSE_HASH_FUNCTION(argv[1], &buckets);

	BUFFER_CTOR(&buffer);
	BUCKETS_CTOR(&buckets);

	BUCKETS_UPLOADER(&buffer, &buckets);
	BucketsFinder(&buffer, &buckets);

	BUCKETS_DTOR(&buckets);
	BUFFER_DTOR(&buffer);

	return HASHTABLE_NO_ERROR;
}
