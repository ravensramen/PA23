//name: main.c
//description: main menu for music player

#include "header.h"

int main() {
    printf("Welcome to bootleg spotify - spootify!\n");

    printf("Here are your options, enter the corresponding number to make a choice: \n");

    FILE* input = fopen("musicPlayList.csv", "r"); //open file for reading
    FILE* output = fopen("musicPlaylistOut.csv", "w"); 

    Node* pHead = NULL; //initialize head of linked list
    int choice = 0; //menu choice

    if (input == NULL) {
        printf("Failed to open the music file.\n");
        return 1;
    }

    loadMusic(input, &pHead);  //save music files into list of structs

    do {
        choice = displayMenu();

        switch (choice) {
        case 1:
            storeMusic(pHead, output); //write nodes to output file
            printf("Your library has been saved.\n\n");
            system("pause");
            system("cls");
            continue; //continue to menu
        case 2:
        {
            int displayChoice;
            printf("Choose display option:\n");
            printf("1. Display all records\n");
            printf("2. Display by artist\n");
            printf("Enter your choice: ");
            scanf("%d", &displayChoice);

            if (displayChoice == 1) {
                //display all
                printList(pHead);
            }
            else if (displayChoice == 2) {
                //display by artist
                char artistName[50];
                printf("Enter the artist name: ");
                getchar();
                fgets(artistName, sizeof(artistName), stdin);
                artistName[strcspn(artistName, "\n")] = '\0';

                searchByArtist(pHead, artistName);
            }
            else {
                printf("Invalid choice, try again.\n");
            }
            system("pause");
            system("cls");
        }
        continue;
        case 3:
            editRecord(&pHead);
            continue;
        case 4:
            rateRecord(&pHead);
            continue;
        case 5:
            playRecord(&pHead);
            continue;
        case 6: //insertRecord(&pHead);//insert at front
            testInsertFunction();
        case 7: deleteRecord(&pHead);
        case 8: sortRecord(&pHead);
        case 9: shuffleLibrary(&pHead);
        case 10: //exit
            system("cls");
            printf("Thank you for using spootify! See you next time :P\n");
            // Exit function: Exit the program
            break;
        }

    } while (choice !=10);

    fclose(input); // Close the file when the program ends
    return 0;
}
