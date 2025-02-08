//file: functions.c
//function: stores all the definitions of user defined functions

#include "header.h"

//name: displayMenu
//desc: displays menu to console, stores user menu choice
//parameters: n/a
//output: menu choice
int displayMenu(void) {
	int choice = 0;

	printf("\n1. Store music from my file\n2. Display my library\n3. Edit my library\n4. Rate a record\n5. Play a record\n6. Insert record");
	printf("\n7. Delete a record\n8. Sort my library\n9. Shuffle my library\n10. Exit\n");
	scanf("%d", &choice);
	return choice;
}

//name:loadMusic
//desc: stores music from csv file into linked list
//parameters: input file, address of head of list
//output: n/a (indirectly updates list)

void loadMusic(FILE* input, Node** pHead) {
	char buffer[100];
	initList(pHead);  // initialize the list

	while (fgets(buffer, sizeof(buffer), input) != NULL) {
		doublyLinkList(pHead, buffer);  // add to list
	}
}

//name: creatNode
//desc: allocates memory for a node and stores struct data
//parameters: buffer of data to be stored
//output: node coresponding to a record 

Node* createNode(char* buffer) {
	Node* new_node = (Node*)malloc(sizeof(Node)); // alocate memory for each node

	if (new_node == NULL) {
		printf("failed to save record data.\n");
		return NULL;
	}

	strcpy(new_node->record.Artist, myStrtok(buffer, ','));
	strcpy(new_node->record.AlbumTitle, myStrtok(NULL, ','));
	strcpy(new_node->record.SongTitle, myStrtok(NULL, ','));
	strcpy(new_node->record.Genre, myStrtok(NULL, ','));

	// extract and convert duration to integers
	char* dur = myStrtok(NULL, ',');
	Duration song_duration = convertDuration(dur);
	new_node->record.duration = song_duration;

	char* timesPlayed = myStrtok(NULL, ',');
	new_node->record.TimesPlayed = atoi(timesPlayed);

	char* recordRating = myStrtok(NULL, ',');
	new_node->record.Rating = atoi(recordRating);

	return new_node;
}

//name: myStrtok
//desc: performs a strtok, but takes in account artist names that include commas too
//parameters: buffer line, delim character
//output: token to the start of each data field

char* myStrtok(char* buffer, char delim) {
	static char* ptr = NULL; // keeps track of where we are in the buffer
	char* token = NULL;

	if (buffer != NULL) {
		ptr = buffer;  // reset ptr to the start of the new string
	}

	if (ptr == NULL) {
		return NULL;
	}

	// skip any leading spaces or delimiters
	while (*ptr == ' ' || *ptr == delim) {
		ptr++;
	}

	if (*ptr == '\0') {
		return NULL; 
	}

	token = ptr; // mark the beginning of the token

	// check for quotes at the beginning of the token (for multi-word names)
	if (*ptr == '\"') {
		ptr++; // skip the opening quote
		token = ptr; 
		while (*ptr != '\"' && *ptr != '\0') {
			ptr++;
		}
		if (*ptr == '\"') {
			*ptr = '\0';
			ptr++;
		}
	}
	else {
		// otherwise, just move the pointer until the delimiter is found
		while (*ptr != delim && *ptr != '\0') {
			ptr++;
		}

		if (*ptr != '\0') {
			*ptr = '\0'; // null-terminate the token at the delimiter
			ptr++; // move past the delimiter for the next token
		}
	}

	return token;
}

//name: convertDuration
//desc: converts string input to integers (to be stored in duration struct)
//parameters: input string
//output: duration (ints of min and sec)

Duration convertDuration(char* input) {
	Duration duration = { 0,0 };
	char* token = strtok(input, ":");

	if (token != NULL) {
		duration.min = atoi(token);
	}
	token = (strtok(NULL, ":"));
	if(token!=NULL){
		duration.seconds = atoi(token); 
	}
	return duration;
}

