/*---------------------------------------------------------------------------
%M Primeiro Trablalho, IMAGENS.
%a Diego Nehab e Vinicius
%d 17 Marco 96
%p Curso de Computacao Grafica, prof. Marcelo Gattass.
---------------------------------------------------------------------------*/

/*- Inclusao das bibliotecas padrao: --------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmpio.h"

/*===========================================================================
  Leitura de Bitmaps 
===========================================================================*/

/*---------------------------------------------------------------------------
%F Retorna o tamanho do arquivo apontado por input.
---------------------------------------------------------------------------*/
unsigned long int filesize(FILE *input)
{
  long int save_pos, size_of_file;

  save_pos = ftell(input);
  fseek(input, 0L, SEEK_END);
  size_of_file = ftell(input);
  fseek(input, save_pos, SEEK_SET);

  return size_of_file;
}

/*---------------------------------------------------------------------------
%F Le uma word de input.
---------------------------------------------------------------------------*/
int getword(FILE *input, WORD *word)
{
  int got;
  BYTE temp[2];
  WORD tempword;

  got = (int)fread(&temp, 1, 2, input);
  if (got != 2) return 0;

  tempword = ((WORD)(temp[1])<<8) | ((WORD)(temp[0]));

  *word = tempword;

  return 1;
}

/*---------------------------------------------------------------------------
%F Le um double word de input.
---------------------------------------------------------------------------*/
int getdword(FILE *input, DWORD *dword)
{
  int got;
  BYTE temp[4];
  DWORD tempdword;

  got = (int)fread(&temp, 1, 4, input);
  if (got != 4) return 0;

  tempdword = ((DWORD)(temp[3])<<24) | ((DWORD)(temp[2])<<16) 
    | ((DWORD)(temp[1])<<8) | ((DWORD)(temp[0]));

  *dword = tempdword;

  return 1;
}

/*---------------------------------------------------------------------------
%F Le um unsigned integer de input.
---------------------------------------------------------------------------*/
int getushort(FILE *input, USHORT *uint)
{
  int got;
  BYTE temp[2];
  USHORT tempuint;

  got = (int)fread(&temp, 1, 2, input);
  if (got != 2) return 0;

  tempuint = ((USHORT)(temp[1])<<8) | ((USHORT)(temp[0]));

  *uint = tempuint;

  return 1;
}

/*---------------------------------------------------------------------------
%F Le um long integer de input.
---------------------------------------------------------------------------*/
int getlong(FILE *input, LONG *longint)
{
  int got;
  BYTE temp[4];
  LONG templongint;

  got = (int)fread(&temp, 1, 4, input);
  if (got != 4) return 0;

  templongint = ((LONG)(temp[3])<<24) | ((LONG)(temp[2])<<16) 
    | ((LONG)(temp[1])<<8) | ((LONG)(temp[0]));

  *longint = templongint;

  return 1;
}

/*---------------------------------------------------------------------------
%F Le o fileheader de input.
---------------------------------------------------------------------------*/
int getfileheader(FILE *input, BITMAPFILEHEADER *fileheader)
{
  int code;

  code = getushort(input, &(fileheader->bfType));
  if (code == 0) {
    fprintf(stderr, "getfileheader: Unexpected end of file.\n");
    return 0;
  }
  /* bfType tem que ter o valor "BM" */
  if (fileheader->bfType != 19778) {
    fprintf(stderr, "getfileheader: File is not a valid bitmap.\n");
    return 0;
  }

  code = getdword(input, &(fileheader->bfSize));
  if (code == 0) {
    fprintf(stderr, "getfileheader: Unexpected end of file.\n");
    return 0;
  }
  /* checa o tamanho do arquivo */
  if (fileheader->bfSize != filesize(input)) {
    fprintf(stderr, "getfileheader: Incorrect file size.\n");
    return 0;
  }

  code = getushort(input, &(fileheader->bfReserved1));
  if (code == 0) {
    fprintf(stderr, "getfileheader: Unexpected end of file.\n");
    return 0;
  }
  if (fileheader->bfReserved1 != 0) {
    fprintf(stderr, "getfileheader: bfReserved1 should be 0.\n");
    return 0;
  }

  code = getushort(input, &(fileheader->bfReserved2));
  if (code == 0) {
    fprintf(stderr, "getfileheader: Unexpected end of file.\n");
    return 0;
  }
  if (fileheader->bfReserved2 != 0) {
    fprintf(stderr, "getfileheader: bfReserved2 should be 0.\n");
    return 0;
  }

  code = getdword(input, &(fileheader->bfOffBits));
  if (code == 0) {
    fprintf(stderr, "getfileheader: Unexpected end of file.\n");
    return 0;
  }

  return 1;
}

