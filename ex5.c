#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    if (playlist->songs)
    {
        for (int i = 0; i < playlist->songsNum; i++)
        {
            free(playlist->songs[i]->title);
            free(playlist->songs[i]->artist);
            free(playlist->songs[i]->lyrics);
            free(playlist->songs[i]);
        }
        free(playlist->songs);
    }
    free(playlist);
}
//
void freeAllPlaylists(Playlist** playlists, int playlistsNum) {
    for (int i = 0; i < playlistsNum; i++) {
        freePlaylist(playlists[i]);
    }
    free(playlists);
}
//
Playlist* createPlaylist(char* name)
{
    Playlist* playlist = (Playlist*)malloc(sizeof(Playlist));
    if (!playlist)
    {
        printf("Failed to allocate memory for Playlist\n");
        exit(1);
    }
    playlist->name = (char*)malloc(strlen(name) + 1);
    if (!playlist->name)
    {
        printf("Failed to allocate memory for playlist name\n");
        free(playlist);
        exit(1);
    }
    strcpy(playlist->name, name);
    playlist->songs = NULL;
    playlist->songsNum = 0;
    return playlist;
}
//
void addPlaylist(Playlist*** playlists, int* playlistsNum, char* name)
{
    int numOfPlaylists = *playlistsNum;
    if (numOfPlaylists == 0)
    {
        *playlists = (Playlist**)malloc(sizeof(Playlist*));
        if (!*playlists)
        {
            printf("Failed to allocate memory for playlist array!\n");
            exit(1);
        }
        (*playlists)[numOfPlaylists] = createPlaylist(name);
    }
    else
    {
        Playlist** temp = (Playlist**)realloc(*playlists, (numOfPlaylists + 1) * sizeof(Playlist*));
        if (!temp)
        {
            printf("Failed to allocate memory for playlists\n");
            freeAllPlaylists(*playlists, *playlistsNum);
            exit(1);
        }
        *playlists = temp;
        (*playlists)[numOfPlaylists] = (*playlists)[numOfPlaylists-1];
        (*playlists)[numOfPlaylists-1] = createPlaylist(name);
    }
    /*
    for (int i = numOfPlaylists; i > 0; i--)
    {
        (*playlists)[i] = (*playlists)[i - 1];
    }
    */
    (*playlistsNum)++;
}
//
void displayPlaylists(Playlist** playlists, int playlistsNum)
{
    if (playlistsNum == 0)
    {
        printf("No playlists available. Add a playlist to get started.\n");
    }
    else
    {
        printf("Choose a playlist:\n");
        for (int i = 0; i < playlistsNum; i++)
        {
            printf("%d. %s\n", i + 1 , playlists[i]->name);
        }
    }
}

void deletePlaylist(Playlist*** playlists, int* playlistsNum ,int deleteIndex) {
    freePlaylist((*playlists)[deleteIndex-1]);

    for (int i = deleteIndex - 1; i < *playlistsNum - 1; i++) {
        (*playlists)[i] = (*playlists)[i + 1];
    }

    (*playlistsNum)--;

    if (*playlistsNum == 0) {
        free(*playlists);
        *playlists = NULL;
    } else {
        Playlist** temp = (Playlist**)realloc(*playlists, sizeof(Playlist*) * (*playlistsNum));
        if (!temp) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        *playlists = temp;
    }

    printf("Playlist deleted.\n");
}

