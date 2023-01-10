#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "stdarg.h"
#include "stdio.h"
#include "ctype.h"
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include "ItemsHandler.h"
#include <vector>

using namespace std;

class DataStructure {
private:
    HEADER_C* Start;
    size_t size;
    DataStructure* copyElements(DataStructure& original);

public:
    DataStructure(HEADER_C* generatedStructure, size_t size);

    DataStructure(); // 1
    DataStructure(char* filename); // 2

    DataStructure(DataStructure& original); // 4
    size_t getItemsNumber(); // 5
    ITEM3* getItem(char* pID); // 6

    // operators 
    void operator+=(ITEM3* anotherStructure); // 7
    void operator-=(char* pID); // 8
    DataStructure& operator=(DataStructure& right); // 9
    int operator==(DataStructure& other); // 10

    void write(char* pFilename); // 11

    // 12
    friend std::ostream& operator<<(std::ostream& out, DataStructure& list) {
        list.printDataStructure();
        return out;
    }

    // item work
    std::vector <ITEM3> getAllItems();
    void insertItem(char* pNewItemID = 0);
    void insertItem(ITEM3& item);
    bool isInStructure(char* itemID);
    void removeItem(char* itemID);

    void getItem(DataStructure& right, char* s);

    // printing func
    void printDataStructure();

    ~DataStructure(); // 3
};

// ITEMS HANDLER START //
void ItemsHandler::printItem(ITEM3* item, size_t* currentNumber) {
    if (!item) { cout << "Nothing to print!" << endl; }

    if (currentNumber) {
        cout <<
            "No." << setfill(' ') << setw(3) << *currentNumber << " | " <<
            "ID:" << setfill(' ') << setw(20) << item->pID << " | " <<
            "CODE:" << setfill(' ') << setw(11) << item->Code << " | " <<
            "TIME: " << setfill('0') << setw(2) << item->Time.Hour << ":" <<
            setw(2) << item->Time.Min << ":" <<
            setw(2) << item->Time.Sec << endl;
        (*currentNumber)++;
    }

    else {
        cout <<
            "ID:" << setfill(' ') << setw(20) << item->pID << " | " <<
            "CODE:" << setfill(' ') << setw(11) << item->Code << " | " <<
            "TIME: " << setfill('0') << setw(2) << item->Time.Hour << ":" <<
            setw(2) << item->Time.Min << ":" <<
            setw(2) << item->Time.Sec << endl;
    }
}

void ItemsHandler::printItemList(ITEM3* head, size_t* currentNumber) {
    for (; head; head = head->pNext) {
        printItem(head, currentNumber);
    }
}

std::vector <ITEM3> ItemsHandler::listToVector(ITEM3* head) {
    std::vector <ITEM3> responce;
    for (; head; head = head->pNext) {
        ITEM3 cache = *head;
        cache.pNext = nullptr;
        responce.push_back(cache);
    }
    return responce;
}

bool ItemsHandler::isInList(ITEM3* head, char* itemID) {
    for (; head; head = head->pNext) {
        if ((std::string)head->pID == (std::string)itemID) { return true; }
    }
    return false;
}

bool ItemsHandler::checkWord(char* word, int len) {
    for (int i = 0; i < len; i++) {
        int ascii = (int)*(word + i);
        if (!((ascii >= 65 || ascii <= 90) || (ascii >= 97 || ascii <= 122) || ascii == 45)) { return true; }
    }
    return false;
}

bool ItemsHandler::validateIDFormat(char* pNewItemID) {
    if (strlen(pNewItemID) < 3) { return false; }

    char* spacePos = strchr(pNewItemID, ' ');

    if (spacePos == 0 || strchr(spacePos + 1, ' ') != 0) { return false; }

    char first = *pNewItemID,
        second = *(spacePos + 1);

    if ((int)first < 65 || (int)first > 90 || (int)second < 65 || (int)second > 90) { return false; }

    int lenFirst = spacePos - pNewItemID,
        lenSecond = strlen(pNewItemID) - lenFirst - 1;

    if (ItemsHandler::checkWord(pNewItemID, lenFirst) || ItemsHandler::checkWord(spacePos + 1, lenSecond)) { return false; }
    return true;
}

