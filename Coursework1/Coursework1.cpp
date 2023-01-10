#pragma warning(disable : 4996)
//#include <iostream>
#include <stdio.h> // for printf
#include <stdlib.h> // for malloc
#include <string.h> // for strcopy, strchr, strlen
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"

//using namespace std;

void PrintDataStructure(HEADER_C* pList)
{

	HEADER_C* pC = pList;
	int counter = 0;

	if (pList == NULL) {
		printf("Error: Structure is missing.\n");
		return;
	}

//	while (pC != nullptr)
	while (pC != NULL)
	{
		ITEM3* pCitem = NULL;
		void** pCitemArr = pC->ppItems;
		for (int letter = 0; letter < 26; letter++)
		{
			if (*(pCitemArr + letter))
			{
				ITEM3* pCitem = (ITEM3*)(*(pCitemArr + letter));
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

				counter++;
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

HEADER_C* InsertItem(HEADER_C* pList, char* pNewItemID = 0)
{
	ITEM3* pNewItem = (ITEM3*)malloc(sizeof(ITEM3));
	if (pNewItemID != 0)
	{
		if (CheckForValidity(pNewItemID) == false)
		{
			//cout << "ERROR! Given index is wrong" << endl;
			printf("ERROR! Given index is wrong\n");

			return pList;
		}
	}

	char abc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char* pWordOne = GetWordOne(pNewItemID);
	char* pWordTwo = (strchr(pNewItemID, ' ') + 1);

	HEADER_C* pC = GetHeaderPos(pList, pWordOne);

	if (pC != NULL)
	{
		void** ppItems = pC->ppItems; //Dummy
		int i = (int)*pWordTwo - 'A';

		//if (*(ppItems + i) != nullptr && *pWordTwo == abc[i])
		if (*(ppItems + i) != NULL && *pWordTwo == abc[i])
		{
			ITEM3* pItems = (ITEM3*)*(ppItems + i); //Dummy
			char* pItemWordOne = GetWordOne(pItems->pID);

			char* pItemWordTwo = (strchr(pItems->pID, ' ') + 1);
			//if (pItemWordOne != nullptr)
			if (pItemWordOne != NULL)
			{
				//Iterate through Items
				for (int j = 0; pItems; pItems = pItems->pNext, j++) //Compare input strings with item strings. Check for duplicate
				{
					pItemWordTwo = (strchr(pItems->pID, ' ') + 1);
					if (StringCompare(pWordOne, pItemWordOne) && StringCompare(pWordTwo, pItemWordTwo))
					{
						//cout << "ERROR! This ID already exists";
						printf("ERROR! This ID already exists\n");
						return pList;
					}
				}

				// INSERTING NEW ITEM //

				HEADER_C* newC = (HEADER_C*)malloc(sizeof(HEADER_C));

				void** ppItems = (void**)malloc(26);

				ITEM3* pLastItem = GetItemLastPos(pItems);
				if (pNewItemID == 0)
				{
					pNewItem = (ITEM3*)GetItem(3, NULL);
				}
				else
				{
					pNewItem = (ITEM3*)GetItem(3, pNewItemID);
				}
				pNewItem->pNext = NULL;
				pLastItem->pNext = pNewItem;
				return pList;
			}
		}


		if (abc[i] == *pWordTwo)
		{
			if (pNewItemID == 0)
			{
				pNewItem = (ITEM3*)GetItem(3, NULL);
			}
			else
			{
				pNewItem = (ITEM3*)GetItem(3, pNewItemID);
			}
			pNewItem->pNext = NULL;
			*(ppItems + i) = (void*)pNewItem;
			return pList;
		}

	}
	else
	{
		pC = pList; //DUMMY. pC has a pointer to the last header
		while (pC->pNext != NULL)
		{
			pC = pC->pNext;
		}

		HEADER_C* newC = (HEADER_C*)malloc(sizeof(HEADER_C));

		void** ppItems = (void**)malloc(1000); // I am too wasted to properly fix this.

		for (int i = 0; i < 26; i++)
		{
			if (abc[i] == *pWordTwo)
			{
				ITEM3* pNewItem = (ITEM3*)GetItem(3, pNewItemID);
				pNewItem->pNext = NULL;
				*(ppItems + i) = (void*)pNewItem;
				continue;
			}
			*(ppItems + i) = NULL;
		}

		newC->cBegin = *pWordOne;
		newC->pNext = NULL;
		newC->ppItems = ppItems;
		pC->pNext = newC;
		return pList;
	}
	return pList;
}

HEADER_C* RemoveItem(HEADER_C* pList, char* pItemID)
{
	if (CheckForValidity(pItemID) == false)
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

						if (StringCompare(pItemWordOne, pWordOne) && StringCompare(pItemWordTwo, pWordTwo))
						{
							ITEM3* pPrevItem = GetItemPreviousPos(pItem, (ITEM3*)*(pC->ppItems + i));
							ITEM3* pNextItem = NULL;
							if (pItem->pNext != NULL)
							{
								ITEM3* pNextItem = pItem->pNext;
							}
							else
							{
								ITEM3* pNextItem = NULL;
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
	const char* Cases[] = { "Z A", "Z Z", "Z K", "A Z", "A A", "A K", "G Z", "G A", "G K", "M A", "MBa", "M Ba", "M Bb", "M Z"};
	
	HEADER_C* data = GetStruct2(3, 33);

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