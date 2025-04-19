#pragma once

#include "HashTable.hpp"

#define LIST_CTOR(list) {							 \
	ht_status = ListCtor(list);						\
	HASHTABLE_ERROR_CHECK(ht_status);				\
}													\

#define LIST_PRINT(list) {							 \
	ht_status = ListPrint(list);					\
	HASHTABLE_ERROR_CHECK(ht_status);				\
}													\

#define LIST_ADD(list, word) {						 \
	ht_status = ListAdd(list, word);				\
	HASHTABLE_ERROR_CHECK(ht_status);				\
}													\

#define LIST_DTOR(data) {							 \
	ht_status = ListDtor(data);						\
	HASHTABLE_ERROR_CHECK(ht_status);				\
}													\

struct Data_t {
	Bucket_t* data;
	Data_t* next;
};

struct List_t {
	Data_t* head;
	Data_t* tail;
	size_t size;
};

HashTableStatusCode ListCtor(List_t* list);
HashTableStatusCode ListDtor(Data_t* cur_data);
HashTableStatusCode ListAdd(List_t* list, char* word);

HashTableStatusCode ListPrint(List_t* list);
HashTableStatusCode ListPrintData(Data_t* data);
