#pragma warning(disable : 4996)
//#include <iostream>
#include <stdio.h> // for printf
#include <stdlib.h> // for malloc
#include <string.h> // for strcopy, strchr, strlen
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include <string>

//using namespace std;

void PrintDataStructure(HEADER_C* pList)
{

	HEADER_C* pC = pList;
	int counter = 0;

	if (pList == NULL) {
		printf("Error: Structure is missing.\n");
		return;
	}

	//while (pC != nullptr)
	while (pC != NULL)
	{
		ITEM3* pCitem = NULL;
		void** pCitemArr = pC->ppItems;
		for (int letter = 0; letter < 26; letter++)
		{
			if (*(pCitemArr + letter))
			{
				ITEM3* pCitem = (ITEM3*)(*(pCitemArr + letter));
				while(pCitem) {
					//if (pCitem != nullptr) { // not needed?
					printf("%c%c | NO.%3d | ID: %32s | CODE: %u | TIME: %02d:%02d:%02d \n",
						pC->cBegin,
						char('A' + letter),
						counter + 1,
						pCitem->pID,
						pCitem->Code,
						pCitem->Time.Hour,
						pCitem->Time.Min,
						pCitem->Time.Sec);
					pCitem = pCitem->pNext;
					counter++;
				}
				
			}
		}
		pC = pC->pNext;
	}
}

char* GetWordOne(char* pNewItemID)
{
	//if (pNewItemID == nullptr) {
	if (pNewItemID == NULL) {
		return NULL;
	}
	char* pWordOne = (char*)malloc(1 * sizeof(char));
	char* pSpace = strchr(pNewItemID, ' ');
	if (!pSpace)
	{
		return NULL;
	}

	for (int i = 1; *(pNewItemID + i); i++)
	{
		if ((pNewItemID + i) == pSpace)
		{
			pWordOne = (char*)malloc((i + 1) * sizeof(char));
			if (*pWordOne != NULL)
			{
				*(pWordOne + i) = '\0';
			}
		}
	}

	if (pSpace)
	{
		for (int i = 0; (pNewItemID + i) != pSpace; *(pWordOne + i) = *(pNewItemID + i), i++);
	}

	return pWordOne;
}

HEADER_C* GetHeaderPos(HEADER_C* pList, char* pChar)
{
	HEADER_C* pC = pList;
	//while (pC != nullptr)
	while (pC != NULL)
	{
		if (pC->cBegin == *pChar)
		{
			return pC;
		}
		pC = pC->pNext;
	}
	return NULL;
}

ITEM3* GetItemLastPos(ITEM3* pItem)
{
	ITEM3* pI = pItem;
	//while (pI != nullptr)
	while (pI != NULL) {
		if (pI->pNext == NULL)
		{
			return pI;
		}
		pI = pI->pNext;
	}

	return pItem;
}

