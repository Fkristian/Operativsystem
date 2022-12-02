#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*************** START OF GLOBALS **************************/

#define FRAMES 256
#define SIZE 256
#define TLB_SIZE 16

int physicalMemory[FRAMES][SIZE];
int frame_available_map[FRAMES]; //true is available

/***************** END OF GLOBALS **************************/

/*************** START OF TLB STUFF **************************/
//create a TLB as an array of rows with given data
struct TLBrow
{
    int pageNr;
    int frame;
    int age; 
    bool empty;
} tlb[TLB_SIZE];

void TLB_init(){
    for(int i = 0; i < TLB_SIZE; i++){
        (&tlb[i])->empty = true;
    }
}

int TLB_check_page(int pageNr){
    for(int i = 0; i < TLB_SIZE; i++){
        if((&tlb[i])->pageNr == pageNr){
            return i;
        }
    }
    return -1;   
}

int TLB_get_frame(int pageNr){
    return (int)(&tlb[TLB_check_page(pageNr)])->frame;
}

//inserts a new item to TLB, if full -> replace the oldest
void TLB_insert(int pageNr, int frame){
    int oldest = 0;
    //check for empty slot
    for(int i = 0; i < TLB_SIZE; i++){
        if((&tlb[i])->empty == true){
            (&tlb[i])->pageNr = pageNr;
            (&tlb[i])->frame = frame;
            (&tlb[i])->age = 0;
            (&tlb[i])->empty = false;
            return;
        }
        //check for index of the oldest tlb item
        if((&tlb[i])->age > (&tlb[oldest])->age){
            oldest = i;
        }
    }
    //if we havn't found any empty slot, replace the oldest item
        (&tlb[oldest])->pageNr = pageNr;
        (&tlb[oldest])->frame = frame;
        (&tlb[oldest])->age = 0;
    return;
}

//increments the age of all non-empty slots
void TLB_update(){
    for(int i = 0; i < TLB_SIZE; i++){
        if((&tlb[i])->empty == false){
            (&tlb[i])->age ++;
        }
    }
}

/********************** END OF TLB STUFF **********************/


/************* START OF PAGE TABLE STUFF **********************/

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

/*************** END OF PAGE TABLE STUFF ***********************/





int main(int argc, char *argv[]){
    
    //initialize tlb to be empty
    TLB_init();

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

    //variables for calculating page faults/TLB hits
    int numberOfPageFaults = 0;
    int numberAdresses = 0;
    int numberOfTLBHits = 0;
    
    //Loop through all the lines of addresses.txt
    while (fgets(line, sizeof(line), file)) {

        //get the address from the text file and extract pageNr & offset
        pageNumber = extract_page(atoi(line));
        offset = extract_offset(atoi(line));
        address = atoi(line);
        
        //check for TLB hit
        if(TLB_check_page(pageNumber) != -1){
            frameNumber = TLB_get_frame(pageNumber);
            physicalAdress = frameNumber * SIZE + offset;
            printf("Virtual address: %d Physical address: %d Value: %d", address, physicalAdress, physicalMemory[frameNumber][offset]);
            printf("\n");
            //printf("TLB HIT WOHOO !!!!!!!!!\n");
            numberAdresses++;
            numberOfTLBHits++;
        }
        else{
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
                
                //add page to TLB
                TLB_insert(pageNumber, frameNumber);

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
                TLB_insert(pageNumber, frameNumber);
                physicalAdress = frameNumber * SIZE + offset;
                printf("Virtual address: %d Physical address: %d Value: %d", address, physicalAdress, physicalMemory[frameNumber][offset]);
                printf("\n");
                numberAdresses++;
            
                break;
            }
        }

        TLB_update();
    }
    //printf("%d %d \n", numberOfPageFaults, numberAdresses);
    float hold = numberOfPageFaults / (float)numberAdresses;
    float holdTLB = numberOfTLBHits / (float)numberAdresses;

    printf("Page-fault rate %.2f\n", hold);
    printf("TLB-hit rate %.2f\n", holdTLB);

    exit(0);
}