//name: initList
//desc: initializes doubly linekd list
//parameters: head pointer of list
//output: n/a, null initialized list

void initList(Node** pList) {
	*pList = NULL;
}

//name: doublyLinkList
//desc: creates and links each node to the previous and next record
//parameters: head address of list and 
//output: n/a, updates og list to include new nodes

void doublyLinkList(Node** pList, char* buffer) {
	Node* pCur = *pList;
	Node* pMem = createNode(buffer); // create a new node from the input buffer

	// empty list
	if (pCur == NULL) {
		*pList = pMem; // first node set as head
		pMem->pPrev = pMem; 
		pMem->pNext = pMem; 
	}
	//non-empty list
	else {
		// traverse to the last node
		while (pCur->pNext != *pList) { 
			pCur = pCur->pNext;
		}

		// double linking, circular from start to end
		pCur->pNext = pMem;  
		pMem->pPrev = pCur;  
		pMem->pNext = *pList; //circular link
		(*pList)->pPrev = pMem; 
	}
}

//name: printList
//desc: prints contents of the library linked list
//parameters: address of first item of list
//output: n/a, displays to console

void printList(Node* pList) {
	Node* pCur = pList;

	if (pCur == NULL) {
		printf("The list is empty.\n");
		return; 
	}

	// traverse the list and print each record
	while (pCur != NULL) {

		printf("Artist: %s\n", pCur->record.Artist);
		printf("Album Title: %s\n", pCur->record.AlbumTitle);
		printf("Song Title: %s\n", pCur->record.SongTitle);
		printf("Genre: %s\n", pCur->record.Genre);
		printf("Duration: %d:%d\n", pCur->record.duration.min, pCur->record.duration.seconds);
		printf("Times Played: %d\n", pCur->record.TimesPlayed);
		printf("Rating: %d\n", pCur->record.Rating);

		// Move to the next node
		pCur = pCur->pNext;

		printf("-------------\n");
	}
}

//name: searchByArtist
//desc: if user wants to play records by specific artist, search list to find correct records 
//parameters: address of first item, artist name
//output: n/a, prints applicable records to console

void searchByArtist(Node* pList, const char* artistName) {
	Node* pCur = pList;
	int found = 0;  // flag to check if any match is found

	// Traverse the list
	while (pCur != NULL) {
		if (strcmp(pCur->record.Artist, artistName) == 0) {
			// If there's a match, print the record
			printf("Artist: %s\n", pCur->record.Artist);
			printf("Album Title: %s\n", pCur->record.AlbumTitle);
			printf("Song Title: %s\n", pCur->record.SongTitle);
			printf("Genre: %s\n", pCur->record.Genre);
			printf("Duration: %d:%d\n", pCur->record.duration.min, pCur->record.duration.seconds);
			printf("Times Played: %d\n", pCur->record.TimesPlayed);
			printf("Rating: %d\n", pCur->record.Rating);
			printf("-------------\n");
			found = 1;  // Set found flag to true
		}
		pCur = pCur->pNext;
	}

	if (!found) {
		printf("No records found for artist: %s\n\ntip: if artist has two names, enter 'last, first',\nalso make sure you've loaded your library already!\n\n", artistName);  // If no match is found
	}
}

//name: storeMusic
//desc: stores music from updated linked list into the output file
//parameters: address of first item, address of output
//output: n/a, prints message to console, prints results to output file

void storeMusic(Node* pList, FILE* output) {

	// Check if the list is empty
	if (pList == NULL) {
		printf("The list is empty, nothing to store.\n");
		fprintf(output, "Library is empty\n");
		return; // Exit the function if the list is empty
	}
	Node* pHead = pList; //save head
	// traverse the list and write each record to the file
	while (pList->pNext != pHead) { //the last node points to the first
		fprintf(output, "Artist: %s, ", pList->record.Artist);
		fprintf(output, "Album: %s, ", pList->record.AlbumTitle);
		fprintf(output, "Song: %s, ", pList->record.SongTitle);
		fprintf(output, "Genre: %s, ", pList->record.Genre);
		fprintf(output, "Duration: %d:%d, ", pList->record.duration.min, pList->record.duration.seconds);
		fprintf(output, "Times Played: %d, ", pList->record.TimesPlayed);
		fprintf(output, "Rating: %d\n\n", pList->record.Rating);

		pList = pList->pNext;
	}
	printf("Music data has been stored successfully.\n");
}