/*---------------------------------------------------------------------------
%F Le o infoheader de input.
---------------------------------------------------------------------------*/
int getinfoheader(FILE *input, BITMAPINFOHEADER *infoheader)
{
  int code;

  code = getdword(input, &(infoheader->biSize));
  if (code == 0) {
    fprintf(stderr, "getinfoheader: Unexpected end of file.\n");
    return 0;
  }

  code = getlong(input, &(infoheader->biWidth));
  if (code == 0) {
    fprintf(stderr, "getinfoheader: Unexpected end of file.\n");
    return 0;
  }

  code = getlong(input, &(infoheader->biHeight));
  if (code == 0) {
    fprintf(stderr, "getinfoheader: Unexpected end of file.\n");
    return 0;
  }

  code = getword(input, &(infoheader->biPlanes));
  if (code == 0) {
    fprintf(stderr, "getinfoheader: Unexpected end of file.\n");
    return 0;
  }
  /* biPlanes tem que ter valor 1 */
  if (infoheader->biPlanes != 1) {
    fprintf(stderr, "getinfoheader: biPlanes should be 1.\n");
    return 0;
  }

  code = getword(input, &(infoheader->biBitCount));
  if (code == 0) {
    fprintf(stderr, "getinfoheader: Unexpected end of file.\n");
    return 0;
  }
  /* biBitCount tem que ser 1, 4, 8 ou 24 */
  code = infoheader->biBitCount;
  if ((code != 1)&&(code != 4)&&(code != 8)&&(code != 24)) {
    fprintf(stderr, "getinfoheader: Invalid biBitCount value.\n");
    return 0;
  }

  code = getdword(input, &(infoheader->biCompression));
  if (code == 0) {
    fprintf(stderr, "getinfoheader: Unexpected end of file.\n");
    return 0;
  }
  /* biCompression tem que ser zero */
  if (infoheader->biCompression != 0) {
    fprintf(stderr, "getinfoheader: Sorry, RLE not implemented.\n");
    return 0;
  }

  code = getdword(input, &(infoheader->biSizeImage));
  if (code == 0) {
    fprintf(stderr, "getinfoheader: Unexpected end of file.\n");
    return 0;
  }

  code = getlong(input, &(infoheader->biXPelsPerMeter));
  if (code == 0) {
    fprintf(stderr, "getinfoheader: Unexpected end of file.\n");
    return 0;
  }

  code = getlong(input, &(infoheader->biYPelsPerMeter));
  if (code == 0) {
    fprintf(stderr, "getinfoheader: Unexpected end of file.\n");
    return 0;
  }

  code = getdword(input, &(infoheader->biClrUsed));
  if (code == 0) {
    fprintf(stderr, "getinfoheader: Unexpected end of file.\n");
    return 0;
  }

  code = getdword(input, &(infoheader->biClrImportant));
  if (code == 0) {
    fprintf(stderr, "getinfoheader: Unexpected end of file.\n");
    return 0;
  }

  return 1;
}

