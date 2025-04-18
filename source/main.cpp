#include "HashTable.hpp"

int main() {

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	Buffer buffer = {};
	Bucket_t buckets = {};

	ht_status = BucketsCtor(&buffer, &buckets);
	HASHTABLE_ERROR_CHECK(ht_status);

	printf("Buckets size: %zu\n", buckets.size);
	printf("Buffer start: %s\n", buffer.data);



	ht_status = BucketsDtor(&buffer, &buckets);
	HASHTABLE_ERROR_CHECK(ht_status);

	return 0;
}