//name: editRecord
//desc: finds record selected by user
//parameters: address of first list item
//output: n/a, user can indirectly access list contents

void editRecord(Node** pList) {
	system("cls");
	char artist[100];
	printf("Enter artist name to edit their record: \n");
	getchar();  
	fgets(artist, sizeof(artist), stdin);
	artist[strcspn(artist, "\n")] = '\0'; 

	Node* pCur = *pList;
	int index = 1;
	int found = 0;

	// search through the list for records matching the artist
	while (pCur != NULL) {
		if (strcmp(pCur->record.Artist, artist) == 0) {
			printf("%s by %s found in your library\n", pCur->record.SongTitle, pCur->record.Artist);
			editField(pCur);

			found = 1;
			index++;
		}
		pCur = pCur->pNext;
	}

	//no records were found
	if (!found) {
		printf("No records found for artist: %s\n", artist);
		return;
	}
}

//name: editField
//desc: user can edit desired field
//parameters: address of node to be edited
//output: n/a, indirectly edits node

void editField(Node* pCur) {
	int option;

	// which field
	printf("Which field would you like to edit?\n");
	printf("1. Artist\n");
	printf("2. Album Title\n");
	printf("3. Song Title\n");
	printf("4. Genre\n");
	printf("5. Duration (min:sec)\n");
	printf("6. Times Played\n");
	printf("7. Rating\n");
	printf("8. Back to menu\n");
	printf("Enter your choice (1-8): ");
	scanf("%d", &option);
	getchar();  

	switch (option) {
	case 1:
		printf("Enter new Artist: ");
		fgets(pCur->record.Artist, sizeof(pCur->record.Artist), stdin);
		pCur->record.Artist[strcspn(pCur->record.Artist, "\n")] = '\0';  // remove newline character
		break;
	case 2:
		printf("Enter new Album Title: ");
		fgets(pCur->record.AlbumTitle, sizeof(pCur->record.AlbumTitle), stdin);
		pCur->record.AlbumTitle[strcspn(pCur->record.AlbumTitle, "\n")] = '\0';
		break;
	case 3:
		printf("Enter new Song Title: ");
		fgets(pCur->record.SongTitle, sizeof(pCur->record.SongTitle), stdin);
		pCur->record.SongTitle[strcspn(pCur->record.SongTitle, "\n")] = '\0';
		break;
	case 4:
		printf("Enter new Genre: ");
		fgets(pCur->record.Genre, sizeof(pCur->record.Genre), stdin);
		pCur->record.Genre[strcspn(pCur->record.Genre, "\n")] = '\0';
		break;
	case 5:
	{
		int min, sec;
		printf("Enter new Duration (min:sec): ");
		scanf("%d:%d", &min, &sec);
		pCur->record.duration.min = min;
		pCur->record.duration.seconds = sec;
	}
	break;
	case 6:
		printf("Enter new Times Played: ");
		scanf("%d", &pCur->record.TimesPlayed);
		break;
	case 7:
		printf("Enter new Rating: ");
		scanf("%d", &pCur->record.Rating);
		break;
	case 8:
		printf("Edit canceled.\n");
		return;
	default:
		printf("Invalid choice.\n");
		break;
	}

	printf("Record updated successfully.\n");
	system("cls");
}

//name: rateRecord
//desc: allows user to rate specific artist
//parameters: address of first item in list
//output: n/a, indirectly modify rating of node

