/* @Author
 * Student Name: Alperen Kantarci
 * Student ID : 150140140
 * Date: 23.11.2016 */


#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define SNAME_LENGTH 50
#define LNAME_LENGTH 50
#define STYLE_LENGTH 30
#define SINGER_LENGTH 50

using namespace std;
struct song;
struct song_node;
struct playlist;
struct mainlist;

struct song {
    char* name   = new char[SNAME_LENGTH];
    char* singer = new char[SINGER_LENGTH];
    char* style  = new char[STYLE_LENGTH];
    int year;
};


struct song_node {
    song* data;
    song_node* next;
    song_node* prev;
    playlist* parent;
};

struct playlist {
    int songnumber;
    char* name = new char[LNAME_LENGTH];
    song_node* head;
    playlist* next;
    playlist* prev;
};

struct list {
    int playlist_count;
    playlist* head;
}*mainlist;

song_node* used_nodes[100]= {NULL};  //  Keeps used node pointers
int cur=0;
//function prototypes
int read_from_file();
void create_default(bool default_playlist,char *name);
void print_main_menu();
bool make_operation(char choice);
void randomize();
void sorting();
void writeToFile();

void createList();
void printList();
void deleteList(bool key,playlist *operation);
void addSong(bool key,playlist *operation);
void removeSong(bool key,playlist *operation);
void play(bool key,playlist *operation);
void exit();

int main() {
    mainlist = new list;
    bool exit_status=0;
    char choice;

    create_default(0,(char *)"");
    read_from_file();
    sorting();
    randomize();

    while(exit_status != 1) {
        print_main_menu();
        cin>>choice;
        exit_status = make_operation(choice);
    }
    return 0;
}

void print_main_menu() {
    system("cls");
    cout << endl << endl;
    cout << "SONG BOX APPLICATION (USING DOUBLY, CIRCULAR MULTI LINKED LIST)" << endl;
    cout << "Choose an operation" << endl;
    cout << "P: Play" << endl;
    cout << "L: Show all playlists" << endl;
    cout << "A: Add songs to a playlist" << endl;
    cout << "R: Remove songs from a playlist" << endl;
    cout << "C: Create a new playlist" << endl;
    cout << "D: Delete a playlist" << endl;
    cout << "W: Write to file (SAVE)" << endl;
    cout << "E: Exit" << endl << endl;
    cout << "Enter a choice {P,L,A,R,C,D,W,E}:";
}

bool make_operation(char choice) {

    bool terminate=false;

    switch(choice) {

    case 'P':
    case 'p' :
        play(0,mainlist->head);
        getchar();
        getchar();
        break;

    case 'L':
    case 'l' :
        printList();
        getchar();
        getchar();
        break;

    case 'A':
    case 'a' :
        addSong(0,mainlist->head);
        getchar();
        getchar();
        break;

    case 'R':
    case 'r' :
        removeSong(0,mainlist->head);
        break;

    case 'C':
    case 'c' :
        createList();
        getchar();
        getchar();
        break;

    case 'D':
    case 'd' :
        deleteList(0,mainlist->head);
        getchar();
        getchar();
        break;

    case 'W':
    case 'w' :
        writeToFile();
        break;

    case 'E':
    case 'e' :
        cout << "Are you sure that you want to terminate the program? (Y/N):";
        cin >> choice;
        if(choice=='Y' || choice=='y')
            terminate=true;
        writeToFile();
        exit();
        break;

    default:
        cout << "Error: You have entered an invalid choice" << endl;
        cout << "Try again {P,L,A,R,C,D,W,E} :" ;
        cin >> choice;
        terminate = make_operation(choice);
        break;
    }

    return terminate;
}

