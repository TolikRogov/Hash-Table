#pragma once

#include "List.hpp"

const size_t LOAD_FACTOR = 15;

#define BUCKETS_CTOR(buffer, buckets) {						 \
	ht_status = BucketsCtor(buffer, buckets);				\
	HASHTABLE_ERROR_CHECK(ht_status);						\
}															\

#define BUFFER_CTOR(buffer) {						 		 \
	ht_status = BufferCtor(buffer);							\
	HASHTABLE_ERROR_CHECK(ht_status);						\
}															\

#define BUCKETS_UPLOADER(buffer, buckets) {					 \
	ht_status = BucketsUploader(buffer, buckets);			\
	HASHTABLE_ERROR_CHECK(ht_status);						\
}															\

#define BUCKETS_DTOR(buckets) {					 			 \
	ht_status = BucketsDtor(buckets);						\
	HASHTABLE_ERROR_CHECK(ht_status);						\
}															\

#define BUFFER_DTOR(buffer) {					 			 \
	ht_status = BufferDtor(buffer);							\
	HASHTABLE_ERROR_CHECK(ht_status);						\
}															\

#define BUCKETS_PRINT(buckets) {					 		 \
	ht_status = BucketsPrint(buckets);						\
	HASHTABLE_ERROR_CHECK(ht_status);						\
}															\

struct Bucket_t {
	size_t size;
	List_t* lists;
};

struct Buffer {
	size_t size;
	size_t words_cnt;
	char* data;
};

HashTableStatusCode BufferCtor(Buffer* buffer);
HashTableStatusCode BufferDtor(Buffer* buffer);

HashTableStatusCode BucketsCtor(Buffer* buffer, Bucket_t* buckets);
HashTableStatusCode BucketsDtor(Bucket_t* buckets);
HashTableStatusCode BucketsUploader(Buffer* buffer, Bucket_t* buckets);

HashTableStatusCode BucketsPrint(Bucket_t* buckets);
