#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <nmmintrin.h>
#include <immintrin.h>
#include <stdint.h>

#define _DIR_IMG					"img/"
#define _DIR_DATA 					"data/"

#define _DIR_DUMP					"Dump/"
#define _FILE_DUMP					_DIR_DUMP "dump.py"
#define _DIR_DUMP_DATA				_DIR_DUMP _DIR_DATA
#define _DIR_DUMP_IMG				_DIR_DUMP _DIR_IMG

#define _FILE_DATA_FORMAT			"dat"
#define _IMG_DUMP_FORMAT			"svg"

#define _FILE_INPUT 				_DIR_DATA "input.txt"
#define _FILE_REWORK 				_DIR_DATA "rework.txt"

#define _REWORK_KEY					"--rework"
#define _DJB2HASH_KEY				"--djb2"
#define _CRC32HASH_KEY				"--crc32"
#define _CRC32_INTRINSIC_HASH_KEY	"--crc32_intrinsic"
#define _CRC32_ASM_HASH_KEY			"--crc32_asm"

#ifdef BASE
	#define _END_SYMBOL '\n'
#else
	#define _END_SYMBOL ' '
#endif

#define _LINE			"----------------------------------------"
#define RED(str) 		"\033[31;1m" str "\033[0m"
#define YELLOW(str) 	"\033[33;4m" str "\033[0m"
#define GREEN(str) 		"\033[32;1m" str "\033[0m"
#define BLUE(str)		"\033[34;1m" str "\033[0m"
#define TEAL(str)		"\033[36;1m" str "\033[0m"

const int ALIGNMENT_COUNT = 32;

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
	HASHTABLE_COMPILE_KEYS_ERROR,
};

const wchar_t* HashTableErrorsMessenger(HashTableStatusCode status);
HashTableStatusCode ReadFromInputFile(char* buffer, size_t buf_size);
HashTableStatusCode DataFileRework();
