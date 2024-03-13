# BMP Operations

## Description

Basic **bmp** image processing tasks. It represents an image as an `NxM` matrix of pixels (`N` rows, `M` columns), where each pixel contains 3 values: **R (Red), G (Green), B (Blue)**, representing the color components. Thus, an image is essentially an `NxMx3` matrix. Each of the color components can take integer values between **0 and 255** inclusive.

## Features

The program supports a variety of operations to manipulate images, including flipping, rotating, cropping, extending, copy-pasting, and applying filters.
The program operates interactively, accepting the following commands:

- **Exit (`e`)**: Exits the program.
- **Load (`l`)**: Loads an image from a specified path. Usage: `l N M path`
- **Save (`s`)**: Saves an image to a specified path. Usage: `s index path`
- **Apply Horizontal Flip (`ah`)**: Flips an image horizontally. Usage: `ah index`
- **Apply Rotate (`ar`)**: Rotates an image 90 degrees to the left. Usage: `ar index`
- **Apply Crop (`ac`)**: Crops an image. Usage: `ac index x y w h`
- **Apply Extend (`ae`)**: Extends an image. Usage: `ae index rows cols R G B`
- **Apply Paste (`ap`)**: Pastes a source image onto a destination image. Usage: `ap index_dst index_src x y`
- **Create Filter (`cf`)**: Creates a filter with specified dimensions and values. Usage: `cf size [list of values]`
- **Apply Filter (`af`)**: Applies a filter to an image. Usage: `af index_img index_filter`
- **Delete Filter (`df`)**: Deletes a filter. Usage: `df index_filter`
- **Delete Image (`di`)**: Deletes an image. Usage: `di index_img`

## Apply Filter

The matrix of pixels with each pixel comprising *Red (R), Green (G), and Blue (B) components*, and a *filter matrix* of size `filter_size x filter_size`, the new value of each pixel after applying the filter is calculated as follows:

- The original value of a pixel at position `(i, j)` in the image as $(R_{ij}, G_{ij}, B_{ij})$.
- The filter matrix as $F$, with elements $f_{mn}$, where $m$ and $n$ run from 1 to `filter_size`.

The new value of a pixel at position `(i, j)` is calculated by applying the filter to the pixel and its neighbors. This operation involves a convolution between the filter matrix and the region of the image surrounding the pixel, taking into account the boundary conditions.

**Note:** When the pixel's neighbors extend beyond the image boundaries, those neighbors are assumed to have the value (0, 0, 0) for the purposes of this calculation.

After calculating the sums for **$R'$, $G'$, and $B'$**, the values are cast to integers and clamped to the range **[0, 255]** to ensure they remain valid color component values:

- If the **sum is less than 0**, the value is set to `0`.
- If the **sum is greater than 255**, the value is set to `255`.

Each pixel's new color values remain within the acceptable range for RGB color components, thereby applying the filter effect to the entire image. This process is repeated for every pixel in the image to produce the filtered image.

## Memory Management

To ensure there are no memory leaks, we recommend regularly checking with `Valgrind`,  memory debugging, memory leak detection. Running your program through Valgrind will help identify where your program might be leaking memory.

```bash
valgrind --leak-check=full --show-leak-kinds=all ./interactive
```
