#include <stdint.h>

volatile uint8_t flag = 0;

int main() {
    while (1) {
        if (flag)  break;
    }
    while (1) {}
    return 0;
}