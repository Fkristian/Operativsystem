#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
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
        printf("Too few argumentos\n");
        exit(0);
    }
    int numberOfPageFaults = 0;
    int numberAdresses = 0;
    char line[8];
    int length;
    FILE * file = fopen (filename, "r"); 
    int pageTabel[FRAMES] = {[0 ... FRAMES-1] = -1};
    int pageNumber;
    int offset;
    int addres;
    int switchVar =1;
    char buffer[SIZE];
    int startOfPage;
    int frameNumber;
    int pysicalAdress;
    
    while (fgets(line, sizeof(line), file)) {
        pageNumber = extract_page(atoi(line));
        offset = extract_offset(atoi(line));
       
        addres = atoi(line);
        
    
        if(pageTabel[pageNumber] == -1){
            switchVar = 1;
        }else{
            switchVar = 2;
        }

        switch (switchVar)
        {
        case 1:
            
            startOfPage = addres - offset; 
            FILE *file2;
            
            file2 = fopen("data/BACKING_STORE.bin","rb"); 
            
            fseek(file2, startOfPage, SEEK_SET);  

            fread(buffer,sizeof(buffer),1,file2);

                    

            frameNumber = find_free();

             
            for(int i = 0; i < SIZE; i++){  	       
                physicalMemory[frameNumber][i] = buffer[i];        
            } 
            pysicalAdress = frameNumber * SIZE + offset;

            pageTabel[pageNumber] = frameNumber;
            printf("Virtual address: %d Physical address: %d Value: %d", addres, pysicalAdress, physicalMemory[frameNumber][offset]);
            printf("\n");
            numberOfPageFaults++;
            numberAdresses++;
           
            break;

        case 2:

            frameNumber = pageTabel[pageNumber];
            pysicalAdress = frameNumber * SIZE + offset;
            printf("Virtual address: %d Physical address: %d Value: %d", addres, pysicalAdress, physicalMemory[frameNumber][offset]);
            printf("\n");
            numberAdresses++;
        
            break;
        }
  
    }
    printf("%d %d \n", numberOfPageFaults, numberAdresses);
    float hold = numberOfPageFaults / (float)numberAdresses;
    printf("Page-fault rate %.2f", hold);

    exit(0);
}