int read_from_file() {
    FILE *songbook;
    char *filename,*keys;
    song_node *tmp,*playlist_song_node;
    playlist *playlist_tmp=mainlist->head->next->next;
    int counter=0;

    keys=(char *)"*****";
    filename = (char *) "songbook.txt";
    if( !(songbook = fopen(filename,"r++")) )
        if(!(songbook= fopen(filename,"w++")) ) {
            cerr << "File couldn't be opened" << endl;
            cerr << "Please try again later!" << endl;
            return 0;
        }
    fseek(songbook,0,SEEK_SET);
    while(!feof(songbook)) { //if all datas are okay
        song *notcntrld_data= new song;

        fscanf(songbook, "%[^\t\n] %[^\t\n]  %d  %[^\n]\n",    //[^\t] it reads till tab character end
                                                                    //at the end of line it ignores new line with \n
               notcntrld_data->name,notcntrld_data->singer,&(notcntrld_data->year),notcntrld_data->style);

        if(strcmp(  keys,notcntrld_data->name) == 0 ) {         //if list is user generated
            create_default(1,notcntrld_data->singer);           //create user generated list
            counter ++;
            playlist_tmp=playlist_tmp->next;
            cur =0;
        } else {

            if(cur == 0 && counter==0) {                        //Add head to default list
                mainlist->head->head = new song_node;
                tmp = mainlist->head->head;
                tmp->data =new song;
                tmp->parent=mainlist->head;
                tmp->data->name    =  (notcntrld_data->name);
                tmp->data->singer  =  notcntrld_data->singer;
                tmp->data->year    =  notcntrld_data->year;
                tmp->data->style   =  notcntrld_data->style;

                tmp->next=tmp;
                tmp->prev=tmp;
                mainlist->head->songnumber++;
                cur ++;
            } else if(cur!=0 && counter==0) {                   //Add  node to default list
                tmp->next=new song_node;
                tmp->next->prev=tmp;
                tmp=tmp->next;
                tmp->data =new song;
                tmp->parent=mainlist->head;
                tmp->next =mainlist->head->head;
                tmp->next->prev = tmp;

                tmp->data->name    =  notcntrld_data->name;
                tmp->data->singer  =  notcntrld_data->singer;
                tmp->data->year    =  notcntrld_data->year;
                tmp->data->style   =  notcntrld_data->style;
                mainlist->head->songnumber++;
                cur++;

            } else if(cur==0 && counter!=0) {                           //Add node to custom list
                playlist_tmp->head=new song_node;
                playlist_song_node=playlist_tmp->head;
                playlist_song_node->data=new song;
                playlist_song_node->parent=playlist_tmp;

                playlist_song_node->data->name    =  notcntrld_data->name;
                playlist_song_node->data->singer  =  notcntrld_data->singer;
                playlist_song_node->data->year    =  notcntrld_data->year;
                playlist_song_node->data->style   =  notcntrld_data->style;
                playlist_song_node->next=playlist_song_node;
                playlist_song_node->prev=playlist_song_node;
                playlist_tmp->songnumber++;
                cur++;

            } else {                                                    //Add node to custom list
                playlist_song_node->next=new song_node;
                playlist_song_node->next->prev=playlist_song_node;
                playlist_song_node=playlist_song_node->next;
                playlist_song_node->data =new song;
                playlist_song_node->parent=playlist_tmp;
                playlist_song_node->next =playlist_tmp->head;
                playlist_song_node->next->prev = playlist_song_node;

                playlist_song_node->data->name    =  notcntrld_data->name;
                playlist_song_node->data->singer  =  notcntrld_data->singer;
                playlist_song_node->data->year    =  notcntrld_data->year;
                playlist_song_node->data->style   =  notcntrld_data->style;
                playlist_tmp->songnumber++;
                cur++;
            }
        }
        delete notcntrld_data;
    }
    fclose(songbook);
    return 0;
}

void create_default(bool default_playlist,char *name) {

    playlist *traverse;

    if(default_playlist == 0) {                 //Add three custom list to mainlist
        mainlist->playlist_count=0;
        mainlist->head = new playlist;
        traverse =mainlist->head;

        traverse->name = (char *) "CHRONOLOGICAL";
        mainlist->playlist_count ++;
        traverse->songnumber=0;
        traverse->next = new playlist;
        traverse->next->prev = traverse;
        traverse = traverse->next ;

        traverse->name = (char *) "SORTED";
        traverse->next = new playlist;
        mainlist->playlist_count ++;
        traverse->songnumber=0;
        traverse->next->prev = traverse;
        traverse= traverse->next;


        traverse->name =(char *) "RANDOM";
        traverse->next = mainlist->head;
        mainlist->playlist_count ++;
        traverse->songnumber=0;
        traverse->next->prev = traverse;
    } else {                 //add custom list to mainlist

        traverse=mainlist->head->prev;
        traverse->next = new playlist;
        traverse->next->prev=traverse;
        traverse=traverse->next;
        traverse->name=(char *)name;
        traverse->songnumber=0;
        traverse->next=mainlist->head;
        traverse->next->prev=traverse;
        mainlist->playlist_count ++;
    }
}

