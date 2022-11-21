#include <stdio.h>
#include <stdlib.h>

//assumes 16bit input
int extract_page(int addr){
    return (addr & 0xFF00) >> 8;
}

//assumes 16bit input
int extract_offset(int addr){
    return addr & 0x00FF;
}

int main(int argc, char *argv[]){

    int addr[7] = {1, 256, 32768, 32769, 128, 65534, 33153};

    char* filename;

    if(argc > 1){
        filename = argv[1];
    }
    else{
        printf("Too few argumentos\n");
        exit(0);
    }
    
    char line[8];
    int length;
    FILE * file = fopen (filename, "r"); 

    int page;
    int offset;
    
    while (fgets(line, sizeof(line), file)) {
        page = extract_page(atoi(line));
        offset = extract_offset(atoi(line));
        printf("Page: %d, Offset: %d \n", page, offset);
    }

    exit(0);
}