void deletePlaylist1(Playlist*** playlists, int* playlistsNum ,int deleteIndex)//***********************************************************8
{
    freePlaylist((*playlists)[deleteIndex-1]);
    for (int i = deleteIndex - 1; i < *playlistsNum - 1; i++)
    {
        playlists[i] = playlists[i + 1];
    }
    (*playlistsNum)--;
        Playlist** temp = (Playlist**)realloc(*playlists,sizeof(Playlist*) * (*playlistsNum));
        if (!temp)
        {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        *playlists = temp;//**********************************************************************************88
    printf("Playlist deleted.\n");
}


/*Songs_Functions=====================================================================================================*/
void freeSong(Song* song)
{
    if (!song) return;
    free(song->title);
    free(song->artist);
    free(song->lyrics);
    free(song);
}
Song* createSong(char* title, char* artist, int year, char* lyrics)
{
    Song* song = (Song*)malloc(sizeof(Song));
    if (!song)
    {
        printf("song malloc error\n");
        exit(1);
    }
    song->title = (char*)malloc(strlen(title) + 1);
    song->artist = (char*)malloc(strlen(artist) + 1);
    song->lyrics = (char*)malloc(strlen(lyrics) + 1);
    if (!song->title || !song->artist || !song->lyrics)
    {
        printf("malloc error\n");
        free(song->title);
        free(song->artist);
        free(song->lyrics);
        free(song);
        exit(1);
    }
    strcpy(song->title, title);
    strcpy(song->artist, artist);
    strcpy(song->lyrics, lyrics);
    song->year = year;
    song->streams = 0;
    return song;
}
//
void addSong(Playlist* playlist, char* title, char* artist, int year, char* lyrics)
{
    int numOfSongs = playlist->songsNum;
    if (numOfSongs == 0)
    {
        playlist->songs = (Song**)malloc(sizeof(Song*));
        if (!playlist->songs)
        {
            printf("Failed to allocate memory for songs\n");
           // free(playlist->songs);
            exit(1);
        }
    }
    else
    {
        Song** temp = (Song**)realloc(playlist->songs,(playlist->songsNum + 1) * sizeof(Song*));
        if (!temp)
        {
            printf("error in realloc\n");
           // free(temp);
            exit(1);
        }
        playlist->songs = temp;
    }
    playlist->songs[playlist->songsNum] = createSong(title, artist, year, lyrics);
    playlist->songsNum++;
}
//
void displaySongs(Playlist* playlist)
{
    if (playlist->songsNum != 0)
        for (int i = 0; i < playlist->songsNum; i++)
        {
            printf("%d. Title: %s\n   Artist: %s\n   Released: %d\n   Streams: %d\n", i + 1 ,
                playlist->songs[i]->title,playlist->songs[i]->artist, playlist->songs[i]->year,
                playlist->songs[i]->streams);
        }
}
//
void deleteSong(Playlist* playlist, int deleteIndex) {
    if (!playlist || playlist->songsNum == 0 || deleteIndex < 1 || deleteIndex > playlist->songsNum) {
        printf("Invalid song index.\n");
        return;
    }

    // שחרור כל השדות של השיר לפני מחיקתו
    free(playlist->songs[deleteIndex - 1]->title);
    free(playlist->songs[deleteIndex - 1]->artist);
    free(playlist->songs[deleteIndex - 1]->lyrics);
    free(playlist->songs[deleteIndex - 1]);  // שחרור השיר עצמו

    // הזזת השירים שמימין לשיר המחוק מקום אחד שמאלה
    for (int i = deleteIndex - 1; i < playlist->songsNum - 1; i++) {
        playlist->songs[i] = playlist->songs[i + 1];
    }

    playlist->songsNum--;

    // טיפול במקרה שבו אין יותר שירים בפלייליסט
    if (playlist->songsNum == 0) {
        free(playlist->songs);
        playlist->songs = NULL;
    } else {
        Song** temp = (Song**)realloc(playlist->songs, sizeof(Song*) * playlist->songsNum);
        if (!temp) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        playlist->songs = temp;
    }

    printf("Song deleted successfully.\n");
}

void deleteSong1(Playlist* playlist, int deleteIndex)
{
    free(playlist->songs[deleteIndex - 1]);

    for (int i = deleteIndex - 1; i < playlist->songsNum - 1; i++)
    {
        playlist->songs[i] = playlist->songs[i + 1];
    }
    playlist->songsNum--;
    if (playlist->songsNum == 0)
    {
        free(playlist->songs);
        playlist->songs = NULL;
    }
    else
    {
        Song** temp = (Song**)realloc(playlist->songs,sizeof(Song*)*(playlist->songsNum-1));
        if (!temp)
        {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        playlist -> songs = temp;
    }
    printf("Song deleted successfully.\n");
}
/*Input_Functions=====================================================================================================*/
char* enterNameInput() {
    char buffer[256]; // קלט בגודל מקסימלי

    if (!fgets(buffer, sizeof(buffer), stdin)) {
        printf("Error reading input.\n");
        exit(1);
    }

    // מחיקת '\n' מסוף הקלט אם קיים
    buffer[strcspn(buffer, "\n")] = '\0';
    buffer[strcspn(buffer, "\r")] = '\0';

    // הקצאת זיכרון דינאמית בהתבסס על האורך של הקלט
    char* name = (char*)malloc(strlen(buffer) + 1);
    if (!name) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    strcpy(name, buffer);
    return name;
}

char* enterNameInput1()
{
    char *name = NULL;
    int size = 10;
    int length = 0;
    char ch;
    name = (char*)malloc(size * sizeof(char));
    if (!name)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
  //  while ((ch = getchar()) == '\n' && (ch = getchar()) != '\r');
    //while ((ch = getchar()) == '\n');
    while ((ch = getchar()) == '\n' && ch != EOF);

    do
    {
        if (length == size - 1)
        {
            size *= 2;
            char *temp = (char*)realloc(name, size * sizeof(char));
            if (!temp)
            {
                printf("Memory allocation failed!\n");
                free(name);
                exit(1);
            }
            name = temp;
        }
        name[length++] = ch;
    }
    //while ((ch = getchar()) != '\n' && (ch = getchar()) != '\r' && ch != EOF);
    //while ((ch = getchar()) == '\n' && ch != EOF);
    while ((ch = getchar()) == '\n');
    name[length] = '\0';
    return name;
}
//

void printPlaylistsMenu()
{
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");   
}

void sortSongs(Playlist* playlist)
{
    int choice;
    scanf("%d",&choice);
    while (getchar() != '\n');
    switch (choice)
    {
        case 1:
            Song* temp1;
        for (int j = playlist->songsNum-1; j > 0; j--)
        {
            int maxPlace = 0;
            int maxYear = 0;
            for (int i = 0; i <= j;i++)
            {
                if (playlist->songs[i]->year > maxYear)
                {
                    maxYear = playlist->songs[i]->year;
                    maxPlace = i;
                }
            }
            temp1 = playlist->songs[j];
            playlist->songs[j] = playlist->songs[maxPlace];
            playlist->songs[maxPlace] = temp1;
        }
            break;
        case 2:
            {
                Song* temp2;
                for (int j = playlist->songsNum-1; j > 0; j--)
                {
                    int maxPlace = 0;
                    int maxStreams = -1;
                    for (int i = 0; i <= j;i++)
                    {
                        if (playlist->songs[i]->streams > maxStreams)
                        {
                            maxStreams = playlist->songs[i]->streams;
                            maxPlace = i;
                        }
                    }
                    temp2 = playlist->songs[j];
                    playlist->songs[j] = playlist->songs[maxPlace];
                    playlist->songs[maxPlace] = temp2;
                }
            }
            break;
        case 3:
            {
                Song* temp3;
                for (int j = 0 ; j < playlist->songsNum; j++)
                {
                    int maxPlace = j;
                    int maxStreams = playlist->songs[j]->streams;
                    for (int i = j; i < playlist->songsNum ;i++)
                    {
                        if (playlist->songs[i]->streams > maxStreams)
                        {
                            maxStreams = playlist->songs[i]->streams;
                            maxPlace = i;
                        }
                    }
                    temp3 = playlist->songs[j];
                    playlist->songs[j] = playlist->songs[maxPlace];
                    playlist->songs[maxPlace] = temp3;
                }
            }
            break;
        default:
            if (!playlist || playlist->songsNum < 2) {
                printf("No need to sort.\n");
                return;
            }

        for (int i = 0; i < playlist->songsNum - 1; i++) {
            int minIndex = i;

            for (int j = i + 1; j < playlist->songsNum; j++) {
                if (strcmp(playlist->songs[j]->title, playlist->songs[minIndex]->title) < 0) {
                    minIndex = j;
                }
            }

            if (minIndex != i) {
                Song* temp = playlist->songs[i];
                playlist->songs[i] = playlist->songs[minIndex];
                playlist->songs[minIndex] = temp;
            }
        }
            break;
    }
    printf("sorted\n");
}



int readIntegerInput() {
    char buffer[100];

    while (1) {
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Error reading input.\n");
            continue;
        }

        int value;
        if (sscanf(buffer, "%d", &value) == 1) {
            return value;  // קלט תקין, מחזירים את המספר
        } else {
            printf("Invalid input, please enter a number: ");
        }
    }
}
/*Main_Functions======================================================================================================*/
//
void playSong(Song* song)
{
    printf("Now playing %s:\n",song->title);
    printf("$ %s $\n",song->lyrics);
    song->streams++;
}
//
void addSongToPlaylist(Playlist* playlists)
{
    int year;
    printf("Enter song's details\n");

    printf("Title:\n");
    //title = enterNameInput();
    char *title = enterNameInput();
    if (title == NULL || strlen(title) == 0) {
        printf("Error: Invalid playlist name\n");
        free(title);
    }

    printf("Artist:\n");
    //artist = enterNameInput();
    char *artist = enterNameInput();
    if (artist == NULL || strlen(artist) == 0) {
        printf("Error: Invalid playlist name\n");
        free(artist);
    }

    printf("Year of release:\n");
    //scanf("%d", &year);
    year = readIntegerInput();


    printf("Lyrics:\n");
    char *lyrics = enterNameInput();
    if (lyrics == NULL || strlen(lyrics) == 0) {
        printf("Error: Invalid playlist name\n");
        free(lyrics);
    }
    addSong(playlists,title,artist,year,lyrics);

}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getIntInput() {
    char buffer[256];
    int number;

    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (sscanf(buffer, "%d", &number) == 1) {
            return number;
        }
    }
    return -1;  // או ערך ברירת מחדל אחר במקרה של שגיאה
}
//