void ItemsHandler::removeElementFromList(ITEM3** head, char* key) {
    ITEM3* temp = *head;
    ITEM3* prev = nullptr;

    if (temp && (std::string)temp->pID == (std::string)key) {
        *head = temp->pNext;
        delete temp;
        return;
    }

    else {
        while (temp && (std::string)temp->pID != (std::string)key) {
            prev = temp;
            temp = temp->pNext;
        }

        if (!temp) { return; }
        prev->pNext = temp->pNext;
        delete temp;
    }

}

void ItemsHandler::freeList(ITEM3** list) { // works pretty fine 
    if (!list) { return; }
    ITEM3* current = *list;
    ITEM3* next = nullptr;

    for (; current; current->pNext) {
        next = current->pNext;
        free(current);
        current = next;
    }
    *list = nullptr;

}

ITEM3* ItemsHandler::getItemFromList(ITEM3* list, char* itemID) {
    for (; list; list = list->pNext) {
        if ((std::string)list->pID == (std::string)itemID) {
            list->pNext = nullptr;
            return list;
        }
    }
}

char* ItemsHandler::serializeItem(ITEM3* item, size_t* size) {
    int n = strlen(item->pID) + 1;
    char* serialItem, * r;
    //*size = n + sizeof(ITEM2) + 3 * sizeof(int) + sizeof(unsigned long int) - sizeof(ITEM2*) - sizeof(TIME*) - sizeof(char*);
    *size = n + sizeof(ITEM3) + 3 * sizeof(int) + sizeof(unsigned long int) - sizeof(ITEM3*) - sizeof(TIME) - sizeof(char*);
    serialItem = (char*)malloc(*size);
    memcpy(r = serialItem, &n, sizeof(int));
    memcpy(r += sizeof(int), item->pID, n);
    memcpy(r += n, &item->Code, sizeof(unsigned long int));
    memcpy(r += sizeof(int), &item->Time.Hour, sizeof(int));
    memcpy(r += sizeof(int), &item->Time.Min, sizeof(int));
    memcpy(r += sizeof(int), &item->Time.Sec, sizeof(int));
    return serialItem;
}

bool ItemsHandler::areSame(ITEM3* list1, ITEM3* list2) {
    for (; list1 || list2; list1 = list1->pNext, list2 = list2->pNext) {
        if (!list1 && list2 || list1 && !list2) { return false; }
        if (!(
            (list1->Code == list2->Code) &&
            ((std::string)list1->pID == (std::string)list2->pID) &&
            (list1->Time.Hour == list2->Time.Hour) &&
            (list1->Time.Min == list2->Time.Min) &&
            (list1->Time.Sec == list2->Time.Sec)
            )) {
            return false;
        }
    }
    return true;
}

// ITEMS HANDLER FINISH //

// HELPER FUNCTIONS START //

int endline(int caseNo) {
    cout << endl << "=-------------------------------------------------------------------=" << endl << endl;
    if (caseNo != 0) {
    std:cout << "TEST CASE " << caseNo << endl << endl;
    }
    return (caseNo + 1);
}

// Task 12
void DataStructure::printDataStructure() {
    HEADER_C* cache = this->Start;
    size_t currentQueue = 1;

    for (; cache; cache = cache->pNext) {
        if (cache == nullptr) { continue; } // if there are null pointers in double linked list at first layer
        else {
            ITEM3** secondLayer = (ITEM3**)cache->ppItems;
            for (int counter = 0; counter < 26; counter++) {
                if (secondLayer[counter]) { ItemsHandler::printItemList((ITEM3*)(secondLayer[counter]), &currentQueue); }
            }
        }
    }
}

