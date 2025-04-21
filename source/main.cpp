#include "HashTable.hpp"

int main(int argc, char* argv[]) {

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	if (argc > 1 && !strcmp(argv[1], _REWORK_KEY)) {
		DATA_FILE_REWORK();
		return HASHTABLE_NO_ERROR;
	}

	Buffer buffer = {};
	Bucket_t buckets = {};

	BUFFER_CTOR(&buffer);
	BUCKETS_CTOR(&buffer, &buckets);

	BUCKETS_UPLOADER(&buffer, &buckets);

	BUCKETS_DTOR(&buckets);
	BUFFER_DTOR(&buffer);

	return 0;
}
