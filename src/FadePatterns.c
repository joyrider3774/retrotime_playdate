#include "pd_api.h"

LCDPattern FadeInPatterns[9] = {
	{
		// Bitmap
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b10000000,
		0b01000000,
		0b00100000,
		0b00010000,
		0b00001000,
		0b00000100,
		0b00000010,
		0b00000001,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b10001000,
		0b01000100,
		0b00100010,
		0b00010001,
		0b10001000,
		0b01000100,
		0b00100010,
		0b00010001,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11001000,
		0b01100100,
		0b00110010,
		0b00011001,
		0b10001100,
		0b01000110,
		0b00100011,
		0b10010001,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11001100,
		0b01100110,
		0b00110011,
		0b10011001,
		0b11001100,
		0b01100110,
		0b00110011,
		0b10011001,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11101100,
		0b01110110,
		0b00111011,
		0b10011101,
		0b11001110,
		0b01100111,
		0b10110011,
		0b11011001,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11101110,
		0b01110111,
		0b10111011,
		0b11011101,
		0b11101110,
		0b01110111,
		0b10111011,
		0b11011101,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11111110,
		0b01111111,
		0b10111111,
		0b11011111,
		0b11101111,
		0b11110111,
		0b11111011,
		0b11111101,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	}
};

LCDPattern FadeInPatterns2[9] = {
	{
		// Bitmap
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11111111,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11111111,
		0b00000000,
		0b00000000,
		0b00000000,
		0b11111111,
		0b00000000,
		0b00000000,
		0b00000000,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11111111,
		0b11111111,
		0b00000000,
		0b00000000,
		0b11111111,
		0b00000000,
		0b00000000,
		0b00000000,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11111111,
		0b11111111,
		0b00000000,
		0b00000000,
		0b11111111,
		0b11111111,
		0b00000000,
		0b00000000,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11111111,
		0b11111111,
		0b11111111,
		0b00000000,
		0b11111111,
		0b11111111,
		0b00000000,
		0b00000000,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11111111,
		0b11111111,
		0b11111111,
		0b00000000,
		0b11111111,
		0b11111111,
		0b11111111,
		0b00000000,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b00000000,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	}
};

LCDPattern FadeInPatterns3[8] = {
	{
		// Bitmap
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b10001000,
		0b00000000,
		0b00100010,
		0b00000000,
		0b10001000,
		0b00000000,
		0b00100010,
		0b00000000,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11001100,
		0b00000000,
		0b00110011,
		0b00000000,
		0b11001100,
		0b00000000,
		0b00110011,
		0b00000000,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11001100,
		0b11001100,
		0b00110011,
		0b00110011,
		0b11001100,
		0b11001100,
		0b00110011,
		0b00110011,


		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11101110,
		0b11001100,
		0b10111011,
		0b00111011,
		0b11101110,
		0b11001100,
		0b10111011,
		0b00111011,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11101110,
		0b11101110,
		0b10111011,
		0b10111011,
		0b11101110,
		0b11101110,
		0b10111011,
		0b10111011,


		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11111111,
		0b11101110,
		0b11111111,
		0b10111011,
		0b11111111,
		0b11101110,
		0b11111111,
		0b10111011,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	},
	{
		// Bitmap
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,

		// Mask
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
	}
};