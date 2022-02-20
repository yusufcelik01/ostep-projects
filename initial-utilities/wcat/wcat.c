#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    if(argc < 2){
        exit(0);
    }

    int i;

    for(i=1; i<argc; i++){
        FILE* file;
        file = fopen(argv[i], "r");
        if(file == NULL){
            printf("wcat: cannot open file\n");
            exit(1);
        }

        char buffer[1024];
        while(fgets(buffer, 1024, file) != NULL){
            printf("%s", buffer);
        }

        fclose(file);
    }
    
    return 0;
}