void rateRecord(Node* pList) {
	char artist[100];
	printf("Enter artist name to rate their song: ");
	getchar();  
	fgets(artist, sizeof(artist), stdin);
	artist[strcspn(artist, "\n")] = '\0';  

	Node* pCur = pList;
	int index = 1;
	int found = 0;

	// search through the list for records matching the artist
	while (pCur != NULL) {
		if (strcmp(pCur->record.Artist, artist) == 0) {
			// Print out the matching record
			printf("Record #%d:\n", index);
			printf("Artist: %s\n", pCur->record.Artist);
			printf("Album Title: %s\n", pCur->record.AlbumTitle);
			printf("Song Title: %s\n", pCur->record.SongTitle);
			printf("Genre: %s\n", pCur->record.Genre);
			printf("Duration: %d:%d\n", pCur->record.duration.min, pCur->record.duration.seconds);
			printf("Times Played: %d\n", pCur->record.TimesPlayed);
			printf("Current Rating: %d\n", pCur->record.Rating);
			printf("-------------\n");
			found = 1;
			index++;
		}
		pCur = pCur->pNext;
	}

	// if no records were found
	if (!found) {
		printf("No records found for artist: %s\n", artist);
		return;
	}

	// orompt the user to choose which record to rate
	int choice;
	printf("Enter the record number to rate: ");
	scanf("%d", &choice);

	pCur = pList;
	index = 1;

	// traverse again to find the selected record
	while (pCur != NULL) {
		if (index == choice) {
			int newRating;
			printf("Enter a new rating for this song (1-5): ");
			scanf("%d", &newRating);

			// validate rating input
			if (newRating < 1 || newRating > 5) {
				printf("Invalid rating! Please enter a rating between 1 and 5.\n");
				return;
			}

			// update the rating
			pCur->record.Rating = newRating;
			printf("Rating updated successfully to %d.\n", newRating);
			return;
		}
		pCur = pCur->pNext;
		index++;
	}

	printf("Invalid choice!\n");
}

//name: playRecord
//desc: simulates playing the library
//parameters: address of first item in list
//output: n/a, displays desired record to the console

void playRecord(Node** pHead) {

	int num;
	char next = NULL;
	int msDur = 0;

	printf("Would you like to: \n1. Play library in order\n2. Play specific song\n");
	scanf("%d", &num);
	getchar(); 

	if (num == 2) {
		char str[100];
		printf("Insert song title: \n");
		fgets(str, 100, stdin);
		str[strcspn(str, "\n")] = '\0'; 

		Node* current = *pHead; // use a temporary pointer to traverse the list
		int found = 0; // flag to check if the song was found

		while (current != NULL) {
			if (strcmp(str, current->record.SongTitle) == 0) {
				printf("Playing %s\n", current->record.SongTitle);
				current->record.TimesPlayed+1; //increments times played to accurately count total

				msDur = DurToMilli(current->record.duration);
				Sleep(msDur); 

				found = 1; // song found
				break;
			}
			current = current->pNext; // move to the next node
		}

		if (!found) {
			printf("Record not found in your library... maybe you can add it.\n");
		}

		system("pause"); 
	}
	else if (num == 1) {
		Node* current = *pHead; // use a temporary pointer to traverse the list

		while (current != NULL) {
			printf("Playing %s\n", current->record.SongTitle);
			msDur = DurToMilli(current->record.duration);
			current->record.TimesPlayed++;
			Sleep(msDur);
			printf("End of Song\n");

			system("pause");
			system("cls"); 
			
			current = current->pNext; 
	
		}
		printf("You reached the end of your library!\nEnter 1. to replay, enter 2 to exit.\n");
		int endchoice = 0;
		scanf("%d", &endchoice);
		if (endchoice == 1) {
			playRecord(pHead); 


		}
		if (endchoice == 2) {
			return;
		}
	}
	else {
		printf("Invalid input... :(\n");
	}
}

//name: clearInputBuffer
//desc: helps when inserting integer fields of song struct
//parameters: n/a
//output: n/a

void clearInputBuffer() {
	while (getchar() != '\n'); // Discard all characters until newline
}

