#pragma once

#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "imageprocessing.h"
#include "bmp.h"

#define CMD_LENGTH 10
#define MAX_IMAGES 100
#define MAX_FILTERS 100
#define PATH_LENGTH 100

typedef struct TImage {
    int ***data;    // image data RGB format
    int N, M;       // (height and width)
} Image;

typedef struct TFilter {
    float **data;   // filter data
    int size;       // filter matrix size
} Filter;

// Images / Filters in memory
typedef struct TImagesFilters {
    Image images[MAX_IMAGES];
    Filter filters[MAX_FILTERS];
    int image_count;
    int filter_count;
} ImagesFilters;

typedef void (*CommandFunc)(ImagesFilters*);

// Hashmap command-functions
typedef struct {
    char cmd[CMD_LENGTH];   // Command string
    CommandFunc func;       // Pointer function
} CommandMap;

/** @brief Load an image from a file and store it in memory. */
void Load_image(ImagesFilters *images_filters);

/** @brief Save an image to a file. */
void Save_image(ImagesFilters *images_filters);

/** @brief Delete an image from memory. */
void Delete_Image(ImagesFilters *images_filters);

/** @brief Apply horizontal flip to an image. */
void Apply_horizontal_flip(ImagesFilters *images_filters);

/** @brief Rotate left an image. */
void Apply_rotate(ImagesFilters *images_filters);

/** @brief Crop an image to a specified region. */
void Apply_crop(ImagesFilters *images_filters);

/** @brief Extend an image by adding a border around it. */
void Apply_extend(ImagesFilters *images_filters);

/** @brief Paste one image onto another image at a specified location. */
void Apply_paste(ImagesFilters *images_filters);

/** @brief Create a new filter matrix. */
void Create_filter(ImagesFilters *images_filters);

/** @brief Apply a filter to an image. */
void Apply_Filter(ImagesFilters *images_filters);

/** @brief Delete a filter from memory. */
void Delete_filter(ImagesFilters *images_filters);

// Array of command-function mappings
const CommandMap commands[] = {
    {"l", Load_image},
    {"s", Save_image},
    {"ah", Apply_horizontal_flip},
    {"ar", Apply_rotate},
    {"ac", Apply_crop},
    {"ae", Apply_extend},
    {"ap", Apply_paste},
    {"cf", Create_filter},
    {"af", Apply_Filter},
    {"df", Delete_filter},
    {"di", Delete_Image},
    {"", NULL}  // Sentinel value (end of the array)
};

/**
 * @brief Execute a command given its string representation.
 *
 * @param cmd The command to execute.
 */
void Execute_command(const char* cmd, ImagesFilters *images_filters);

#endif  // INTERACTIVE_H
