#include <stdio.h>
#include <stdlib.h>

int main(){

    unsigned char buffer[256];
    FILE *file;

    file = fopen("data/BACKING_STORE.bin","rb");  // r for read, b for binary
    
    fseek(file, 53504, SEEK_SET);  //file är backstoreing, 3098 är ?, seek_set= ??

    fread(buffer,sizeof(buffer),1,file);

// for(int i = 0; i<256; i++){  	        //read out the frame got.
//     printf("%u ", buffer[i]); 
// }
    printf("%u ", buffer[0]); 

    printf("\n");
    return 0;
}