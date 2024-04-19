#include <iostream>
#include <stdio.h>
#include <math.h>

#define W 750
#define H 500

/* This function operates on the magnitude of the Sobel difference in horizontal and vertical directions, then combines them into one gradient. */
double combineGradient(double magnitude_x, double magnitude_y, double constant, int mode, int threshold) {

	double gradient = 0;

	/* Divide the magnitudes by some constant, then normalize them. */
	magnitude_x = magnitude_x / constant;
	magnitude_x = magnitude_x * 255;

	magnitude_y = magnitude_y / constant;
	magnitude_y = magnitude_y * 255;

	/* Formula to combine horizontal and vertical gradients:
	gradient = sqrt(G(x)^2 + G(y)^2)  */
	magnitude_x = round(pow(magnitude_x, 2));
	magnitude_y = round(pow(magnitude_y, 2));

	gradient = magnitude_x + magnitude_y;
	gradient = round(sqrt(gradient));

	/* Run these for non-threshold ops. Broader range of whites/blacks/greys. */
	if (mode == 0) {
		if (gradient > 255)
			return 255;
		else
			return gradient;
	}

	/* Run these for threshold ops. Set pixels value 35 or higher to high intensity, 200. All other pixels are pure black, 0. */
	else if (mode == 1) {
		if (gradient > threshold)
			return 200;
		else
			return 0;
	}

	/* Inverted threshold. Trim the white lines by making high-difference edges black, but leaving low difference (inner white line) as white*/
	else if (mode == 2) {
		if (gradient > threshold)
			return 0;
		else
			return 200;
	}
}

