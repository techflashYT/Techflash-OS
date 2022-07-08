#include <stdint.h>

typedef struct {
	uint32_t magic;
	uint32_t version;
	uint32_t headerSize;
	uint32_t flags;
	uint32_t numberOfGlyphs;
	uint32_t bytesPerGlyph;
	uint32_t height;
	uint32_t width;
	uint8_t glyphs;
} __attribute__((packed)) psf2_t;
