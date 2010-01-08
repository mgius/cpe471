/*===========================================================================
%M Segundo Trablalho, Ray-Tracing.
%a Diego Nehab e Vinicius
%d 6 Maio 96
%p Curso de Computacao Grafica, prof. Marcelo Gattass.
===========================================================================*/

/*- Inclusao das bibliotecas padrao: --------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmpio.h"

/*===========================================================================
%V Rotinas para gravacao de Bitmaps no formato 24 bits.
===========================================================================*/

/*---------------------------------------------------------------------------
%F Escreve uma word
---------------------------------------------------------------------------*/
static int putword(FILE *output, WORD word)
{
  int put;
  BYTE temp[2];

  temp[0] = word & 0xff;
  temp[1] = (word >> 8) & 0xff;

  put = (int)fwrite(&temp, 1, 2, output);
  if (put != 2) return 0;

  return 1;
}

/*---------------------------------------------------------------------------
%F Escreve uma double word
---------------------------------------------------------------------------*/
static int putdword(FILE *output, DWORD dword)
{
  int put;
  BYTE temp[4];

  temp[0] = (BYTE) (dword & 0xff);
  temp[1] = (BYTE) ((dword >> 8) & 0xff);
  temp[2] = (BYTE) ((dword >> 16) & 0xff);
  temp[3] = (BYTE) ((dword >> 24) & 0xff);

  put = (int)fwrite(&temp, 1, 4, output);

  if (put != 4) return 0;

  return 1;
}

/*---------------------------------------------------------------------------
%F Escreve um unsigned integer 
---------------------------------------------------------------------------*/
static int putushort(FILE *output, USHORT uint)
{
  int put;
  BYTE temp[2];

  temp[0] = uint & 0xff;
  temp[1] = (uint >> 8) & 0xff;

  put = (int)fwrite(&temp, 1, 2, output);
  if (put != 2) return 0;
 
  return 1;
}

/*---------------------------------------------------------------------------
%F Escreve um long integer
---------------------------------------------------------------------------*/
static int putlong(FILE *output, LONG longint)
{
  int put;
  BYTE temp[4];

  temp[0] = (BYTE)(longint & 0xff);
  temp[1] = (BYTE)((longint >> 8) & 0xff);
  temp[2] = (BYTE)((longint >> 16) & 0xff);
  temp[3] = (BYTE)((longint >> 24) & 0xff);

  put = (int)fwrite(&temp, 1, 4, output);

  if (put != 4) return 0;

  return 1;
}

/*--------------------------------------------------------------------------
%F Calcula o tamanho de um arquivo BMP_24.
---------------------------------------------------------------------------*/
static DWORD getBMP_24size(BMP_24 *bmp)
{
  DWORD temp;
  int linesize;

  /* linha deve terminar em uma double word boundary */
  linesize = bmp->w * 3;
  if (linesize & 3) {
    linesize |= 3;
    linesize ++;
  }

  temp = 14 +                    /* file header size */
         40 +                    /* info header size */
         bmp->h * linesize;      /* image data size */

  return temp;
}

/*--------------------------------------------------------------------------
%F Escreve o fileheader
---------------------------------------------------------------------------*/
static int putfileheader(FILE *output, BMP_24 *bmp)
{
  int code;

  /* bfType */  
  code = putushort(output, 19778);     /* type = "BM" */
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  /* bfSize */
  code = putdword(output, getBMP_24size(bmp));        
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }
  
  /* bfReserved1 */  
  code = putushort(output, 0);         /* reservado, deve ser 0 */
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  /* bfReserved2 */  
  code = putushort(output, 0);         /* reservado, deve ser 0 */
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  /* bfOffBits */
  code = putdword(output, 54);        
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  return 1;
}

/*---------------------------------------------------------------------------
%F Escreve o infoheader
---------------------------------------------------------------------------*/
static int putinfoheader(FILE *output, BMP_24 *bmp)
{
  int code;

  /* biSize */
  code = putdword(output, 40);      
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  /* biWidth */
  code = putlong(output,  bmp->w);  
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  /* biHeight */
  code = putlong(output, bmp->h);   
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  /* biPlanes, tem que ser 1 */
  code = putword(output, 1);            
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  /* biBitCount, 24 para 24 bits */
  code = putword(output, 24);     
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  /* biCompression, sem compressao */
  code = putdword(output, 0);           
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  /* biSizeImage, nao eh usado sem compressao */
  code = putdword(output, 0);
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  /* biXPelsPerMeter */
  code = putlong(output, 0);            
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  /* biYPelsPerMeter */
  code = putlong(output, 0);
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  /* biClrUsed, numero de cores na palheta */
  code = putdword(output, 0);
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  /* biClrImportant, 0 pq todas sao importantes */
  code = putdword(output, 0);           
  if (code == 0) { 
    fprintf(stderr, "putfileheader: Disk full.");
    return 0;
  }

  return 1;
}

