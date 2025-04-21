#include "HashTable.hpp"

HashTableStatusCode BucketsCtor(Buffer* buffer, Bucket_t* buckets) {
	struct stat file_stat = {};
	stat(_FILE_REWORK, &file_stat);
	buffer->size = (size_t)file_stat.st_size;

	buffer->data = (char*)calloc(buffer->size, sizeof(char));
	if (!buffer->data)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	FILE* rework = fopen(_FILE_REWORK, "r");
	if (!rework)
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_OPEN_ERROR);

	if (fread(buffer->data, sizeof(char), buffer->size, rework) != buffer->size)
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_READ_ERROR);

	if (fclose(rework))
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_CLOSE_ERROR);

	for (size_t i = 0; i < buffer->size; i++) {
		if (buffer->data[i] == '\n') {
			buffer->data[i] = '\0';
			buckets->size++;
		}
	}

	buckets->lists = (List_t*)calloc(buckets->size, sizeof(List_t));
	if (!buckets->lists)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode BucketsUploader(Buffer* buffer, Bucket_t* buckets) {

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	size_t shift = 0;
	for (size_t i = 0; i < buckets->size; i++) {
		char* word = buffer->data + shift;
		size_t hash = DJB2Hash(word);
		List_t* list = &buckets->lists[hash % buckets->size];
		LIST_CTOR(list);
		LIST_ADD(list, word);
		shift += strlen(word) + 1;
	}

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode BucketsDtor(Buffer* buffer, Bucket_t* buckets) {

	if (buffer && buffer->data) {
		free(buffer->data);
		buffer->data = NULL;
	}

	if (buckets && buckets->lists) {
		free(buckets->lists);
		buckets->lists = NULL;
	}

	return HASHTABLE_NO_ERROR;
}

size_t DJB2Hash(const char* string) {

	size_t hash = 5381;

	for (size_t i = 0; string[i] != '\0'; i++)
		hash = hash * 33 ^ (size_t)string[i];

	return hash;
}
