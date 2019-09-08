#include<iostream>
#include "../include/mainwindow.h"

using std::cout; using std::cin;
using std::endl; using std::cerr;

int main(int argc, char **argv){
    SMainWindow window=basicWindowInit("PLAYER PLAYLIST");

    if(window==nullptr){
        cerr<<"Sorry, terminal doesn't support colors\n";
        return -1;
    }

    window->start();
    return 0;
}
