#include "List.hpp"

HashTableStatusCode ListCtor(List_t* list) {

	list->head = list->tail = (Data_t*)calloc(1, sizeof(Data_t));
	if (!list->head)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	list->head->data = (Bucket_t*)calloc(1, sizeof(Bucket_t));
	if (!list->head->data)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	list->size = 0;

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode ListDtor(Data_t* cur_data) {

	if (cur_data->data) {
		free(cur_data->data);
		cur_data->data = NULL;
	}

	if (cur_data->next == NULL)
		return HASHTABLE_NO_ERROR;

	ListDtor(cur_data->next);

	if (cur_data) {
		free(cur_data);
		cur_data = NULL;
	}

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode ListAdd(List_t* list, char* word) {

	if (!list->tail)
		HASHTABLE_ERROR_CHECK(HASHTABLE_NULL_POINTER);

	if (list->size == 0) {
		list->tail->data->word = word;
		list->tail->data->frequency = 1;
		list->size++;
		return HASHTABLE_NO_ERROR;
	}

	list->tail->next = (Data_t*)calloc(1, sizeof(Data_t));
	if (!list->tail->next)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	list->tail = list->tail->next;

	list->tail->data = (Bucket_t*)calloc(1, sizeof(Bucket_t));
	if (!list->tail->data)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	list->tail->data->word = word;
	list->tail->data->frequency = 1;
	list->size++;

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode ListPrint(List_t* list) {

	printf(RED("List address: %p")"\n", list);
	printf(BLUE("List size: %zu")"\n", list->size);
	printf(BLUE("Head address: %p")"\n", list->head);
	printf(BLUE("Tail address: %p")"\n", list->tail);
	ListPrintData(list->head);

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode ListPrintData(Data_t* data) {

	if (!data)
		return HASHTABLE_NO_ERROR;

	printf(_LINE"\n");
	printf(BLUE("Data address: %p")"\n", data);
	PrintBucket(data->data);
	printf(BLUE("Next address: %p")"\n", data->next);
	printf(_LINE"\n");

	ListPrintData(data->next);

	return HASHTABLE_NO_ERROR;
}
