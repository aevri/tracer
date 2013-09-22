#include <stdio.h>
int main() {
    printf("P6 512 512 255 ");
    for (int x=0; x<512; ++x) {
        for (int y=0; y<512; ++y) {
            printf("%c%c%c", 255, 128, 128);
        }
    }
}
