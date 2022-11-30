#include <stdio.h>
#include <stdlib.h>

int main(){

    unsigned char buffer[256];
    FILE *file;

    file = fopen("data/BACKING_STORE.bin","rb");  // r for read, b for binary
    
    fseek(file, 30198, SEEK_SET);

    fread(buffer,sizeof(buffer),1,file);

    //for(int i = 0; i<256; i++)
    printf("%u ", buffer[0]); 

    printf("\n");
    return 0;
}