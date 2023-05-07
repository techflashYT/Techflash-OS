/*
 * This is bin2c program, which allows you to convert binary file to
 * C language array, for use as embedded resource, for instance you can
 * embed graphics or audio file directly into your program.
 * This is public domain software, use it on your own risk.
 * Contact Serge Fukanchik at fuxx@mail.ru  if you have any questions.
 *
 * Some modifications were made by Gwilym Kuiper (kuiper.gwilym@gmail.com)
 * I have decided not to change the licence.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *buf;
    char *ident;
    unsigned int i, fileSize, needComma;

    FILE *f_input, *f_output;

    if (argc < 4) {
        fprintf(stderr, "Usage: %s binary_file output_file array_name\n", argv[0]);
        return -1;
    }

    f_input = fopen(argv[1], "rb");
    if (f_input == NULL) {
        fprintf(stderr, "%s: can't open %s for reading\n", argv[0], argv[1]);
        return -1;
    }

    // Get the file length
    fseek(f_input, 0, SEEK_END);
    fileSize = ftell(f_input);
    fseek(f_input, 0, SEEK_SET);

    buf = (char *) malloc(fileSize);
    assert(buf);

    fread(buf, fileSize, 1, f_input);
    fclose(f_input);

    f_output = fopen(argv[2], "w");
    if (f_output == NULL) {
        fprintf(stderr, "%s: can't open %s for writing\n", argv[0], argv[2]);
        return -1;
    }

    ident = argv[3];

    needComma = 0;

    fprintf(f_output, "const char %s[%i] = {", ident, fileSize);
    for (i = 0; i < fileSize; ++i) {
        if (needComma) {
            fprintf(f_output, ", ");
		}
        else
            needComma = 1;
        if ((i % 11) == 0) {
            fprintf(f_output, "\n\t");
		}
        fprintf(f_output, "0x%.2x", buf[i] & 0xff);
    }
    fprintf(f_output, "\n};\n\n");

    fprintf(f_output, "const int %s_length = %i;\n", ident, fileSize);

    fclose(f_output);

    free(buf);

    return 0;
}
