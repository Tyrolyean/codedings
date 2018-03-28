
/*
 bf.c - a brainfuck interpreter in c 
 Copyright © 2018 tyrolyean 

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

bool debug = false;
int cursor_offset = 0;
signed int cursor = 0;
char * text = NULL;
char *band = NULL;
bool exitting = false;
unsigned int band_size = 0;
int switch_brace_count = 0;

void alter_position(bool positive);
int main(int argc, const char *argv[]){
        
        for(int i = 0; i < argc; i++){
                if(strcmp("-d",argv[i]) == 0){
                        debug = true;
                }
        }
        

        int fd = open(argv[argc-1],O_RDONLY);
        
        struct stat st;                                                         
        stat(argv[argc-1], &st);                            
        text = malloc(st.st_size);
        
        read(fd,text,st.st_size);

        if(debug){
                printf("INIT\n");
        }

        band_size = 1;
        band = malloc(band_size);
        memset(band,0,band_size);
        for(int i = 0; i < st.st_size; i++){
                
                switch(text[i]){

                case '+':
                        band[cursor+cursor_offset]++;
                        break;
                case '-':
                        band[cursor+cursor_offset]--;
                        break;
                case ',':
                        printf("Awaiting input at %i:",i);
                        band[cursor+cursor_offset] = getchar();
                        break;
                case '.':
                        printf("%c",band[cursor+cursor_offset]);
                        fflush(stdout);
                        break;
                case '<':
                        alter_position(false);
                        break;
                case '>':
                        alter_position(true);
                        break;
                case '[':
                        // If exit condition
                        if(band[cursor + cursor_offset] == 0){
                                switch_brace_count = 1;

                                for(int ii = i+1; ii < st.st_size; ii++){
                                        if(text[ii] == '['){
                                                switch_brace_count++;
                                        }else if(text[ii] == ']'){
                                                switch_brace_count--;
                                        }
                                        
                                        if(switch_brace_count == 0){
                                                // I have found my closing!
                                                // Now ignore it and go on
                                                i = ii;
                                                if(debug){
                                                        printf("leaving array to %i\n",
                                                                i);
                                                }
                                                break;
                                        }
                                }

                        }else{
                                continue;
                        }

                        break;

                case ']':
                        // Scan for my [
                        switch_brace_count = 1;
                        if(band[cursor + cursor_offset] == 0){
                                continue;
                        }
                        for(int ii = i-1; ii >=0;ii--){
                                
                                if(text[ii] == '['){
                                        switch_brace_count--;
                                }else if(text[ii] == ']'){
                                        switch_brace_count++;
                                }
                                        
                                if(switch_brace_count == 0){
                                        // I have found my closing!
                                        // Now ignore it and go on
                                        i = --ii;
                                        break;
                                }
                        }

                        break;

                }

        }
        printf("\n"); 
        return 0;
}

void alter_position(bool positive){

        if(positive){
                cursor++;

                if(band_size <= (cursor + cursor_offset)){
                        
                        if(debug){
                                printf("altering array size to %i\n",
                                                band_size+1);
                        }

                        char* band_tmp = malloc(++band_size);
                        memcpy(band_tmp,band,band_size-1);
                        band[cursor+cursor_offset] = 0;
                        //free(band);
                        band = band_tmp;

                }

        }else{

                cursor--;
                if((cursor+cursor_offset) < 0){
                
                        if(debug){
                                printf("altering array size to %i\n",
                                                band_size+1);
                        }

                        // under the 0 index...
                        // make it 1 larger and shift the array by one
                        char* band_tmp = malloc(++band_size);
                        memcpy(band_tmp+1,band,band_size-1);
                        //free(band);
                        band = band_tmp;
                        cursor_offset++;

                }
        }

        
        if(debug){
                printf("altering cursor positione to %i and offset to %i\n",
                                cursor,cursor_offset);
        }

        return;
}
