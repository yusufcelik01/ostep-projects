#include <stdio.h>
#include <stdlib.h>


int* compute_prefix_array(char* pat, size_t m);
int kmp_search(char* pat, int m, int* lpa, char* txt);

int main(int argc, char** argv){
    if(argc < 2){
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }
    char* pattern = argv[1];
    int* prefix_function = NULL;
    int m;//size of the pattern

    for(m = 0; pattern[m] != '\0'; m++) 
        ;

    prefix_function = compute_prefix_array(pattern, m);


    if(argc == 2){//pattern given but no files
        int i = 0;
        int line_size = 64;
        char* line;
        char c;

        line = malloc(line_size * sizeof(char));
        while( (c = getc(stdin)) != EOF){
            line[i++] = c;
            if(i >= line_size){
                line_size += line_size;
                line = realloc(line, line_size * sizeof(char));
                if(line == NULL){
                    fprintf(stderr, "MEMORY ERROR\n");
                    exit(1);
                }
            }

            if(c == '\n'){
                line[i] = '\0';
                //kmp_search expects a null terminated str
                if(kmp_search(pattern, m, prefix_function, line)){
                    int j = 0;
                    while(putchar(line[j++]) != '\n'){
                    }
                }
                i = 0;//start reading new line
            }

        }
        
        
        free(line);
    }
    else{//both pattern and files are present
        for(int fileno=2; fileno < argc; fileno++){
            FILE* fileptr = fopen(argv[fileno], "r");
            if(fileptr == NULL){
                printf("wgrep: cannot open file\n");
                free(prefix_function);
                exit(1);
            }

            int i = 0;
            int line_size = 64;
            char* line;
            char c;

            line = malloc(line_size * sizeof(char));
            while( (c = getc(fileptr)) != EOF){
                line[i++] = c;
                if(i >= line_size){
                    line_size += line_size;
                    line = realloc(line, line_size * sizeof(char));
                    if(line == NULL){
                        fprintf(stderr, "MEMORY ERROR\n");
                        exit(1);
                    }
                }

                if(c == '\n'){
                    line[i] = '\0';
                    //kmp_search expects a null terminated str
                    if(kmp_search(pattern, m, prefix_function, line)){
                        int j = 0;
                        while(putchar(line[j++]) != '\n'){
                        }
                    }
                    i = 0;//start reading new line
                }

            }
            free(line);
            fclose(fileptr);
        }
    }




    free(prefix_function);

    return 0;
}


int kmp_search(char* pat, int m, int* lpa, char* txt){

    int i, j;
    
    //int* lpa = compute_prefix_array(pat, m);

    for(i = 0, j = -1; txt[i] != '\0'; i++){

        while(j> -1 && txt[i] != pat[j+1]){
            j = lpa[j];
        }
        if(txt[i] == pat[j+1]){
            j++;
        }
        if(j+1 == m){
            return 1;//match found print the line
        }

    }


    //free(lpa);
    return 0;// no matches have been found
}

int* compute_prefix_array(char* pat, size_t m){

    int* prefix_array = calloc(m, sizeof(int));

    int k = -1;
    prefix_array[0] = k;
    for(int q=1; q<m; q++){
        while(k > -1 && pat[k+1] != pat[q]){
            k = prefix_array[k];
        }
        if(pat[k+1] == pat[q]){
            k++;
        }
        prefix_array[q] = k;
    }

    return prefix_array;
}
