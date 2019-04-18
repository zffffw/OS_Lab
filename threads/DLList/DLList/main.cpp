//
//  main.cpp
//  DLList
//
//  Created by 张 on 2019/3/6.
//  Copyright © 2019 张. All rights reserved.
//

#include "dlist.cpp"
int main(int argc, char* argv[]) {
    DLList D;
    char s[]="abcdefg";
    // printf("11\n");
    char *p = s;
    for(int i = 0; i < 6; ++i) {
        putchar(*p);
        D.Append(p);
        p++;
    }
    // D.printList();
}
