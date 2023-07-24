#include <stdint.h>
// TFF, Techflash Font Format.  Not to be confused with TTF, TrueType fonts.
typedef struct {
	struct {
		// {'T', 'F', 'F', '\x69}
		char ident[4];

		// (3 4-bit numbers packed in 16-bits.  Use PACK_VERSION(maj, min, pat); for this.
		uint16_t version;

		// the number of glyphs in this font
		uint8_t numGlyphs;

		// 2 4-bit numbers.  Use PACK_WIDTH_HEIGHT(width, height); for this.
		uint8_t widthHeight;

		// the glyph to use when the current one isn't found
		uint8_t unkCharGlyph;
	} __attribute__((packed)) header;
	/* glyphs represented as bit arrays, e.g. 8x16 A would be:
	00000000
	00000000
	00000000
	00011000
	01100110
	11000011
	11000011
	11000011
	11111111
	11000011
	11000011
	11000011
	00000000
	00000000
	00000000
	*/
	struct {
		uint8_t glyphNum;
		uint8_t glyphBitField[16];
	} __attribute__((packed)) glyphs[];
} __attribute__((packed)) font_t;
extern font_t font;
#define PACK_VERSION(a, b, c) (((uint16_t)(a & 0xF) << 8) | ((uint16_t)(b & 0xF) << 4) | ((uint16_t)(c & 0xF)))
#define UNPACK_VERSION(num, a, b, c) \
	a = (num >> 8) & 0xF; \
	b = (num >> 4) & 0xF; \
	c = num & 0xF;

#define PACK_WIDTH_HEIGHT(a, b) (((uint8_t)((a - 1) & 0xF) << 4) | ((uint8_t)((b - 1) & 0xF)))
#define UNPACK_WIDTH_HEIGHT(num, a, b) \
	a = (((num) >> 4) & 0xF) + 1; \
	b = ((num)& 0xF) + 1;
