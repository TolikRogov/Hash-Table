#pragma once

#include "HashTable_utilities.hpp"

struct Element_t {
	char* word;
	size_t frequency;
};

HashTableStatusCode PrintElement(Element_t* Element);
