#include "image.h"
#include "errors.h"
#include "utils.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int img_get_array_size(img_t *image) {
    if (image->color_mode == COLOR_BW || image->color_mode == COLOR_GRAY)
        return image->width * image->height;

    else if (image->color_mode == COLOR_RGB)
        return image->width * image->height * 3;

    return -1;
}


int img_allocate(img_t *image, int width, int height, int max_value, color_mode_t color_mode) {
    image->color_mode = color_mode;
    image->width = width;
    image->height = height;
    image->max_value = max_value;
    image->array = calloc(img_get_array_size(image), sizeof(int));
    return 0;
}


int img_deallocate(img_t *image) {
    image->color_mode = 0;
    image->width = 0;
    image->height = 0;
    image->max_value = 0;
    free(image->array);

    return 0;
}


int img_load(const char *path, img_t *out) {
    FILE *fd;

    if ((fd = fopen(path, "r")) == NULL) {
        return err("Cannot open image", -1);
    }

    char buffer[2048];
    int main_line = 0;
    int index = 0;

    while (fgets(buffer, 2048, fd) != NULL) {
        if (buffer[0] == '#')
            continue;

        if (main_line == 0) {
            if (strncmp("P1", buffer, 2) == 0)
                out->color_mode = COLOR_BW;

            else if (strncmp("P2", buffer, 2) == 0)
                out->color_mode = COLOR_GRAY;

            else if (strncmp("P3", buffer, 2) == 0)
                out->color_mode = COLOR_RGB;

            else
                return err("Cannot open file 1 (unknown image format)", -1);

        } else if (main_line == 1) {

            char *ptr = strtok(buffer, " \r\n");

            int i = 0;
            int width = 0;
            int height = 0;

            while (ptr != NULL) {
                if (i == 0) {
                    if (parse_int(ptr, &width) < 0)
                        return -1;
                } else if (i == 1) {
                    if (parse_int(ptr, &height) < 0)
                        return -1;
                }

                i++;

                ptr = strtok(NULL, " \r\n");
            }

            if (i != 2)
                return err("Cannot open file 2 (unknown image format)", -1);

            out->width = width;
            out->height = height;

        } else if (main_line == 2) {
            int max_value = 0;

            if (parse_int(buffer, &max_value) < 0)
                return err("Cannot open file 3 (unknown image format)", -1);

            out->max_value = max_value;
            out->array = malloc(img_get_array_size(out) * sizeof(int));

        } else {
            char *ptr = strtok(buffer, " \r\n");

            while (ptr != NULL) {
                int tmp;

                if (parse_int(ptr, &tmp) < 0) {
                    return err("Cannot open file 4 (unknown image format)", -1);
                }

                out->array[index++] = tmp;

                ptr = strtok(NULL, " \r\n");
            }
        }

        main_line++;
    }

    if (index != img_get_array_size(out))
        return err("Cannot open file 5 (unknown image format)", -1);

    fclose(fd);

    return 0;
}


int img_save(const char *path, img_t *image) {
    FILE *fd;

    if ((fd = fopen(path, "w")) != NULL) {
        switch (image->color_mode) {
            case COLOR_BW:
                fprintf(fd, "%s", "P1\n");
                break;

            case COLOR_GRAY:
                fprintf(fd, "%s", "P2\n");
                break;

            case COLOR_RGB:
                fprintf(fd, "%s", "P3\n");
                break;

            default:
                return err("Cannot save file", -1);
        }

        fprintf(fd, "%d %d\n", image->width, image->height);

        fprintf(fd, "%d", image->max_value);

        for (int i = 0; i < img_get_array_size(image); i++) {
            if (i % 15 == 0)
                fprintf(fd, "\n");

            fprintf(fd, "%d ", image->array[i]);
        }
    }

    fclose(fd);

    return 0;
}


int img_new(img_t *out, img_t *image) {
    return img_allocate(out, image->width, image->height, image->max_value, image->color_mode);
}


int img_getpixelindex(img_t *image, int x, int y, int *index) {
    *index = x + (y * image->width);

    if (image->color_mode == COLOR_RGB)
        *index *= 3;

    if (*index >= img_get_array_size(image))
        return -1;

    return 0;
}