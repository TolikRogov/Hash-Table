#include "HashTable.hpp"

HashTableStatusCode PrintElement(Element_t* Element) {

	printf(BLUE("Element address: %p")"\n", Element);
	printf(GREEN("Word: %s")"\n", Element->word);
	printf(GREEN("Word frequency: %zu")"\n", Element->frequency);

	return HASHTABLE_NO_ERROR;
}
