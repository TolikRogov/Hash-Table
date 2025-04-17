#include "HashTable_utilities.hpp"

const wchar_t* HashTableErrorsMessenger(HashTableStatusCode status) {
	switch (status) {
		case HASHTABLE_NO_ERROR:							return L"HASHTABLE ERROR - NO ERROR";
		case HASHTABLE_ALLOCATION_ERROR:					return L"HASHTABLE ERROR - MEMORY WAS ALLOCATED WITH ERROR";
		case HASHTABLE_NULL_POINTER:						return L"HASHTABLE ERROR - NULL POINTER";
		case HASHTABLE_UNDEFINED_ERROR:						return L"HASHTABLE ERROR - ERROR IS UNDEFINED, SORRY!";
		case HASHTABLE_FILE_OPEN_ERROR:						return L"HASHTABLE ERROR - FILE WAS NOT OPENED";
		case HASHTABLE_FILE_CLOSE_ERROR:					return L"HASHTABLE ERROR - FILE WAS NOT CLOSED";
		case HASHTABLE_STREAM_BUFFER_ERROR:					return L"HASHTABLE ERROR - STREAM BUFFERING FAILED";
		default: 											return L"UNDEFINED ERROR";
	}
}

HashTableStatusCode DataFileRework() {
	struct stat file_stat = {};
	stat(_FILE_LOTR, &file_stat);
	char* buffer = (char*)calloc((size_t)file_stat.st_size, sizeof(char));
	if (!buffer)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	FILE* input = fopen(_FILE_LOTR, "r");
	if (!input)
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_OPEN_ERROR);

	// if (setvbuf(input, buffer, _IOFBF, (size_t)file_stat.st_size))
	// 	HASHTABLE_ERROR_CHECK(HASHTABLE_STREAM_BUFFER_ERROR);

	FILE* output = fopen(_FILE_REWORK, "w");
	if (!output)
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_OPEN_ERROR);

	size_t index = 0;
	while (buffer[index] != ' ' && index < (size_t)file_stat.st_size) index++;
	buffer[index] = '\0';
	printf("%s\n", buffer);

	if (fclose(input))
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_CLOSE_ERROR);

	if (fclose(output))
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_CLOSE_ERROR);

	return HASHTABLE_NO_ERROR;
}
