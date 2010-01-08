#ifndef __BMP_H__
#define __BMP_H__

#include "image.h"

/*- Compiler dependent definitions ----------------------------------*/
typedef unsigned char BYTE;       /* byte                                  */
typedef unsigned short int USHORT;  /* inteiro sem sinal com dois bytes      */
typedef unsigned short int WORD;  /* inteiro sem sinal com dois bytes      */
typedef long int LONG;            /* inteiro com quatro bytes              */
typedef unsigned long int DWORD;  /* inteiro sem sinal com quatro bytes    */

#ifdef WIN32
#include <windows.h>
#else
typedef struct {                        /* bmfh */
  USHORT    bfType;                       /* "BM" = 19788                    */
  DWORD   bfSize;                       /* file size in bytes              */
  USHORT    bfReserved1;                  /* must be zero                    */
  USHORT    bfReserved2;                  /* must be zero                    */
  DWORD   bfOffBits;                    /* offset in bits to data          */
} BITMAPFILEHEADER;

typedef struct  {                       /* bmih */
  DWORD   biSize;                       /* structure size in bytes         */
  LONG    biWidth;                      /* image width in pixels           */
  LONG    biHeight;                     /* image height in pixels          */
  WORD    biPlanes;                     /* device planes, must be 1        */
  WORD    biBitCount;                   /* bitmap color depth              */
  DWORD   biCompression;                /* compression type                */
  DWORD   biSizeImage;                  /* image size in bytes, 0 in RGB   */
  LONG    biXPelsPerMeter;              /* not useful                      */
  LONG    biYPelsPerMeter;              /* not useful                      */
  DWORD   biClrUsed;                    /* # colors used                   */
  DWORD   biClrImportant;               /* # important colors              */
} BITMAPINFOHEADER;
#endif

typedef struct {
  BYTE *red;                            /* red channel                     */
  BYTE *green;                          /* green channel                   */
  BYTE *blue;                           /* blue channel                    */
  LONG width;                           /* bitmap width in pixels          */
  LONG height;                          /* bitmap height in pixels         */
  BYTE *p_red;                          /* color palette                   */
  BYTE *p_green;                        /* color palette                   */
  BYTE *p_blue;                         /* color palette                   */
  DWORD nc;                             /* colors in palette               */
} BMP;                                  /* bitmap context                  */

typedef struct {
  LONG w;                               /* bitmap width in pixels          */
  LONG h;                               /* bitmap height in pixels         */
  BYTE *red;                            /* red channel                     */
  BYTE *green;                          /* blue channel                    */
  BYTE *blue;                           /* green channel                   */
} BMP_24;                               /* true color bitmap context       */

int BMPWriteImage (Image *img, FILE *fp);
Image * BMPReadImage (FILE *fp);

#endif
