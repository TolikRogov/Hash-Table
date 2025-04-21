#include "HashTable.hpp"

HashTableStatusCode BufferCtor(Buffer* buffer) {
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

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode BucketsCtor(Buffer* buffer, Bucket_t* buckets) {

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	for (size_t i = 0; i < buffer->size; i++) {
		if (buffer->data[i] == '\n') {
			buffer->data[i] = '\0';
			buckets->size++;
		}
	}

	buffer->words_cnt = buckets->size;
	buckets->size /= LOAD_FACTOR;
	buckets->lists = (List_t*)calloc(buckets->size, sizeof(List_t));
	if (!buckets->lists)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	for (size_t i = 0; i < buckets->size; i++)
		LIST_CTOR((buckets->lists + i));

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode BucketsUploader(Buffer* buffer, Bucket_t* buckets) {

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	size_t shift = 0;
	for (size_t i = 0; i < buffer->words_cnt; i++) {
		char* word = buffer->data + shift;
		size_t hash = DJB2Hash(word);
		List_t* list = &buckets->lists[hash % buckets->size];
		LIST_ADD(list, word);
		shift += strlen(word) + 1;
	}

#ifdef DEBUG
	BUCKETS_PRINT(buckets);
#endif

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode BucketsPrint(Bucket_t* buckets) {

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	for (size_t i = 0; i < buckets->size; i++) {
		printf(YELLOW("Bucket number: %zu")"\n", i);
		LIST_PRINT((buckets->lists + i));
	}

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode BucketsDtor(Bucket_t* buckets) {

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	if (!buckets)
		HASHTABLE_ERROR_CHECK(HASHTABLE_NULL_POINTER);

	if (!buckets->lists)
		return HASHTABLE_NO_ERROR;

	for (size_t i = 0; i < buckets->size; i++)
		LIST_DTOR(buckets->lists[i].head);

	free(buckets->lists);
	buckets->lists = NULL;

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode BufferDtor(Buffer* buffer) {

	if (!buffer)
		HASHTABLE_ERROR_CHECK(HASHTABLE_NULL_POINTER);

	if (buffer->data) {
		free(buffer->data);
		buffer->data = NULL;
	}

	return HASHTABLE_NO_ERROR;
}
