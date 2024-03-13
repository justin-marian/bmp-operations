#pragma once

#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#define MAX_PIXEL_VALUE 255

/**
 * @brief Allocate memory for a new image.
 * 
 * @param N Number of rows.
 * @param M Number of columns.
 * @return Pointer to the allocated image.
 */
int ***allocate_image(int N, int M);

/**
 * @brief Free memory allocated for the image.
 * 
 * @param image Pointer to the image.
 * @param N Number of rows.
 */
void free_image(int ***image, int N, int M);

/**
 * @brief Free memory allocated for the filter.
 * 
 * @param filter Pointer to the filter.
 * @param filter_size Size of the filter.
 */
void free_filter(float **filter, int filter_size);

/**
 * @brief Flip the image horizontally.
 * 
 * @param image Pointer to the image.
 * @param N Number of rows.
 * @param M Number of columns.
 * @return Pointer to the flipped image.
 */
int ***flip_horizontal(int ***image, int N, int M);

/**
 * @brief Rotate the image left.
 * 
 * @param image Pointer to the image.
 * @param N Number of rows.
 * @param M Number of columns.
 * @return Pointer to the rotated image.
 */
int ***rotate_left(int ***image, int N, int M);

/**
 * @brief Crop the image.
 * 
 * @param image Pointer to the image.
 * @param N Number of rows.
 * @param M Number of columns.
 * @param x X-coordinate of the top-left corner of the crop region.
 * @param y Y-coordinate of the top-left corner of the crop region.
 * @param h Height of the crop region.
 * @param w Width of the crop region.
 * @return Pointer to the cropped image.
 */
int ***crop(int ***image, int N, int M,
            int x, int y, int h, int w);

/**
 * @brief Extend the image.
 * 
 * @param image Pointer to the image.
 * @param N Number of rows.
 * @param M Number of columns.
 * @param rows Number of rows to extend.
 * @param cols Number of columns to extend.
 * @param new_R Red component of the border color.
 * @param new_G Green component of the border color.
 * @param new_B Blue component of the border color.
 * @return Pointer to the extended image.
 */
int ***extend(int ***image, int N, int M,
              int rows, int cols,
              int new_R, int new_G, int new_B);

/**
 * @brief Paste an image onto another image.
 * 
 * @param image_dst Pointer to the destination image.
 * @param N_dst Number of rows in the destination image.
 * @param M_dst Number of columns in the destination image.
 * @param image_src Pointer to the source image.
 * @param N_src Number of rows in the source image.
 * @param M_src Number of columns in the source image.
 * @param x X-coordinate of the top-left corner of the paste region.
 * @param y Y-coordinate of the top-left corner of the paste region.
 * @return Pointer to the destination image after pasting.
 */
int ***paste(int ***image_dst, int N_dst, int M_dst,
             int ***image_src, int N_src, int M_src, int x, int y);

/**
 * @brief Apply a filter to the image.
 * 
 * @param image Pointer to the image.
 * @param N Number of rows.
 * @param M Number of columns.
 * @param filter 2D array representing the filter kernel.
 * @param filter_size Size of the filter kernel.
 * @return Pointer to the filtered image.
 */
int ***apply_filter(int ***image, int N, int M,
                    float **filter, int filter_size);

#endif  // IMAGEPROCESSING_H