bool StringCompare(char* str1, char* str2)
{
	if (strlen(str1) != strlen(str2))
	{
		return false;
	}
	else
	{
		size_t stlen = strlen(str1);
		for (int i = 0; i < stlen; i++)
		{
			if (*(str1 + i) == *(str2 + i))
			{
				if (i + 1 == stlen)
				{
					return true;
				}
				return false;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

ITEM3* GetItemPreviousPos(ITEM3* pItem, ITEM3* pItemInit)
{
	ITEM3* pPrevItem = pItemInit;

	while (pPrevItem)
	{
		if (pPrevItem->pNext == pItem)
		{
			return pPrevItem;
		}
		else if (pPrevItem = pItem)
		{
			return NULL;
		}
		pPrevItem = pPrevItem->pNext;
	}
}

bool CheckForValidity(char* pChar)
{
	char* pSpace = strchr(pChar, ' ');
	if (pSpace != NULL)
	{
		if (*(pChar + 1) != ' ')
		{
			return false;
		}

		if (*(pSpace + 2) != NULL)
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool checkWord(char* word, int len) {
	for (int i = 0; i < len; i++) {
		int ascii = (int)*(word + i);
		if (!((ascii >= 65 || ascii <= 90) || (ascii >= 97 || ascii <= 122) || ascii == 45)) { return true; }
	}
	return false;
}

bool validateIDFormat(char* pNewItemID) {
	if (strlen(pNewItemID) < 3) { return false; }

	char* spacePos = strchr(pNewItemID, ' ');

	if (spacePos == 0 || strchr(spacePos + 1, ' ') != 0) { return false; }

	char first = *pNewItemID,
		second = *(spacePos + 1);

	if ((int)first < 65 || (int)first > 90 || (int)second < 65 || (int)second > 90) { return false; }

	int lenFirst = spacePos - pNewItemID,
		lenSecond = strlen(pNewItemID) - lenFirst - 1;

	if (checkWord(pNewItemID, lenFirst) || checkWord(spacePos + 1, lenSecond)) { return false; }
	return true;
}

bool isInList(ITEM3* head, char* itemID) {
	for (; head; head = head->pNext) {
		if ((std::string)head->pID == (std::string)itemID) { return true; }
	}
	return false;
}

bool isInStructure(char* itemID, HEADER_C* pC) {
	HEADER_C* cache = pC;

	for (; cache; cache = cache->pNext) {
		if (cache == nullptr) { continue; } // if there are null pointers in double linked list at first layer
		else {
			ITEM3** secondLayer = (ITEM3**)cache->ppItems;
			for (int counter = 0; counter < 26; counter++) {
				if (secondLayer[counter]) {
					if (isInList(secondLayer[counter], itemID)) { return true; }
				}
			}
		}
	}
	return false;
}

HEADER_C* InsertItem(HEADER_C* pList, char* pNewItemID = 0)
{
	ITEM3* pNewItem = (ITEM3*)malloc(sizeof(ITEM3));
	if (pNewItemID != 0)
	{
		//if (CheckForValidity(pNewItemID) == false)
		if (validateIDFormat(pNewItemID) == false)
		{
			//cout << "ERROR! Given index is wrong" << endl;
			printf("ERROR! Given index is wrong\n");

			return pList;
		}
	}
	if (pNewItemID && isInStructure(pNewItemID, pList)) {
		printf("Already exists in structure.");
		throw 1;
		// throw 1; // Already exists exception
	}

	char abc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char* pWordOne = GetWordOne(pNewItemID);
	char* pWordTwo = (strchr(pNewItemID, ' ') + 1);
	char pLetterTwo = pWordTwo[0];

	HEADER_C* pC = GetHeaderPos(pList, pWordOne);

	if (pC != NULL) // first letter
	{
		ITEM3* pCitem = NULL;
		void** pCitemArr = pC->ppItems;
		int i = (int)*pWordTwo - 'A';

		void** ppItems = (void**)malloc(26);

		for (int letter = 0; letter < 26; letter++)
		{
			if (*(pCitemArr + letter)) // 2nd letter exists
			{
				ITEM3* pCitem = (ITEM3*)(*(pCitemArr + letter));
				// Find second word first letter
				int spacePos = std::string(pCitem->pID).find(' ');
				int secondItemPos = spacePos + 1;

				if (pCitem->pID[secondItemPos] == pLetterTwo) { // 2nd letter true

					pNewItem = (ITEM3*)GetItem(3, pNewItemID);
					pNewItem->pNext = pCitem;

					pC->ppItems[letter] = pNewItem;
					//printf(pCitem->pID, "\n");
					return pList;
				}
			}
		}

		// 2nd letter does not exist
		pNewItem = (ITEM3*)GetItem(3, pNewItemID);
		pC->ppItems[i] = pNewItem;
		
		return pList;
	
	}
	else
	{
		pC = pList;
		// New item should be inserted in the beginning
		if (pNewItemID[0] < pC->cBegin)
		{
			HEADER_C* newC = (HEADER_C*)malloc(sizeof(HEADER_C));
			newC->ppItems = (void**)malloc(100);
			//void** ppItems = (void**)malloc(100);

			newC->cBegin = pNewItemID[0];
			newC->pNext = pC;

			ITEM3* pNewItem = (ITEM3*)GetItem(3, pNewItemID);

			int i = (int)*pWordTwo - 'A';

			for (int i = 0; i < 26; i++)
			{
				if (abc[i] == *pWordTwo)
				{
					ITEM3* pNewItem = (ITEM3*)GetItem(3, pNewItemID);
					pNewItem->pNext = NULL;
					*(newC->ppItems + i) = (void*)pNewItem;
					continue;
				}
				*(newC->ppItems + i) = NULL;
			}

			newC->ppItems[i] = pNewItem;

			//pList = newC;

			return newC;
		}
		else
		{
			// Find position after which new item will be inserted
			while (pC->pNext && pC->pNext->cBegin < pNewItemID[0])
			{
				pC = pC->pNext;
			}
			// New item should be inserted in the end
			if (!pC->pNext)
			{
				HEADER_C* newC = (HEADER_C*)malloc(sizeof(HEADER_C));
				newC->ppItems = (void**)malloc(100);
				//void** ppItems = (void**)malloc(100);

				newC->cBegin = pNewItemID[0];
				newC->pNext = NULL;
			
				ITEM3* pNewItem = (ITEM3*)GetItem(3, pNewItemID);

				int i = (int)*pWordTwo - 'A';

				for (int i = 0; i < 26; i++)
				{
					if (abc[i] == *pWordTwo)
					{
						ITEM3* pNewItem = (ITEM3*)GetItem(3, pNewItemID);
						pNewItem->pNext = NULL;
						*(newC->ppItems + i) = (void*)pNewItem;
						continue;
					}
					*(newC->ppItems + i) = NULL;
				}

				newC->ppItems[i] = pNewItem;

				pC->pNext = newC;
			}
			// New item should be inserted in the middle
			else
			{
				HEADER_C* newC = (HEADER_C*)malloc(sizeof(HEADER_C));
				newC->ppItems = (void**)malloc(100);
				//void** ppItems = (void**)malloc(100);

				newC->cBegin = pNewItemID[0];
				newC->pNext = pC->pNext;

				ITEM3* pNewItem = (ITEM3*)GetItem(3, pNewItemID);

				int i = (int)*pWordTwo - 'A';

				for (int i = 0; i < 26; i++)
				{
					if (abc[i] == *pWordTwo)
					{
						ITEM3* pNewItem = (ITEM3*)GetItem(3, pNewItemID);
						pNewItem->pNext = NULL;
						*(newC->ppItems + i) = (void*)pNewItem;
						continue;
					}
					*(newC->ppItems + i) = NULL;
				}

				newC->ppItems[i] = pNewItem;

				pC->pNext = newC;

				//pCreatedHeader->pNext = pC->pNext;
				//pC->pNext = pCreatedHeader;
			}
		}
	}
	return pList;
}

HEADER_C* RemoveItem(HEADER_C* pList, char* pItemID)
{
	if (validateIDFormat(pItemID) == false)
	{
		//cout << "ERROR! Given index is wrong" << endl;
		printf("ERROR! Given index is wrong\n");
		return pList;
	}

	char* pWordOne = GetWordOne(pItemID);
	char* pWordTwo = (strchr(pItemID, ' ') + 1);
	char abc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	HEADER_C* pC = pList;

	while (pC)
	{
		if (pC->cBegin == *pWordOne)
		{
			for (int i = 0; i < 26; i++)
			{
				if (*(pC->ppItems + i) != NULL && abc[i] == *pWordTwo)
				{
					ITEM3* pItem = (ITEM3*)*(pC->ppItems + i); //DUMMY

					while (pItem != NULL)
					{
						if (pItem == NULL) { break; }
						char* pItemWordOne = GetWordOne(pItem->pID);
						char* pItemWordTwo = (strchr(pItem->pID, ' ') + 1);

						if (strcmp(pItem->pID, pItemID) == 0)
						{
							ITEM3* pPrevItem = GetItemPreviousPos(pItem, (ITEM3*)*(pC->ppItems + i));
							ITEM3* pNextItem = NULL;
							if (pItem->pNext != NULL)
							{
								pNextItem = pItem->pNext;
							}
							else
							{
								pNextItem = NULL;
							}

							if (pPrevItem == NULL)
							{
								*(pC->ppItems + i) = pNextItem;
								delete(pItem);
								return pList;
							}

							if (pNextItem == NULL)
							{
								pPrevItem->pNext = NULL;
								delete(pItem);
								return pList;
							}
							else
							{
								pPrevItem->pNext = pNextItem;
								delete(pItem);
								return pList;
							}

						}
						pItem = pItem->pNext;
					}
				}
			}
		}
		pC = pC->pNext;
	}
	//cout << "ERROR! No index matches with give input";
	printf("ERROR! No index matches with give input\n");
	return pList;
}

int coursework1()
{
	const char* Cases[] = { "Z A", "Z Z", "Z K", "A Z", "A A", "A K", "G Z", "G A", "G K", "M A", "MBa", "M Ba", "M Bb", "M Z" };
	//const char* Cases[] = { "A A", "A K" };
	//const char* Cases[] = { "M Ga", "M Gb", "M Z" };
	HEADER_C* data = GetStruct2(3, 30);

	printf("======INITIAL DATA START======\n");
	PrintDataStructure(data);
	printf("======INITIAL DATA STOP=======\n");

	int len = sizeof(Cases) / sizeof(Cases[0]);
	//for (int i = 0; i < 14; i++)
	for (int i = 0; i < len; i++)
	{
		char* newChars = (char*)malloc(strlen(Cases[i]) * sizeof(char));
		strcpy(newChars, Cases[i]);
		data = InsertItem(data, newChars);
	}

	printf("======NEW DATA START======\n");
	PrintDataStructure(data);
	printf("======NEW DATA STOP=======\n");

	for (int i = 0; i < len; i++)
	{
		char* newChars = (char*)malloc(strlen(Cases[i]) * sizeof(char));
		strcpy(newChars, Cases[i]);
		data = RemoveItem(data, newChars);
	}
	printf("======REM DATA START======\n");
	PrintDataStructure(data);
	printf("======REM DATA STOP=======\n");

	return 0;
}
/*
int main() {
	coursework1();

	return 0;
}*/