#include <kernel/newFont.h>
font_t font = {
	.header = {
		.ident = {'T', 'F', 'F'},
		.version = PACK_VERSION(0, 0, 1),
		.numGlyphs = 1,
		.widthHeight = PACK_WIDTH_HEIGHT(8, 16)
	},
	.glyphs = {
		{	.glyphNum = 'A',
			.glyphBitField = {
				0b00000000,
				0b00000000,
				0b00000000,
				0b00011000,
				0b01100110,
				0b11000011,
				0b11000011,
				0b11000011,
				0b11111111,
				0b11000011,
				0b11000011,
				0b11000011,
				0b00000000,
				0b00000000,
				0b00000000
			}
		}, {.glyphNum = 'B',
			.glyphBitField = {
				0b00000000,
				0b00000000,
				0b00000000,
				0b11111100,
				0b11000011,
				0b11000011,
				0b11000011,
				0b11111100,
				0b11111100,
				0b11000011,
				0b11000011,
				0b11111100,
				0b00000000,
				0b00000000,
				0b00000000
			}
		}
	}
};