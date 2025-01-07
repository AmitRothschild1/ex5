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
void freePlaylist(Playlist** playlists, int playlistsNum)
{
    for (int i = 0; i < playlistsNum; i++)
    {
        free(playlists[i]->name);
        free(playlists[i]);
    }
    free(playlists);
}
//
Playlist* createPlaylist(char* name,int *playlistsNum)
{
    Playlist* playlist = (Playlist*)malloc(sizeof(Playlist));
    if (!playlist)
    {
        printf("Failed to allocate memory for Playlist\n");
        exit(0);
    }
    playlist->name = (char*)malloc(strlen(name) + 1);
    if (!playlist->name)
    {
        printf("Failed to allocate memory for playlist name\n");
        free(playlist);
        exit(0);
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
            free(playlists);
            exit(0);
        }
    }
    else
    {
        Playlist** temp = (Playlist**)realloc(*playlists, (numOfPlaylists + 1) * sizeof(Playlist*));
        if (!temp)
        {
            printf("Failed to allocate memory for playlists\n");
            free(temp);
            exit(0);
        }
        *playlists = temp;
    }
    for (int i = numOfPlaylists; i > 0; i--)
    {
        (*playlists)[i] = (*playlists)[i - 1];
    }
    (*playlists)[0] = createPlaylist(name,&numOfPlaylists);
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

char* enterNameInput()
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
    while ((ch = getchar()) != '\n' && ch != EOF)
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
    name[length] = '\0';
    return name;
}
//
/*
char* playlistNameInput1()
{
    char *name;
    char temp[100];
    scanf("%s", temp);
    name = (char*)malloc(strlen(temp) + 1);
    if (name == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(0);
    }
    strcpy(name, temp);
    return name;
}
*/
/*Songs_Functions=====================================================================================================*/
Song* createSong(char* title, char* artist, int year, char* lyrics, Playlist* playlist)
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
//
    return song;
}

void addSong(Playlist* playlist, char* title, char* artist, int year, char* lyrics)
{
    int numOfSongs = playlist->songsNum;
    if (numOfSongs == 0)
    {
        playlist->songs = (Song**)malloc(sizeof(Song*));
        if (!playlist->songs)
        {
            printf("Failed to allocate memory for songs\n");
            free(playlist->songs);
            exit(0);
        }
    }
    else
    {
        Song** temp = (Song**)realloc(playlist->songs,(playlist->songsNum + 1) * sizeof(Song*));
      //  playlist->songs = (Song**)realloc(playlist->songs, (playlist->songsNum + 1) * sizeof(Song*));
        if (!temp)
        {
            printf("error in realloc\n");
            free(temp);
            exit(1);
        }
        playlist->songs = temp;
    }
    playlist->songs[playlist->songsNum] = createSong(title, artist, year, lyrics, playlist);
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



void deleteSong()
{
    printf("Song deleted successfully.\n");
}
void playSong(Song* song)
{
    printf("Now playing %s:\n",song->title);
    printf("$ %s $\n",song->lyrics);
    song->streams++;
}
void freeSong()
{}
void printPlaylistsMenu()
{
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");   
}
void sortPlaylist()
{}
void sortSongs(Playlist* playlist,int playlistsNum)
{
    int choice;
    scanf("%d",&choice);
    switch (choice)
    {
        case 1:
            Song** temp = (Song**)malloc(sizeof(Song*)*playlist->songsNum);
        if (!temp)
        {
            printf("Failed to allocate memory for playlists\n");
            exit(0);
        }
        for (int j = 0; j < playlistsNum; j++)
        {
            int place = 0;
            int newestYear = 0;
            for (int i = 0; i < playlist->songsNum; i++)
            {
                if (playlist->songs[i] != NULL && playlist->songs[i]->year > newestYear)
                {
                    newestYear = playlist->songs[i]->year;
                    place = i;
                }
            }
            temp[playlist->songsNum -(j+1)] = playlist->songs[place];
            playlist->songs[place]=NULL;
        }
        free(playlist->songs);
        playlist->songs = temp;
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }
    printf("sorted\n");
}
/*Main_Functions======================================================================================================*/
void addSongToPlaylist(Playlist** playlists)
{
    char *title, *artist, *lyrics;
    int year;
    printf("Enter song's details:\n");
    printf("Title:\n");
    title = enterNameInput();
    printf("Artist:\n");
    artist = enterNameInput();
    printf("Year:\n");
    scanf("%d", &year);
    printf("Lyrics:\n");
    lyrics = enterNameInput();
    addSong(*playlists,title,artist,year,lyrics);
}


int main()
{
    Playlist** playlists = NULL;
    int playlistsNum = 0;
    addPlaylist(&playlists, &playlistsNum, "Back to Menu");
    //
    int option = 0;
    while (option != 4)
    {
        printPlaylistsMenu();
        scanf("%d", &option);
        getchar();
        switch (option)
        {
        case 1:
            displayPlaylists(playlists, playlistsNum);
            int playlistChoice;
            scanf("%d", &playlistChoice);
            printf("playlist %s:\n",playlists[playlistChoice-1]->name);
            int case1Choice = 0;
            while (case1Choice!=6)
            {
                printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
                scanf("%d", &case1Choice);
                switch (case1Choice)
                    {
                    case 1:
                    {
                        int songChoice;
                            printf("choose a song to play, or 0 to quit:\n");
                            displaySongs(*playlists);
                            scanf("%d", &songChoice);
                        while (songChoice<=(*playlists)->songsNum && songChoice>0)
                        {
                            playSong((*playlists)->songs[songChoice-1]);
                            printf("choose a song to play, or 0 to quit:\n");
                            displaySongs(*playlists);
                            scanf("%d", &songChoice);
                        }
                    }
                            break;
                        case 2:
                            addSongToPlaylist(playlists);
                            break;
                        case 3:
                            break;
                        case 4:
                            printf("choose:\n");
                    printf("1. sort by year\n2. sort by streams - ascending order\n");
                    printf("3. sort by streams - descending order\n4. sort alphabetically\n");
                    sortSongs(playlists[playlistChoice-1],playlistsNum);
                            break;
                        case 5:
                            {
                                for (int i = 0; i < (*playlists)->songsNum; i++)
                                {
                                    playSong((*playlists)->songs[i]);
                                    printf("\n");
                                }
                            }
                            break;
                        case 6:
                            break;
                    }
            }
            break;
            //
        case 2:
            printf("Enter playlist's name:\n");
            char *input = enterNameInput();
            addPlaylist(&playlists, &playlistsNum, input);
            free(input);
            break;
            //
        case 3:
            break;
            //
        case 4:
            freePlaylist(playlists, playlistsNum);
            printf("Goodbye!\n");
            break;
            //
        default:
            break;
        }
    }
}
