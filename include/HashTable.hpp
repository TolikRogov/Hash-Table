#pragma once

#include "HashTable_utilities.hpp"

const size_t MAX_WORD_SIZE = 128;

struct Bucket_t {
	char* word;
	size_t frequency;
};

HashTableStatusCode PrintBucket(Bucket_t* bucket);