int main() {

	/* Declarations: input image and output image. File ptrs for IO*/
	unsigned char image[H][W];
	unsigned char output[H][W];

	FILE* inPtr;
	FILE* outPtr;

	/* These are 3x3 and 5x5 filters for horizontal and vertical sobel operations.
	Max value of one sobel op with a 3x3 filter is 1020:
	0*-1 + 0 + 1*255
	0*-2 + 0 + 2*255
	0*-1 + 0 + 1*255
	=1020

	Using the same method for the 5x5 filter, we get 7650.

	These two constants will be used to normalize the output results.
	Ie, 3x3 filter result will be divided by 1020, 5x5 results divdied by 7650.
	*/
	float kernel_H3x3[3][3] = {
		{ -1, 0, 1 },
		{ -2, 0, 2 },
		{ -1, 0, 1 } };

	float kernel_V3x3[3][3] = {
		{ 1,  2,  1},
		{ 0,  0,  0},
		{-1, -2, -1} };

	float kernel_H5x5[5][5] = {
		{-1, -2, 0, 2, 1},
		{-2, -4, 0, 4, 2},
		{-4, -8, 0, 8, 4},
		{-2, -4, 0, 4, 2},
		{-1, -2, 0, 2, 1} };

	float kernel_V5x5[5][5] = {
		{ 1,  2,  4,  2,  1},
		{ 2,  4,  8,  4,  2},
		{ 0,  0,  0,  0,  0},
		{-2, -4, -8, -4, -2},
		{-1, -2, -4, -2, -1} };

	/* Select how to manipulate pixels, and count loop iterations */
	int input_choice = 0;
	int sobel_method = 0;

	/* Controls whether we do threshold or no threshold. */
	int mode = 0;

	/* Variable storing the result of a sobel operation, one for x, one for y directions */
	double magnitude_x = 0;
	double magnitude_y = 0;

	/* The value which holds the combined gradients in horizontal/veritcal directions. */
	double gradient = 0;

	/* PLEASE NOTE: case 3 and 4 ONLY WORK in Release mode if you're using Visual Studio. I have no idea why, but
	 * if you're in Debug mode and run case 3 or 4, the colors are inverted. */
	std::cout << "Select which image you'd like to do Sobel operation on: " << std::endl
		<< "1: unesco 3x3, no threshold: Variety of blacks, greys, whites. " << std::endl
		<< "2: unesco 5x5, no threshold: Variety of blacks, greys, whites." << std::endl
		<< "3: unesco 3x3, with threshold: Black and white. - If on Visual Studio, THIS ONLY WORKS ON RELEASE MODE!" << std::endl
		<< "4: unesco 5x5, with threshold: Black and white. - If on Visual Studio, THIS ONLY WORKS ON RELEASE MODE!"  << std::endl
		<< "5: Generate input for case 3. INTERMEDIATE STEP: You don't need to use this, its already done for you." << std::endl
		<< "6: Generate input for case 4. INTERMEDIATE STEP: You don't need to use this, its already done for you. " << std::endl;

	std::cin >> input_choice;

	/* Do one of these based on user input choice */
	switch (input_choice) {

	/* Run a normal sobel 3x3 operation. X/V gradients combined. */
	case 1:
		mode = 0;
		inPtr = fopen("unesco750.raw", "rb");
		fread(image, sizeof(unsigned char), H * W, inPtr);
		fclose(inPtr);

		for (int i = 1; i < H - 1; i++) {
			for (int j = 1; j < W - 1; j++) { // start op for a new pixel in output image
				magnitude_x = 0;
				magnitude_y = 0;
				gradient = 0;

				for (int k = 0; k < 3; k++) {
					for (int l = 0; l < 3; l++) {
						magnitude_x += image[i - 1 + k][j - 1 + l] * kernel_H3x3[k][l];
						magnitude_y += image[i - 1 + k][j - 1 + l] * kernel_V3x3[k][l];
					}
				}
				// Merge x and y into a single gradient value
				gradient = combineGradient(magnitude_x, magnitude_y, 1020, mode, 35);
				output[i][j] = gradient; // output magnitude for one pixel
				//std::cout << i << "x" << j << ": " << gradient << std::endl;
			}
		}
		break;

	/* Run a normal sobel 5x5 operation. X/V gradients combined. */
	case 2:
		mode = 0;
		inPtr = fopen("unesco750.raw", "rb");
		fread(image, sizeof(unsigned char), H * W, inPtr);
		fclose(inPtr);

		for (int i = 2; i < H - 2; i++) {
			for (int j = 2; j < W - 2; j++) { // start op for a new pixel in output image
				magnitude_x = 0;
				magnitude_y = 0;
				gradient = 0;

				for (int k = 0; k < 5; k++) {
					for (int l = 0; l < 5; l++) {
						magnitude_x += image[i - 1 + k][j - 1 + l] * kernel_H5x5[k][l];
						magnitude_y += image[i - 1 + k][j - 1 + l] * kernel_V5x5[k][l];
					}
				}
				gradient = combineGradient(magnitude_x, magnitude_y, 9000, mode, 35);
				output[i][j] = gradient; // output magnitude for one pixel
				//std::cout << i << "x" << j << ": " << gradient << std::endl;
			}
		}
		break;

	/* Trim the lines of a threshold 3x3 image. */
	case 3:
		mode = 2;
		inPtr = fopen("test.raw", "rb");
		fread(image, sizeof(unsigned char), H * W, inPtr);
		fclose(inPtr);

		for (int i = 1; i < H - 1; i++) {
			for (int j = 1; j < W - 1; j++) { // start op for a new pixel in output image

				// Leave blacks as black. Trim any white lines. 
				if (image[i][j] != 0) {
					magnitude_x = 0;
					magnitude_y = 0;
					gradient = 0;

					for (int k = 0; k < 3; k++) {
						for (int l = 0; l < 3; l++) {


							magnitude_x += image[i - 1 + k][j - 1 + l] * kernel_H3x3[k][l];
							magnitude_y += image[i - 1 + k][j - 1 + l] * kernel_V3x3[k][l];

						}
					}
					// Merge x and y into a single gradient value
					gradient = combineGradient(magnitude_x, magnitude_y, 1020, mode, 180);
					output[i][j] = gradient; // output magnitude for one pixel
					//std::cout << i << "x" << j << ": " << gradient << std::endl;

				} // end j loop
			} // end i loop
		}
		break;

	/* Trim the lines of a threshold 5x5 image. */
	case 4:
		mode = 2;
		inPtr = fopen("unesco_5x5_threshold.raw", "rb");
		fread(image, sizeof(unsigned char), H * W, inPtr);
		fclose(inPtr);

		for (int i = 2; i < H - 2; i++) {
			for (int j = 2; j < W - 2; j++) { // start op for a new pixel in output image
				if (image[i][j] != 0) {
					magnitude_x = 0;
					magnitude_y = 0;
					gradient = 0;

					for (int k = 0; k < 5; k++) {
						for (int l = 0; l < 5; l++) {
							magnitude_x += image[i - 1 + k][j - 1 + l] * kernel_H5x5[k][l];
							magnitude_y += image[i - 1 + k][j - 1 + l] * kernel_V5x5[k][l];
						}
					}
					gradient = combineGradient(magnitude_x, magnitude_y, 7650, mode, 180);
					output[i][j] = gradient; // output magnitude for one pixel
					//std::cout << i << "x" << j << ": " << gradient << std::endl;
				}
			}
		}
		break;

	/* 3x3 threshold intermediate - use this for input in case 3. */
	case 5:
		mode = 1;
		inPtr = fopen("unesco750.raw", "rb");
		fread(image, sizeof(unsigned char), H * W, inPtr);
		fclose(inPtr);

		for (int i = 1; i < H - 1; i++) {
			for (int j = 1; j < W - 1; j++) { // start op for a new pixel in output image
				magnitude_x = 0;
				magnitude_y = 0;
				gradient = 0;

				for (int k = 0; k < 3; k++) {
					for (int l = 0; l < 3; l++) {
						magnitude_x += image[i - 1 + k][j - 1 + l] * kernel_H3x3[k][l];
						magnitude_y += image[i - 1 + k][j - 1 + l] * kernel_V3x3[k][l];
					}
				}
				// Merge x and y into a single gradient value
				gradient = combineGradient(magnitude_x, magnitude_y, 1020, mode, 35);
				output[i][j] = gradient; // output magnitude for one pixel
				//std::cout << i << "x" << j << ": " << gradient << std::endl;
			}
		}
		break;

	/* 5x5 threshold intermediate - use this for input in case 4. */
	case 6:
		mode = 1;
		inPtr = fopen("unesco750.raw", "rb");
		fread(image, sizeof(unsigned char), H * W, inPtr);
		fclose(inPtr);

		for (int i = 2; i < H - 2; i++) {
			for (int j = 2; j < W - 2; j++) { // start op for a new pixel in output image
				magnitude_x = 0;
				magnitude_y = 0;
				gradient = 0;

				for (int k = 0; k < 5; k++) {
					for (int l = 0; l < 5; l++) {
						magnitude_x += image[i - 1 + k][j - 1 + l] * kernel_H5x5[k][l];
						magnitude_y += image[i - 1 + k][j - 1 + l] * kernel_V5x5[k][l];
					}
				}
				gradient = combineGradient(magnitude_x, magnitude_y, 7650, mode, 35);
				output[i][j] = gradient; // output magnitude for one pixel
				//std::cout << i << "x" << j << ": " << gradient << std::endl;
			}
		}
		break;
	}
	/* Map the output array into an output image raw file. */
	outPtr = fopen("myOutput.raw", "wb");
	fwrite(output, sizeof(unsigned char), H * W, outPtr);
	fclose(outPtr);

	return 0;
}