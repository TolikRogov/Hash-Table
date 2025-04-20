#pragma once

#include "List.hpp"

struct Bucket_t {
	size_t size;
	List_t* lists;
};

struct Buffer {
	size_t size;
	char* data;
};

HashTableStatusCode BucketsCtor(Buffer* buffer, Bucket_t* buckets);
HashTableStatusCode BucketsDtor(Buffer* buffer, Bucket_t* buckets);
