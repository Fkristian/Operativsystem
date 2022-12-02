#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define FRAMES 256
#define SIZE 256

int physicalMemory[FRAMES][SIZE];
int frame_available_map[FRAMES]; //true is available

int find_free() {
  for(int i = 0; i < FRAMES; i++) {
    if(frame_available_map[i] == false ) {
      frame_available_map[i] = true;
      return i;
    }
  }
  return -1;
}

//assumes 16bit input
int extract_page(int addr){
    return (addr & 0xFF00) >> 8;
}

//assumes 16bit input
int extract_offset(int addr){
    return addr & 0x00FF;
}

int main(int argc, char *argv[]){
    
    char* filename;
    if(argc > 1){
        filename = argv[1];
    }
    else{
        printf("Too few arguments\n");
        exit(0);
    }



    //variables for reading from file
    FILE * file = fopen (filename, "r"); 
    char line[8];
    char buffer[SIZE];

    //variables for page table
    int pageTabel[FRAMES] = {[0 ... FRAMES-1] = -1}; //initialize pagetable to -1 meaning no page is loaded
    int pageNumber;
    int offset;
    int address;
    int switchVar =1;
    int startOfPage;

    //variables for frames
    int frameNumber;
    int physicalAdress;

    //variables for calculating page faults
    int numberOfPageFaults = 0;
    int numberAdresses = 0;
    
    //Loop through all the lines of addresses.txt
    while (fgets(line, sizeof(line), file)) {

        //get the address from the text file and extract pageNr & offset
        pageNumber = extract_page(atoi(line));
        offset = extract_offset(atoi(line));
        address = atoi(line);
        
        //check if page is loaded to physical memory
        if(pageTabel[pageNumber] == -1){
            switchVar = 1;
        }
        else{
            switchVar = 2;
        }

        switch (switchVar)
        {
        
        //Page fault, get data from backing store and find free frame
        case 1: 
            startOfPage = address - offset; 
            FILE *file2;
            file2 = fopen("data/BACKING_STORE.bin","rb"); 
            
            fseek(file2, startOfPage, SEEK_SET);  
            fread(buffer,sizeof(buffer),1,file2);
            frameNumber = find_free();
            
            //load entire page
            for(int i = 0; i < SIZE; i++){  	       
                physicalMemory[frameNumber][i] = buffer[i];        
            } 
            physicalAdress = frameNumber * SIZE + offset;

            pageTabel[pageNumber] = frameNumber;
            printf("Virtual address: %d Physical address: %d Value: %d", address, physicalAdress, physicalMemory[frameNumber][offset]);
            printf("\n");
            numberOfPageFaults++;
            numberAdresses++;
           
            break;

        //Page is in pagetable, get frame from page table
        case 2:
            frameNumber = pageTabel[pageNumber];
            physicalAdress = frameNumber * SIZE + offset;
            printf("Virtual address: %d Physical address: %d Value: %d", address, physicalAdress, physicalMemory[frameNumber][offset]);
            printf("\n");
            numberAdresses++;
        
            break;
        }
  
    }
    //printf("%d %d \n", numberOfPageFaults, numberAdresses);
    float hold = numberOfPageFaults / (float)numberAdresses;
    printf("Page-fault rate %.2f", hold);

    exit(0);
}