/*Written by YU Jincheng @ZJU
**simple support in jpeg for watcom and dosbox
2016/6/17/
*/

#ifndef SIMPLEJPEG_H
#define SIMPLEJPEG_H
#include <graphics.h>
const unsigned char *readJPEGtoBMP(const char *filePath);
//int writeJPEGfromBMP(const unsigned char *source, const char *filePath);
void putJPEGxy(int x,int y, const char *filePath);



#endif
