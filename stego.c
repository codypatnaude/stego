#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef unsigned char BYTE;

typedef struct {
    char *input_file;
    char *input_image;
    char *output_file;
} Settings;

static Settings settings;

int readbit(int pos, BYTE b){
    return (b >> pos) & 1;
}

BYTE writebit(int pos, int val, BYTE b){
    if(val > 0){
        b |= val << pos;
    }else{
        b &= ~(1 << pos);
    }
    
    return b;
}

void printbyte(BYTE b){
    for(int i = 0; i < 8; i++){
        printf("%d", readbit(i, b));
    }
}

void encrypt()
{
    printf("Output: %s\n", settings.output_file);
    printf("Image: %s\n", settings.input_image);
    printf("Output: %s\n", settings.input_file);

    //message input vars 
    FILE *msg_in_ptr = fopen(settings.input_file, "rb");
    fseek(msg_in_ptr, 0, SEEK_END);
    long msg_in_len = ftell(msg_in_ptr);
    rewind(msg_in_ptr);
    BYTE *msg_in_byte = (BYTE *)malloc(1);
    
    printf("msg len: %d\n", (int)msg_in_len);

    //image input vars
    FILE *img_in_ptr = fopen(settings.input_image, "rb");
    fseek(img_in_ptr, 0, SEEK_END);
    long img_in_len = ftell(img_in_ptr);
    rewind(img_in_ptr);
    BYTE *img_in_byte = (BYTE *)malloc(1);

    //output file
    FILE *img_out_ptr = fopen(settings.output_file, "w");

    printf("img len: %d\n", (int)img_in_len);

    int msg_pos = 0; //number of bytes we've read from input message
    int img_pos = 0; //number of bytes we've read from input image
    int byte_pos = 8; //position in the current byte
    while(img_pos < img_in_len){

        if(byte_pos == 8){

            printf("NEXT MESSAGE POS\n");
            if(msg_pos == msg_in_len){
                *msg_in_byte = '\x02'; //end of transmission char
                printf("EOT\n");
            }else if(msg_pos >= msg_in_len){
                *msg_in_byte = '\x03'; //skip char
            }
            else{
                fread(msg_in_byte, 1, 1, msg_in_ptr);
            }

            byte_pos = 0;
            ++msg_pos;
            
        }
        fread(img_in_byte, 1, 1, img_in_ptr);
        
        if(img_pos < 500){
            fwrite(img_in_byte, 1, 1, img_out_ptr);
            ++img_pos;
            continue;
        }

        if(*msg_in_byte != '\x03'){
            printf("BEFORE:\t");
            printbyte(*img_in_byte);
            printf("\n");

            int bit = readbit(byte_pos, *msg_in_byte);

            //set last two bits on each byte

            *img_in_byte = writebit(6, readbit(byte_pos, *msg_in_byte), *img_in_byte);
            byte_pos += 1;

            *img_in_byte = writebit(7, readbit(byte_pos, *msg_in_byte), *img_in_byte);
            byte_pos += 1;

            printf("AFTER:\t");
            printbyte(*img_in_byte);
            printf("\n");
            printf("\n");

            fwrite(img_in_byte, 1, 1, img_out_ptr);
        }else{
            fwrite(img_in_byte, 1, 1, img_out_ptr);
        }
        
        ++img_pos;
    }

    fclose(img_out_ptr);
    fclose(img_in_ptr);
    fclose(msg_in_ptr);
}

void decrypt(){
    printf("Output: %s\n", settings.output_file);
    printf("Image: %s\n", settings.input_image);
    printf("Output: %s\n", settings.input_file);

    //message input vars 
    FILE *input_ptr = fopen(settings.input_image, "rb");

    //if(!input_ptr){
    //    printf("File: %s does not exist", settings.input_image);
    //}

    fseek(input_ptr, 0, SEEK_END);
    long input_len = ftell(input_ptr);
    rewind(input_ptr);
    BYTE *input_byte = (BYTE *)malloc(1);

    remove(settings.output_file);

    printf("After remove\n");
    printf("LEN: %d\n", (int)input_len);

    //output file
    FILE *output_ptr = fopen(settings.output_file, "w");

    int img_pos = 0; //number of bytes we've read from input image
    while(img_pos < input_len){
        if(img_pos < 500){
            fread(input_byte, 1, 1, input_ptr);
            img_pos++;
            continue;
        }
        

        BYTE cur_byte = '\x00';
        int byte_pos = 0;
        for(int i = 0; i < 4; i++){
            fread(input_byte, 1, 1, input_ptr);
            img_pos++;

            int bit = readbit(6, *input_byte);
            cur_byte = writebit(byte_pos, bit, cur_byte);
            ++byte_pos;

            bit = readbit(7, *input_byte);
            cur_byte = writebit(byte_pos, bit, cur_byte);
            ++byte_pos;
        }

        if(cur_byte == '\x02'){
            break;
        }

        printf("%c", cur_byte);
        fwrite(&cur_byte, 1, 1, output_ptr);

    }

    fclose(input_ptr);
    fclose(output_ptr);
}

enum actions {
    ACTION_NULL = 0,
    ACTION_ENCRYPT = -1,
    ACTION_DECRYPT = -2
};

typedef enum ARG_TYPES {
    ARG_NULL = 0,
    ARG_IMG_INPUT = -1,
    ARG_FILE_INPUT = -2,
    ARG_FILE_OUT = -3
} ARG_TYPE;

int main(int argc, char **argv)
{
    int action = ACTION_NULL;
    ARG_TYPE arg_type = ARG_NULL;

    for(int i = 0; i < argc; ++i){
        printf("%s\n", argv[i]);
        if(strcmp(argv[i], "-o") == 0){
            printf("SETTING OUTPUT\n");
            ++i;
            settings.output_file = argv[i];
            continue;
        }
        if(strcmp(argv[i], "-f") == 0){
            printf("SETTING FILE INPUT\n");
            ++i;
            settings.input_file = argv[i];
            continue;
        }
        if(strcmp(argv[i], "-i") == 0){
            printf("SETTING IMAGE INPUT\n");
            ++i;
            settings.input_image = argv[i];
            continue;
        }
        if(strcmp(argv[i], "-encrypt") == 0){
            printf("ENCRYPTING\n");
            encrypt();
        }
        
        if(strcmp(argv[i], "-decrypt") == 0){
            printf("DECRYPTING\n");
            decrypt();
        }

    }

}