//
int main()
{
    Playlist** playlists = NULL;
    int playlistsNum = 0;
    addPlaylist(&playlists, &playlistsNum, "Back to main menu");
    //
    int option = 0;
    while (option != 4)
    {
        printPlaylistsMenu();
        //scanf("%d", &option);
             //   while (getchar() != '\n' && getchar()!='\r');//88888888888888888888888888888888888888888888888888888888
        option = readIntegerInput();
        //printf("option: %d\n",option);********************************************************************************************
        switch (option)
        {
        case 1:
            int case1Choice = 6;
            while (case1Choice==6)
            {
            displayPlaylists(playlists, playlistsNum);
            int playlistChoice;
           // scanf("%d", &playlistChoice);//88888888888888888888888888888888888888888888888888888888888888888888
            playlistChoice = readIntegerInput();
            //printf("playlistChoice: %d\n",playlistChoice);*****************************************************************************


            //    while (getchar() != '\n' && getchar()!='\r');//88888888888888888888888888888888888888888888888888888888
            if (playlistChoice == playlistsNum)
                break;
            printf("playlist %s:\n",playlists[playlistChoice-1]->name);
                case1Choice = 0;
            while (case1Choice!=6)
            {
                printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
                // scanf("%d", &case1Choice);
                case1Choice = readIntegerInput();
                //printf("case1choice: %d\n",case1Choice);***********************************************************************************

                // while (getchar() != '\n' && getchar()!='\r');//88888888888888888888888888888888888888888888888888888888
                switch (case1Choice)
                {
                    //  printf("playlistChoice: %d\n",playlistChoice);
                case 1:
                    {
                        int songChoice;
                        displaySongs(playlists[playlistChoice-1]);
                        printf("choose a song to play, or 0 to quit:\n");
                        //  scanf("%d", &songChoice);
                        songChoice = readIntegerInput();
                        // printf("songchoice: %d\n",songChoice);//**********************************************************

                        //  while (getchar() != '\n' && getchar()!='\r');//88888888888888888888888888888888888888888888888888888888
                        while (songChoice<=playlists[playlistChoice-1]->songsNum && songChoice>0)
                        {
                            playSong(playlists[playlistChoice-1]->songs[songChoice-1]);
                            printf("choose a song to play, or 0 to quit:\n");
                            //displaySongs(*playlists);
                            //  scanf("%d", &songChoice);
                            songChoice = readIntegerInput();
                            //  printf("songchoice: %d\n",songChoice);//***************************************************************************8

                            // while (getchar() != '\n' && getchar()!='\r');//88888888888888888888888888888888888888888888888888888888
                        }
                    }
                    break;
                case 2:
                    //addSongToPlaylist(playlists);
                        addSongToPlaylist(playlists[playlistChoice-1]);
                    break;
                case 3:
                    displaySongs(playlists[playlistChoice-1]);
                    printf("choose a song to delete, or 0 to quit:\n");
                    int deleteIndex;
                    // scanf("%d",&deleteIndex);
                    deleteIndex = readIntegerInput();
                    // printf("deleteindex: %d\n",deleteIndex);********************************************************************

                    // while (getchar() != '\n' && getchar()!='\r');//88888888888888888888888888888888888888888888888888888888
                    if (deleteIndex>0 && deleteIndex<=(*playlists)->songsNum)
                        deleteSong(playlists[playlistChoice-1], deleteIndex);
                    break;
                case 4:
                    printf("choose:\n");
                    printf("1. sort by year\n2. sort by streams - ascending order\n");
                    printf("3. sort by streams - descending order\n4. sort alphabetically\n");
                    sortSongs(playlists[playlistChoice-1]);
                    break;
                case 5:
                    {
                        for (int i = 0; i < playlists[playlistChoice-1]->songsNum; i++)
                        {
                            //playSong((*playlists)->songs[i]);
                            playSong(playlists[playlistChoice-1]->songs[i]);
                            printf("\n");
                        }
                    }
                    break;
                case 6:
                    break;
                }
            }
            }
            break;
            //
        case 2:
            printf("Enter playlist's name:\n");
            char *input = enterNameInput();
            if (input == NULL || strlen(input) == 0) {
                printf("Error: Invalid playlist name\n");
                free(input);
                continue;
            }
            addPlaylist(&playlists, &playlistsNum, input);
            free(input);
            break;
            //
        case 3:
            int deleteIndex;
            displayPlaylists(playlists, playlistsNum);
           // scanf("%d", &deleteIndex);
            deleteIndex = readIntegerInput();
          //  printf("deleteindex: %d\n",deleteIndex);*********************************************************************************
              //  while (getchar() != '\n' && getchar()!='\r');//88888888888888888888888888888888888888888888888888888888
            if (deleteIndex>0 && deleteIndex<=playlistsNum-1)
            {
                deletePlaylist(&playlists,&playlistsNum, deleteIndex);
            }
            /*
            else
            {
                if(deleteIndex==playlistsNum)
                {
                    printf("1. Back to main menu\n");
                }
            }*/
            break;
            //
        case 4:
            freeAllPlaylists(playlists, playlistsNum);
            printf("Goodbye!\n");
            break;
            //
        default:
            break;
        }
    }
}
