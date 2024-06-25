#include <stdio.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const char style[] = ""
" ---------\n"
"|    .    |\n"
"|   / \\   |\n"
"|   UwU   |\n"
"|  /   \\  |\n"
"| /\\   /\\ |\n"
"|   | |   |\n"
"|   - -   |\n"
" ---------\n";

int main(int argc, char **argv) {
    int input_w, input_h, input_c, output_w = 80, output_h = 32, output_c;
    unsigned char * img = stbi_load(argv[1], &input_w, &input_h, &input_c, 0);
    
    if (img == NULL) {
        printf("Error loading image\n");
        return 1;
    }    
    
    for(int _ = 0; _ < 50; _++) printf("-"); printf("\n\n");
    printf("%s\n", style);
    printf("image loaded: %s\n", argv[1]);
    printf("image shape: (%d, %d, %d)\n", input_w, input_h, input_c);
    
    assert (input_c == 3 || input_c == 1); 

    output_c = input_c;
    switch (argc) {
        case 5:
            output_c = atoi(argv[4]);
            assert (output_c == 1 || output_c == 3);
        case 4:
            output_h = atoi(argv[3]);
        case 3:
            output_w = atoi(argv[2]);
            break;
    }

    unsigned char * resized_img = stbir_resize_uint8_srgb(img, input_w, input_h, 0, 
                                                          NULL, output_w, output_h, 0,
                                                          input_c);     // !
    if (resized_img == NULL) {
        printf("Error resizing image\n");
        stbi_image_free(img);
        return 1;
    }
    stbi_image_free(img);

    // ! actual shape : (output_w, output_h, input_c)
    printf("reshaped image: (%d, %d, %d)\n", output_w, output_h, output_c);
    for(int _ = 0; _ < 50; _++) printf("-"); printf("\n\n");

    // TEST: write
    // stbi_write_jpg("./test_out.jpg", output_w, output_h, input_c, resized_img, 100);

    unsigned char * pixel, r, g, b;
    unsigned char gray, g_mapped;
    long long pos;
    for (int y = 0; y < output_h; y++) {
        for (int x = 0; x < output_w; x++) {
            pos = (y*output_w + x)*input_c;     // !
            pixel = resized_img + pos;
            r = pixel[0], g = pixel[1],  b = pixel[2];
            
            if (output_c == 3)
                // \033[38;2;<r>;<g>;<b>m
                printf("\033[38;2;%d;%d;%dm█", r, g, b);
            else {
                gray = (unsigned char)floor(0.2126*pixel[0] + 0.7152*pixel[1] + 0.0722*pixel[2]);
                // printf("%hhu ", gray);
                g_mapped = 232 + (gray * 23) / 255;
                printf("\033[38;5;%dm█", g_mapped);
            }
        }
        printf("\n");
    }
    printf("\033[0m\n");

    stbi_image_free(resized_img);
    
    return 0;
}

