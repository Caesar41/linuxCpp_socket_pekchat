//
// Created by kumo on 5/8/21.
//

#include "simple_code.h"
#include <iostream>

using namespace std;

int keys[6] = {8, 26, 12, 13, 2, 24};

void encryption(string &c) {
    int len = c.size();
    for (int i = 0; i < len; i++) {
        c[i] = c[i] ^ keys[i % 6];
    }
}

void decode(string &c) {
    int len = c.size();
    for (int i = 0; i < len; i++) {
        c[i] = c[i] ^ keys[i % 6];
    }
}