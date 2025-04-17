#include "List.hpp"

HashTableStatusCode ListCtor(List_t* list) {

	list->size = 1;
	list->capacity = LIST_DEFAULT_CAPACITY;
	list->head = list->tail = (ListData_t*)calloc(list->capacity, sizeof(ListData_t));
	if (!list->head)
		return HASHTABLE_ALLOCATION_ERROR;

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode ListDtor(ListData_t* data) {

	if (data) {
		free(data);
		data = NULL;
	}

	if (!data->next)
		return HASHTABLE_NO_ERROR;

	ListDtor(data->next);

	return HASHTABLE_NO_ERROR;
}
