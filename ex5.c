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
/*Functions===========================================================================================================*/
void freeBuffer(char** buffer, int lengthNum)//**************************************************************************
{
    for (int i = 0; i < lengthNum; i++)
        free(buffer[i]);
}
//
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
char* playlistNameInput()//***********************************************************************************************
{
    char* buffer = (char*)malloc(sizeof(char));
    if (!buffer)
    {
        printf("No more memory for playlist name\n");
        exit(1);
    }
    int *length = 1;
    scanf(" %c", &buffer);
    while (buffer[*length - 1] != '\n')
    {
        int *temp = (int*)realloc(buffer, sizeof(int) * (*length)++);
        if (!temp)
        {
            printf("No more memory for playlist name\n");
            freeBuffer(&buffer,*length); //****************************************************************************
            exit(1);
        }
        (*length)++;
        scanf(" %c", &buffer[*length - 1]);
    }
    if (buffer[*length - 1] == '\n')
    {
        buffer[*length - 1] = '\0';
    }
    return buffer;
}
//
Playlist* createPlaylist(const char* name)
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
void addPlaylist(Playlist*** playlists, int* playlistsNum, const char* name)
{
    int hasBackToMenu = (*playlistsNum > 0);

    Playlist* newPlaylist = createPlaylist(name);
    Playlist** temp = (Playlist**)realloc(*playlists, (*playlistsNum + 1) * sizeof(Playlist*));
    if (!temp)
    {
        printf("Memory allocation failed for playlists array!\n");
        freePlaylist(&newPlaylist,*playlistsNum);//*********************************************************************
        return;
    }
    *playlists = temp;
    (*playlists)[*playlistsNum] = newPlaylist;
    (*playlistsNum)++;
    if (!*playlists)
    {
        printf("Memory allocation failed for playlists array!\n");
        exit(0);
    }
    if (hasBackToMenu == 0)
    {
        (*playlists)[hasBackToMenu] = createPlaylist(name);
    }
    if (hasBackToMenu)
    {
        (*playlists)[*playlistsNum - 1] = (*playlists)[*playlistsNum - 2];
    }
    (*playlists)[*playlistsNum - 2] = createPlaylist(name);
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
            break;
            //
        case 2:
            printf("Enter playlist's name:\n");
            addPlaylist(&playlists, &playlistsNum, playlistNameInput());
            free(playlistNameInput());
            displayPlaylists(playlists, playlistsNum);
            break;
            //
        case 3:
            break;
            //
        case 4:
            printf("Goodbye!\n");
            break;
            //
        default:
            break;
        }
    }
    freePlaylist(playlists, playlistsNum);
}
