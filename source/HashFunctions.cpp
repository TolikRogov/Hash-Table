#include "HashFunctions.hpp"

const char* GetHashFunctionName(HashFunction hash_func_num) {
	switch (hash_func_num) {
		case HASH_DJB2:					return "DJB2";
		case HASH_CRC32:				return "CRC32";
		case HASH_CRC32_INTRINSIC:		return "CRC32Intrinsic";
		default: 						return "Undefined function";
	}
}

const char* GetHashFunctionDataFileName(HashFunction hash_func_num) {
	switch (hash_func_num) {
		case HASH_DJB2:					return _DIR_DUMP_DATA "DJB2."  		 _FILE_DATA_FORMAT;
		case HASH_CRC32:				return _DIR_DUMP_DATA "CRC32." 		 _FILE_DATA_FORMAT;
		case HASH_CRC32_INTRINSIC:		return _DIR_DUMP_DATA "CRC32Intrinsic." _FILE_DATA_FORMAT;
		default: 						return "Undefined function";
	}
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

	for (size_t i = 0; i < size_in_bytes; i++)
		crc = (crc >> 8) ^ crc32_table[(crc ^ *(const hash_t*)(u_byte++)) & 0xFF];

	return crc ^ 0xFFFFFFFF;
}

hash_t DJB2Hash(const void* bytes, const size_t size_in_bytes) {

	hash_t hash = 5381;

	for (size_t i = 0; i < size_in_bytes; i++)
		hash = hash * 33 ^ (hash_t)(*((const char*)bytes + i));

	return hash;
}
