#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*Defines=============================================================================================================*/
#define oneSize 1
#define view 1
#define due 2
#define addTwo 2
#define addToList 2
#define chooseDelete 3
#define exitProgram 4
#define chooseSort 4
#define playAllSongs 5
#define exitToPlaylists 6
#define sizeOfNum 100
#define sortByYear 1
#define sortByAscend 2
#define sortByDescend 3
/*Structs=============================================================================================================*/
typedef struct Song
{
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;
typedef struct Playlist
{
    char* name;
    Song** songs;
    int songsNum;
} Playlist;
/*Playlists_Functions=================================================================================================*/
void freePlaylist(Playlist* playlist)
{
    if (playlist->songs) // Check if the playlist contains songs
    {
        for (int i = 0; i < playlist->songsNum; i++) // Iterate through all songs in the playlist
        {
            free(playlist->songs[i]->title);  // Free memory allocated for the song title
            free(playlist->songs[i]->artist); // Free memory allocated for the song artist
            free(playlist->songs[i]->lyrics); // Free memory allocated for the song lyrics
            free(playlist->songs[i]);         // Free the song structure itself
        }
        free(playlist->songs); // Free the array of song pointers
    }
    free(playlist->name); // Free the memory allocated for the playlist name
    free(playlist);       // Free the playlist structure itself
}
//
void freeAllPlaylists(Playlist** playlists, int playlistsNum)
{
    for (int i = 0; i < playlistsNum; i++) // Iterate through all playlists
    {
        freePlaylist(playlists[i]); // Free each individual playlist
    }
    free(playlists); // Free the array of playlist pointers
}
//
Playlist* createPlaylist(char* name)
{
    // Allocate memory for the Playlist structure
    Playlist* playlist = (Playlist*)malloc(sizeof(Playlist));
    if (!playlist) // Check if memory allocation failed
    {
        printf("Failed to allocate memory for Playlist\n");
        return NULL; // Return NULL to indicate failure
    }

    // Allocate memory for the playlist name (including space for the null terminator)
    playlist->name = (char*)malloc(strlen(name) + oneSize);
    if (!playlist->name) // Check if memory allocation failed
    {
        printf("Failed to allocate memory for playlist name\n");
        free(playlist); // Free the allocated playlist structure before returning
        return NULL; // Return NULL to indicate failure
    }

    strcpy(playlist->name, name); // Copy the provided name into the allocated memory
    playlist->songs = NULL;       // Initialize song array pointer to NULL
    playlist->songsNum = 0;       // Initialize the number of songs to zero

    return playlist; // Return the newly created playlist
}
//
void addPlaylist(Playlist*** playlists, int* playlistsNum, char* name)
{
    // Create a new playlist using the provided name
    Playlist* newPlaylist = createPlaylist(name);
    int numOfPlaylists = *playlistsNum; // Get the current number of playlists

    if (numOfPlaylists == 0) // If this is the first playlist being added
    {
        // Allocate memory for the playlist array
        *playlists = (Playlist**)malloc(sizeof(Playlist*));
        if (!*playlists) // Check if allocation failed
        {
            printf("Failed to allocate memory for playlist array!\n");
            return; // Exit the function without modifying playlistsNum
        }

        // Assign the new playlist to the array if it was successfully created
        if (newPlaylist != NULL)
            (*playlists)[numOfPlaylists] = newPlaylist;
    }
    else // If there are already existing playlists
    {
        // Expand the playlist array using realloc
        Playlist** temp = (Playlist**)realloc(*playlists, (numOfPlaylists + oneSize) * sizeof(Playlist*));
        if (!temp) // Check if reallocation failed
        {
            printf("Failed to allocate memory for playlists\n");
            freeAllPlaylists(*playlists, *playlistsNum); // Free all allocated playlists before exiting
            return;
        }

        if (newPlaylist != NULL) // Ensure the new playlist is valid before assigning
        {
            *playlists = temp; // Update the playlist array pointer

            // Shift the last playlist to make room for the new one
            (*playlists)[numOfPlaylists] = (*playlists)[numOfPlaylists - oneSize];
            (*playlists)[numOfPlaylists - oneSize] = newPlaylist;
        }
    }

    if (newPlaylist != NULL) // Increment the playlist count if a new playlist was added
        (*playlistsNum)++;
}
//
void displayPlaylists(Playlist** playlists, int playlistsNum)
{
    if (playlistsNum == 0) // Check if there are no playlists
    {
        printf("No playlists available. Add a playlist to get started.\n");
    }
    else // If there are playlists available
    {
        printf("Choose a playlist:\n");
        for (int i = 0; i < playlistsNum; i++) // Iterate through all playlists
        {
            printf("%d. %s\n", i + oneSize , playlists[i]->name);
            // Print each playlist with its index (starting from 1)
        }
    }
}
//
void deletePlaylist(Playlist*** playlists, int* playlistsNum, int deleteIndex)
{
    // Free the memory of the selected playlist
    freePlaylist((*playlists)[deleteIndex - oneSize]);

    // Shift the remaining playlists to fill the gap left by the deleted playlist
    for (int i = deleteIndex - oneSize; i < *playlistsNum - oneSize; i++)
    {
        (*playlists)[i] = (*playlists)[i + oneSize];
    }

    (*playlistsNum)--; // Decrease the number of playlists

    if (*playlistsNum == 0) // If no playlists remain, free the entire array
    {
        free(*playlists);
        *playlists = NULL; // Set pointer to NULL to prevent dangling pointer issues
    }
    else // If there are still playlists left, resize the array using realloc
    {
        Playlist** temp = (Playlist**)realloc(*playlists, sizeof(Playlist*) * (*playlistsNum));
        if (!temp) // Check if realloc failed
        {
            printf("Memory allocation failed!\n");
            exit(1); // Terminate the program to prevent memory corruption
        }
        *playlists = temp; // Update the playlists pointer with the resized array
    }

    printf("Playlist deleted.\n");
}
/*Songs_Functions=====================================================================================================*/
void freeSong(Song* song)
{
    if (!song) return; // Check if the song pointer is NULL before attempting to free

    free(song->title);  // Free the memory allocated for the song title
    free(song->artist); // Free the memory allocated for the song artist
    free(song->lyrics); // Free the memory allocated for the song lyrics
    free(song);         // Free the song structure itself
}
//
Song* createSong(char* title, char* artist, int year, char* lyrics)
{
    // Allocate memory for the Song structure
    Song* song = (Song*)malloc(sizeof(Song));
    if (!song) // Check if memory allocation failed
    {
        printf("song malloc error\n");
        exit(1); // Exit the program to prevent further execution with NULL pointer
    }

    // Allocate memory for the song's title, artist, and lyrics
    song->title = (char*)malloc(strlen(title) + oneSize);
    song->artist = (char*)malloc(strlen(artist) + oneSize);
    song->lyrics = (char*)malloc(strlen(lyrics) + oneSize);

    // Check if any of the memory allocations failed
    if (!song->title || !song->artist || !song->lyrics)
    {
        printf("malloc error\n");

        // Free any allocated memory to prevent memory leaks
        free(song->title);
        free(song->artist);
        free(song->lyrics);
        free(song);

        exit(1); // Exit the program since memory allocation failed
    }

    // Copy the input strings into the newly allocated memory
    strcpy(song->title, title);
    strcpy(song->artist, artist);
    strcpy(song->lyrics, lyrics);

    // Initialize the song's metadata
    song->year = year;
    song->streams = 0;

    return song; // Return the newly created song
}
//
void addSong(Playlist* playlist, char* title, char* artist, int year, char* lyrics)
{
    int numOfSongs = playlist->songsNum; // Get the current number of songs in the playlist

    if (numOfSongs == 0) // If this is the first song being added
    {
        // Allocate memory for the song pointer array
        playlist->songs = (Song**)malloc(sizeof(Song*));
        if (!playlist->songs) // Check if memory allocation failed
        {
            printf("Failed to allocate memory for songs\n");
            exit(1); // Terminate program to prevent further errors
        }
    }
    else // If there are existing songs in the playlist
    {
        // Expand the song array using realloc
        Song** temp = (Song**)realloc(playlist->songs, (playlist->songsNum + oneSize) * sizeof(Song*));
        if (!temp) // Check if reallocation failed
        {
            printf("error in realloc\n");
            exit(1); // Terminate program to prevent memory corruption
        }
        playlist->songs = temp; // Update the song array pointer
    }

    // Create a new song and add it to the playlist
    playlist->songs[playlist->songsNum] = createSong(title, artist, year, lyrics);

    // Increase the number of songs in the playlist
    playlist->songsNum++;
}
//
void displaySongs(Playlist* playlist)
{
    if (playlist->songsNum != 0) // Check if the playlist contains songs
    {
        for (int i = 0; i < playlist->songsNum; i++) // Iterate through all songs in the playlist
        {
            printf("%d. Title: %s\n   Artist: %s\n   Released: %d\n   Streams: %d\n",
                   i + oneSize, // Display song number (starting from 1)
                   playlist->songs[i]->title,   // Print the song title
                   playlist->songs[i]->artist,  // Print the artist name
                   playlist->songs[i]->year,    // Print the release year
                   playlist->songs[i]->streams  // Print the number of streams
            );
        }
    }
}
//
void deleteSong(Playlist* playlist, int deleteIndex)
{
    // Validate input: ensure the playlist is not NULL, contains songs, and the delete index is within range
    if (!playlist || playlist->songsNum == 0 || deleteIndex < 1 || deleteIndex > playlist->songsNum)
    {
        printf("Invalid song index.\n");
        return;
    }

    // Free memory allocated for the song being deleted
    free(playlist->songs[deleteIndex - oneSize]->title);
    free(playlist->songs[deleteIndex - oneSize]->artist);
    free(playlist->songs[deleteIndex - oneSize]->lyrics);
    free(playlist->songs[deleteIndex - oneSize]);

    // Shift all songs after the deleted song one position to the left
    for (int i = deleteIndex - oneSize; i < playlist->songsNum - oneSize; i++)
    {
        playlist->songs[i] = playlist->songs[i + oneSize];
    }

    // Decrease the number of songs in the playlist
    playlist->songsNum--;

    if (playlist->songsNum == 0) // If no songs remain, free the song array
    {
        free(playlist->songs);
        playlist->songs = NULL;
    }
    else // Resize the song array using realloc
    {
        Song** temp = (Song**)realloc(playlist->songs, sizeof(Song*) * playlist->songsNum);
        if (!temp) // Check if realloc failed
        {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        playlist->songs = temp; // Update the song array pointer
    }

    printf("Song deleted successfully.\n");
}
/*Input_Functions=====================================================================================================*/
char *enterNameInput()
{
    char *input = NULL; // Pointer to store the dynamically allocated string
    int size = 0;       // Keeps track of the string's length
    char c;

    // Read characters until newline ('\n') or end-of-file (EOF) is encountered
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // Allocate/reallocate memory for the string (including space for '\0' at the end)
        char *temp = realloc(input, (size + addTwo) * sizeof(char));

        // If memory allocation fails, free the previously allocated memory and exit
        if (!temp)
        {
            free(input);
            printf("Memory allocation error\n");
            exit(1);
        }

        // Update input pointer to the newly allocated memory
        input = temp;
        input[size] = c; // Store the newly read character in the string

        // Increase the string length
        size++;
    }

    // If input is not NULL, append the null terminator at the end of the string
    if (input)
    {
        input[size] = '\0';
    }
    // If the input is empty, allocate a single null character to represent an empty string
    else
    {
        input = malloc(1);
        if (!input) // Check if memory allocation failed
        {
            printf("Memory allocation error\n");
            exit(1);
        }
        input[0] = '\0'; // Set the string to an empty string
    }

    // If the last character in the string is a carriage return ('\r'), replace it with '\0'
    if (size > 0 && input[size - oneSize] == '\r')
    {
        input[size - oneSize] = '\0';
    }

    // Return the dynamically allocated input string
    return input;
}
//
void printPlaylistsMenu()
{
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");   
}
//
int compareStrings(const char* str1, const char* str2)
{
    while (*str1 && *str2) // Loop while both strings have characters
    {
        if (*str1 < *str2) return -1; // str1 comes first alphabetically
        if (*str1 > *str2) return 1;  // str2 comes first alphabetically
        str1++;
        str2++;
    }

    // If we reach here, one string is shorter than the other
    if (*str1 == '\0' && *str2 == '\0') return 0; // Both are equal
    if (*str1 == '\0') return -1; // str1 is shorter, so it comes first
    return 1; // str2 is shorter, so it comes first
}
//
void sortSongs(Playlist* playlist)
{
    int choice;
    scanf("%d",&choice);
    while (getchar() != '\n');
    switch (choice)
    {
        case sortByYear: // Sorting songs by year (descending order)
            Song* temp1;

        // Perform selection sort to sort songs by year
        for (int j = playlist->songsNum - oneSize; j > 0; j--)
        {
            int maxPlace = 0; // Position of the song with the highest year
            int maxYear = 0;   // Stores the maximum year found

            for (int i = 0; i <= j; i++)
            {
                // Find the song with the highest release year
                if (playlist->songs[i]->year > maxYear)
                {
                    maxYear = playlist->songs[i]->year;
                    maxPlace = i;
                }
            }

            // Swap the song with the highest year to the current position
            temp1 = playlist->songs[j];
            playlist->songs[j] = playlist->songs[maxPlace];
            playlist->songs[maxPlace] = temp1;
        }
            break;
        case sortByAscend:
            {
                Song* temp2;
                // Perform selection sort to sort songs by streams (descending order)
                for (int j = playlist->songsNum - oneSize; j > 0; j--)
                {
                    int maxPlace = 0;      // Position of the song with the highest stream count
                    int maxStreams = -oneSize; // Initialize maxStreams to a very low value

                    // Iterate through the unsorted section of the playlist
                    for (int i = 0; i <= j; i++)
                    {
                        // Find the song with the highest number of streams
                        if (playlist->songs[i]->streams > maxStreams)
                        {
                            maxStreams = playlist->songs[i]->streams;
                            maxPlace = i;
                        }
                    }

                    // Swap the song with the highest stream count to the current position
                    temp2 = playlist->songs[j];
                    playlist->songs[j] = playlist->songs[maxPlace];
                    playlist->songs[maxPlace] = temp2;
                }
            }
            break;
        case sortByDescend:
            {
                Song* temp3;
                // Perform selection sort to sort songs by streams (descending order)
                for (int j = 0; j < playlist->songsNum; j++)
                {
                    int maxPlace = j;                         // Assume the current index has the highest streams
                    int maxStreams = playlist->songs[j]->streams; // Store the stream count of the current song

                    // Iterate through the unsorted section of the playlist
                    for (int i = j; i < playlist->songsNum; i++)
                    {
                        // Find the song with the highest number of streams
                        if (playlist->songs[i]->streams > maxStreams)
                        {
                            maxStreams = playlist->songs[i]->streams;
                            maxPlace = i;
                        }
                    }

                    // Swap the song with the highest stream count to its correct position
                    temp3 = playlist->songs[j];
                    playlist->songs[j] = playlist->songs[maxPlace];
                    playlist->songs[maxPlace] = temp3;
                }
            }
            break;
        default:
            // Check if the playlist is valid and has at least two songs to sort
                if (!playlist || playlist->songsNum < due)
                {
                    printf("sorted\n"); // If there are fewer than 'due' songs, sorting is not needed
                    return;
                }

        // Implementing selection sort to sort the songs alphabetically by title
        for (int i = 0; i < playlist->songsNum - oneSize; i++)
        {
            int minIndex = i; // Assume the current index has the smallest title alphabetically

            // Iterate through the unsorted section of the playlist
            for (int j = i + oneSize; j < playlist->songsNum; j++)
            {
                // Compare titles using a custom function (compareStrings) instead of strcmp
                if (compareStrings(playlist->songs[j]->title, playlist->songs[minIndex]->title) < 0)
                {
                    minIndex = j; // Update minIndex if a smaller (alphabetically earlier) title is found
                }
            }

            // Swap the current song with the song that has the smallest title
            if (minIndex != i)
            {
                Song* temp = playlist->songs[i];
                playlist->songs[i] = playlist->songs[minIndex];
                playlist->songs[minIndex] = temp;
            }
        }
            break;
    }
    printf("sorted\n");
}
//
int readIntegerInput()
{
    char buffer[sizeOfNum]; // Buffer to store user input

    while (1) // Infinite loop to continuously prompt until valid input is received
    {
        // Read a line of input from the user
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            printf("Error reading input.\n");
            continue; // Restart the loop if input reading fails
        }

        int value;
        // Attempt to parse an integer from the input buffer
        if (sscanf(buffer, "%d", &value) == oneSize)
        {
            return value; // Return the valid integer input
        }
        else
        {
            printf("Invalid input, please enter a number: "); // Prompt user again if input is not a number
        }
    }
}
/*Main_Functions======================================================================================================*/
void playSong(Song* song)
{
    // Display the song title
    printf("Now playing %s:\n", song->title);

    // Display the song lyrics surrounded by '$' symbols for formatting
    printf("$ %s $\n", song->lyrics);

    // Increment the stream count to reflect that the song has been played
    song->streams++;
}
//
void addSongToPlaylist(Playlist* playlists)
{
    int year;
    printf("Enter song's details\n");

    // Get the song title from user input
    printf("Title:\n");
    char *title = enterNameInput();
    if (title == NULL || strlen(title) == 0) // Validate title input
    {
        printf("Error: Invalid song title\n");
        free(title); // Free memory to avoid memory leaks
        return; // Exit the function to prevent adding an invalid song
    }

    // Get the artist name from user input
    printf("Artist:\n");
    char *artist = enterNameInput();
    if (artist == NULL || strlen(artist) == 0) // Validate artist input
    {
        printf("Error: Invalid artist name\n");
        free(title); // Free previously allocated title
        free(artist); // Free artist memory
        return; // Exit function
    }

    // Get the song's release year from user input
    printf("Year of release:\n");
    year = readIntegerInput();


    // Get the song lyrics from user input
    printf("Lyrics:\n");
    char *lyrics = enterNameInput();
    if (lyrics == NULL || strlen(lyrics) == 0) // Validate lyrics input
    {
        printf("Error: Invalid lyrics\n");
        free(title);  // Free previously allocated title
        free(artist); // Free previously allocated artist
        free(lyrics); // Free lyrics memory
        return; // Exit function
    }

    // Add the song to the playlist
    addSong(playlists, title, artist, year, lyrics);

    // Free dynamically allocated memory after the song has been added
    free(title);
    free(artist);
    free(lyrics);
}
//
int main()
{
    Playlist** playlists = NULL; // Pointer to an array of playlists
    int playlistsNum = 0; // Number of playlists

    // Initialize the playlist system with a "Back to main menu" option
    addPlaylist(&playlists, &playlistsNum, "Back to main menu");

    int option = 0; // User's menu selection

    // Main program loop - runs until the user selects option 4 (exit)
    while (option != exitProgram)
    {
        printPlaylistsMenu(); // Display the main menu
        option = readIntegerInput(); // Read user's menu choice

        switch (option)
        {
        case view: // View and manage playlists
            {
                int case1Choice = exitToPlaylists; // Initialize with exit condition
                while (case1Choice == exitToPlaylists) // Loop until the user exits
                {
                    displayPlaylists(playlists, playlistsNum); // Show available playlists
                    int playlistChoice = readIntegerInput(); // Get user selection


                    if (playlistChoice == playlistsNum) // If "Back to main menu" is selected, exit loop
                        break;

                    printf("playlist %s:\n", playlists[playlistChoice - oneSize]->name); // Display selected playlist name
                    case1Choice = 0; // Reset choice to enter the playlist menu

                    // Playlist menu loop
                    while (case1Choice != exitToPlaylists) // Run until the user exits the playlist menu
                    {
                        // Show playlist options
                 printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
                        case1Choice = readIntegerInput(); // Get user's playlist menu choice


                        switch (case1Choice)
                        {
                        case view: // Show playlist and play songs
                            {
                                int songChoice;
                                displaySongs(playlists[playlistChoice - oneSize]); // Display all songs in the selected playlist

                                printf("choose a song to play, or 0 to quit:\n");
                                songChoice = readIntegerInput(); // Get user choice


                                // Loop until user chooses to quit (0)
                                while (songChoice <= playlists[playlistChoice - oneSize]->songsNum && songChoice > 0)
                                {
                                    playSong(playlists[playlistChoice - oneSize]->songs[songChoice - oneSize]); // Play the selected song
                                    printf("choose a song to play, or 0 to quit:\n");
                                    songChoice = readIntegerInput(); // Read next song choice

                                }
                            }
                            break;

                        case addToList: // Add a song to the selected playlist
                            addSongToPlaylist(playlists[playlistChoice - oneSize]);
                            break;

                        case chooseDelete: // Delete a song from the selected playlist
                            displaySongs(playlists[playlistChoice - oneSize]); // Show songs to allow selection

                            printf("choose a song to delete, or 0 to quit:\n");
                            int deleteIndex = readIntegerInput(); // Get song index to delete


                            // Validate the input before deleting
                            if (deleteIndex > 0 && deleteIndex <= playlists[playlistChoice - oneSize]->songsNum)
                                deleteSong(playlists[playlistChoice - oneSize], deleteIndex);
                            break;

                        case chooseSort: // Sort songs in the playlist based on user preference
                            printf("choose:\n");
                            printf("1. sort by year\n2. sort by streams - ascending order\n");
                            printf("3. sort by streams - descending order\n4. sort alphabetically\n");

                            // Sort the selected playlist
                            sortSongs(playlists[playlistChoice - oneSize]);
                            break;

                        case playAllSongs: // Play all songs in the selected playlist
                            {
                                for (int i = 0; i < playlists[playlistChoice - oneSize]->songsNum; i++)
                                {
                                    playSong(playlists[playlistChoice - oneSize]->songs[i]); // Play each song
                                    printf("\n"); // Add space between songs
                                }
                            }
                            break;
                        case exitToPlaylists:
                            break;
                        }
                    }
                }
                break;
                //
                case addToList: // Add a new playlist
                    printf("Enter playlist's name:\n");
                char *input = enterNameInput(); // Read user input for the playlist name

                // Validate input to ensure the playlist name is not empty
                if (input == NULL || strlen(input) == 0)
                {
                    printf("Error: Invalid playlist name\n");
                    free(input); // Free allocated memory
                    continue; // Skip the rest of the loop iteration
                }

                // Add the new playlist to the list
                addPlaylist(&playlists, &playlistsNum, input);
                free(input); // Free allocated memory after adding the playlist
                break;

                case chooseDelete: // Delete an existing playlist
                    int deleteIndex;

                // Display the available playlists to allow user selection
                displayPlaylists(playlists, playlistsNum);
                deleteIndex = readIntegerInput(); // Get user input for the playlist to delete


                // Validate the playlist index before attempting to delete
                if (deleteIndex > 0 && deleteIndex <= playlistsNum - oneSize)
                {
                    deletePlaylist(&playlists, &playlistsNum, deleteIndex);
                }
                break;

                case exitProgram: // Exit the program and free all allocated memory
                    freeAllPlaylists(playlists, playlistsNum);
                printf("Goodbye!\n");
                break;

                //
                default:
                    break;
            }
        }
    }
}