void randomize() {
    bool flag=1;
    int randomnumber=0;
    srand( time( NULL ) );              // initializes the pseudo-random number generator

    int used_songs[100],randomized=0;
    playlist *tmp=mainlist->head->next;
    playlist *tmp_random=mainlist->head->next->next;
    song_node *tmp_song_node = tmp->head;
    song_node *tmp_random_song_node=tmp_random->head;

    while(randomized!=cur) {
        tmp_song_node=tmp->head;
        flag=1;

        do {
            flag=1;
            randomnumber = rand() % cur;
            for(int i=0; i<randomized; i++) {
                if(randomnumber == used_songs[i] )
                    flag=0;
            }

        } while(flag==0);

        for(int i=0; i<randomnumber; i++) {
            tmp_song_node=tmp_song_node->next;
        }

        if(randomized==0) {                     //Head node of randomized list is adding
            used_songs[randomized]=randomnumber;
            tmp_random_song_node=new song_node;
            tmp_random->head=tmp_random_song_node;
            tmp_random_song_node->data = tmp_song_node->data;
            tmp_random_song_node->next=tmp_random_song_node;
            tmp_random_song_node->prev=tmp_random_song_node;
            tmp_random_song_node->parent=tmp_random;
            tmp_random->songnumber++;
            randomized++;

        }else {                            //Node adding to randomized list
            used_songs[randomized]=randomnumber;
            tmp_random_song_node->next=new song_node;
            tmp_random_song_node->next->prev=tmp_random_song_node;
            tmp_random_song_node=tmp_random_song_node->next;
            tmp_random_song_node->next = tmp_random->head;
            tmp_random_song_node->data = tmp_song_node->data;
            tmp_random_song_node->parent=tmp_random;
            tmp_random->songnumber++;

            randomized++;
        }
    } // end of while
}

void play(bool key,playlist *operation) {
    char* name = new char[SNAME_LENGTH];
    int number;
    char choice,exit_condition;
    song_node *traverse=operation->head;
    playlist *traverse_playlist;

    if(key == 0) {
        system("cls");
        cout << "Play >" << endl ;
        cout << "Choose an option" << endl ;
        cout << "L: Play a playlist starting from first song" << endl ;
        cout << "S: Play a playlist starting from a spesific song" << endl ;
        cout << "P:Play a single song" << endl ;
        cout << "E:Exit" << endl << endl ;
        cout << "Enter a choice {L, S, P, E}:";

        cin.clear();
        cin.ignore(1000,'\n');
        choice = getchar();
        while(choice != 'E' && choice != 'e' && exit_condition!='e' && exit_condition != 'E') {
                traverse_playlist = mainlist->head;
            switch(choice) {

            case 'L': case 'l':
            cout << "Enter the name of the playlist which you want play." << endl ;
            cin.clear();
            cin.ignore(1000,'\n');
            cin.getline(name,SNAME_LENGTH);

            for(int i=0 ;i<mainlist->playlist_count ; i++){

                if(strcmp(name,traverse_playlist->name) == 0)
                {
                    play(1,traverse_playlist);
                    return;
                }
                else
                    traverse_playlist=traverse_playlist->next;
            }
                break;

            case 'S': case 's':

                cout << "PLAYLISTS :" << endl ;
                for(int i=0; i<mainlist->playlist_count; i++) {
                cout << i+1 << ": " << traverse_playlist->name << endl ;

                traverse_playlist=traverse_playlist->next;
                }

                cout << " Enter the number of the playlist which you want to choose song from it." << endl ;
                cin >> number ;

                traverse_playlist=mainlist->head;

                for(int i=0 ; i<number-1 ;i++)
                    traverse_playlist=traverse_playlist->next;

                traverse=traverse_playlist->head;
                for(int i=0 ; i<traverse_playlist->songnumber;i++){

                    cout << "Song " << i+1 << ": " << traverse->data->name << " " << traverse->data->singer << " "
                            << traverse->data->year << " " << traverse->data->style  << endl;
                    traverse=traverse->next;
                }
                cout << "Enter the number of the song which you want to start to play." << endl ;

                cin >> number ;
                traverse=traverse_playlist->head;
                for(int i=0;i<number-1;i++)
                {
                    traverse=traverse->next;
                }

                cout << "Play >" << endl;
                while(exit_condition!= 'E' && exit_condition!='e') {

                cout << "Playing . . ." << endl ;
                cout << traverse->data->name << " - " << traverse->data->singer << " - " << traverse->data->style << endl ;
                cout << "NEXT(N) - PREVIOUS(P) - EXIT(E)" << endl ;

                cin.clear();
                cin.ignore(1000,'\n');
                choice=getchar();

                if(choice == 'N' || choice == 'n')
                    traverse = traverse->next;
                else if(choice == 'P' || choice == 'p')
                    traverse = traverse->prev;
                else exit_condition = 'E';
        }
                break;

            case 'P': case 'p':
                traverse_playlist=mainlist->head->next;
                traverse=traverse_playlist->head;
                system("cls");
                for(int i=0 ; i<traverse_playlist->songnumber ;i++)
                {
                    cout << "Song " << i+1 << ": " << traverse->data->name << " " << traverse->data->singer << " "
                            << traverse->data->year << " " << traverse->data->style  << endl;
                    traverse=traverse->next;
                }
                traverse=traverse_playlist->head;
                cout << "Enter the number of the song which you want to play for once." << endl ;
                cin >> number ;


                for(int i=0 ; i < number-1; i++)
                    traverse=traverse->next;

                cout << "Play >" << endl;
                cout << "Playing . . ." << endl ;
                cout << traverse->data->name << " - " << traverse->data->singer << " - " << traverse->data->style << endl ;
                getchar();
                exit_condition = 'E';
                break;

            case 'E': case 'e':
                exit_condition='E';
                break;
            }
        }
        getchar();

    } else {
        cout << "Play >" << endl;
        while(exit_condition!= 'E' && exit_condition!='e') {

            cout << "Playing . . ." << endl ;
            cout << traverse->data->name << " - " << traverse->data->singer << " - " << traverse->data->style << endl ;
            cout << "NEXT(N) - PREVIOUS(P) - EXIT(E)" << endl ;

            cin.clear();
            cin.ignore(1000,'\n');
            choice=getchar();

            if(choice == 'N' || choice == 'n')
                traverse = traverse->next;
            else if(choice == 'P' || choice == 'p')
                traverse = traverse->prev;
            else exit_condition = 'E';
        }
    }
}

