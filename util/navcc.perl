#!/usr/bin/perl

# Credit: nathanrsfba (NathanR#9714) on Discord
# Nathan's Amazing VGA to C Converter
# License: MIT

if( scalar( @ARGV ) != 1 ) {
	die "Usage: $0 <fontfile>\n";
}

open my $fh, "<:raw", $ARGV[0];
printf(
"#include <kernel/newFont.h>
font_t font = {
	.header = {
		.ident = {'T', 'F', 'F'},
		.version = PACK_VERSION(0, 0, 1),
		.numGlyphs = 0xFF,
		.widthHeight = PACK_WIDTH_HEIGHT(8, 16),
		.unkCharGlyph = 0x00
	},
	.glyphs = {
		{	.glyphNum = 0x00,
			.glyphBitField = {
				0b10101010,
				0b00000001,
				0b10111100,
				0b01100111,
				0b11100110,
				0b00001101,
				0b10011000,
				0b00011001,
				0b10011000,
				0b00011001,
				0b10000000,
				0b00111101,
				0b10111100,
				0b00000001,
				0b10000000,
				0b01010101
			}
		}, {"
);
read $fh, my $junk, 16;
for $i (0..254) {
	printf(".glyphNum = 0x%02X,\r\n\t\t\t.glyphBitField = {\r\n", $i + 1);
	for $l (0..15) {
		read $fh, my $c, 1;
		printf( "\t\t\t\t0b%08b", ord( $c ) );
		if ($l == 15) {
			print("\r\n\t\t\t}\r\n");
		}
		else {
			print(",");
		}

		print "\r\n";
	}
	if ($i != 254) {
		print "\t\t}, {";
	}
}
printf(
"\t\t}
\t}
};");
