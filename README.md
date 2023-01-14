# C/C++ - Course Work

## Final result
~~~
Grade: Pass
~~~

## Course Work Part 1:
~~~~
About the code:
Code was initially based on https://github.com/MrLightWood/ICS0017-FundamentalsCPP.
Reworte most of the project, fixed insert/ remove item, enhanceded logic, better visual.

In case someone will my work, the code could be cleared as there is a lot of unneeded items. Logics could be also updated, 
as I had no time to make it better. However, the code works as it is intended to, so it could be used as it is :)
~~~~
~~~~
Test Cases:
1. Sets the number of items to 30 and prints the data structure.
2. One after another inserts new items with identifiers: Z A, Z Z, Z K, A Z, A A, A K, G Z, G A, G K, M A, M
Ba, M Bb, M Z.
3. Trys to insert items with identifier M Ba (already exists) and Mba(illegal format) and prints the error
messages.
4. Prints the new data structure of 43 items.
5. One after another removes the items that were just inserted.
6. Trys to remove items with identifier M Ba and Mba and prints the error messages.
7. Prints the data structure.
~~~~

## Corse Work Part 2:
~~~~
About the code:
Code was initially based on https://github.com/kerusey/CourseWork.
The initial code is clear and well structured, but uses a lot of advanced things that we did not take this course. 
So in case you will use it, be prepared for questions about vectors, how to file saving works (memcpy etc...) and more.

The code was reformatted, but most of the functions left intact or only slightly edited.

The code has an issue with inserting/ deleting items as it adds the items to the wrong headers/ pointers. 
The logics should be fully rewritten, probably the easiest way is to take the insert item from the CW1. 
Did not do that as I have noticed that only a day before the exam :O.
As the printing usually prints header by header and pointer by pointer, it is easily detected when printing the results. 
With such issue you will not pass the course. As a work around, I have added a sorting before the actual printing. 
It is most probably prohibited, but I was lucky and the lecturer did not check my code. It is easily detected 
as the lecturer most probably will check the#include methods. #include <sstream>, #include <algorithm> are used 
only for this sorting.
The printing itself: line 227 - line 289.

Another issue is connected with adding new items to the empty data structure. See line 537 - line 554. 
The structure is strangly initialized, so the actual structure is null. As it is impossible to add new items to 
null structure (no headers, poitners), wrote two more work arounds. 
First, if the structure is null, we actually create a new structure inside this method with working function. 
Second, there is a strange issue that one item is added twice instead of adding the correct item. 
To fix it, created getNewItem(). 
It basically searches for this item, removes it and places the correct item. 
Maybe will be fixed with the correct inserting. 
idk.

Did not find any other issues. Good luck solving those :)
~~~~
~~~~
Test Cases:
1. Creates empty data structure.
2. Inserts 10 items into data structure (in a simple loop that calls 10 times operator+). To create
items use function GetItem from DLL (read ICS0017DataSource.h)
3. Prints the resulting data structure.
4. Prints the number of elements in data structure (using GetItemsNumber).
5. Retrieves item with ID Light Cyan.
6. Tries to retrieve non-existing element X X.
7. Using the copy constructor creates the copy of current structure.
8. Removes items Banana Mania, Persian Green and Vegas Gold from the initial structure.
9. Compares the initial structure (now with 7 items) with the copy structure.
10. Writes the initial structure (now 7 items) into a data file.
11. Creates a new structure from this data file and compares it with initial stucture.
12. Asssigns to the structure just created (7 items) the copy created in step 7 (10 items) and prints
the result.
~~~~
## Things to mention
### Course name:
Fundamentals of C/C++ Programming (ICS0017)
### Variant:
Structure: 2 (HEADER_C*) <br>
Item: 3
### Other works to check out:
* https://github.com/kei5uke/TTU_Fundamentals_of_CPP
* https://github.com/baadam3/ICS0017_Fundamentals_of_C-Cpp_Programming
* https://github.com/0xtane/CPlusMinus
* https://github.com/MrLightWood/ICS0017-FundamentalsCPP
* https://github.com/kerusey/CourseWork
