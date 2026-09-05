//
// Created by os on 9/2/26.
//

#ifndef PROJECT_BASE_V1_1_PRINTING_H
#define PROJECT_BASE_V1_1_PRINTING_H
#include "../lib/console.h"



inline void printString(const char* str) {
    while (*str) {
        __putc(*str++);
    }
}

inline void printInt(int num) {
    if (num == 0) {
        __putc('0');
        return;
    }
    if (num < 0) {
        __putc('-');
        num = -num;
    }
    char buf[12];
    int i = 0;
    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    while (i > 0) {
        __putc(buf[--i]);
    }
}


#endif //PROJECT_BASE_V1_1_PRINTING_H