/*---------------------------------------------------------------------------
%F Pega a palheta de cores.
---------------------------------------------------------------------------*/
int getpalette(FILE *input, BMP *bmp)
{
  WORD i, j;
  BYTE *palettedata;
  unsigned long int got;

  /* aloca espacco para a area de trabalho */
  palettedata = (BYTE *) malloc(4*bmp->nc);

  /* aloca espacco para a palheta */
  if (bmp->p_red) free(bmp->p_red);
  if (bmp->p_green) free(bmp->p_green);
  if (bmp->p_blue) free(bmp->p_blue);
  bmp->p_red = (BYTE *) malloc(bmp->nc);
  bmp->p_green = (BYTE *) malloc(bmp->nc);
  bmp->p_blue = (BYTE *) malloc(bmp->nc);

  /* checa se faltou memoria */
  if ((palettedata == NULL) || (bmp->p_red == NULL)
    || (bmp->p_green == NULL) || (bmp->p_blue == NULL)) {
    if (palettedata) free(palettedata);
    if (bmp->p_red) free(bmp->p_red);
    bmp->p_red = NULL;
    if (bmp->p_green) free(bmp->p_green);
    bmp->p_green = NULL;
    if (bmp->p_blue) free(bmp->p_blue);
    bmp->p_blue = NULL;
    fprintf(stderr, "getpalette: Not enough memory.\n");
    return 0;
  }

  /* pega os dados do arquivo */
  got = (int)fread(palettedata, 4*bmp->nc, 1, input);
  if (got != 1) {
    fprintf(stderr, "getpalette: Unexpected end of file.\n");
    free(palettedata);
    return 0;
  }

  /* passa os valores para a palheta */
  for (j=0, i=0; i<bmp->nc; i++, j++) {
    bmp->p_blue[i] = palettedata[j++];
    bmp->p_green[i] = palettedata[j++];
    bmp->p_red[i] = palettedata[j++];
  }

  free(palettedata);
  return 1;
}

/*---------------------------------------------------------------------------
%F Le o formato 24bits.
---------------------------------------------------------------------------*/
int get24bits(FILE *input, BMP *bmp)
{
  LONG i, j, k, l, linesize;
  BYTE *linedata;
  unsigned long int got;

  /* a linha deve terminar em uma fronteira de dword */
  linesize = 3*bmp->width;
  if (linesize & 3) {
    linesize |= 3;
    linesize++;
  }
  
  /* aloca espaco para a area de trabalho */ 
  linedata = (BYTE *) malloc(linesize);
  if (linedata == NULL) {
    fprintf(stderr, "get24bits: Not enough memory.\n");
    return 0;
  }
    
  /* pega as componentes de cada pixel */
  for (k=0, i=0; i<bmp->height; i++) {
    got = (int)fread(linedata, linesize, 1, input);
    if (got != 1) {
      free(linedata);
      fprintf(stderr, "get24bits: Unexpected end of file.\n");
      return 0;
    }
    for (l=0, j=0; j<bmp->width; j++, k++) {
      bmp->blue[k] = linedata[l++];
      bmp->green[k] = linedata[l++]; 
      bmp->red[k] = linedata[l++]; 
    }
  }

  free(linedata);
  return 1;
}

/*---------------------------------------------------------------------------
%F Le o formato 8 bits.
---------------------------------------------------------------------------*/
int get8bits(FILE *input, BMP *bmp)
{
  LONG i, j, k, l, linesize;
  BYTE *linedata;
  unsigned long int got;

  /* a linha deve terminar em uma fronteira de dword */
  linesize = bmp->width;
  if (linesize & 3) {
    linesize |= 3;
    linesize++;
  }

  /* aloca espaco para a area de trabalho */
  linedata = (BYTE *) malloc(linesize);
  if (linedata == NULL) {
    fprintf(stderr, "get8bits: Not enough memory.\n");
    return 0;
  }

  /* pega as componentes de cada pixel */
  for (k=0, i=0; i<bmp->height; i++) {
    got = (int)fread(linedata, linesize, 1, input);
    if (got != 1) {
      free(linedata);
      fprintf(stderr, "get8bits: Unexpected end of file.\n");
      return 0;
    }
    for (j=0; j<bmp->width; j++, k++) {
      l = linedata[j];
      bmp->blue[k] = bmp->p_blue[l];
      bmp->green[k] = bmp->p_green[l];
      bmp->red[k] = bmp->p_red[l];
    }
  }

  free(linedata);
  return 1;
}

