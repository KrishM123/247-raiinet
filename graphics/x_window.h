#ifndef X_WINDOW_H
#define X_WINDOW_H

#include <string>

class XWindow final {
public:
  XWindow();
  virtual ~XWindow();

  int getWidth() const;
  int getHeight() const;

  void fillRectangle(int x, int y, int width, int height);
  void drawString(int x, int y, const std::string &msg);
};

#endif // X_WINDOW_H