#include "HashTable_utilities.hpp"

const wchar_t* HashTableErrorsMessenger(HashTableStatusCode status) {
	switch (status) {
		case HASHTABLE_NO_ERROR:							return L"HASHTABLE ERROR - NO ERROR";
		case HASHTABLE_ALLOCATION_ERROR:					return L"HASHTABLE ERROR - MEMORY WAS ALLOCATED WITH ERROR";
		case HASHTABLE_NULL_POINTER:						return L"HASHTABLE ERROR - NULL POINTER";
		case HASHTABLE_UNDEFINED_ERROR:						return L"HASHTABLE ERROR - ERROR IS UNDEFINED, SORRY!";
		case HASHTABLE_FILE_OPEN_ERROR:						return L"HASHTABLE ERROR - FILE WAS NOT OPENED";
		case HASHTABLE_FILE_CLOSE_ERROR:					return L"HASHTABLE ERROR - FILE WAS NOT CLOSED";
		case HASHTABLE_FILE_READ_ERROR:						return L"HASHTABLE ERROR - FILE READING ENDED WITH ERROR";
		case HASHTABLE_SYSTEM_SHELL_ERROR:					return L"HASHTABLE ERROR - CREATE SHELL COMMAND ENDED WITH ERROR";
		default: 											return L"UNDEFINED ERROR";
	}
}

size_t DJB2Hash(const char* string) {

	size_t hash = 5381;

	for (size_t i = 0; string[i] != '\0'; i++)
		hash = hash * 33 ^ (size_t)string[i];

	return hash;
}

HashTableStatusCode DataFileRework() {
	struct stat file_stat = {};
	stat(_FILE_INPUT, &file_stat);
	size_t buf_size = (size_t)file_stat.st_size;

	char* buffer = (char*)calloc(buf_size, sizeof(buffer[0]));
	if (!buffer)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	FILE* input = fopen(_FILE_INPUT, "r");
	if (!input)
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_OPEN_ERROR);

	if (fread(buffer, sizeof(buffer[0]), buf_size, input) != buf_size)
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_READ_ERROR);

	if (fclose(input))
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_CLOSE_ERROR);

	FILE* output = fopen(_FILE_REWORK, "w");
	if (!output)
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_OPEN_ERROR);

	size_t start_index = 0, end_index = 0;
	while(end_index < buf_size) {
		while ((isdigit(buffer[end_index]) || isalpha(buffer[end_index]) || buffer[end_index] == '-')) end_index++;
		if (end_index == start_index) { start_index = ++end_index; continue; }
		buffer[end_index] = '\0';
		fprintf(output, "%s\n", buffer + start_index);
		start_index = ++end_index;
	}

	if (fclose(output))
		HASHTABLE_ERROR_CHECK(HASHTABLE_FILE_CLOSE_ERROR);

	return HASHTABLE_NO_ERROR;
}
