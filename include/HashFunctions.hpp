#pragma once

#include "HashTable_utilities.hpp"

typedef u_int64_t hash_t;
typedef hash_t (*hash_func_t) (const void* bytes, size_t size_in_bytes);

const hash_t CRC32_POLYNOMIAL 	= 0xEDB88320;
const hash_t CRC32_INIT_CRC 	= 0xFFFFFFFF;
const size_t CRC32_TABLE_SIZE	= UCHAR_MAX + 1;

const hash_t MURMUR1_SEED 		= 0;
const hash_t MURMUR1_CONST		= 0xC6A4A793;
const hash_t MURMUR1_ROTATION   = 16;

enum HashFunction {
	HASH_STRLEN,
	HASH_ASCIIsum,
	HASH_MURMUR1,
	HASH_CRC32,
	HASH_CRC32_INTRINSIC,
	HASH_CRC32_ASM,
};

extern "C" hash_t crc32Asm(const void*, const size_t);

const char* GetHashFunctionName(HashFunction hash_func_name);
const char* GetHashFunctionDataFileName(HashFunction hash_func_num);

HashTableStatusCode crc32HashGentable(hash_t* table);
hash_t crc32Hash(const void* bytes, const size_t size_in_bytes);
hash_t crc32IntrinsicHash(const void* bytes, const size_t size_in_bytes);
hash_t ASCIIsumHash(const void* bytes, const size_t size_in_bytes);
hash_t MurMur1Hash(const void* bytes, const size_t size_in_bytes);
hash_t StrlenHash(const void* bytes, const size_t size_in_bytes);