void createList() {
    char* name = new char[SNAME_LENGTH];
    int choice,choice2=0;
    song_node *traverse_songnode,*traverse_sorted;
    playlist *traverse,*all_list;
    string namestr;

    system("cls");
    cout << "Enter your new playlist name(Write as uppercase letters.Example: SLOW ). " << endl ;

    cin.ignore();
    cin.getline(name,50);

    do {
        cout << "For creating new list select the kind of the list." << endl ;
        cout << "1. Songs of a spesific style " << endl ;
        cout << "2. Songs of a spesific singer " << endl ;
        cout << "3. A combination of existing playlists " << endl ;
        cout << "4. A combination of existing songs " << endl ;

        cin>> choice ;
        if(choice >5 || choice < 0)
            cout << "Invalid option , please enter the option number that you want." << endl ;

    } while(choice > 5 || choice < 0);

    create_default(1,name);
    traverse=mainlist->head->prev;

    if(choice == 1) {

        cout << "Enter the spesific style which you want to add to the playlist.(All letters should be uppercase)." << endl ;

        cin.ignore();
        cin.getline(name,50);

        all_list=mainlist->head;
        traverse_sorted=all_list->head;


        for(int i=0 ; i< all_list->songnumber ; i++ ) {

            if(strcmp(name,traverse_sorted->data->style) == 0) {

                if(traverse->songnumber==0) {
                    traverse->head=new song_node;
                    traverse->head->data=traverse_sorted->data;
                    traverse_songnode = traverse->head;

                    traverse_songnode->next = traverse_songnode;
                    traverse_songnode->prev = traverse_songnode;
                    traverse_songnode->parent = traverse ;
                    traverse->songnumber++;

                } else {
                    traverse_songnode->next= new song_node;
                    traverse_songnode->next->prev=traverse_songnode;
                    traverse_songnode->next->next=traverse->head;

                    traverse_songnode=traverse_songnode->next;
                    traverse_songnode->parent=traverse;
                    traverse_songnode->data = traverse_sorted->data;
                    traverse->songnumber++;
                }
                traverse_sorted = traverse_sorted->next;
            } else
                traverse_sorted = traverse_sorted->next;
        }
    } else if(choice == 2) {
        cout << "Enter the spesific singer which you want to add to the playlist.(All letters should be uppercase)." << endl ;

        cin.ignore();
        cin.getline(name,50);

        all_list=mainlist->head;
        traverse_sorted=all_list->head;

        for(int i=0 ; i< all_list->songnumber ; i++ ) {

            if(strcmp(name,traverse_sorted->data->singer) == 0) {

                if(traverse->songnumber==0) {
                    traverse->head=new song_node;
                    traverse->head->data=traverse_sorted->data;
                    traverse_songnode = traverse->head;

                    traverse_songnode->next = traverse_songnode;
                    traverse_songnode->prev = traverse_songnode;
                    traverse_songnode->parent = traverse ;
                    traverse->songnumber++;

                } else {
                    traverse_songnode->next= new song_node;
                    traverse_songnode->next->prev=traverse_songnode;
                    traverse_songnode->next->next=traverse->head;

                    traverse_songnode=traverse_songnode->next;
                    traverse_songnode->parent=traverse;
                    traverse_songnode->data = traverse_sorted->data;
                    traverse->songnumber++;
                }
                traverse_sorted = traverse_sorted->next;
            } else
                traverse_sorted = traverse_sorted->next;
        }
    } else if(choice == 3) {

        while(1) {
            all_list=mainlist->head;
            system("cls");
            for(int i=0 ; i< mainlist->playlist_count-1 ; i++) {
                cout << "List " << i+1 << ": " << all_list->name  << endl;

                all_list=all_list->next;
            }

            cout << "Enter the number of the playlist which do you want to add to your list.(-1 for finish adding.)" << endl ;
            cin >> choice2 ;

            if(choice2== -1) break;

            all_list=mainlist->head;
            for(int i=0 ; i<choice2-1 ; i++)
                all_list=all_list->next;

            traverse_sorted=all_list->head;
            for(int i=0; i< all_list->songnumber ; i++) {
                if(traverse->songnumber==0) {
                    traverse->head=new song_node;
                    traverse->head->data=traverse_sorted->data;
                    traverse_songnode = traverse->head;

                    traverse_songnode->next = traverse_songnode;
                    traverse_songnode->prev = traverse_songnode;
                    traverse_songnode->parent = traverse ;
                    traverse->songnumber++;

                } else {
                    traverse_songnode->next= new song_node;
                    traverse_songnode->next->prev=traverse_songnode;
                    traverse_songnode->next->next=traverse->head;

                    traverse_songnode=traverse_songnode->next;
                    traverse_songnode->parent=traverse;
                    traverse_songnode->data = traverse_sorted->data;
                    traverse->songnumber++;

                }
                traverse_sorted=traverse_sorted->next;
            }
            cout << "List has been added." << endl ;
            getchar();
        }
    } else {

        while(1) {
            traverse_sorted=mainlist->head->next->head;
            system("cls");
            for(int i=0 ; i<traverse_sorted->parent->songnumber ; i++) {
                cout << "Song " << i+1 << ": " << traverse_sorted->data->name << " " << traverse_sorted->data->singer << " "
                     << traverse_sorted->data->year << " " << traverse_sorted->data->style  << endl;

                traverse_sorted = traverse_sorted ->next;
            }

            cout << "Enter the number of the song which do you want to add to your list.(-1 for finish adding.)" << endl ;
            cin >> choice2 ;
            if(choice2== -1) break;

            for(int i=0 ; i<choice2-1 ; i++)
                traverse_sorted=traverse_sorted->next;

            if(traverse->songnumber==0) {
                traverse->head=new song_node;
                traverse->head->data=traverse_sorted->data;
                traverse_songnode = traverse->head;

                traverse_songnode->next = traverse_songnode;
                traverse_songnode->prev = traverse_songnode;
                traverse_songnode->parent = traverse ;
                traverse->songnumber++;
                cout << "Song has been added." << endl ;
                getchar();
            } else {
                traverse_songnode->next= new song_node;
                traverse_songnode->next->prev=traverse_songnode;
                traverse_songnode->next->next=traverse->head;

                traverse_songnode=traverse_songnode->next;
                traverse_songnode->parent=traverse;
                traverse_songnode->data = traverse_sorted->data;
                traverse->songnumber++;
                cout << "Song has been added." << endl ;
                getchar();
            }
        }
    }
}

