/*****************************************************************************/
// File: main1.cpp
// Author: David Taubman & Renee Lu
// Last Revised: 18 July, 2020
/*****************************************************************************/
// Copyright 2007, David Taubman, The University of New South Wales (UNSW)
/*****************************************************************************/

#include "io_bmp.h"
#include "image_comps.h"
#include <math.h>
#include <tuple>
#include <vector>
#include <iostream> 
#include <cstdlib>
#include <time.h>

using namespace std;

/* ========================================================================= */
/*                 IMPLEMENTATION OF 'my_image_comp' FUNCTIONS               */
/* ========================================================================= */

/*---------------------------------------------------------------------------*/
/*                  my_image_comp::perform_boundary_extension                */
/*                           Symmetric Extension                             */
/*---------------------------------------------------------------------------*/

void my_image_comp::perform_boundary_extension()
{
    int r, c;

    // First extend upwards by border
    float* first_line = buf;
    for (r = 1; r <= border; r++)
        for (c = 0; c < width; c++)
            first_line[-r * stride + c] = first_line[r * stride + c];

    // Now extend downwards by border
    float* last_line = buf + (height - 1) * stride;
    for (r = 1; r <= border; r++)
        for (c = 0; c < width; c++)
            last_line[r * stride + c] = last_line[-r * stride + c];

    // Now extend all rows to the left and to the right
    float* left_edge = buf - border * stride;
    float* right_edge = left_edge + width - 1;
    for (r = height + 2 * border; r > 0; r--, left_edge += stride, right_edge += stride)
        for (c = 1; c <= border; c++)
        {
            left_edge[-c] = left_edge[c];
            right_edge[c] = right_edge[-c];
        }
}

/* ========================================================================= */
/*                              Global Functions                             */
/* ========================================================================= */

/*---------------------------------------------------------------------------*/
/*                                apply_filter                               */
/*---------------------------------------------------------------------------*/

void apply_LOG_filter(my_image_comp* in, my_image_comp* out,
    float sigma, int H, float alpha, int debug)
{
#define PI 3.1415926F
#define FILTER_DIM (2*H + 1)
#define FILTER_TAPS (FILTER_DIM*FILTER_DIM)

    /* Create filter vector 'mirror_psf' of size FILTER_SIZE * FILTER_SIZE */
    // Origin of filter is at position mirror_psf[H][H]
    // Filter is initialised to 1
    int rows = FILTER_DIM; // height
    int cols = FILTER_DIM; // width
    // Create filter values using Laplacian of Gaussian PSF equation
    vector<vector<float>> mirror_psf(rows, vector<float>(cols, 1));
    for (int row_index = -H; row_index <= H; row_index++)
        for (int col_index = -H; col_index <= H; col_index++)
        {
            mirror_psf[H + row_index][H + col_index] = ((row_index * row_index + col_index * col_index - 2 * sigma * sigma) /
                (2 * PI * pow(sigma, 6))) * exp(-(row_index * row_index + col_index * col_index) / (2 * sigma * sigma));
        }

    /* Debugging filter */
    if (debug)
    {
        printf("Checking the filters vector...\n");
        for (int i = 0; i < mirror_psf.size(); i++)
        {
            for (int j = 0; j < mirror_psf[i].size(); j++)
                printf("%f   ", mirror_psf[i][j]);
            printf("\n");
        }
    }

    /* Check that input has enough boundary extension for filtering */
    assert(in->border >= H);

    // Perform the convolution
    for (int r = 0; r < out->height; r++)
        for (int c = 0; c < out->width; c++)
        {
            float* ip = in->buf + r * in->stride + c;
            float* op = out->buf + r * out->stride + c;
            float sum = 0.0F;
            for (int row_index = -H; row_index <= H; row_index++)
                for (int col_index = -H; col_index <= H; col_index++)
                    sum += ip[row_index * in->stride + col_index] * mirror_psf[H + row_index][H + col_index];
            *op = (sum * alpha) + 128;    // Output = filtered output scaled by alpha + 128
        }

}

/*---------------------------------------------------------------------------*/
/*                                    main                                   */
/*---------------------------------------------------------------------------*/

