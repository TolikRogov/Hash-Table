#include "HashFunctions.hpp"

const char* GetHashFunctionName(HashFunction hash_func_num) {
	switch (hash_func_num) {
		case HASH_STRLEN:				return "Strlen";
		case HASH_MURMUR1:				return "MURMUR1";
		case HASH_ASCIIsum:				return "ASCIIsum";
		case HASH_CRC32:				return "CRC32";
		case HASH_CRC32_INTRINSIC:		return "CRC32Intrinsic";
		case HASH_CRC32_ASM:			return "CRC32ASM";
		default: 						return NULL;
	}
}

hash_t StrlenHash(const void* bytes, const size_t size_in_bytes) {
	return (hash_t)size_in_bytes;
}

hash_t MurMur1Hash(const void* bytes, const size_t size_in_bytes) {

    hash_t hash = MURMUR1_SEED ^ ((hash_t)size_in_bytes * MURMUR1_CONST);
    const u_char* data = (const u_char*)bytes;

	size_t size = size_in_bytes;
    while (size >= 4) {
        u_int32_t k = *(u_int32_t*)data;

        k *= MURMUR1_CONST;
        k ^= k >> MURMUR1_ROTATION;
        k *= MURMUR1_CONST;

        hash *= MURMUR1_CONST;
        hash ^= k;

        data += 4;
        size -= 4;
    }

    switch(size) {
        case 3: hash ^= (hash_t)(data[2] << 16);
        case 2: hash ^= (hash_t)(data[1] << 8);
        case 1: hash ^= (hash_t)(data[0]);
                hash *= MURMUR1_CONST;
		default: break;
    };

    hash ^= hash >> 13;
    hash *= MURMUR1_CONST;
    hash ^= hash >> 15;

    return hash;
}

hash_t ASCIIsumHash(const void* bytes, const size_t size_in_bytes) {

	hash_t hash = 0;
	const u_char* byte = (const u_char*)bytes;

	for (size_t i = 0; i < size_in_bytes; i++)
		hash += *byte++;

	return hash;
}

hash_t crc32IntrinsicHash(const void* bytes, const size_t size_in_bytes) {

    hash_t crc = CRC32_INIT_CRC;
	const u_char* byte = (const u_char*)bytes;

	for (size_t i = 0; i < size_in_bytes; i++)
		crc = _mm_crc32_u8(crc, *byte++);

    return crc ^ 0xFFFFFFFF;
}

HashTableStatusCode crc32HashGentable(hash_t* table) {

	hash_t crc = 0;

	for (hash_t byte_value = 0; byte_value < CRC32_TABLE_SIZE; byte_value++) {
		crc = byte_value;

		for (size_t bit_value = 0; bit_value < CHAR_BIT; bit_value++) {
			if (crc & 1)
				crc = (crc >> 1) ^ CRC32_POLYNOMIAL;
			else
				crc >>= 1;
		}
		table[byte_value] = crc;
	}

	return HASHTABLE_NO_ERROR;
}

hash_t crc32Hash(const void* bytes, const size_t size_in_bytes) {

	hash_t crc = CRC32_INIT_CRC;
	const u_char* u_byte = (const u_char*)bytes;
	extern hash_t crc32_table[CRC32_TABLE_SIZE];

	for (size_t i = 0; i < size_in_bytes && *u_byte != '\0'; i++)
		crc = (crc >> 8) ^ crc32_table[(crc ^ *(const hash_t*)(u_byte++)) & 0xFF];

	return crc ^ 0xFFFFFFFF;
}

const char* GetHashFunctionDataFileName(HashFunction hash_func_num) {
	switch (hash_func_num) {
		case HASH_STRLEN: 				return _DIR_DUMP_DATA "Strlen."			 _FILE_DATA_FORMAT;
		case HASH_MURMUR1:				return _DIR_DUMP_DATA "MURMUR1."		 _FILE_DATA_FORMAT;
		case HASH_ASCIIsum:				return _DIR_DUMP_DATA "ASCIIsum."		 _FILE_DATA_FORMAT;
		case HASH_CRC32:				return _DIR_DUMP_DATA "CRC32." 		 	 _FILE_DATA_FORMAT;
		case HASH_CRC32_INTRINSIC:		return _DIR_DUMP_DATA "CRC32Intrinsic."  _FILE_DATA_FORMAT;
		case HASH_CRC32_ASM:			return _DIR_DUMP_DATA "CRC32ASM."		 _FILE_DATA_FORMAT;
		default: 						return NULL;
	}
}