void deleteList(bool key, playlist* operation) {

    playlist *traverse = mainlist->head;
    song_node *traverse_song;
    int choice=0;

    if(key==0) {
        system("cls");
        cout << "PLAYLISTS :" << endl ;
        for(int i=0; i<mainlist->playlist_count; i++) {
            cout << i+1 << ": " << traverse->name << endl ;

            traverse=traverse->next;
        }
        cout << "Select one of the playlists for remove the list.(Enter the number of the list.)" << endl ;
        do {
            cin >> choice ;

            if(choice > mainlist->playlist_count || choice < 0)
                cout << "You have entered invalid number.Please enter valid number." << endl ;
            else if(choice > 0 && choice <4) {
                cout << "You can not remove the default playlists (first tree lists)" << endl <<"Exiting from operation."  ;
                getchar();
                return;
            }
        } while(choice > mainlist->playlist_count || choice < 0);

        traverse=mainlist->head;
        for(int i=0; i<choice-1; i++)
            traverse = traverse->next;

    } else
        traverse=operation;

    traverse_song=traverse->head->prev;

    while(traverse_song != traverse->head) {
        traverse_song=traverse_song->prev;
        delete traverse_song->next;
        traverse->songnumber --;
    }
    delete traverse_song;

    traverse->next->prev = traverse->prev;
    traverse->prev->next=traverse->next;
    mainlist->playlist_count--;
    delete [] traverse->name;
    delete traverse;
}

