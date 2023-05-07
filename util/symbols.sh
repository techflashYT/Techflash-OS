#!/bin/sh
# build symbols
echo "Generating kernel symbols file..."
if [ ! -f "bin/genSyms" ] || [ "$(find bin/genSyms -newer util/symToFile.c)" ]; then
	gcc -Ofast -march=native -mtune=native -fsanitize=address,undefined symToFile.c -o bin/genSyms
fi
bin/genSyms
if [ ! -f "bin/bin2c" ] || [ "$(find bin/bin2c -newer file2)" ]; then
	gcc -Ofast -march=native -mtune=native -fsanitize=address,undefined symToFile.c -o bin/genSyms
fi
bin/bin2c -o build/symbols.c build/symbols.bin
gcc build/symbols.c -c -o build/symbols.o