//name: insertRecord
//desc: user can manually insert a song
//parameters: address of first node
//output: adds new record to the list

//SEPARATE INTO CREATE NODE, SEND NODE TO HEAD AFTER
void insertRecord(Node** pHead) {

	bool valid = true; //makes sure all fields are correct
	Node* pMem = malloc(sizeof(Node));
	if (pMem == NULL) {
		printf("Memory allocation failed!\n");
		return;
	}

	clearInputBuffer();

	printf("Artist name: ");
	fgets(pMem->record.Artist, sizeof(pMem->record.Artist), stdin);
	pMem->record.Artist[strcspn(pMem->record.Artist, "\n")] = '\0'; // remove newline

	printf("Album title: ");
	fgets(pMem->record.AlbumTitle, sizeof(pMem->record.AlbumTitle), stdin);
	pMem->record.AlbumTitle[strcspn(pMem->record.AlbumTitle, "\n")] = '\0'; 

	printf("Song title: ");
	fgets(pMem->record.SongTitle, sizeof(pMem->record.SongTitle), stdin);
	pMem->record.SongTitle[strcspn(pMem->record.SongTitle, "\n")] = '\0'; 

	printf("Genre: ");
	fgets(pMem->record.Genre, sizeof(pMem->record.Genre), stdin);
	pMem->record.Genre[strcspn(pMem->record.Genre, "\n")] = '\0'; 

	printf("Duration MIN: ");
	scanf("%lf", &pMem->record.duration.min);
	clearInputBuffer(); // clear the input buffer

	printf("Duration SEC: ");
	scanf("%d", &pMem->record.duration.seconds);
	clearInputBuffer(); 

	printf("Times Played: ");
	scanf("%d", &pMem->record.TimesPlayed);
	if ((&pMem->record.TimesPlayed) < 0) {
		valid = false; //mark node as invalid, prevent from adding later
	}
	clearInputBuffer(); 

	printf("Rating (1-5): ");
	scanf("%d", &pMem->record.Rating);
	if ((&pMem->record.Rating) > 5 || &pMem->record.Rating < 0) {
		valid = false; //mark as invalid
	}
	clearInputBuffer(); 

	if (valid == true) {
		// link the new node to the list
		pMem->pNext = *pHead;
		if (*pHead != NULL) {
			(*pHead)->pPrev = pMem;
		}
		*pHead = pMem;
	}

	else {
		printf("That track is invalid, please try again.\n");
	}

	system("cls"); 
	printf("Your record has been added to your library.\n");
}

//name: deleteRecord
//desc: deletes a selected record from the list, relinks surrounding nodes
//parameters: address of first ndoe
//output: n/a, indirectly modifies list

void deleteRecord(Node** pHead) {
	char record[100]; // for target title
	int found = 0;
	system("cls");
	printf("What is the title of the record you want to delete? \n");
	getchar(); 
	fgets(record, 100, stdin);

	// remove the newline character from the input
	record[strcspn(record, "\n")] = '\0';

	Node* current = *pHead;
	Node* temp = NULL;

	while (current != NULL && found == 0) {
		if (strcmp(current->record.SongTitle, record) == 0) {
			found = 1;

			// if the node to be deleted is the head node
			if (current == *pHead) {
				*pHead = current->pNext;
				if (*pHead != NULL) {
					(*pHead)->pPrev = NULL;
				}
			}
			else {
				// update the next node's pPrev if it exists
				if (current->pNext != NULL) {
					current->pNext->pPrev = current->pPrev;
				}
				// update the previous node's pNext if it exists
				if (current->pPrev != NULL) {
					current->pPrev->pNext = current->pNext;
				}
			}

			// free the memory of the node to be deleted
			free(current);
			printf("Record successfully deleted.\n");
		}
		else {
			current = current->pNext;
		}
	}

	if (found == 0) {
		printf("Record not found, no need to delete it!\n");
	}
	system("pause");
	system("cls");
}
//name: sortRecord
//desc: sorts records based on user desired method
//parameters: address of first node in list
//output: n/a, indirectly modifies/sorts list
void sortRecord(Node** pHead) {
	int choice = 0;
	char field = NULL;
	system("cls");
	printf("How would you like to sort your library?");
	printf("\n1. Artist Name\n2. Album Title \n3. Rating\n4. Times Played\n");
	scanf("%d", &choice);

	if (choice == 1 || choice == 2) {
		if (choice == 1) {
			field = 'n';
		}
		if (choice == 2) {
			field = 't';
		}
		alphaSort(pHead, field);
		return;
	}
	if (choice == 3 || choice == 4) {
		if (choice == 3) {
			field == 'r';
		}
		if (choice == 4) {
			field = 'p';
		}
		numSort(pHead, field);
		return;
	}
	//returns edited list
}


