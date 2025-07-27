#include "controller/controller.h"

int main(int argc, char* argv[]) {
    Controller controller;
    
    controller.init(argc, argv);
    
    controller.play();
    
    return 0;
} 