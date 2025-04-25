#pragma once

#include "HashFunctions.hpp"
#include "List.hpp"

const size_t AMOUNT_UNIQUE_WORDS 	= 19030;
const size_t LOAD_FACTOR 			= 15;
const size_t FINDER_ITERATIONS 		= 700;

#define BUCKETS_CTOR(buffer, buckets) {						\
	ht_status = BucketsCtor(buffer, buckets);				\
	HASHTABLE_ERROR_CHECK(ht_status);						\
}															\

#define BUFFER_CTOR(buffer) {						 		\
	ht_status = BufferCtor(buffer);							\
	HASHTABLE_ERROR_CHECK(ht_status);						\
}															\

#define BUCKETS_UPLOADER(buffer, buckets) {					\
	ht_status = BucketsUploader(buffer, buckets);			\
	HASHTABLE_ERROR_CHECK(ht_status);						\
}															\

#define BUCKETS_DTOR(buckets) {					 			\
	ht_status = BucketsDtor(buckets);						\
	HASHTABLE_ERROR_CHECK(ht_status);						\
}															\

#define BUFFER_DTOR(buffer) {					 			\
	ht_status = BufferDtor(buffer);							\
	HASHTABLE_ERROR_CHECK(ht_status);						\
}															\

#define BUCKETS_DUMP(buckets) {					 		 	\
	ht_status = BucketsDump(buckets);						\
	HASHTABLE_ERROR_CHECK(ht_status);						\
}															\

struct Bucket_t {
	size_t size;
	List_t* lists;
	hash_func_t hash_function;
	HashFunction hash_func_num;
};

struct Buffer {
	size_t size;
	char* data;
	size_t words_cnt;
	size_t shift;
};

HashTableStatusCode BufferCtor(Buffer* buffer);
HashTableStatusCode BufferDtor(Buffer* buffer);

HashTableStatusCode BucketsCtor(Buffer* buffer, Bucket_t* buckets);
HashTableStatusCode BucketsDtor(Bucket_t* buckets);

List_t* FindListForWord(Buffer* buffer, Bucket_t* buckets, char** word);
HashTableStatusCode BucketsUploader(Buffer* buffer, Bucket_t* buckets);
HashTableStatusCode BucketsFinder(Buffer* buffer, Bucket_t* buckets);

HashTableStatusCode BucketsDump(Bucket_t* buckets);
