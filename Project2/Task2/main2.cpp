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

void apply_LOG_filter(my_image_comp* in, my_image_comp* out, my_image_comp* inter_1,
    my_image_comp* inter_2, my_image_comp* y1, my_image_comp* y2,
    float sigma, int H, float alpha, int debug)
{
#define PI 3.1415926F
#define FILTER_TAPS (2*H+1)

    /* Decompose Laplacian of Gaussian filter into 4 filters */
    // Origin of filters is at position filter[H]
    // Filters are initialised to 1
    vector<float> h_11(FILTER_TAPS, 1); // Partial of s1 -> s1 component
    vector<float> h_12(FILTER_TAPS, 1); // Partial of s1 -> s2 component
    vector<float> h_21(FILTER_TAPS, 1); // Partial of s2 -> s1 component
    vector<float> h_22(FILTER_TAPS, 1); // Partial of s2 -> s2 component
    for (int location = -H; location <= H; location++)
    {
        h_11[H + location] = (-location * location + sigma * sigma) /
            (2 * PI * pow(sigma, 6) * exp((location * location) / (2 * sigma * sigma)));
        h_12[H + location] = exp(-(location * location) / (2 * sigma * sigma));
        h_21[H + location] = exp(-(location * location) / (2 * sigma * sigma));
        h_22[H + location] = (-location * location + sigma * sigma) /
            (2 * PI * pow(sigma, 6) * exp((location * location) / (2 * sigma * sigma)));

    }

    /* Debugging filter */
    if (debug)
    {
        printf("Checking the h_11 vector...\n");
        for (int i = 0; i < h_11.size(); i++)
            printf("%f   ", h_11[i]);
        printf("\n");
        printf("Checking the h_12 vector...\n");
        for (int i = 0; i < h_12.size(); i++)
            printf("%f   ", h_12[i]);
        printf("\n");
        printf("Checking the h_21 vector...\n");
        for (int i = 0; i < h_21.size(); i++)
            printf("%f   ", h_21[i]);
        printf("\n");
        printf("Checking the h_22 vector...\n");
        for (int i = 0; i < h_22.size(); i++)
            printf("%f   ", h_22[i]);
        printf("\n");
    }

    /* Check that input has enough boundary extension for filtering */
    assert(in->border >= H);

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

        /*------------------------------- TASK 2 -------------------------------*/
        int debug = 1;

        // Symmetric extension for input
        for (n = 0; n < num_comps; n++)
            input_comps[n].perform_boundary_extension();

        // Allocate storage for the filtered outputs
        my_image_comp* output_comps = new my_image_comp[num_comps];  // output = y1 + y2
        my_image_comp* inter_1_comps = new my_image_comp[num_comps]; // intermediate y1   
        my_image_comp* inter_2_comps = new my_image_comp[num_comps]; // intermediate y2
        my_image_comp* y1_comps = new my_image_comp[num_comps];      // Partial of s1
        my_image_comp* y2_comps = new my_image_comp[num_comps];      // Partial of s2
        for (n = 0; n < num_comps; n++)
        {
            output_comps[n].init(height, width, 0);
            inter_1_comps[n].init(height, width, H);
            inter_2_comps[n].init(height, width, H);
            y1_comps[n].init(height, width, 0);
            y2_comps[n].init(height, width, 0);
        }

        // Process the image, all in floating point (easy)
        for (n = 0; n < num_comps; n++)
            apply_LOG_filter(input_comps + n, output_comps + n, inter_1_comps + n, 
                inter_2_comps + n, y1_comps + n, y2_comps + n, sigma, H, alpha, debug);

        /*-------------------------------------------------------------------------*/

       //// Write the image back out again
       // bmp_out out;
       // if ((err_code = bmp_out__open(&out, argv[2], width, height, num_comps)) != 0)
       //     throw err_code;
       // for (r = height - 1; r >= 0; r--)
       // { // "r" holds the true row index we are writing, since the image is
       //   // written upside down in BMP files.
       //     for (n = 0; n < num_comps; n++)
       //     {
       //         io_byte* dst = line + n; // Points to first sample of component n
       //         float* src = output_comps[n].buf + r * output_comps[n].stride;
       //         for (int c = 0; c < width; c++, dst += num_comps)
       //         {
       //             // Deal with overflow and underflow
       //             if (src[c] > 255)
       //                 src[c] = 255;
       //             else if (src[c] < 0)
       //                 src[c] = 0;
       //             // Write output to destination
       //             *dst = (io_byte)src[c];
       //         }
       //     }
       //     bmp_out__put_line(&out, line);
       // }
       // bmp_out__close(&out);
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