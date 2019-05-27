#ifndef SYSOPY_IMAGE_H
#define SYSOPY_IMAGE_H

#define COLOR_BW (color_mode_t)1

#define COLOR_GRAY (color_mode_t)2

#define COLOR_RGB (color_mode_t)3

typedef int color_mode_t;

typedef struct img_t {
    int* array;
    int width;
    int height;
    int max_value;
    color_mode_t color_mode;
} img_t;

int img_allocate(img_t* image,
                 int width,
                 int height,
                 int max_value,
                 color_mode_t color_mode);

int img_deallocate(img_t* image);

int img_load(const char* path, img_t* out);

int img_save(const char* path, img_t* image);

int img_new(img_t* out, img_t* image);

int img_getpixelindex(img_t* image, int x, int y, int* index);

#endif  // SYSOPY_IMAGE_H
