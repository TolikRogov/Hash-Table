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

	for (size_t i = 0; i < buffer->size; i++) {
		if (buffer->data[i] == '\n') {
			buffer->data[i] = '\0';
			buffer->words_cnt++;
		}
	}

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode BucketsCtor(Buffer* buffer, Bucket_t* buckets) {

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	buckets->size = AMOUNT_UNIQUE_WORDS / LOAD_FACTOR;
	buckets->lists = (List_t*)calloc(buckets->size, sizeof(List_t));
	if (!buckets->lists)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	for (size_t i = 0; i < buckets->size; i++)
		LIST_CTOR((buckets->lists + i));

	return HASHTABLE_NO_ERROR;
}

List_t* FindListForWord(Buffer* buffer, Bucket_t* buckets, char** word) {
	*word = buffer->data + buffer->shift;
	size_t length = strlen(*word);
	buffer->shift += length + 1;

	size_t hash = crc32Hash(*word, length);
	return buckets->lists + (hash % buckets->size);
}

HashTableStatusCode BucketsUploader(Buffer* buffer, Bucket_t* buckets) {

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	char* word = NULL;

	for (size_t i = 0; i < buffer->words_cnt; i++) {
		List_t* list = FindListForWord(buffer, buckets, &word);
		LIST_ADD(list, word);
	}

	buffer->shift = 0;

#ifdef DEBUG
	BUCKETS_DUMP(buckets);
#endif

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode BucketsFinder(Buffer* buffer, Bucket_t* buckets) {

	for (size_t n = 0; n < FINDER_ITERATIONS; n++) {

		char* word = NULL;

		for (size_t word_num = 0; word_num < buffer->words_cnt; word_num++) {
			List_t* list = FindListForWord(buffer, buckets, &word);
			Data_t* found = ListFindElement(list->head, word);
			if (!found)
				printf(RED("%s was not found")"\n", word);
		}

		buffer->shift = 0;
	}

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode BucketsDump(Bucket_t* buckets) {

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	for (size_t i = 0; i < buckets->size; i++) {
		printf(YELLOW("Bucket number: %zu")"\n", i);
		LIST_PRINT((buckets->lists + i));
	}

	FILE* dump = fopen(_DIR_DUMP _FILE_DUMP, "w");
	if (!dump)
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_OPEN_ERROR);

	fprintf(dump, "import pandas as pd\n"
				  "import matplotlib.pyplot as plt\n"
				  "data = pd.read_csv('%s', sep=' ')\n"
				  "plt.plot(data['x'], data['y'], 'b-', label='Data')\n"
				  "plt.xlabel('Bucket number')\n"
				  "plt.ylabel('Bucket size')\n"
				  "plt.title('Buckets dump')\n"
				  "plt.grid(True)\n"
			      "plt.legend()\n"
				  "plt.savefig('plot.svg', dpi=300)\n"
				  "plt.show()\n", _FILE_DATA);

	if (fclose(dump))
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_CLOSE_ERROR);

	FILE* data = fopen(_DIR_DUMP _FILE_DATA, "w");
	if (!data)
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_OPEN_ERROR);

	fprintf(data, "x y\n");
	for (size_t i = 0; i < buckets->size; i++)
		fprintf(data, "%zu %zu\n", i, buckets->lists[i].size);

	if (fclose(data))
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_CLOSE_ERROR);

	if (system("cd " _DIR_DUMP "; python3 " _FILE_DUMP "; cd ../") == -1)
		HASHTABLE_ERROR_CHECK(HASHTABLE_SYSTEM_SHELL_ERROR);

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