void printList() {
    playlist *traverse=mainlist->head;
    playlist *traverse_selected=mainlist->head;
    char exit_condition='A', choice,choice2;
    int playlist_no=0;
    while(exit_condition!='E' && exit_condition != 'e' ) {
        exit_condition='A';
        system("cls");
        cout << "PLAYLISTS :" << endl ;
        for(int i=0; i<mainlist->playlist_count; i++) {
            cout << i+1 << ": " << traverse->name << endl ;


            traverse=traverse->next;
        }
        cout << endl << playlist_no+1 << ": " << traverse_selected->name << endl ;
        cout << "SELECT(S) - NEXT(N) - PREVIOUS(P) - EXIT(E)" << endl ;

        getchar();
        choice=getchar();
        switch(choice) {
        case 'n' :
        case 'N':
            traverse_selected=traverse_selected->next;
            playlist_no++;
            if(playlist_no>=mainlist->playlist_count)
                playlist_no=0;
            break;

        case 'P' :
        case 'p' :
            traverse_selected=traverse_selected->prev;
            playlist_no--;
            if(playlist_no < 0)
                playlist_no=mainlist->playlist_count -1;
            break;

        case 'S':
        case 's':
            while(exit_condition!= 's' && exit_condition!= 's') {
                cout << "Choose the operation for this playlist." << endl ;
                cout << "Delete Playlist(D), Add song(A), Remove Song(R),Play songs in playlist(P),Return List Choice Menu(E):" << endl ;
                getchar();
                choice2=getchar();
                switch(choice2) {
                case 'D':
                case 'd':
                    if(playlist_no>=0 && playlist_no < 3) {
                        cout << "You select default playlist.You can't remove default playlist." << endl ;
                        break;
                    }
                    deleteList(1,traverse_selected);
                    break;

                case 'A':
                case 'a':
                    if(playlist_no>=0 && playlist_no < 3) {
                        cout << "You select default playlist.You can't remove or add any song from it." << endl ;
                        break;
                    }
                    addSong(1,traverse_selected);
                    break;

                case 'R':
                case 'r':
                    if(playlist_no>=0 && playlist_no < 3) {
                        cout << "You select default playlist.You can't remove or add any song from it." << endl ;
                        break;
                    }
                    removeSong(1,traverse_selected);
                    break;

                case 'P':
                case 'p':
                    play(1,traverse_selected);
                    break;
                case 'E': case 'e':
                    exit_condition= 's';
                    break;
                }

            }
            break;

        case 'E':
        case 'e':
            exit_condition='e';
            break;
        default:
            cout << "Your choice is invalid please choose again." << endl ;
            getchar();
            break;
        }
    }
}

