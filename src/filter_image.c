#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    float sum = 0;

    for (int i = 0; i < im.c*im.w*im.h; i++){
        sum += im.data[i];
    }
    for (int i = 0; i < im.c*im.w*im.h; i++){
        im.data[i] /= sum;
    }
}

image make_box_filter(int w)
{
    // TODO
    image new_image = make_image(w, w, 1);

    for (int i = 0; i < new_image.h; ++i)
    {
        for(int j = 0; j < new_image.w; ++j)
        {
            set_pixel(new_image, j, i, 0, 1);
        }
    }

    l1_normalize(new_image);

    return new_image;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    assert(im.c == filter.c || filter.c == 1);

    image output_image;

    if (preserve)
    {
        output_image = make_image(im.w, im.h, im.c);
    }
    else
    {
        output_image = make_image(im.w, im.h, 1);
    }

    for (int j = 0; j < im.h; j++)
    {
        for(int i = 0; i < im.w; i++)
        {
            int shift = filter.w / 2;

            for (int channel = 0; channel < im.c; ++channel)
            {
                float sum = 0;
                for (int f_row = 0; f_row < filter.w; ++f_row)
                {
                    for (int f_col = 0; f_col < filter.w; ++f_col)
                    {
                        sum += get_pixel(filter, f_col, f_row, 0) *
                               get_pixel(im, i + f_col - shift, j + f_row - shift, channel);
                    }
                }

                if (preserve)
                {
                    set_pixel(output_image, i, j, channel, sum);
                }
                else
                {
                    float current_value = get_pixel(output_image, i, j, 0);
                    set_pixel(output_image, i, j, 0, current_value + sum);
                }
            }
        }
    }
    return output_image;
}

image make_highpass_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);

    filter.data[0] = 0;
    filter.data[1] = -1;
    filter.data[2] = 0;

    filter.data[3] = -1;
    filter.data[4] = 4;
    filter.data[5] = -1;

    filter.data[6] = 0;
    filter.data[7] = -1;
    filter.data[8] = 0;

    return filter;
}

image make_sharpen_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);

    filter.data[0] = 0;
    filter.data[1] = -1;
    filter.data[2] = 0;

    filter.data[3] = -1;
    filter.data[4] = 5;
    filter.data[5] = -1;

    filter.data[6] = 0;
    filter.data[7] = -1;
    filter.data[8] = 0;

    return filter;
}

image make_emboss_filter()
{
    // TODO
    image filter = make_image(3, 3, 1);

    filter.data[0] = -2;
    filter.data[1] = -1;
    filter.data[2] = 0;

    filter.data[3] = -1;
    filter.data[4] = 1;
    filter.data[5] = 1;

    filter.data[6] = 0;
    filter.data[7] = 1;
    filter.data[8] = 2;

    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    int size = sigma * 6;
    if(size % 2 == 0) size += 1;

    image filter = make_image(size, size, 1);

    float a, b, value, ox, oy;

    for(int y= 0; y < filter.h; y++){
        for(int x = 0; x < filter.w; x++){
            ox = x - (size/2);
            oy = y - (size/2);

            a = 1 / (TWOPI * pow(sigma, 2));
            b = -1 * ( (pow(ox,2) + pow(oy, 2)) / (2 * pow(sigma, 2)) );
            value = a * exp(b);

            set_pixel(filter, x, y, 0, value);
        }
    }

    l1_normalize(filter);

    return filter;
}

image add_image(image a, image b)
{
    assert(a.w == b.w);
    assert(a.h == b.h);
    assert(a.c == b.c);
    image output_image = make_image(a.w, a.h, a.c);

    int max_index = a.w * a.h * a.c;

    for (int i = 0; i < max_index; ++i)
    {
        output_image.data[i] = a.data[i] + b.data[i];
    }

    return output_image;
}

image sub_image(image a, image b)
{
    assert(a.w == b.w);
    assert(a.h == b.h);
    assert(a.c == b.c);
    image output_image = make_image(a.w, a.h, a.c);

    int max_index = a.w * a.h * a.c;

    for (int i = 0; i < max_index; ++i)
    {
        output_image.data[i] = a.data[i] - b.data[i];
    }

    return output_image;
}

image make_gx_filter()
{
    image filter = make_image(3, 3, 1);

    filter.data[0] = -1;
    filter.data[1] = 0;
    filter.data[2] = 1;

    filter.data[3] = -2;
    filter.data[4] = 0;
    filter.data[5] = 2;

    filter.data[6] = -1;
    filter.data[7] = 0;
    filter.data[8] = 1;

    return filter;
}

image make_gy_filter()
{
    image filter = make_image(3, 3, 1);

    filter.data[0] = -1;
    filter.data[1] = -2;
    filter.data[2] = -1;

    filter.data[3] = 0;
    filter.data[4] = 0;
    filter.data[5] = 0;

    filter.data[6] = 1;
    filter.data[7] = 2;
    filter.data[8] = 1;

    return filter;
}

void feature_normalize(image im)
{
    float min = im.data[0];
    float max = im.data[0];

    for(int i = 0; i < im.w * im.h * im.c; i++){
        if(im.data[i] < min){
            min = im.data[i];
        } else if(im.data[i] > max){
            max = im.data[i];
        }
    }

    for(int i = 0; i < im.w * im.h * im.c; i++){
        im.data[i] = (im.data[i] - min) / (max - min);
    }
}

image *sobel_image(image im)
{
    image* images = calloc(2, sizeof(image));

    images[0] = make_image(im.w, im.h, 1);
    images[1] = make_image(im.w, im.h, 1);

    image dx = make_gx_filter();
    image dy = make_gy_filter();

    image gx = convolve_image(im, dx, 0);
    image gy = convolve_image(im, dy, 0);

    free_image(dx);
    free_image(dy);

    for (int i = 0; i < im.w * im.h; i++)
    {
        images[0].data[i] = sqrtf(pow(gx.data[i], 2) + pow(gy.data[i], 2));
        images[1].data[i] = atan2(gy.data[i], gx.data[i]);
    }

    free_image(gx);
    free_image(gy);

    return images;
}

image colorize_sobel(image im)
{
    image *sobel = sobel_image(im);
    clamp_image(sobel[0]);

    image preserve = make_image(im.w, im.h, im.c);
    image invert = make_image(im.w, im.h, im.c);

    feature_normalize(sobel[0]);
    for (int xy = 0; xy < im.w*im.h; xy++) {
        for (int i = 0; i < im.c; i++) {
            invert.data[xy + i * im.w * im.h] = 1 - im.data[i*im.w*im.h + xy];
            preserve.data[xy + i * im.w * im.h] = sobel[0].data[xy];
        }
    }
    clamp_image(preserve);
    image sub = sub_image(preserve, invert);
    clamp_image(sub);
    return sub;
}