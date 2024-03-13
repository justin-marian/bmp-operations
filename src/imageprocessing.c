#include <stdio.h>
#include <stdlib.h>

#include "../include/imageprocessing.h"

// Helper : Find most appropriate range value
int clamp(int value, int min, int max) {
    if (value < min) return min;
    else if (value > max) return max;
    return value;
}

int ***allocate_image(int N, int M) {
    // Allocate memory for the image pointer
    int ***image = (int ***)malloc(N * sizeof(int **));
    if (image == NULL) {
        fprintf(stderr, "[ERROR] : Allocate BMP image...\n");
        return NULL;
    }

    // Allocate memory for each row of the image
    for (int i = 0; i < N; ++i) {
        // Allocate memory for the row pointer array
        image[i] = (int **)malloc(M * sizeof(int *));

        if (image[i] == NULL) {
            // Deallocate previously allocated rows if allocation fails
            fprintf(stderr, "[ERROR] : Allocate BMP image rows...\n");
            for (int j = 0; j < i; ++j) {
                free(image[j]);
            }
            free(image);
            // Allocation terminated
            return NULL;
        }

        // Allocate memory for each pixel in the row (RGB channels)
        for (int j = 0; j < M; ++j) {
            // Allocate memory for the pixel (RGB channels)
            image[i][j] = (int *)malloc(3 * sizeof(int));

            if (image[i][j] == NULL) {
                // Deallocate previously allocated pixels if allocation fails
                fprintf(stderr, "[ERROR] : Allocate BMP image pixels...\n");
                for (int k = 0; k < j; ++k) {
                    free(image[i][k]);
                }
                free(image[i]);
                // Deallocate previously allocated rows
                for (int k = 0; k < i; ++k) {
                    for (int l = 0; l < M; ++l) {
                        free(image[k][l]);
                    }
                    free(image[k]);
                }
                free(image);
                // Allocation terminated
                return NULL;
            }
        }
    }

    return image;
}

int ***flip_horizontal(int ***image, int N, int M) {
    int ***new_image = allocate_image(N, M);
    if (new_image == NULL) return NULL;

    // Flip horizontally
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            for (int k = 0; k < 3; ++k) {
                new_image[i][j][k] = image[i][M - j - 1][k];
            }
        }
    }

    return new_image;
}

int ***rotate_left(int ***image, int N, int M) {
    int ***new_image = allocate_image(M, N);
    if (new_image == NULL) return NULL;

    // Rotate left
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < 3; ++k) {
                new_image[i][j][k] = image[j][M - i - 1][k];
            }
        }
    }

    return new_image;
}

int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
    int ***new_image = allocate_image(h, w);
    if (new_image == NULL) return NULL;

    // Check bounds and copy pixels
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            for (int k = 0; k < 3; ++k) {
                if (y + i < N && x + j < M) {
                    new_image[i][j][k] = image[y + i][x + j][k];
                } else {
                    // Handle out-of-bounds access
                    new_image[i][j][k] = 0;
                }
            }
        }
    }

    return new_image;
}

int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
    int ***new_image = allocate_image(N + 2 * rows, M + 2 * cols);
    if (new_image == NULL) return NULL;

    // Copy existing image data to the extended image
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < 3; k++) {
                new_image[i + rows][j + cols][k] = image[i][j][k];
            }
        }
    }

    // Fill border with specified color
    for (int i = 0; i < N + 2 * rows; i++) {
        for (int j = 0; j < M + 2 * cols; j++) {
            if (i < rows || i >= N + rows || j < cols || j >= M + cols) {
                new_image[i][j][0] = new_R;
                new_image[i][j][1] = new_G;
                new_image[i][j][2] = new_B;
            }
        }
    }

    return new_image;
}

int ***paste(int ***image_dst, int N_dst, int M_dst, int ***image_src, int N_src, int M_src, int x, int y) {
    // Paste
    for (int i = 0; i < N_src && i + y < N_dst; ++i) {
        for (int j = 0; j < M_src && j + x < M_dst; ++j) {
            for (int k = 0; k < 3; ++k) {
                image_dst[i + y][j + x][k] = image_src[i][j][k];
            }
        }
    }

    return image_dst;
}

int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
    int ***new_image = allocate_image(N, M);
    if (new_image == NULL) return NULL;

    int center = filter_size / 2;

    // Apply filter
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            float R = 0, G = 0, B = 0;

            // Apply the filter to the neighbors of each pixel
            for (int k = -center; k <= center; ++k) {
                for (int l = -center; l <= center; ++l) {
                    // Calculate the coordinates of the neighbor
                    int x = i + k;
                    int y = j + l;

                    // Check if the neighbor is within the image boundaries
                    if (x >= 0 && x < N && y >= 0 && y < M) {
                        // Apply the filter to each color channel
                        int filter_i = k + center;
                        int filter_j = l + center;
                        R += (float)image[x][y][0] * filter[filter_i][filter_j];
                        G += (float)image[x][y][1] * filter[filter_i][filter_j];
                        B += (float)image[x][y][2] * filter[filter_i][filter_j];
                    }
                }
            }

            // Round and clamp the resulting values
            int new_R = clamp((int)R, 0, MAX_PIXEL_VALUE);
            int new_G = clamp((int)G, 0, MAX_PIXEL_VALUE);
            int new_B = clamp((int)B, 0, MAX_PIXEL_VALUE);

            new_image[i][j][0] = new_R;
            new_image[i][j][1] = new_G;
            new_image[i][j][2] = new_B;
        }
    }

    return new_image;
}

// Helper : Free memory of image data (RGB channels)
void free_image(int ***image, int N, int M) {
    if (image != NULL) {
        for (int i = 0; i < N; ++i) {
            if (image[i] != NULL) {
                for (int j = 0; j < M; ++j) {
                    free(image[i][j]);
                }
                free(image[i]);
            }
        }
        free(image);
    }
}

// Helper : Free memory of filter data (2D array)
void free_filter(float **filter, int filter_size) {
    if (filter != NULL) {
        for (int i = 0; i < filter_size; ++i) {
            free(filter[i]);
        }
        free(filter);
    }
}