//name: alphaSort
//desc: sorts based on alphabetical comparison
//parameters: address of first item, char coresponding to sort type
//output: sorted list

//REFERENCED GEEKSFORGEEKS BUBBLE SORT
void alphaSort(Node** pHead, char field) {
	int len = detLength(pHead);
	int start = 0;
	int swapped;

	if (field = 'n') {
		while (start < len) {
			Node* traverseNode = *pHead;
			Node* prevNode = NULL;

			swapped = 0;
			while (traverseNode->pNext != NULL) {
				Node* ptr = traverseNode->pNext; // next node

				if (strcmp(traverseNode->record.Artist, ptr->record.Artist) > 0) { // second node comes before
					swapped = 1;

					// Swap the nodes
					traverseNode->pNext = ptr->pNext;
					if (ptr->pNext != NULL) {
						ptr->pNext->pPrev = traverseNode;
					}
					ptr->pNext = traverseNode;
					ptr->pPrev = traverseNode->pPrev;
					traverseNode->pPrev = ptr;

					if (ptr->pPrev != NULL) {
						ptr->pPrev->pNext = ptr;
					}
					else {
						*pHead = ptr; // Update the head if necessary
					}

					// Update prevNode to point to the new node
					prevNode = ptr;
				}
				else {
					prevNode = traverseNode;
					traverseNode = traverseNode->pNext; // go on to compare the next pair
				}
			}
			if (!swapped) {
				break;
			}
			++start;
		}
	}

	if (field = 't') {
		while (start < len) {
			Node* traverseNode = *pHead;
			Node* prevNode = NULL;

			swapped = 0;
			while (traverseNode->pNext != NULL) {
				Node* ptr = traverseNode->pNext; // next node

				if (strcmp(traverseNode->record.AlbumTitle, ptr->record.AlbumTitle) > 0) { // second node comes before
					swapped = 1;

					// Swap the nodes
					traverseNode->pNext = ptr->pNext;
					if (ptr->pNext != NULL) {
						ptr->pNext->pPrev = traverseNode;
					}
					ptr->pNext = traverseNode;
					ptr->pPrev = traverseNode->pPrev;
					traverseNode->pPrev = ptr;

					if (ptr->pPrev != NULL) {
						ptr->pPrev->pNext = ptr;
					}
					else {
						*pHead = ptr; // Update the head if necessary
					}

					// Update prevNode to point to the new node
					prevNode = ptr;
				}
				else {
					prevNode = traverseNode;
					traverseNode = traverseNode->pNext; // go on to compare the next pair
				}
			}
			if (!swapped) {
				break;
			}
			++start;
		}
	}

}

//name: numSort
//desc: sorts based on number values
//parameters: address of first item, char coresponding to field
//output: sorted list

