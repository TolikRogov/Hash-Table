#pragma once

#include "HashTable.hpp"

const size_t LIST_DEFAULT_CAPACITY	=	8;

struct ListData_t {
	Bucket_t* bucket;
	ListData_t* next;
};

struct List_t {
	ListData_t* head;
	ListData_t* tail;

	size_t capacity;
	size_t size;
};
