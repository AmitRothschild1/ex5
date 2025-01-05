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
//
char* playlistNameInput()
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
/*Songs_Functions=====================================================================================================*/
Song* createSong(char* title,char* artist,int year,char* lyrics,Playlist* playlist)
{
    Song* song = (Song*)malloc(sizeof(Song));
    if (!song)
    {
        printf("Failed to allocate memory for song\n");
        free(song);
        exit(0);
    }
    song->title = (char*)malloc(strlen(title) + 1);
    if (!song->title)
    {
        printf("Failed to allocate memory for song title\n");
        free(song);
        exit(0);
    }
    song->artist = (char*)malloc(strlen(artist) + 1);
    if (!song->artist)
    {
        printf("Failed to allocate memory for song title\n");
        free(song);
        exit(0);
    }
    song->lyrics = (char*)malloc(strlen(lyrics) + 1);
    if (!song->lyrics)
    {
        printf("Failed to allocate memory for song title\n");
        free(song);
        exit(0);
    }
    strcpy(song->title, title);
    strcpy(song->artist, artist);
    song->year = year;
    song->lyrics = lyrics;
    song->streams = 0;
    playlist->songs[playlist->songsNum] = song;
    playlist->songsNum++;
    return song;
}
void addSong(Song** songs,Playlist* playlist, char* title,char* artist,int year,char* lyrics)
{
    int numOfSongs = playlist->songsNum;
    if (numOfSongs == 0)
    {
        *songs = (Song**)malloc(sizeof(Song*));
        if (!*songs)
        {
            printf("Failed to allocate memory for songs\n");
            free(songs);
            exit(0);
        }
    }
    else
    {
        Song** temp = (Song**)realloc(*songs, (numOfSongs + 1) * sizeof(Song*));
        if (!temp)
        {
            printf("Failed to allocate memory for playlists\n");
            free(temp);
            exit(0);
        }
        playlist->songs = temp;
    }
    for (int i = numOfSongs; i > 0; i--)
    {
        playlist->songs[i] = playlist->songs[i - 1];
    }
        playlist->songs[0] = createSong(title,artist,year,lyrics,playlist);
        playlist->songsNum++;
}
//
void displaySongs(Playlist* playlist)
{
    if (playlist->songsNum == 0)
    {
        printf("No playlists available. Add a playlist to get started.\n");
    }
    else
    {
        printf("Choose a playlist:\n");
        for (int i = 0; i < playlist->songsNum; i++)
        {
            printf("%d. Title: %s\n   Artist: %s\n   Released: %d\n   Streams: %d", i + 1 ,
                playlist->songs[i]->title,playlist->songs[i]->artist, playlist->songs[i]->year,
                playlist->songs[i]->streams);
        }
    }
}



void deleteSong()
{
    printf("Song deleted successfully.\n");
}
void playSong()
{}
void freeSong()
{}
void printPlaylistsMenu()
{
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");   
}
void sortPlaylist()
{
    printf("sorted\n");
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
            int case1Choice = 0;
            scanf("%d", &case1Choice);
            if (case1Choice!=playlistsNum)
            {
                printf("playlist %s:\n",playlists[case1Choice-1]->name);
                printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
                int songChoice = 0;
                scanf("%d", &songChoice);
                while (songChoice!=6)
                {
                    switch (songChoice)
                    {
                        case 1:
                            char *title, *artist, *lyrics;
                        int year;
                        printf("Enter song's details:\n");
                        printf("Title:\n");
                        title = playlistNameInput();
                        printf("Artist:\n");
                        artist = playlistNameInput();
                        printf("Year:\n");
                        scanf("%d", &year);
                        printf("Lyrics:\n");
                        lyrics = playlistNameInput();
                        addSong(playlists-)
                            break;
                        case 2:
                            break;
                        case 3:
                            break;
                        case 4:
                            break;
                        case 5:
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
            char *input = playlistNameInput();
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