void removeSong(bool key, playlist * operation) {
    playlist *traverse = mainlist->head;
    song_node *traverse_song;
    int choice=0;

    if(key==0) {
        system("cls");
        cout << "PLAYLISTS :" << endl ;
        for(int i=0; i<mainlist->playlist_count; i++) {
            cout << i+1 << ": " << traverse->name << endl ;
            traverse=traverse->next;
        }
        cout << "Select one of the playlist for remove song within.(Enter the number of the list.)" << endl ;
        do {
            cin >> choice ;

            if(choice > mainlist->playlist_count || choice < 0)
                cout << "You have entered invalid number.Please enter valid number." << endl ;
            else if(choice > 0 && choice <4) {
                cout << "You can not remove any songs from default playlist(first tree lists)" << endl <<"Exiting from operation."  ;
                getchar();
                return;
            }
        } while(choice > mainlist->playlist_count || choice < 0);

        traverse=mainlist->head;
        for(int i=0; i<choice-1; i++)
            traverse = traverse->next;
    } else
        traverse=operation;

    traverse_song=traverse->head;
    for(int i=0; i < traverse->songnumber ; i++) {
        cout << "Song " << i+1 << ": " << traverse_song->data->name << " " << traverse_song->data->singer << " "
             << traverse_song->data->year << " " << traverse_song->data->style  << endl;

        traverse_song = traverse_song ->next;
    }

    traverse_song=traverse->head;
    cout << "Please enter the song number which you want to remove from list." << endl ;
    cin >> choice ;

    for(int i=0 ; i<choice-1 ; i++) {
        traverse_song=traverse_song->next;
    }
    if(choice==1)
    traverse_song->parent->head=traverse_song->next;
    traverse_song->prev->next = traverse_song->next;
    traverse_song->next->prev = traverse_song->prev;
    traverse_song->parent->songnumber --;

    delete traverse_song;
}

void addSong(bool key, playlist* operation) {
    int choice;
    playlist *traverse = mainlist->head;
    song_node *traverse_song,*tmppp;

    if(key==0) {
        system("cls");
        cout << "PLAYLISTS :" << endl ;
        for(int i=0; i<mainlist->playlist_count; i++) {
            cout << i+1 << ": " << traverse->name << endl ;
            traverse=traverse->next;
        }
        cout << "Select one of the user generated playlist for add song to list.(Enter the number of the list.)" << endl ;
        do {
            cin >> choice ;

            if(choice > mainlist->playlist_count || choice < 0)
                cout << "You have entered invalid number.Please enter valid number." << endl ;
            else if(choice > 0 && choice <4) {
                cout << "You can not add any song to default playlist(first tree lists)" << endl <<"Exiting from operation."  ;
                getchar();
                return;
            }
        }while(choice > mainlist->playlist_count || choice < 0);

        traverse=mainlist->head;
        for(int i=0; i<choice-1; i++)
            traverse = traverse->next;
    } else
        traverse=operation;

    song_node *sorted_song = mainlist->head->next->head;

    for(int i=0; i< mainlist->head->next->songnumber ; i++) {
        cout << "Song " << i+1 << ": " << sorted_song->data->name << " " << sorted_song->data->singer << " "
             << sorted_song->data->year << " " << sorted_song->data->style  << endl;

        sorted_song = sorted_song ->next;
    }
    sorted_song=mainlist->head->next->head;
    cout << "Please enter the song number which you want to add to the list." << endl ;
    cin >> choice ;

    for(int i=0 ; i<choice-1 ; i++) {
        sorted_song=sorted_song->next;
    }

    traverse_song=traverse->head;
    tmppp=traverse_song->prev;

    traverse_song->prev->next=new song_node;
    traverse_song->prev=traverse_song->prev->next;
    traverse_song->prev->next=traverse_song;
    traverse_song->prev->prev=tmppp;

    traverse_song=traverse_song->prev;
    traverse_song->data=sorted_song->data;
    traverse_song->parent=traverse;
    traverse->songnumber ++;

    cout << traverse_song->data->name <<  traverse_song->data->singer <<  traverse_song->data->style << traverse_song->data->year << endl ;

}