void numSort(Node** pHead, char field) {
	printf("numbers");
	int length = 0;
	int start = 0;
	int swapped = 0;
	length = detLength(pHead);

	if (field = 'r') {
		while (start < length) {
			Node* traverseNode = *pHead;
			Node* prevNode = NULL;
			swapped = 0;

			while (traverseNode->pNext != NULL) {
				Node* ptr = traverseNode->pNext;
				if ((traverseNode->record.Rating) < (traverseNode->pNext->record.Rating)) {
					swapped = 1;
					traverseNode->pNext = ptr->pNext;
					if (ptr->pNext != NULL) {
						ptr->pNext->pPrev = traverseNode;
					}
					ptr->pNext = traverseNode;
					ptr->pPrev = traverseNode->pPrev;
					traverseNode->pPrev = ptr;
					if (ptr->pPrev != NULL) {
						ptr->pPrev->pNext = ptr;
					}
					else {
						*pHead=ptr;
					}
					prevNode = ptr;

				}
				else {
					prevNode = traverseNode;
					traverseNode = traverseNode->pNext;
				}
			}
			if (!swapped) {
				break;
			}

			++start;
		}
	}

	if (field = 'p') {
		while (start < length) {
			Node* traverseNode = *pHead;
			Node* prevNode = NULL;
			swapped = 0;

			while (traverseNode->pNext != NULL) {
				Node* ptr = traverseNode->pNext;
				if ((traverseNode->record.TimesPlayed) < (traverseNode->pNext->record.TimesPlayed)) {
					swapped = 1;
					traverseNode->pNext = ptr->pNext;
					if (ptr->pNext != NULL) {
						ptr->pNext->pPrev = traverseNode;
					}
					ptr->pNext = traverseNode;
					ptr->pPrev = traverseNode->pPrev;
					traverseNode->pPrev = ptr;
					if (ptr->pPrev != NULL) {
						ptr->pPrev->pNext = ptr;
					}
					else {
						*pHead = ptr;
					}
					prevNode = ptr;

				}
				else {
					prevNode = traverseNode;
					traverseNode = traverseNode->pNext;
				}
			}
			if (!swapped) {
				break;
			}

			++start;
		}
	}

}

//name: detLength
//desc: determines length of the linked list
//parameters: address of first link
//output: int of number of loop iterations to encounter end (or num of items in list)

int detLength(Node** pHead) {
	int l = 0;

	Node* pCur = *pHead;
	while (pCur != NULL) {
		l++;
		pCur = pCur->pNext;
	}
	
	return l;
}

//name: shuffleLibrary
//desc: randomly shuffles records
//parameters: address of first item
//output: n/a, displays record to console, simulate random playing

void shuffleLibrary(Node** pHead) {
	srand(time(NULL)); 
	int l = detLength(pHead); 

	if (l == 0) {
		printf("Your library is empty.\n");
		return;
	}

	//initialize array to track which items have been played
	char* played = (char*)calloc(l, sizeof(char)); // initialize all elements to 0
	if (played == NULL) {
		printf("Memory allocation failed.\n");
		return;
	}

	int allplayed = 0; // flag to check if ALL songs have been played
	system("cls");
	while (allplayed == 0) {
		int play = rand() % l; //FIX?

		if (played[play] == 0) { // if the song has not been played
			Node* pCur = *pHead; // start from the head of the list

			
			for (int i = 0; i < play; i++) {
				pCur = pCur->pNext;
			}

			printf("Playing %s by %s\n", pCur->record.SongTitle, pCur->record.Artist); // play the song
			played[play] = 1; // mark song as played
			system("pause");
			system("cls");
			// check if all songs have been played
			allplayed = 1;
			for (int i = 0; i < l; i++) {
				if (played[i] == 0) {
					allplayed = 0; // not all songs have been played
					break;
				}
			}
		}
	}

	free(played); // free the allocated memory
	system("pause");
	system("cls"); 
}

//name: DurToMilli
//desc: converts duration struct to milliseconds to simulate playing with sleep() function
//parameters: duration of item in list
//output: milliseconds duration

int DurToMilli(Duration dur) {
	int milliseconds = 0;
	milliseconds = dur.min * 60000;
	milliseconds += dur.seconds * 1000;
	return milliseconds;
}
