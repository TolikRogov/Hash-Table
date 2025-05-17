#pragma once

#include "HashTable_utilities.hpp"

#ifdef BASE
	typedef char List_key_t;
#else
	typedef __m256i List_key_t;
#endif

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

struct Element_t {
	List_key_t* word;
	size_t frequency;
};

struct Data_t {
	Element_t* data;
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
Data_t* ListFindElement(Data_t* data, List_key_t* word);

HashTableStatusCode ListPrint(List_t* list);
HashTableStatusCode ListPrintData(Data_t* data);
HashTableStatusCode PrintElement(Element_t* Element);
