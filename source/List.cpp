#include "List.hpp"

HashTableStatusCode ListCtor(List_t* list) {

	list->head = list->tail = (Data_t*)calloc(1, sizeof(Data_t));
	if (!list->head)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	list->head->data = (Element_t*)calloc(1, sizeof(Element_t));
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

	if (cur_data->next != NULL)
		ListDtor(cur_data->next);

	if (cur_data) {
		free(cur_data);
		cur_data = NULL;
	}

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode ListAdd(List_t* list, char* word) {

	if (!list)
		HASHTABLE_ERROR_CHECK(HASHTABLE_NULL_POINTER);

	if (list->size == 0) {
		list->tail->data->word = word;
		list->tail->data->frequency = 1;
		list->size++;
		return HASHTABLE_NO_ERROR;
	}

	Data_t* founded = ListFindElement(list->head, word);
	if (founded) {
		founded->data->frequency++;
		return HASHTABLE_NO_ERROR;
	}

	if (!list->tail)
		HASHTABLE_ERROR_CHECK(HASHTABLE_NULL_POINTER);

	list->tail->next = (Data_t*)calloc(1, sizeof(Data_t));
	if (!list->tail->next)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	list->tail = list->tail->next;

	list->tail->data = (Element_t*)calloc(1, sizeof(Element_t));
	if (!list->tail->data)
		HASHTABLE_ERROR_CHECK(HASHTABLE_ALLOCATION_ERROR);

	list->tail->data->word = word;
	list->tail->data->frequency = 1;
	list->size++;

	return HASHTABLE_NO_ERROR;
}

Data_t* ListFindElement(Data_t* data, char* word) {

	if (!data)
		return NULL;

#ifdef BASE
	if (!strcmp(data->data->word, word))
		return data;
#else
	if (_mm256_movemask_epi8(_mm256_cmpeq_epi64(*(__m256i*)data->data->word, *(__m256i*)word)) == -1)
		return data;
#endif

	return ListFindElement(data->next, word);
}

HashTableStatusCode ListPrint(List_t* list) {

	printf(RED("List address: %p")"\n", list);
	printf(BLUE("List size: %zu")"\n", list->size);
	printf(BLUE("Head address: %p")"\n", list->head);
	printf(BLUE("Tail address: %p")"\n", list->tail);
	if (list->size)
		ListPrintData(list->head);

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode ListPrintData(Data_t* data) {

	if (!data)
		return HASHTABLE_NO_ERROR;

	printf(_LINE"\n");
	printf(BLUE("Data address: %p")"\n", data);
	PrintElement(data->data);
	printf(BLUE("Next address: %p")"\n", data->next);
	printf(_LINE"\n");

	ListPrintData(data->next);

	return HASHTABLE_NO_ERROR;
}

HashTableStatusCode PrintElement(Element_t* Element) {

	printf(BLUE("Element address: %p")"\n", Element);
	printf(GREEN("Word: %s")"\n", Element->word);
	printf(GREEN("Word frequency: %zu")"\n", Element->frequency);

	return HASHTABLE_NO_ERROR;
}
