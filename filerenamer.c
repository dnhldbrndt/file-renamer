#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <io.h>
#include <time.h>
#include <errno.h>


char *build_timestring(int year, int month, int day) {
    int size = 10;
    char *tmp_string = malloc(size+1);

    snprintf(tmp_string, 11,"%d-%d-%d", year + 1900, month + 1, day);
    return tmp_string;
}
char *add_fileextension(char *string, char *type) {
    int size = strlen(string);
 
    char *tmp_string = malloc(size+1);

    snprintf(tmp_string, size+5,"%s%s", string, type);
 
    return tmp_string;
}
int findindex (char *string, char p){
    int index = 0;
    int size = strlen(string);
 
    for (int i = 0; i < size; i++) {

        if (p == string[i]){

            index = i;
            return index;

        }
    }
    return -1;
}
char *get_fileversion (char *string, int begin, int end) {
    int length = end - begin;
    int string_size = strlen(string);
    char *tmp_string;
    int count = 0;
    tmp_string = malloc(length);
 
    for (int i = begin+1; i < end; i++) {

        tmp_string[count] = string[i];
        count++;

    }
    return tmp_string;
}
char *append_fileexists(char *string){
    int size = strlen(string);
    char *tmp_string = malloc(size+5);      
      
    if (findindex(string, '(') < 0) {

        snprintf(tmp_string, 19,"%s (1)", string);

    } else {

        int indexp_open = findindex(string, '(');
        int indexp_close = findindex(string, ')');
        char res_string[10];
        strcpy(res_string, get_fileversion(string,indexp_open,indexp_close));
        char *ptr;
        int new_number = strtol(res_string,&ptr,10) + 1;
        char stringstart[10];
        strncpy(stringstart, string, indexp_open-1);
        snprintf(tmp_string,19,"%s (%d)",stringstart,new_number);

    }
    return tmp_string;
}


void print_chars (char *string) {
    int size = strlen(string);

    printf("Size: %d\n", size );

    for (int i = 0; i < size; i++) {
        printf("[%c] ", string[i]);
    }
    printf("\n");
}

void rename_files () {

    // get all files in directory
    // get dates for individual files
    // rename all files with their dates
    // exit
    struct _finddata_t c_file;
    intptr_t hFile;
    struct tm newtime;
    errno_t err;
    char old_fname[100];
    char new_fname[100];
    char tmp[100];
    char *date;
    char *tmp_append;
    char *full_fname;
    /*
    if ((hFile = _findfirst("*.jpg", &c_file)) == -1L) {
        printf("No *.jpg files in current directory! \n");
    } else {
        printf("Listing of .jpg files\n");
        printf("-------------------------\n");
        printf( " %-12s %.24s %9ld\n", c_file.name, ctime( &( c_file.time_create ) ), c_file.size );
        while(_findnext(hFile, &c_file ) == 0 ){   
        printf( " %-12s %.24s %9ld\n", c_file.name, ctime( &( c_file.time_create ) ), c_file.size );

        }
  
    }   */  
    if ((hFile = _findfirst("*.jpg", &c_file)) == -1L) {
        printf("No *.jpg files in current directory! \n");
    } else {
        printf("Renaming all .jpg files\n");
        printf("-------------------------\n");
        
        do {   
 
            err = localtime_s(&newtime, &c_file.time_create);
            char *date = build_timestring(newtime.tm_year ,newtime.tm_mon ,newtime.tm_mday);
            //char *tmp_append;
            //char *full_fname;
 
            strcpy(old_fname,c_file.name);        
            strcpy(new_fname,date);
  
            full_fname = add_fileextension(date,".jpg");
            strcpy(tmp,new_fname);  
            printf("Name (Old): %s | Name (New Full): %s \n", old_fname, full_fname);
            print_chars(old_fname);
            print_chars(new_fname);
            print_chars(tmp);
            print_chars(full_fname);
            err = rename(old_fname, full_fname);

            if (err != 0 && errno != EACCES) { 
                while (errno == EEXIST) {
                printf("Unable to rename file.\n"); 

                tmp_append = append_fileexists(tmp);
                strcpy(tmp,tmp_append);  
                full_fname = add_fileextension(tmp_append,".jpg");
                err = rename(old_fname, full_fname);
                }
            } else if (errno == EACCES) {
                perror("Error: ");
                break; 
            } else {
                printf("File %s succesfully renamed.\n", c_file.name);
            }
            free(date);  
        } while(_findnext(hFile, &c_file ) == 0 );
        free(tmp_append); 
        free(full_fname); 
        _findclose( hFile );
    } 
}

int main (int argc, char *argv[]){
    
    rename_files();

    return 0;
}

