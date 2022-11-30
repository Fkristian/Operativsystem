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
    int pageSice = 256;
    int addr[7] = {1, 256, 32768, 32769, 128, 65534, 33153};
    int physicalMemory[pageSice][pageSice];
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
        // printf("Page: %d, Offset: %d \n", page, offset);
    }

    //Function start here
    int addres = 53683;
    
    unsigned char buffer[pageSice];
    int startOfPage = addres % pageSice;  //  53504
    FILE *file2;
    
    file2 = fopen("data/BACKING_STORE.bin","rb");  // r for read, b for binary
    
    fseek(file2, addres, SEEK_SET);  //file är backstoreing, 3098 är ?, seek_set= ??

    fread(buffer,sizeof(buffer),1,file2);

                
    int frameNumber = startOfPage / 256;            //find frame number
    printf("The frame loaded is: \n");
    for(int i = 0; i < pageSice; i++){  	        //read out the frame got.
        physicalMemory[frameNumber][i] = buffer[i];     //fills in the pysical memory from what was gatherd
        printf("%d ", physicalMemory[frameNumber][i]);   
    } 
        printf("\n");


    //Function end here

    exit(0);
}