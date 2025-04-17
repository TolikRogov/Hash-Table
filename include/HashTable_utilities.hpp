#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define _DIR_DATA 		"data/"
#define _FILE_LOTR 		_DIR_DATA "LOTR.txt"
#define _FILE_REWORK 	_DIR_DATA "output.txt"

#define RED(str) 		"\033[31;1m" str "\033[0m"
#define YELLOW(str) 	"\033[33;4m" str "\033[0m"
#define GREEN(str) 		"\033[32;1m" str "\033[0m"
#define BLUE(str)		"\033[34;1m" str "\033[0m"
#define TEAL(str)		"\033[36;1m" str "\033[0m"

#define HASHTABLE_ERROR_CHECK(status) {																			 \
	if (status != HASHTABLE_NO_ERROR) {																			\
		fprintf(stderr, "\n\n" RED("Error (code %d): %ls, ") YELLOW("File: %s, Function: %s, Line: %d\n\n"),   	\
					status, HashTableErrorsMessenger(status), __FILE__, __PRETTY_FUNCTION__, __LINE__);			\
		fclose(stderr);																							\
		return status;																							\
	}																											\
}

#define HASHTABLE_ERROR_MESSAGE(status) {																		 \
	if (status != HASHTABLE_NO_ERROR) {																			\
		fprintf(stderr, "\n\n" RED("Error (code %d): %ls, ") YELLOW("File: %s, Function: %s, Line: %d\n\n"),   	\
					status, HashTableErrorsMessenger(status), __FILE__, __PRETTY_FUNCTION__, __LINE__);			\
		fclose(stderr);																							\
		break;																									\
	}																											\
}

enum HashTableStatusCode {
	HASHTABLE_NO_ERROR,
	HASHTABLE_ALLOCATION_ERROR,
	HASHTABLE_NULL_POINTER,
	HASHTABLE_UNDEFINED_ERROR,
	HASHTABLE_FILE_OPEN_ERROR,
	HASHTABLE_FILE_CLOSE_ERROR,
	HASHTABLE_STREAM_BUFFER_ERROR,
};

const wchar_t* HashTableErrorsMessenger(HashTableStatusCode status);
HashTableStatusCode DataFileRework();