/*---------------------------------------------------------------------------
%F Le o formato 4 bits.
---------------------------------------------------------------------------*/
int get4bits(FILE *input, BMP *bmp)
{
  LONG i, j, k, l, masked, linesize;
  BYTE *linedata;
  unsigned long int got;

  /* a linha deve terminar em uma fronteira de dword */
  linesize = (bmp->width+1)>>1;
  if (linesize & 3) {
    linesize |= 3;
    linesize++;
  }

  /* aloca espaco para a area de trabalho */
  linedata = (BYTE *) malloc(linesize);
  if (linedata == NULL) {
    fprintf(stderr, "get4bits: Not enough memory.\n");
    return 0;
  }

  /* pega as componentes de cada pixel */
  for (k=0, i=0; i<bmp->height; i++) {
    got = (int)fread(linedata, linesize, 1, input);
    if (got != 1) {
      free(linedata);
      fprintf(stderr, "get4bits: Unexpected end of file.\n");
      return 0;
    }
    for (l=0, j=0; j<bmp->width; j++, k++) {
      masked = (j&1)?(linedata[l]&0x0f):(linedata[l]>>4);
      bmp->red[k] = bmp->p_red[masked];
      bmp->green[k] = bmp->p_green[masked];
      bmp->blue[k] = bmp->p_blue[masked];
      if (j&1) l++;
    }
  }

  free(linedata);
  return 1;
}

/*---------------------------------------------------------------------------
%F Le o formato 1 bit.
---------------------------------------------------------------------------*/
int getmono(FILE *input, BMP *bmp)
{
  LONG i, j, k, l, linesize;
  BYTE m, masked, *linedata;
  unsigned long int got;

  /* a linha deve terminar em uma fronteira de dword */
  linesize = (bmp->width+7)>>3;
  if (linesize & 3) {
    linesize |= 3;
    linesize++;
  }

  /* aloca espaco para a area de trabalho */
  linedata = (BYTE *) malloc(linesize);
  if (linedata == NULL) {
    fprintf(stderr, "getmono: Not enough memory.\n");
    return 0;
  }

  /* pega as componentes de cada pixel */
  for (k=0, i=0; i<bmp->height; i++) {
    got = (int)fread(linedata, linesize, 1, input);
    if (got != 1) {
      free(linedata);
      fprintf(stderr, "getmono: Unexpected end of file.\n");
      return 0;
    }
    for (l=0, m=8, j=0; j<bmp->width; j++, k++) {
      m--;
      masked = (linedata[l]>>(m))&0x01;
      bmp->red[k] = bmp->p_red[masked];
      bmp->green[k] = bmp->p_green[masked];
      bmp->blue[k] = bmp->p_blue[masked];
      if (m==0) {
        m=8;
        l++;
      }
    }
  }

  free(linedata);
  return 1;
}

