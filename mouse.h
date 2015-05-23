#ifndef MOUSE_H
#define MOUSE_H

void mouseClick(cv::Mat *hsv, int x, int y);
void mouseEvent(int evt, int x, int y, int flags, void* param);

#endif // MOUSE_H
