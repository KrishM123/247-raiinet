#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <X11/Xlib.h>
#include <iostream>
#include <string>

class Xwindow {
  Display *d;
  Window w;
  int s, width, height;
  GC gc;
  unsigned long colours[12];

public:
  Xwindow(int width = 500,
          int height = 500); // Constructor; displays the window.
  ~Xwindow();                // Destructor; destroys the window.

  enum {
    White = 0,
    Seashell1,
    Brown1,
    Seagreen2,
    Gray30,
    Gray50,
    Black,
    Darkseagreen1,
    Lightcoral,
    Darkorange,
    Gray75,
    Slategray1
  }; // Available colours.

  int getWidth() const;
  int getHeight() const;

  // Draws a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour = Black);

  // Draws a string
  void drawString(int x, int y, std::string msg);
};

#endif