void DataStructure::getItem(DataStructure& right, char* s) {
    DataStructure tempStruct;
    tempStruct = right;

    try {
        ItemsHandler::printItem(tempStruct.getItem(s));
    }
    catch (const exception& msg) {
        cerr << msg.what() << endl;
    }
    tempStruct.~DataStructure(); // there is no need to have this structure later on, right?
}

DataStructure::DataStructure(HEADER_C* generatedStructure, size_t size) :
    Start(generatedStructure), size(size) {}

// Task 4, 9
DataStructure* DataStructure::copyElements(DataStructure& original) {
    std::vector <ITEM3> itemsToBeAppended = original.getAllItems();
    DataStructure* newDataStruct = new DataStructure(GetStruct2(3, 1), 1);
    ITEM3 itemToBeRemoved = newDataStruct->getAllItems()[0];
    //newDataStruct->insertItem((char*)"Vasja Pupkin"); // setting up static cache entry point 
    newDataStruct->removeItem(itemToBeRemoved.pID);
    for (int i = 0; i < itemsToBeAppended.size(); i++) { newDataStruct->insertItem(itemsToBeAppended[i]); }
    //newDataStruct->removeItem((char*)"Vasja Pupkin"); // removing it
    return newDataStruct;
}

std::vector <ITEM3> DataStructure::getAllItems() {
    HEADER_C* entryPoint = this->Start;
    std::vector <ITEM3> result = {};
    for (; entryPoint; entryPoint = entryPoint->pNext) {
        for (int i = 0; i < 26; i++) {
            std::vector <ITEM3> cache = ItemsHandler::listToVector((ITEM3*)(entryPoint->ppItems)[i]);
            result.insert(result.end(), cache.begin(), cache.end());
        }
    }
    reverse(result.begin(), result.end());
    return result;
}

void DataStructure::insertItem(char* pNewItemID)
{
    if (pNewItemID && isInStructure(pNewItemID)) {
        throw exception("Item already exists");
        // throw 1; // Already exists exception
    }

    if (pNewItemID == 0 || ItemsHandler::validateIDFormat(pNewItemID)) {
        ITEM3* newItem = (ITEM3*)GetItem(3, ((char*)pNewItemID));
        newItem->pNext = (ITEM3*)this->Start->ppItems[0];
        this->Start->ppItems[0] = newItem;
        this->size++;
    }
    else {
        throw exception("Format Error (Invalid ID Format or Nullptr)");
        // throw 2; // Format error
    }
}

void DataStructure::insertItem(ITEM3& item) {
    ITEM3* newNode = (ITEM3*)malloc(sizeof(ITEM3));

    newNode->Code = item.Code;
    newNode->pID = item.pID;
    newNode->Time.Hour = item.Time.Hour;
    newNode->Time.Min = item.Time.Min;
    newNode->Time.Sec = item.Time.Sec;

    newNode->pNext = (ITEM3*)*(this->Start->ppItems);
    *(this->Start->ppItems) = newNode;

    this->size++;
}

bool DataStructure::isInStructure(char* itemID) {
    HEADER_C* cache = this->Start;

    for (; cache; cache = cache->pNext) {
        if (cache == nullptr) { continue; } // if there are null pointers in double linked list at first layer
        else {
            ITEM3** secondLayer = (ITEM3**)cache->ppItems;
            for (int counter = 0; counter < 26; counter++) {
                if (secondLayer[counter]) {
                    if (ItemsHandler::isInList(secondLayer[counter], itemID)) { return true; }
                }
            }
        }
    }
    return false;
}

