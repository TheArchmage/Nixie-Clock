/* This document contains the lookup tables corresponding to the 
physical construction of the Divergence Meter Tubes and Drivers 
Board. These tables are to be used in conjunction with a 
bitstream generation algorithm to convert intended display
parameters into a bitstream which can be clocked out over the 
data line. The data contained in these LUT's is the cathode number
associated with the pins of each tube. The 10th and 11th index 
positions correspond to the left and right decimal points 
respectively

Date: 8-20-15
Author: Jim Herner
*/
#ifndef _TUBE_BOARD_LUT_H_
#define _TUBE_BOARD_LUT_H_ 1

static const uint8_t Nixie[8][12]={
{11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 13, 1},
{24, 14, 15, 17, 16, 18, 20, 22, 21, 23, 19, 12	},
{35, 26, 27, 28, 29, 30, 31, 32, 33, 34, 36, 25},
{47, 38, 39, 40, 41, 42, 43, 44, 45, 46, 48, 37},
{59, 50, 51, 52, 53, 54, 55, 56, 57, 58, 60, 49},
{71, 62, 63, 64, 65, 66, 67, 68, 69, 70, 72, 61},
{83, 74, 75, 77, 76, 78, 80, 82, 81, 79, 84, 73},
{95, 85, 87, 88, 89, 90, 91, 92, 93, 94, 96, 86}
};
#endif