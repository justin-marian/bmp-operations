#include "../include/interactive.h"

int main(void) {
    char cmd[CMD_LENGTH];
    ImagesFilters images_filters = {0};

    while (true) {
        scanf("%s", cmd);
        if (!strcmp(cmd, "e")) break;
        Execute_command(cmd, &images_filters);
    }

    // Free all images and filters before exiting
    for (int i = 0; i < images_filters.image_count; ++i) {
        free_image(images_filters.images[i].data,
            images_filters.images[i].N, images_filters.images[i].M);
    }
    for (int i = 0; i < images_filters.filter_count; ++i) {
        free_filter(images_filters.filters[i].data, images_filters.filters[i].size);
    }

    return EXIT_SUCCESS;
}

void Execute_command(const char* cmd, ImagesFilters *image_filters) {
    for (int i = 0; commands[i].func != NULL; ++i) {
        if (!strcmp(commands[i].cmd, cmd)) {
            commands[i].func(image_filters);
            return;
        }
    }
    fprintf(stdout, "Invalid cmd.\n");
}

void Load_image(ImagesFilters *images_filters) {
    int N = 0, M = 0;
    char path[PATH_LENGTH];
    scanf("%d %d %s", &N, &M, path);

    // Allocate memory for the image
    int ***image_data = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        image_data[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            image_data[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }

    // Load image data from BMP file
    read_from_bmp(image_data, N, M, path);
    // Assign new data to the image
    images_filters->images[images_filters->image_count].data = image_data;
    images_filters->images[images_filters->image_count].N = N;
    images_filters->images[images_filters->image_count].M = M;
    images_filters->image_count++;
}

void Save_image(ImagesFilters *images_filters) {
    int index = 0;
    char path[PATH_LENGTH];
    scanf("%d %s", &index, path);

    // Save image data to BMP file
    write_to_bmp(images_filters->images[index].data,
        images_filters->images[index].N, images_filters->images[index].M, path);
}

void Apply_horizontal_flip(ImagesFilters *images_filters) {
    int index = 0;
    scanf("%d", &index);

    int ***new_data = flip_horizontal(images_filters->images[index].data,
                        images_filters->images[index].N, images_filters->images[index].M);

    // Free the memory of the original image data
    free_image(images_filters->images[index].data,
        images_filters->images[index].N, images_filters->images[index].M);

    // Update data
    images_filters->images[index].data = new_data;
}

void Apply_rotate(ImagesFilters *images_filters) {
    int index = 0;
    scanf("%d", &index);

    int ***new_data = rotate_left(images_filters->images[index].data,
                        images_filters->images[index].N, images_filters->images[index].M);

    // Free the memory of the original image data
    free_image(images_filters->images[index].data,
        images_filters->images[index].N, images_filters->images[index].M);

    // Update data and dimensions
    images_filters->images[index].data = new_data;
    int temp = images_filters->images[index].N;
    images_filters->images[index].N = images_filters->images[index].M;
    images_filters->images[index].M = temp;
}

void Apply_crop(ImagesFilters *images_filters) {
    int index = 0, x = 0, y = 0, h = 0, w = 0;
    scanf("%d %d %d %d %d", &index, &x, &y, &w, &h);

    int ***new_data = crop(images_filters->images[index].data,
                images_filters->images[index].N, images_filters->images[index].M, x, y, h, w);

    // Free the memory of the original image data
    free_image(images_filters->images[index].data,
            images_filters->images[index].N, images_filters->images[index].M);

    // Update data and dimensions
    images_filters->images[index].data = new_data;
    images_filters->images[index].N = h;
    images_filters->images[index].M = w;
}

void Apply_extend(ImagesFilters *images_filters) {
    int index = 0, rows = 0, cols = 0, new_R = 0, new_G = 0, new_B = 0;
    scanf("%d %d %d %d %d %d", &index, &rows, &cols, &new_R, &new_G, &new_B);

    // Get the dimensions of the existing image
    int old_N = images_filters->images[index].N;
    int old_M = images_filters->images[index].M;

    // Calculate the dimensions of the extended image
    int new_N = old_N + 2 * rows;
    int new_M = old_M + 2 * cols;

    int ***extended_data = extend(images_filters->images[index].data,
                        old_N, old_M, rows, cols, new_R, new_G, new_B);

    // Free the memory of the original image data
    free_image(images_filters->images[index].data, old_N, old_M);

    // Update image properties
    images_filters->images[index].data = extended_data;
    images_filters->images[index].N = new_N;
    images_filters->images[index].M = new_M;
}

void Apply_paste(ImagesFilters *images_filters) {
    int index_dst = 0, index_src = 0, x = 0, y = 0;
    scanf("%d %d %d %d", &index_dst, &index_src, &x, &y);

    paste(images_filters->images[index_dst].data,
        images_filters->images[index_dst].N, images_filters->images[index_dst].M,
        images_filters->images[index_src].data,
        images_filters-> images[index_src].N, images_filters->images[index_src].M, x, y);
}

void Create_filter(ImagesFilters *images_filters) {
    int size = 0;
    scanf("%d", &size);

    // Allocate memory for filter data
    images_filters->filters[images_filters->filter_count].data = (float **)malloc(size * sizeof(float *));
    if (images_filters->filters[images_filters->filter_count].data == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i < size; ++i) {
        images_filters->filters[images_filters->filter_count].data[i] = (float *)malloc(size * sizeof(float));
        if (images_filters->filters[images_filters->filter_count].data[i] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            // Free previously allocated memory
            for (int j = 0; j < i; ++j) {
                free(images_filters->filters[images_filters->filter_count].data[j]);
            }
            free(images_filters->filters[images_filters->filter_count].data);
            return;
        }

        for (int j = 0; j < size; ++j) {
            scanf("%f", &images_filters->filters[images_filters->filter_count].data[i][j]);
        }
    }

    images_filters->filters[images_filters->filter_count].size = size;
    images_filters->filter_count++;
}

void Apply_Filter(ImagesFilters *images_filters)  {
    int index_img = 0, index_filter = 0;
    scanf("%d %d", &index_img, &index_filter);

    int ***new_data = apply_filter(images_filters->images[index_img].data,
        images_filters->images[index_img].N, images_filters->images[index_img].M,
        images_filters->filters[index_filter].data, images_filters->filters[index_filter].size);

    // Free the memory of the original image data
    free_image(images_filters->images[index_img].data,
        images_filters->images[index_img].N, images_filters->images[index_img].M);

    // Update data
    images_filters->images[index_img].data = new_data;
}

void Delete_filter(ImagesFilters *images_filters) {
    int index_filter = 0;
    scanf("%d", &index_filter);

    // Free the filter data at specified index only if it's not in use
    free_filter(images_filters->filters[index_filter].data,
            images_filters->filters[index_filter].size);

    // Shift remaining filters to fill the gap
    for (int i = index_filter; i < images_filters->filter_count - 1; ++i) {
       images_filters->filters[i] = images_filters->filters[i + 1];
    }
    images_filters->filter_count--;
}

void Delete_Image(ImagesFilters *images_filters) {
    int index_img = 0;
    scanf("%d", &index_img);

    // Free the image data at specified index only if it's not in use
    free_image(images_filters->images[index_img].data,
            images_filters->images[index_img].N, images_filters->images[index_img].M);

    // Shift remaining images to fill the gap
    for (int i = index_img; i < images_filters->image_count - 1; ++i) {
        images_filters->images[i] = images_filters->images[i + 1];
    }
    images_filters->image_count--;
}
