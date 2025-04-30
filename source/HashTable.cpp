#include "HashTable.hpp"

HashTableStatusCode BufferCtor(Buffer* buffer) {
	struct stat file_stat = {};
	stat(_FILE_REWORK, &file_stat);
	buffer->size = (size_t)file_stat.st_size;

	buffer->data = (char*)aligned_alloc(ALIGNMENT_COUNT, buffer->size * sizeof(char));
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
#ifdef BASE
		if (buffer->data[i] == '\n') {
			buffer->data[i] = '\0';
			buffer->words_cnt++;
		}
#else
		if (buffer->data[i] == ' ') {
			buffer->data[i++] = '\0';
			while (i < buffer->size && (buffer->data[i] == ' ' || buffer->data[i] == '\n')) {buffer->data[i++] = '\0';}
			buffer->words_cnt++;
		}
#endif
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
	buffer->shift += length;

#ifdef BASE
	buffer->shift++;
#else
	while (buffer->shift < buffer->size && buffer->data[buffer->shift] == '\0') buffer->shift++;
#endif

	size_t hash = buckets->hash_function(*word, length);
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
				printf(RED("'%s' was not found")"\n", word);
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

	FILE* dump = fopen(_FILE_DUMP, "w");
	if (!dump)
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_OPEN_ERROR);

	const char* hash_func_name = GetHashFunctionName(buckets->hash_func_num);
	fprintf(dump, "import pandas as pd\n"
				  "import matplotlib.pyplot as plt\n"
				  "data = pd.read_csv('%s%s.%s', sep=' ')\n"
				  "fig = plt.figure(figsize=(20, 10))\n"
				  "ax = fig.add_subplot()\n"
				  "ax.patch.set_facecolor('#addaed')\n"
				  "ax.patch.set_alpha(0.2)\n"
				  "ax.bar(data['x'], data['y'], width = 2, color = '#82b2fa')\n"
				  "ax.set_title('Buckets dump')\n"
				  "ax.set_xlabel('Bucket number')\n"
			      "ax.set_ylabel('Bucket size')\n"
				  "plt.savefig('%s%s.%s', dpi=300)\n"
				  "plt.show()\n",
				  _DIR_DUMP_DATA, hash_func_name, _FILE_DATA_FORMAT,
				  _DIR_DUMP_IMG, hash_func_name, _IMG_DUMP_FORMAT);

	if (fclose(dump))
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_CLOSE_ERROR);

	FILE* data = fopen(GetHashFunctionDataFileName(buckets->hash_func_num), "w");
	if (!data)
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_OPEN_ERROR);

	fprintf(data, "x y\n");
	for (size_t i = 0; i < buckets->size; i++)
		fprintf(data, "%zu %zu\n", i, buckets->lists[i].size);

	if (fclose(data))
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_CLOSE_ERROR);

	if (system("python3 " _FILE_DUMP) == -1)
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
