#include "HashTable.hpp"

hash_t crc32_table[CRC32_TABLE_SIZE] = {};

HashTableStatusCode BufferCtor(Buffer* buffer) {
	struct stat file_stat = {};
	stat(_FILE_REWORK, &file_stat);
	buffer->size = (size_t)file_stat.st_size;

	buffer->data = (char*)aligned_alloc(ALIGNMENT_COUNT, (buffer->size + ALIGNMENT_COUNT - (buffer->size % ALIGNMENT_COUNT)) * sizeof(char));
	if (!buffer->data)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	FILE* rework = fopen(_FILE_REWORK, "r" _FILE_OPEN_MODE);
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
			#ifndef BASE
				i = ALIGNMENT_COUNT * (buffer->words_cnt + 1) - 1;
			#endif
		}
	}

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode BucketsCtor(Bucket_t* buckets) {

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	buckets->size = OPTIMIZE_BUCKETS_SIZE;
	buckets->lists = (List_t*)calloc(buckets->size, sizeof(List_t));
	if (!buckets->lists)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	for (size_t i = 0; i < buckets->size; i++)
		LIST_CTOR((buckets->lists + i));

	return HASHTABLE_NO_ERROR;
}

#ifdef BASE
	List_t* FindListForWord(Buffer* buffer, Bucket_t* buckets, char** word) {
		*word = buffer->data + buffer->shift;
		size_t length = strlen(*word);
		size_t hash = buckets->hash_function(*word, length);

		buffer->shift += length + 1;
		return buckets->lists + (hash % buckets->size);
	}
#else
	List_t* FindListForWord(Buffer* buffer, Bucket_t* buckets, char** word) {
		List_t* list = NULL;
		asm volatile (
			 "mov 0x8(%[buffer]), %%r12\n"								//r12 = buffer->data
			 "add 0x18(%[buffer]), %%r12\n"								//r12 += buffer->shift
			 "mov %%r12, (%[word])\n"									//*word = r12;

			 "mov %[buckets], %%rbx\n"									//rbx = buckets
			 "mov $0x20, %%rsi\n"										//rsi = length
			 "mov %%r12, %%rdi\n"
			 "call crc32Asm\n"											//
			 "mov %%rax, %%r8\n"										//hash = buckets->hash_function(*word, length)

			 "mov $0x20, %%rcx\n"										//rcx = ALIGNMENT_COUNT
			 "add %%rcx, 0x18(%[buffer])\n"								//buffer->shift += rcx

			 "and $0x3ff, %%r8\n"										//hash &= 1024 - 1 | hash %= buckets->size
			 "mov 0x8(%%rbx), %%rax\n"									//rax = buckets->lists
			 "lea (%%r8, %%r8, 2), %%r8\n"								//r8 = 3 * r8
			 "lea (%%rax, %%r8, 8), %%rax\n"							//rax = rax + 8 * r8 | buckets->lists + (hash % buckets->size)
			 "mov %%rax, %[list]\n"										//list = rax

			 : [list] 		"=r" (list),								//output parameter
			   "+o" (word)												//input and output parameter
			 : [buffer] 	"r" (buffer),								//input parameters
			   [buckets] 	"r" (buckets),
			   [word] 		"r" (word)
			 : "%r12", "%rbx", "%rsi", "%rdi", "%r8",					//changed registers
			   "%rax", "%rcx", "%r11", "memory"
		);
		return list;
	}
#endif

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

	clock_t start_t = clock();

	for (size_t n = 0; n < FINDER_ITERATIONS; n++) {

		char* word = NULL;

		for (size_t word_num = 0; word_num < buffer->words_cnt; word_num++) {
			List_t* list = FindListForWord(buffer, buckets, &word);
#ifdef BASE
			Data_t* found = ListFindElement(list->head, word);
#else
			List_key_t key = _mm256_load_si256((List_key_t*)word);
			Data_t* found = ListFindElement(list->head, &key);
#endif
			if (!found)
				printf(RED("'%s' was not found")"\n", word);
		}

		buffer->shift = 0;
	}

	printf("FINDER TIME: %lg sec\n", (double)(clock() - start_t) / CLOCKS_PER_SEC);

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
				  "import numpy as np\n"
				  "data = pd.read_csv('%s%s.%s', sep=' ')\n"
				  "fig = plt.figure(figsize=(20, 10))\n"
				  "ax = fig.add_subplot()\n"
				  "ax.patch.set_facecolor('#addaed')\n"
				  "ax.patch.set_alpha(0.2)\n"
				  "ax.bar(data['x'], data['y'], width = 2, color = '#82b2fa')\n"
				  "ax.set_title('%s dump', fontsize=24)\n"
				  "ax.set_xlabel('Bucket number', fontsize=20)\n"
			      "ax.set_ylabel('Bucket size', fontsize=20)\n"
				  "fig.text(0.11, 0.05, f\"Dispersion: {np.var(data['y']):.2f}\", fontsize=20)\n"
				  "plt.savefig('%s%s.%s', dpi=300)\n"
				  "plt.show()\n",
				  _DIR_DUMP_DATA, hash_func_name, _FILE_DATA_FORMAT,
				  hash_func_name,
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

HashTableStatusCode ChooseHashFunction(const char* cmd_key, Bucket_t* buckets) {

	if (!strcmp(cmd_key, _CRC32HASH_KEY)) {
		crc32HashGentable(crc32_table);
		buckets->hash_function = crc32Hash;
		buckets->hash_func_num = HASH_CRC32;
	}
	else if (!strcmp(cmd_key, _STRLEN_HASH_KEY)) {
		buckets->hash_function = StrlenHash;
		buckets->hash_func_num = HASH_STRLEN;
	}
	else if (!strcmp(cmd_key, _MURMUR1_HASH_KEY)) {
		buckets->hash_function = MurMur1Hash;
		buckets->hash_func_num = HASH_MURMUR1;
	}
	else if (!strcmp(cmd_key, _ASCIIsum_HASH_KEY)) {
		buckets->hash_function = ASCIIsumHash;
		buckets->hash_func_num = HASH_ASCIIsum;
	}
	else if (!strcmp(cmd_key, _CRC32_INTRINSIC_HASH_KEY)) {
		buckets->hash_function = crc32IntrinsicHash;
		buckets->hash_func_num = HASH_CRC32_INTRINSIC;
	}
	else if (!strcmp(cmd_key, _CRC32_ASM_HASH_KEY)) {
		buckets->hash_function = crc32Asm;
		buckets->hash_func_num = HASH_CRC32_ASM;
	}
	else
		HASHTABLE_ERROR_CHECK(HASHTABLE_COMPILE_KEYS_ERROR);

	return HASHTABLE_NO_ERROR;
}
