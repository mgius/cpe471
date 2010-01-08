#include "image.h"
#include "bmpio.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>


/**
 * Image
 **/
Image::Image (int width_, int height_)
{
    assert(width_ > 0);
    assert(height_ > 0);

    width           = width_;
    height          = height_;
    num_pixels      = width * height;
    pixels          = new Pixel[num_pixels];
    sampling_method = IMAGE_SAMPLING_POINT;

    assert(pixels != NULL);
}


Image::Image (const Image& src)
{
    width           = src.width;
    height          = src.height;
    num_pixels      = width * height;
    pixels          = new Pixel[num_pixels];
    sampling_method = IMAGE_SAMPLING_POINT;

    assert(pixels != NULL);
    memcpy(pixels, src.pixels, src.width * src.height * sizeof(Pixel));
}


Image::~Image ()
{
    delete [] pixels;
    pixels = NULL;
}


/* Empty functions for old Princeton class - CSC 471 students IGNORE these */
void Image::AddNoise (double factor)
{
}


void Image::Brighten (double factor)
{   
}


void Image::ChangeContrast (double factor)
{ 
}


void Image::ChangeSaturation(double factor)
{   
}


Image* Image::Crop(int x, int y, int w, int h)
{
    return NULL;
}


void Image::ExtractChannel(int channel)
{
}


void Image::Quantize (int nbits)
{
}


void Image::RandomDither (int nbits)
{
}


void ImageComposite(Image *bottom, Image *top, Image *result)
{
}


void Image::Blur(int n)
{
}


void Image::EdgeDetect()
{
}


/* Matrix for Bayer's 4x4 pattern dither. */
/* uncomment its definition if you need it */

/*
static int Bayer4[4][4] =
{
    {15, 7, 13, 5},
    {3, 11, 1, 9},
    {12, 4, 14, 6},
    {0, 8, 2, 10}
};
*/

void Image::OrderedDither(int nbits)
{
}


/* Error-diffusion parameters */
const double
    ALPHA = 7.0 / 16.0,
    BETA  = 3.0 / 16.0,
    GAMMA = 5.0 / 16.0,
    DELTA = 1.0 / 16.0;

void Image::FloydSteinbergDither(int nbits)
{
}


Image* Image::Scale(double sx, double sy)
{
    return NULL;
}


Image* Image::Rotate(double angle)
{
    return NULL;
}


void Image::Fun()
{
}


Image* ImageMorph (Image* I0, Image* I1, int numLines, Line* L0, Line* L1, double t)
{
    return NULL;
}


/**
 * Image Sample
 **/
void Image::SetSamplingMethod(int method)
{
    assert((method >= 0) && (method < IMAGE_N_SAMPLING_METHODS));
    sampling_method = method;
}

Pixel Image::Sample (double u, double v)
{
    return Pixel();
}

