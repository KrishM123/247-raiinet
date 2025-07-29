#include "controller/controller.h"

int main(int argc, char *argv[])
{
  Controller controller(2, 8);

  controller.init(argc, argv);

  controller.play();

  return 0;
}