void DataStructure::removeItem(char* itemID) {
    if (itemID == nullptr || !ItemsHandler::validateIDFormat(itemID)) {
        throw exception("Format Error (Invalid ID Format or Nullptr)");
        //throw 2; // Format error
    }
    else if (!isInStructure(itemID)) {
        throw exception("No such item in the structure");
        //throw 1; // No such item in structure
    }
    else {
        HEADER_C* cache = this->Start;
        for (; cache; cache = cache->pNext) {
            if (cache == nullptr) { continue; }
            else {
                ITEM3** secondLayer = (ITEM3**)cache->ppItems;
                for (int counter = 0; counter < 26; counter++) {
                    if (secondLayer[counter]) {
                        if (ItemsHandler::isInList(secondLayer[counter], itemID)) {
                            ItemsHandler::removeElementFromList(&secondLayer[counter], itemID);
                            this->size--;
                        }
                    }
                }
            }
        }

    }
}

// HELPER FUNCTIONS STOP //

// TASKS START //
// 
// Task 1
DataStructure::DataStructure()
{
    Start = NULL;
}

// Task 2
DataStructure::DataStructure(char* pFilename)
{
    FILE* pFile = fopen(pFilename, "rb");
    char* pData;
    long lSize;
    if (pFile) {
        fseek(pFile, 0, SEEK_END);
        lSize = ftell(pFile);
        rewind(pFile);
        pData = (char*)malloc(lSize);
        int n = fread(pData, 1, lSize, pFile);
        fclose(pFile);
    }
    else { cout << "file error" << endl; throw exception("File Error"); } //return; }

    DataStructure* newDataStruct = new DataStructure(GetStruct2(3, 1), (size_t)1);
    ITEM3 itemToBeRemoved = newDataStruct->getAllItems()[0];
    newDataStruct->insertItem((char*)"Vasja Pupkin"); // setting up static cache entry point
    newDataStruct->removeItem(itemToBeRemoved.pID);
    char* cursor;
    int totalItems = 0;
    memcpy(&totalItems, cursor = pData, sizeof(int));
    cursor += sizeof(int);
    for (int i = 0; i < totalItems; i++) {
        int pIdSize = 0;
        char* pID;
        unsigned long int code = 0;
        int sec = 0,
            min = 0,
            hour = 0;
        memcpy(&pIdSize, cursor, sizeof(int));
        pID = (char*)malloc(pIdSize * sizeof(char));
        memcpy(pID, cursor += sizeof(int), pIdSize);
        memcpy(&code, cursor += pIdSize, sizeof(unsigned long int));
        memcpy(&hour, cursor += sizeof(int), sizeof(int));
        memcpy(&min, cursor += sizeof(char*), sizeof(int));
        memcpy(&sec, cursor += sizeof(int), sizeof(int));
        cursor += sizeof(int);
        ITEM3* pointer = (ITEM3*)GetItem(3, pID);
        pointer->Time.Hour = hour;
        pointer->Time.Min = min;
        pointer->Time.Sec = sec;
        pointer->Code = code;
        pointer->pID = pID;

        newDataStruct->insertItem(*pointer);
    }
    newDataStruct->removeItem((char*)"Vasja Pupkin"); // removing it

    this->Start = newDataStruct->Start;
    this->size = newDataStruct->size;
};

// Task 3
DataStructure::~DataStructure() {
    if (!this->Start) { return; }
    for (; this->Start; this->Start = this->Start->pNext) {
        for (int counter = 0; counter < 26; counter++) {
            ItemsHandler::freeList(&(((ITEM3**)this->Start->ppItems)[counter]));
        }
    }
}

// Task 4
DataStructure::DataStructure(DataStructure& original) {
    DataStructure* cache = copyElements(original);
    this->Start = cache->Start;
    this->size = cache->size;
}

// Task 5 
size_t DataStructure::getItemsNumber() { return this->size; }