int
main(int argc, char* argv[])
{
    /* Get the args */
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <input bmp file> <output bmp file> <sigma> <alpha>\n", argv[0]);
        return -1;
    }
    float sigma = atof(argv[3]);    // Determines LoG filter co-efficients
    float alpha = atoi(argv[4]);    // Scaling factor alpha
    int H = ceil(3 * sigma);        // Extent of filter, for Nyquist Bandlimited

    /* Create the input image storage */
    int err_code = 0;
    try {
        // Read the input image
        bmp_in in;
        if ((err_code = bmp_in__open(&in, argv[1])) != 0)
            throw err_code;
        // Get input and input border dimensions
        int width = in.cols, height = in.rows;
        int n, num_comps = in.num_components;   // Number of colour components
        // Initialise the input storage
        my_image_comp* input_comps = new my_image_comp[num_comps];
        for (n = 0; n < num_comps; n++)
            input_comps[n].init(height, width, H); // Leave a border of H

        int r; // Declare row index
        io_byte* line = new io_byte[width * num_comps];
        for (r = height - 1; r >= 0; r--)
        { // "r" holds the true row index we are reading, since the image is
          // stored upside down in the BMP file.
            if ((err_code = bmp_in__get_line(&in, line)) != 0)
                throw err_code;
            for (n = 0; n < num_comps; n++)
            {
                io_byte* src = line + n; // Points to first sample of component n
                float* dst = input_comps[n].buf + r * input_comps[n].stride;
                for (int c = 0; c < width; c++, src += num_comps)
                    dst[c] = (float)*src; // The cast to type "float" is not
                          // strictly required here, since bytes can always be
                          // converted to floats without any loss of information.
            }
        }
        bmp_in__close(&in);

        /*------------------------------- TASK 1 -------------------------------*/
        int debug = 0;

        // Symmetric extension for input
        for (n = 0; n < num_comps; n++)
            input_comps[n].perform_boundary_extension();

        // Allocate storage for the filtered output
        my_image_comp* output_comps = new my_image_comp[num_comps];
        for (n = 0; n < num_comps; n++)
        {
            output_comps[n].init(height, width, 0);
        }

        // Process the image, all in floating point (easy)
        for (n = 0; n < num_comps; n++)
            apply_LOG_filter(input_comps + n, output_comps + n, sigma, H, alpha, debug);

        /*-------------------------------------------------------------------------*/

       // Write the image back out again
        bmp_out out;
        if ((err_code = bmp_out__open(&out, argv[2], width, height, num_comps)) != 0)
            throw err_code;
        for (r = height - 1; r >= 0; r--)
        { // "r" holds the true row index we are writing, since the image is
          // written upside down in BMP files.
            for (n = 0; n < num_comps; n++)
            {
                io_byte* dst = line + n; // Points to first sample of component n
                float* src = output_comps[n].buf + r * output_comps[n].stride;
                for (int c = 0; c < width; c++, dst += num_comps)
                {
                    // Deal with overflow and underflow
                    if (src[c] > 255)
                        src[c] = 255;
                    else if (src[c] < 0)
                        src[c] = 0;
                    // Write output to destination
                    *dst = (io_byte)src[c];
                }
            }
            bmp_out__put_line(&out, line);
        }
        bmp_out__close(&out);
        delete[] line;
        delete[] input_comps;
        delete[] output_comps;
    }
    catch (int exc) {
        if (exc == IO_ERR_NO_FILE)
            fprintf(stderr, "Cannot open supplied input or output file.\n");
        else if (exc == IO_ERR_FILE_HEADER)
            fprintf(stderr, "Error encountered while parsing BMP file header.\n");
        else if (exc == IO_ERR_UNSUPPORTED)
            fprintf(stderr, "Input uses an unsupported BMP file format.\n  Current "
                "simple example supports only 8-bit and 24-bit data.\n");
        else if (exc == IO_ERR_FILE_TRUNC)
            fprintf(stderr, "Input or output file truncated unexpectedly.\n");
        else if (exc == IO_ERR_FILE_NOT_OPEN)
            fprintf(stderr, "Trying to access a file which is not open!(?)\n");
        return -1;
    }
    return 0;
}