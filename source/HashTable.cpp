#include "HashTable.hpp"

HashTableStatusCode PrintBucket(Bucket_t* bucket) {

	printf(BLUE("Bucket address: %p")"\n", bucket);
	printf(GREEN("Word: %s")"\n", bucket->word);
	printf(GREEN("Word frequency: %zu")"\n", bucket->frequency);

	return HASHTABLE_NO_ERROR;
}