// Task 6
ITEM3* DataStructure::getItem(char* itemID) {

    HEADER_C* cache = this->Start;

    if (itemID == nullptr || !ItemsHandler::validateIDFormat(itemID)) {
        throw exception("Format Error (Invalid ID Format or Nullptr)");
        //throw 2; // Format error
    }
    else if (!isInStructure(itemID)) {
        throw exception("No such item in the structure");
        //throw 1; // No such thing in structure 
    }

    for (; cache; cache = cache->pNext) {
        if (cache == nullptr) { continue; } // if there are null pointers in double linked list at first layer
        else {
            ITEM3** secondLayer = (ITEM3**)cache->ppItems;
            for (int counter = 0; counter < 26; counter++) {
                if (secondLayer[counter]) {
                    if (ItemsHandler::isInList(secondLayer[counter], itemID)) {
                        return ItemsHandler::getItemFromList(secondLayer[counter], itemID);
                    }
                }
            }
        }
    }
    return nullptr;
}

// Task 7
void DataStructure::operator+=(ITEM3* item) {
    if (this->Start == nullptr) {
        DataStructure* newDataStruct = new DataStructure(GetStruct2(3, 1), (size_t)1);
        //ITEM3 itemToBeRemoved = newDataStruct->getAllItems()[0]; // not needed here
        //newDataStruct->removeItem(itemToBeRemoved.pID); // not needed here

        this->Start = newDataStruct->Start;
        this->size = newDataStruct->size;
    }
    else {
        item->pNext = (ITEM3*)this->Start->ppItems[0];
        this->Start->ppItems[0] = item;
        this->size++;
    }
}

// Task 8
void DataStructure::operator-=(char* item) {
    try {
        removeItem(item);
    }
    catch (const exception& msg) {
        cerr << msg.what() << endl;
    }
}

// Task 9
DataStructure& DataStructure::operator=(DataStructure& right) {
    this->~DataStructure();
    DataStructure* newOne = copyElements(right);
    this->Start = newOne->Start;
    this->size = newOne->size;
    return *this;
}

// Task 10
int DataStructure::operator==(DataStructure& other) {
    if (this->size != other.getItemsNumber()) { return 0; }
    std::vector <ITEM3> thisItems = this->getAllItems();
    std::vector <ITEM3> otherItems = other.getAllItems();
    for (int i = 0; i < thisItems.size(); i++) {
        if (!ItemsHandler::areSame(&thisItems[i], &otherItems[i])) { return 0; }
    }

    return 1;
}

// Task 11
void DataStructure::write(char* filename) {
    FILE* pFile = fopen(filename, "wb");
    if (!pFile) { cout << "File not opened" << endl; throw exception("File Error"); } // return; }

    std::vector <ITEM3> itemsToBeWritten = this->getAllItems();
    char* totalNum = (char*)malloc(sizeof(int));

    memcpy(totalNum, &this->size, sizeof(int));
    fwrite(totalNum, 1, sizeof(int), pFile);
    for (int i = 0; i < this->size; i++) {
        size_t n = 0;
        char* serialized = ItemsHandler::serializeItem(&itemsToBeWritten[i], &n);
        fwrite(serialized, 1, n, pFile);
    }

    fclose(pFile);
}


