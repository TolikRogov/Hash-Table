#include "List.hpp"

int main() {

	HashTableStatusCode ht_status = HASHTABLE_NO_ERROR;

	List_t list = {};

	LIST_CTOR(&list);
	LIST_PRINT(&list);

	LIST_ADD(&list, "HUI");
	LIST_ADD(&list, "HUISHE");
	LIST_PRINT(&list);

	LIST_DTOR(list.head);

	return 0;
}