void sorting() {
    int sorted=0;               //number of sorted song
    bool flag=1;                //if flag is 1 it means it is okay to add
    playlist *traverse = mainlist->head->next;
    cur = (traverse->prev->songnumber);
    song_node *sort_traverse_node;

    while(sorted != cur) {      //while sorted is not same with chronological list number
        song_node *tmp = mainlist->head->head;
        song_node *biggest=tmp;
        tmp=tmp->next;

        while(tmp!=mainlist->head->head) {
            flag=1;

            if( strcmp(tmp->data->singer,biggest->data->singer) <0 ) {
                for(int i=0; i<sorted; i++)
                    if(used_nodes[i] == tmp ) {
                        flag=0;
                    }
                if(flag==1) {
                    biggest=tmp;
                    tmp=tmp->next;
                } else {
                    tmp=tmp->next;
                }
            } else if (strcmp(tmp->data->singer,biggest->data->singer) > 0) {
                for(int i=0; i<sorted; i++)
                    if(used_nodes[i] == biggest ) {
                        flag=0;
                    }
                if(flag==0) {
                    biggest=tmp;
                    tmp=tmp->next;
                } else
                    tmp=tmp->next;
            } else {
                if( strcmp(tmp->data->name,biggest->data->name) <0 ) {
                    for(int i=0; i<sorted; i++)
                        if(used_nodes[i] == tmp ) {
                            flag=0;
                        }

                    if(flag==1) {
                        biggest=tmp;
                        tmp=tmp->next;
                    } else
                        tmp=tmp->next;
                } else if(strcmp(tmp->data->name,biggest->data->name) == 0) {
                    for(int i=0; i<sorted; i++)
                        if(used_nodes[i] == tmp ) {
                            flag=0;
                        }

                    if(flag==1) {
                        biggest=tmp;
                        tmp=tmp->next;
                    } else
                        tmp=tmp->next;
                } else
                    tmp=tmp->next;
            }
        }  // end of inner while  one loop

        if(sorted == 0) {   //adding head to sorted list

            traverse->head = new song_node;
            traverse->head->data = biggest->data;
            used_nodes[sorted] = biggest;
            traverse->head->next = traverse->head;
            traverse->head->prev = traverse->head;
            traverse->head->parent =traverse;

            traverse->songnumber ++;
            sorted++;
            sort_traverse_node = traverse->head;
        } else if (sorted !=0 ) {               //adding nodes to sorted list
            sort_traverse_node->next = new song_node;
            sort_traverse_node->next->prev = sort_traverse_node;
            sort_traverse_node = sort_traverse_node->next;

            sort_traverse_node->parent = traverse;
            sort_traverse_node->next = traverse->head;
            sort_traverse_node->next->prev = sort_traverse_node;
            sort_traverse_node->data = biggest->data ;
            used_nodes[sorted] = biggest;
            traverse->songnumber ++;
            sorted ++;
        }
    }
}

void writeToFile() {
    FILE *songbook;
    char *filename,*keys;
    playlist *traverse = mainlist->head;
    playlist *lastList = mainlist->head->next->next;
    song_node *p;

    keys=(char *)"*****";
    filename = (char *) "songbook.txt";

    if( !(songbook = fopen(filename,"w++")) ) {
        cerr << "File couldn't be opened" << endl;
        cerr << "Please try again later!" << endl;
        return;
    }
    fseek(songbook,0,SEEK_SET);

    while(traverse != lastList ) {
        p=traverse->head;
        if(traverse != mainlist->head)
            fprintf(songbook,"%s\n%s\n",keys,traverse->name);

        for(int i=0; i<traverse->songnumber; i++) {
            fprintf(songbook,"%s\t%s\t%d\t%s\n",
                    p->data->name,p->data->singer,p->data->year,p->data->style);
            p=p->next;
        }
        traverse=traverse->prev;
    }
    fclose(songbook);
    getchar();
}

void exit(){
    playlist *traverse=mainlist->head;
    playlist *traverse_saver=traverse;
    song_node *p,*q;
    for(int i=0; i<mainlist->playlist_count ;i++){    //delete all lists
        p=traverse->head;
        for(int j=0 ; j<traverse->songnumber ; j++){

            if( i == mainlist->playlist_count-1 || i>2 )          //if the list is chronological delete song datas
            {
                delete [] p->data->name;
                delete [] p->data->singer;
                delete [] p->data->style;
            }
            q=p;
            if(j != traverse->songnumber-1)
                p=p->next;

            q->next=NULL;
            q->parent=NULL;
            q->prev=NULL;

            delete q;
        }
        traverse_saver=traverse;

         if(i != mainlist->playlist_count-1)
            traverse=traverse->next;
        traverse_saver->next=NULL;
        traverse_saver->prev=NULL;
        traverse_saver->songnumber=0;
        traverse_saver->head=NULL;

        delete traverse_saver;
    }
    delete mainlist;
}