void coursework1() {
    // TEST CASES START //
    cout << "=--------------------------- COURSEWORK 1 --------------------------=" << endl;

    int testcase = 1;
    testcase = endline(testcase);

    // TEST CASE 1 START //
    // Sets the number of items to 30 and prints the data structure.

    DataStructure* structure = new DataStructure(GetStruct2(3, 30), 30);
    structure->printDataStructure();

    // TEST CASE 1 STOP //

    testcase = endline(testcase);

    // TEST CASE 2 START //
    // One after another inserts new items with identifiers: Z A, Z Z, Z K, A Z, A A, A K, G Z, G A, G K, M A, M Ba, M Bb, M Z.
    char insert[][13] = { "Z A", "Z Z", "Z K", "A Z", "A A", "A K", "G Z", "G A", "G K", "M A", "M Ba", "M Bb", "M Z" };

    int len = sizeof(insert) / sizeof(insert[0]); // detect the length of insert array

    for (int i = 0; i < len; i++) {
        try {
            structure->insertItem(insert[i]);
        }
        catch (const exception& msg) {
            cerr << msg.what() << endl;
        }
    } // Append to structure from dict

    // Check that the new size is eq to 43
    if (structure->getItemsNumber() == 43) {
        cout << "Done" << endl;
    }
    else {
        cout << "The size of structure is not eq to 43. Please verify." << endl;
    }
    // PS. To insert an item from the given item list, use structure->insertItem();
    // TEST CASE 2 STOP //

    testcase = endline(testcase);

    // TEST CASE 3 START //
    // Trys to insert items with identifier M Ba (already exists) and Mba(illegal format) and prints the error messages.
    try {
        structure->insertItem((char*)"M Ba");
    }
    catch (const exception& msg) {
        cerr << msg.what() << endl;
    }// Rised exception; id already in dict

    try {
        structure->insertItem((char*)"Mba");
    }
    catch (const exception& msg) {
        cerr << msg.what() << endl;
    } // Rised exception; error formatting

    // TEST CASE 3 STOP //

    testcase = endline(testcase);

    // TEST CASE 4 START //
    // Prints the new data structure of 43 items.

    structure->printDataStructure(); // New data structure print

    // TEST CASE 4 STOP //

    testcase = endline(testcase);

    // TEST CASE 5 START //
    // One after another removes the items that were just inserted.

    for (int i = 0; i < len; i++) {
        try {
            structure->removeItem(insert[i]);
        }
        catch (const exception& msg) {
            cerr << msg.what() << endl;
        }
    } // remoiving items, we previously added

    // TEST CASE 5 STOP //

    testcase = endline(testcase);

    // TEST CASE 6 START //
    // Trys to remove items with identifier M Ba and Mba and prints the error messages.
    try {
        structure->removeItem((char*)"M Ba");
    }
    catch (const exception& msg) {
        cerr << msg.what() << endl;
    }

    try {
        structure->removeItem((char*)"Mba");
    }
    catch (const exception& msg) {
        cerr << msg.what() << endl;
    }

    // TEST CASE 6 STOP //

    testcase = endline(testcase);

    // TEST CASE 7 START //
    // Prints the data structure.

    structure->printDataStructure();

    // TEST CASE 7 STOP //

    testcase = endline(0);

    // just in case
    structure->~DataStructure();
}

