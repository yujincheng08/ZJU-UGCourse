#include "easyjpeg.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#define put2bits(array,offset,value)  \
    (array[offset] = (char) ((value) & 0xFF), \
    array[offset+1] = (char) (((value) >> 8) & 0xFF))
#define put4bits(array,offset,value)  \
    (array[offset] = (char) ((value) & 0xFF), \
    array[offset+1] = (char) (((value) >> 8) & 0xFF), \
    array[offset+2] = (char) (((value) >> 16) & 0xFF), \
    array[offset+3] = (char) (((value) >> 24) & 0xFF))
      
typedef j_decompress_ptr jptr;
typedef struct
{
    int step;
    long headerSize;
    long size;
    int bits;
    int entries;
    int width;
    int height;
    int rowWidth;
    int densityUnit;
    INT32 Xdensity;
}BMPinfo;

static BMPinfo readInfo(jptr jinfo)
{
    BMPinfo binfo;
    binfo.height = jinfo->output_height;
    binfo.width = jinfo->output_width;
    
    binfo.rowWidth = binfo.width * jinfo->output_components;
    if(jinfo->out_color_space == JCS_RGB)
    {
        if(jinfo->quantize_colors)
            binfo.bits = 8, binfo.entries = 256;
        else
            binfo.bits = 24, binfo.entries = 0;
    }
    else
        binfo.bits = 8, binfo.entries = 256;
    
    binfo.headerSize = 14 + 40 + binfo.entries * 4;
    
    binfo.step = binfo.rowWidth;
    
    while(binfo.step & 3) binfo.step++;
    
    binfo.size = binfo.headerSize + (long)binfo.step * (long)binfo.height;
    
    binfo.densityUnit = jinfo->density_unit;
    binfo.Xdensity = jinfo->X_density * 100;
    
    return binfo;
}

static const char *generateHeader(BMPinfo *info)
{
    char fileHeader[14];
    char infoHeader[40];
    char *header;
    
    memset(fileHeader, 0, sizeof(fileHeader));
    memset(infoHeader, 0, sizeof(infoHeader));
    
    fileHeader[0] = 0x42;
    fileHeader[1] = 0x4D;
    
    put4bits(fileHeader, 2, info->size);
    put4bits(fileHeader, 10, info->headerSize);
    
    put2bits(infoHeader, 0, 40);
    put4bits(infoHeader, 4, info->width);
    put4bits(infoHeader, 8, info->height);
    put2bits(infoHeader, 12, 1);
    put2bits(infoHeader, 14, info->bits);
    
    if(info->densityUnit == 2)
    {
        put4bits(infoHeader, 24, info->Xdensity);
        put4bits(infoHeader, 28, info->Xdensity);
    }
    
    put2bits(infoHeader, 32, info->entries);
    
    header = (char*)malloc((14+40)*sizeof(char));
    
    memcpy(header,fileHeader,14);
    memcpy(header+14,infoHeader,40);
    return header;
}

static void line2line(char *dest, const JSAMPARRAY *src, const size_t rowWidth)
{
    int i = 0;
    for(i=0;i<rowWidth;i+=3)
    {
        dest[i+2] = (**src)[i];
        dest[i+1] = (**src)[i+1];
        dest[i] = (**src)[i+2];
    }
}

extern const unsigned char *readJPEGtoBMP(const char *filePath)
{
    const char *header;
    struct jpeg_decompress_struct jinfo;
    struct jpeg_error_mgr jerr;
    BMPinfo binfo;
    FILE *in;
    JSAMPARRAY jbuff;
    char *data,*ptr;
    
    jinfo.err = jpeg_std_error(&jerr);
    if(!(in = fopen(filePath,"rb")))
        return NULL;
    jpeg_create_decompress(&jinfo);
    
    jpeg_stdio_src(&jinfo,in);
    
    (void) jpeg_read_header(&jinfo, TRUE);
    
    (void) jpeg_start_decompress(&jinfo);
    
    binfo = readInfo(&jinfo);
    
    jbuff = (*jinfo.mem->alloc_sarray)
        ((j_common_ptr) &jinfo, JPOOL_IMAGE, binfo.rowWidth, 1);
    
    
    header = generateHeader(&binfo);
    if(!header) return NULL;
    
    data = (char*)malloc(binfo.size);
    if(!data) return NULL;
    memset(data,0,binfo.size);
    memcpy(data,header,binfo.headerSize);
    
    ptr = data + binfo.size - binfo.step;
    while (jinfo.output_scanline < binfo.height)
    {
        (void) jpeg_read_scanlines(&jinfo, jbuff, 1);
        line2line(ptr,&jbuff,binfo.rowWidth);
        ptr -= binfo.step;
    }
    
    (void) jpeg_finish_decompress(&jinfo);
    jpeg_destroy_decompress(&jinfo);
    fclose(in);
    return data;
}


/* extern int writeJPEGfromBMP(const unsigned char *source, const char *filePath)
{
    return 0;
} */

static void putLineToScreen(int x,int y, const JSAMPARRAY *src,const size_t width)
{
    int i;
    byte *vp = _vp + _active_page*_page_gap + (y*_width + x) * (_color_bits/8);
    for(i=0;i<width&&i+x<_width;i++)
    {
        vp[3*i] = (**src)[3*i+2];
        vp[3*i+1] = (**src)[3*i+1];
        vp[3*i+2] = (**src)[3*i];
    }
}

extern void putJPEGxy(int x,int y, const char *filePath)
{
    struct jpeg_decompress_struct jinfo;
    struct jpeg_error_mgr jerr;
    JSAMPARRAY jbuff;
    FILE *in;
    jinfo.err = jpeg_std_error(&jerr);
    if(!(in = fopen(filePath,"rb")))
        return;
    jpeg_create_decompress(&jinfo);
    
    jpeg_stdio_src(&jinfo,in);
    
    (void) jpeg_read_header(&jinfo, TRUE);
    
    (void) jpeg_start_decompress(&jinfo);
    
    jbuff = (*jinfo.mem->alloc_sarray)
        ((j_common_ptr) &jinfo, JPOOL_IMAGE, jinfo.output_width * jinfo.output_components, 1);
    
    while (jinfo.output_scanline < jinfo.output_height)
    {
        (void) jpeg_read_scanlines(&jinfo, jbuff, 1);
        putLineToScreen(x,y++,&jbuff,jinfo.output_width);
    }
}

// int main(int argc, char *argv[])
// {
    // int driver=0, mode=VESA_1024x768x24bit;
    // initgraph(&driver, &mode, "");
    // if(argc<4) return -1;
    // putJPEGxy(atoi(argv[1]),atoi(argv[2]),argv[3]);
    // getchar();
    // closegraph();
    // /* const unsigned char * data = readJPEGtoBMP("t.jpg");
    // FILE * bmp = fopen("out.bmp", "wb");
    // fwrite(data,sizeof(char),size,bmp); */
    // return 0;
// }