/*---------------------------------------------------------------------------
%F Le um arquivo no formato BMP colocando as componentes de cada pixel em
  vetores r, g e b e as dimensoes em iw e ih.
---------------------------------------------------------------------------*/
Image * BMPReadImage (FILE *input)
{
  BMP bmp;
  memset(&bmp,0,sizeof(BMP));

  int code;
  unsigned long int size;
  BITMAPFILEHEADER fileheader;
  BITMAPINFOHEADER infoheader;

  /* abre o arquivo para leitura */
  if (input == NULL) {
    fprintf(stderr, "readBMP: Passed null file pointer.\n");
    return 0;
  }

  /* pega o fileheader */
  code = getfileheader(input, &fileheader);
  if (code == 0) {
    fprintf(stderr, "readBMP: getfileheader returned a bad status.\n");
    return 0;
  }

  /* pega o infoheader */
  code = getinfoheader(input, &infoheader);
  if (code == 0) {
    fprintf(stderr, "readBMP: getinfoheader returned a bad status.\n");
    return 0;
  }

  /* calcula o numero de pixels da imagem */
  bmp.width = infoheader.biWidth;
  bmp.height = infoheader.biHeight;
  size = bmp.width * bmp.height;

  /* obtem memoria para a imagem */
  if (bmp.red) free(bmp.red);
  if (bmp.green) free(bmp.green);
  if (bmp.blue) free(bmp.blue);
  bmp.red  = (BYTE *) malloc(size);
  bmp.green  = (BYTE *) malloc(size);
  bmp.blue  = (BYTE *) malloc(size);

  if ((bmp.red == NULL)||(bmp.green == NULL)||(bmp.blue == NULL)) {
    fprintf(stderr, "readBMP: Not enough memory.\n");
    if (bmp.red) free(bmp.red);
    bmp.red = NULL;
    if (bmp.green) free(bmp.green);
    bmp.green = NULL;
    if (bmp.blue) free(bmp.blue);
    bmp.blue = NULL;
    return 0;
  }

  /* pega a palheta de cores e processa a imagem */
  switch (infoheader.biBitCount) {
    case  1:
      bmp.nc = 2;
      code = getpalette(input, &bmp);
      if (code == 0) {
        fprintf(stderr, "readBMP: getpalette returned a bad status.\n");
        return 0;
      }
      code = getmono(input, &bmp);
      if (code == 0) {
        fprintf(stderr, "readBMP: getmono returned a bad status.\n");
        return 0;
      }
      break;
    case  4:
      if (infoheader.biClrUsed == 0) bmp.nc = 16;
      else bmp.nc = infoheader.biClrUsed;
      code = getpalette(input, &bmp);
      if (code == 0) {
        fprintf(stderr, "readBMP: getpalette returned a bad status.\n");
        return 0;
      }
      code = get4bits(input, &bmp);
      if (code == 0) {
        fprintf(stderr, "get4bits: Unable to get image data.\n");
        return 0;
      }
      break;
    case  8:
      if (infoheader.biClrUsed == 0) bmp.nc = 256;
      else bmp.nc = infoheader.biClrUsed;
      code = getpalette(input, &bmp);
      if (code == 0) {
        fprintf(stderr, "readBMP: getpalette returned a bad status.\n");
        return 0;
      }
      code = get8bits(input, &bmp);
      if (code == 0) {
        fprintf(stderr, "get8bits: Unable to get image data.\n");
        return 0;
      }
      break;
    case 24:
      code = get24bits(input, &bmp);
      if (code == 0) {
        fprintf(stderr, "get24bits: Unable to get image data.\n");
        return 0;
      }
      break;
    default:
      break;
  }

  Image *img = new Image(bmp.width,bmp.height);

  Pixel *pim = img->pixels;
  unsigned char *r = 0;
  unsigned char *g = 0;
  unsigned char *b = 0;
  for (int j = 0; j < bmp.height; j++)
  {
	  r = bmp.red + (bmp.height - 1 - j) * bmp.width;
      g = bmp.green + (bmp.height - 1 - j) * bmp.width;
      b = bmp.blue + (bmp.height - 1 - j) * bmp.width;
	  for (int i = 0; i < bmp.width; i++, pim++, r++, g++, b++)
	  {
		  pim->r = *r; pim->g = *g; pim->b = *b; pim->a = 255;
	  }
  }      

  if (bmp.red)
    free (bmp.red);
  if (bmp.green)
    free (bmp.green);
  if (bmp.blue)
    free (bmp.blue);
  if (bmp.p_red)
    free (bmp.p_red);
  if (bmp.p_green)
    free (bmp.p_green);
  if (bmp.p_blue)
    free (bmp.p_blue);

  return img;
}
