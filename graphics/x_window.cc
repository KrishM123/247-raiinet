#include "x_window.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;

Xwindow::Xwindow(int width, int height) : width{width}, height{height} {
  // Open the display
  d = XOpenDisplay(NULL);
  // If the display is not open
  if (d == NULL) {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  // Get the default screen
  s = DefaultScreen(d);
  // Create a simple window
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                          BlackPixel(d, s), WhitePixel(d, s));
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapRaised(d, w);

  // Create a pixmap
  Pixmap pix =
      XCreatePixmap(d, w, width, height, DefaultDepth(d, DefaultScreen(d)));
  // Create a graphics context
  gc = XCreateGC(d, pix, 0, (XGCValues *)0);

  XFlush(d);
  XFlush(d);

  // Set up colours.
  XColor xcolour;
  Colormap cmap;
  char color_vals[12][20] = {"white",      "seashell1",     "brown1",
                             "seagreen2",  "gray30",        "gray50",
                             "black",      "darkseagreen1", "lightcoral",
                             "darkorange", "gray75",        "slategray1"};

  cmap = DefaultColormap(d, DefaultScreen(d));
  // Loop through the colors
  for (int i = 0; i < 12; ++i) {
    // Parse the color
    XParseColor(d, cmap, color_vals[i], &xcolour);
    // Allocate the color
    XAllocColor(d, cmap, &xcolour);
    // Set the color
    colours[i] = xcolour.pixel;
  }

  // Set the foreground color
  XSetForeground(d, gc, colours[Black]);

  // Make window non-resizeable
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize);
  hints.height = hints.base_height = hints.min_height = hints.max_height =
      height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  // Synchronize the display
  // XSynchronize(d, True);
  // Sleep for 50ms
  // usleep(50000);
  // Create an event
  XEvent event;
  // Do while the event is not an expose
  do {
    // Get the next event
    XNextEvent(d, &event);
    // If the event is an expose
  } while (event.type != Expose);
}

// Destructor
Xwindow::~Xwindow() {
  // Free the graphics context
  XFreeGC(d, gc);
  // Close the display
  XCloseDisplay(d);
}

// Get the width
int Xwindow::getWidth() const { return width; }
// Get the height
int Xwindow::getHeight() const { return height; }

// Fill a rectangle
void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
  // Set the foreground color
  XSetForeground(d, gc, colours[colour]);
  // Fill the rectangle
  XFillRectangle(d, w, gc, x, y, width, height);
  // Set the foreground color to black
  XSetForeground(d, gc, colours[Black]);
  XFlush(d);
}

void Xwindow::drawString(int x, int y, string msg) {
  XDrawString(d, w, DefaultGC(d, s), x, y, msg.c_str(), msg.length());
  XFlush(d);
}