/*---------------------------------------------------------------------------
%F Coloca o array de indices em output.
---------------------------------------------------------------------------*/
static int put24bits(FILE *output, BMP_24 *bmp)
{
  int i, j, k;
  BYTE *filedata, *red, *green, *blue;
  int linesize, put;

  /* a linha deve terminar em uma double word boundary */
  linesize = bmp->w * 3;
  if (linesize & 3) {
    linesize |= 3;
    linesize ++;
  }

  /* aloca espacco para a area de trabalho */
  filedata = (BYTE *) malloc(linesize);
  if (filedata == NULL) {
    fprintf(stderr, "put24bits: Not enough memory.");
    return 0;
  }

  /* a linha deve ser zero padded */
  for (i=0; i<(linesize-(3*bmp->w)); i++) filedata[linesize-1-i] = 0;

  /* inicializa o ponteiro para as componentes */
  red = bmp->red;
  green = bmp->green;
  blue = bmp->blue;

  /* para cada linha */
  for (i=0; i<bmp->h; i++) {

    /* coloca as componentes BGR no buffer */    
    k=0;
    for (j=0; j<bmp->w; j++) {
      filedata[k++] = blue[i*bmp->w + j];
      filedata[k++] = green[i*bmp->w + j];
      filedata[k++] = red[i*bmp->w + j];
    }
    
    /* joga para o arquivo */
    put = (int)fwrite(filedata, linesize, 1, output);
    if (put != 1) {
     fprintf(stderr, "put24bits: Disk full.");
      free(filedata);
      return 0;
    }
  }

  free(filedata);
  return 1;
}


/*---------------------------------------------------------------------------
%F Grava em filename o BMP_24 bmp.
---------------------------------------------------------------------------*/
int BMPWriteImage (Image *img, FILE *output)
{
  /* abre o arquivo para escrita */
  if (output == NULL) {
    fprintf(stderr, "writeBMP: Passed null file pointer.\n");
    return 0;
  }

    // Convert pixel values to RGB bytes
  BMP_24 bmp;
  bmp.w = img->width;
  bmp.h = img->height;
  bmp.red = new BYTE[bmp.w*bmp.h];
  bmp.green = new BYTE[bmp.w*bmp.h];
  bmp.blue = new BYTE[bmp.w*bmp.h];

  Pixel *pim = img->pixels;
  for (int j=0; j<bmp.h; j++)
    {
		for (int i=0; i<bmp.w; i++, pim++)
		{
			bmp.red[(bmp.h - 1 - j)*bmp.w+i] = pim->r;
			bmp.green[(bmp.h - 1 - j)*bmp.w+i] = pim->g;
			bmp.blue[(bmp.h - 1 - j)*bmp.w+i] = pim->b;
		}
	}

  /* salva BitmapFileheader */
  int code = putfileheader(output, &bmp);
  if (code == 0) {
    fprintf(stderr, "writeBMP: putfileheader returned a bad status.\n");
	delete [] bmp.red;
	delete [] bmp.green;
	delete [] bmp.blue;
    return 0;
  }

  /* salva BitmapInfoHeader */
  code = putinfoheader(output, &bmp);
  if(code == 0) {
    fprintf(stderr, "writeBMP: putinfoheader returned a bad status.\n");
	delete [] bmp.red;
	delete [] bmp.green;
	delete [] bmp.blue;
    return 0;
  }

  /* salva as componentes */
  code = put24bits(output, &bmp);
  if(code == 0) {
    fprintf(stderr, "writeBMP: put24bits returned a bad status.\n");
	delete [] bmp.red;
	delete [] bmp.green;
	delete [] bmp.blue;
    return 0;
  }

  delete [] bmp.red;
  delete [] bmp.green;
  delete [] bmp.blue;

  return 1;
}
