#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <kernel/tty.h>
#include <kernel/graphics.h>

bool escapeCompleted = false;
char escapeSequence[256];
int escapeSequenceLength = 0;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstack-usage="
bool FB_HandleEsc(char ch) {
    escapeSequence[escapeSequenceLength++] = ch;
    escapeSequence[escapeSequenceLength] = '\0';

    // Check if the escape sequence has completed
    if (ch >= 0x40 && ch <= 0x7E) {

        // Parse color codes from the escape sequence
        char* colorCodeStart = strstr(escapeSequence, "[");
        if (colorCodeStart != NULL) {
            char* colorCodeEnd = strstr(colorCodeStart, "m");
            if (colorCodeEnd != NULL) {
                // it was able to find the `m`, therefore it is now done
                escapeCompleted = true;
                // Extract the color code substring
                int codeLength = colorCodeEnd - colorCodeStart - 1;
                char colorCode[codeLength + 1];
                memcpy(colorCode, colorCodeStart + 1, codeLength);
                colorCode[codeLength] = '\0';

                // Handle color and attribute switching based on the color code
                int code = atoi(colorCode);
                switch (code) {
                    case 0:
                        // Reset all attributes
                        TTY_Color = colors.vga.lgray;
                        TTY_TextBackground = colors.vga.black;
                        TTY_Bold = false;
                        // TODO: TTY_Underline = false;
                        break;
                    case 1:
                        // Set text style to bold
                        TTY_Bold = true;
                        break;
                    case 22:
                        // Disable bold
                        TTY_Bold = false;
                        break;
                    case 30:
                        // Set text color to black
                        TTY_Color = colors.vga.black;
                        break;
                    case 31:
                        // Set text color to red
                        TTY_Color = colors.vga.red;
                        break;
                    case 32:
                        // Set text color to green
                        TTY_Color = colors.vga.green;
                        break;
                    case 33:
                        // Set text color to yellow
                        TTY_Color = colors.vga.yellow;
                        break;
                    case 34:
                        // Set text color to blue
                        TTY_Color = colors.vga.blue;
                        break;
                    case 35:
                        // Set text color to magenta
                        TTY_Color = colors.vga.purple;
                        break;
                    case 36:
                        // Set text color to cyan
                        TTY_Color = colors.vga.cyan;
                        break;
                    case 37:
                        // Set text color to white
                        TTY_Color = colors.vga.lgray;
                        break;
                    case 40:
                        // Set background color to black
                        TTY_TextBackground = colors.vga.black;
                        break;
                    case 41:
                        // Set background color to red
                        TTY_TextBackground = colors.vga.red;
                        break;
                    case 42:
                        // Set background color to green
                        TTY_TextBackground = colors.vga.green;
                        break;
                    case 43:
                        // Set background color to yellow
                        TTY_TextBackground = colors.vga.yellow;
                        break;
                    case 44:
                        // Set background color to blue
                        TTY_TextBackground = colors.vga.blue;
                        break;
                    case 45:
                        // Set background color to magenta
                        TTY_TextBackground = colors.vga.purple;
                        break;
                    case 46:
                        // Set background color to cyan
                        TTY_TextBackground = colors.vga.cyan;
                        break;
                    case 47:
                        // Set background color to white
                        TTY_TextBackground = colors.vga.white;
                        break;
                    default:
                        // Unsupported color code, do nothing
                        break;
                }
            }
        }
    }
    if (escapeCompleted) {
        escapeCompleted = false;
        escapeSequenceLength = 0;
        return true;
    }
    return false;    
}

#pragma GCC diagnostic pop