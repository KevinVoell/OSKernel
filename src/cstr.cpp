#include "cstr.h"

char uintto_stringOutput[128];
const char* to_str(uint64_t value){
    uint8_t size;

    uint64_t sizetest = value;
    while(sizetest/10 > 0) {
        sizetest /= 10;
        size++;
    }

    uint8_t index = 0;
    while(value/10 > 0){
        uint8_t remainder = value % 10;
        value /= 10;
        uintto_stringOutput[size-index]= remainder + '0';
        index++;
    }

    uint8_t remainder = value % 10;
    uintto_stringOutput[size-index] = remainder + '0';
    uintto_stringOutput[size + 1] = 0;

    return uintto_stringOutput;
}

char intto_stringOutput[128];
const char* to_str(int64_t value){
    uint8_t isNegative = 0;

    if (value < 0) {
        isNegative = 1;
        value *= -1;

    }

    uint8_t size;
    uint64_t sizetest = value;
    while(sizetest/10 > 0) {
        sizetest /= 10;
        size++;
        intto_stringOutput[0] = '-';
    }

    uint8_t index = 0;
    while(value/10 > 0){
        uint8_t remainder = value % 10;
        value /= 10;
        intto_stringOutput[isNegative+size-index]= remainder + '0';
        index++;
    }

    uint8_t remainder = value % 10;
    intto_stringOutput[isNegative+size-index] = remainder + '0';
    intto_stringOutput[isNegative+size + 1] = 0;

    return intto_stringOutput;
}

char hexto_stringOutput[128];
const char* to_hstr(uint64_t value){
    uint64_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 -1;
    for(uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexto_stringOutput[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = (*ptr & 0x0F);
        hexto_stringOutput[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }

    hexto_stringOutput[size + 1] = 0;

    return hexto_stringOutput;
}

char hexto_stringOutput32[128];
const char* to_hstr(uint32_t value){
    uint32_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 4 * 2 -1;
    for(uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexto_stringOutput32[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = (*ptr & 0x0F);
        hexto_stringOutput32[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }

    hexto_stringOutput32[size + 1] = 0;

    return hexto_stringOutput32;
}

char hexto_stringOutput16[128];
const char* to_hstr(uint16_t value){
    uint16_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 2 * 2 -1;
    for(uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexto_stringOutput16[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = (*ptr & 0x0F);
        hexto_stringOutput16[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }

    hexto_stringOutput16[size + 1] = 0;

    return hexto_stringOutput16;
}

char hexto_stringOutput8[128];
const char* to_hstr(uint8_t value){
    uint8_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 1 * 2 -1;
    for(uint8_t i = 0; i < size; i++){
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexto_stringOutput8[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = (*ptr & 0x0F);
        hexto_stringOutput8[size - (i * 2)] = tmp + (tmp > 9 ? 55 : '0');
    }

    hexto_stringOutput8[size + 1] = 0;

    return hexto_stringOutput8;
}


char doubleto_stringOutput[128];
const char* to_str(double value, uint8_t decimalPlaces){
    if (decimalPlaces > 20) decimalPlaces = 20;

    char* intPtr = (char*)to_str((int64_t) value);

    char* doublePtr = doubleto_stringOutput;

    if (value < 0){
        value *= -1;
    }

    while (*intPtr != 0){
        *doublePtr = *intPtr;
        intPtr++;
        doublePtr++;
    }

    *doublePtr = '.';
    doublePtr++;

    double newValue = value - (int)value;

    for (uint8_t i = 0; i < decimalPlaces; i++){
        newValue *= 10;
        *doublePtr = (int)newValue + '0';
        newValue -= (int)newValue;
        doublePtr++;
    }

    *doublePtr = 0;

    return doubleto_stringOutput;
}

const char* to_str(double value){
    return to_str(value, 2);
}