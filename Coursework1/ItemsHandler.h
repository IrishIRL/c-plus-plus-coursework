#pragma once
#pragma once
#include <iostream>
#include <vector>
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"

namespace ItemsHandler {
	void printItem(ITEM3* item, size_t* currentNumber = 0);
	void printItemList(ITEM3* head, size_t* currentNumber = 0);
	void removeElementFromList(ITEM3** head, char* key);
	void freeList(ITEM3** list);
	//ITEM3* copyList(ITEM3* head);
	ITEM3* getItemFromList(ITEM3* list, char* itemID);
	bool checkWord(char* word, int len);
	bool areSame(ITEM3* list1, ITEM3* list2);
	bool validateIDFormat(char* pNewItemID);
	std::vector <ITEM3> listToVector(ITEM3* head);
	bool isInList(ITEM3* head, char* itemID);
	char* serializeItem(ITEM3* item, size_t* size);
}