int coursework2()
{
    // TEST CASES START //
    cout << "=--------------------------- COURSEWORK 2 --------------------------=" << endl;

    int testcase = 1;
    //DataStructure* structure = new DataStructure(GetStruct2(3, 10), 10);

    testcase = endline(testcase);

    // TEST CASE 1 START //
    // Creates empty data structure.

    DataStructure* structure = new DataStructure();
    if (structure) {
        cout << "Done" << endl;
    }

    // TEST CASE 1 STOP //

    testcase = endline(testcase);

    // TEST CASE 2 START //
    // Inserts 10 items into data structure (in a simple loop that calls 10 times operator+). To create
    // items use function GetItem from DLL(read ICS0017DataSource.h)
    int noOfElements = 10;
    for (int i = 0; i < noOfElements; i++) {
        ITEM3* toBeAdded = (ITEM3*)GetItem(3);
        //ItemsHandler::printItem(toBeAdded);
        *structure += toBeAdded;
    }
    if (noOfElements == structure->getItemsNumber()) {
        cout << "Done" << endl;
    }

    // TEST CASE 2 STOP //

    testcase = endline(testcase);

    // TEST CASE 3 START //
    // Prints the resulting data structure.

    structure->printDataStructure();

    // TEST CASE 3 STOP //

    testcase = endline(testcase);

    // TEST CASE 4 START //
    // Prints the number of elements in data structure (using GetItemsNumber).

    cerr << "Number of items: " << structure->getItemsNumber() << endl;

    // TEST CASE 4 STOP //

    testcase = endline(testcase);

    // TEST CASE 5 START //
    // Retrieves item with ID Light Cyan.

    structure->getItem(*structure, (char*)"Light Cyan");

    // TEST CASE 5 STOP //

    testcase = endline(testcase);

    // TEST CASE 6 START //
    // Tries to retrieve non-existing element X X.

    structure->getItem(*structure, (char*)"X X");

    // TEST CASE 6 STOP //

    testcase = endline(testcase);

    // TEST CASE 7 START //
    // Using the copy constructor creates the copy of current structure.

    cout << "New struct: " << endl;
    DataStructure anotherStructure;
    anotherStructure = *structure;
    anotherStructure.printDataStructure();

    // TEST CASE 7 STOP //

    testcase = endline(testcase);

    // TEST CASE 8 START //
    // Removes items Banana Mania, Persian Green and Vegas Gold from the initial structure.

    *structure -= (char*)"Banana Mania";
    *structure -= (char*)"Persian Green";
    *structure -= (char*)"Vegas Gold";
    structure->printDataStructure();


    // TEST CASE 8 STOP //

    testcase = endline(testcase);

    // TEST CASE 9 START //
    // Compares the initial structure (now with 7 items) with the copy structure.

    switch (*structure == anotherStructure) {
    case(false):
        cout << ("Structures are not identical") << endl;
        break;
    case(true):
        cout << ("Structures are identical") << endl;
        break;
    default:
        cout << ("Structures are not identical") << endl;
        break;
    }
    // TEST CASE 9 STOP //

    testcase = endline(testcase);

    // TEST CASE 10 START //
    // Writes the initial structure (now 7 items) into a data file.

    structure->printDataStructure();
    // The folder should be created.
    // File could be created or will be rewritten otherwise.
    structure->write((char*)"C:\\CPP-Coursework\\dataset.dat");

    // TEST CASE 10 STOP //

    testcase = endline(testcase);

    // TEST CASE 11 START //
    // Creates a new structure from this data file and compares it with initial stucture.

    DataStructure* fromFile = new DataStructure((char*)"C:\\CPP-Coursework\\dataset.dat");

    switch (*structure == *fromFile) {
    case(false):
        cout << ("Structures are not identical") << endl;
        break;
    case(true):
        cout << ("Structures are identical") << endl;
        break;
    default:
        cout << ("Structures are not identical") << endl;
        break;
    }

    fromFile->printDataStructure();
    // TEST CASE 11 STOP //

    testcase = endline(testcase);

    // TEST CASE 12 START //
    // Asssigns to the structure just created (7 items) the copy created in step 7 (10 items) and prints
    // the result.

    anotherStructure = *fromFile; // or from *structure?
    anotherStructure.printDataStructure();

    // TEST CASE 12 STOP //

    // TEST CASES STOP //

    endline(0);

    // DESTRUCTOR CHECK
    // cout << "destructor check" << endl << endl;
    //structure->~DataStructure();
    //anotherStructure.~DataStructure();
    //fromFile->~DataStructure();

    // VERIFY THAT STRUCTURES ARE DESTROYED
    //structure->printDataStructure();
    //anotherStructure.printDataStructure();
    //fromFile->printDataStructure();

    // TRYING TO COMPARE TWO STRUCTURES WITH SIMILAR ITEMS
    // (ORDER MATTERS)
    //cout << (*structure == anotherStructure) << endl;
    //structure->insertItem((char*)"Game Over");
    //structure->insertItem((char*)"Does It");
    //cout << (*structure == anotherStructure) << endl;
    // 
    //anotherStructure.insertItem((char*)"Does It");
    //anotherStructure.insertItem((char*)"Game Over");
    // 
    //cout << (*structure == anotherStructure) << endl;

    return 0;
}


int main() {
    coursework1();
    coursework2();

    return 0;
}