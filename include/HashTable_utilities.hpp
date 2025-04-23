#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <nmmintrin.h>

const u_int32_t CRC32_POLYNOMIAL 	= 0xEDB88320;
const u_int32_t CRC32_INIT_CRC 		= 0xFFFFFFFF;

#define _DIR_DUMP		"Dump/"
#define _FILE_DUMP		"dump.py"
#define _FILE_DATA		"data.dat"

#define _DIR_DATA 		"data/"
#define _FILE_INPUT 	_DIR_DATA "input.txt"
#define _FILE_REWORK 	_DIR_DATA "rework.txt"

#define _LINE			"----------------------------------------"
#define _REWORK_KEY		"--rework"

#define RED(str) 		"\033[31;1m" str "\033[0m"
#define YELLOW(str) 	"\033[33;4m" str "\033[0m"
#define GREEN(str) 		"\033[32;1m" str "\033[0m"
#define BLUE(str)		"\033[34;1m" str "\033[0m"
#define TEAL(str)		"\033[36;1m" str "\033[0m"

#define DATA_FILE_REWORK() {					 \
	ht_status = DataFileRework();				\
	HASHTABLE_ERROR_CHECK(ht_status);			\
}												\

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
	HASHTABLE_FILE_READ_ERROR,
	HASHTABLE_SYSTEM_SHELL_ERROR,
};

const wchar_t* HashTableErrorsMessenger(HashTableStatusCode status);
size_t DJB2Hash(const char* string);
u_int32_t crc32(u_int32_t crc, u_int32_t val);
HashTableStatusCode DataFileRework();
