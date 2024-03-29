#!/usr/bin/python3
# shamelessly stolen with permission from https://github.com/mine-man3000/SeshOS/blob/main/tools/icontest
from sys import argv
from datetime import date
from os import path
from time import ctime
from cv2 import imread
from cv2 import IMREAD_UNCHANGED

today = date.today()

dateRightFormat = str(today).split("-")

day   = dateRightFormat[2]
month = dateRightFormat[1]
year  = dateRightFormat[0]

dateRightFormat[0] = month
dateRightFormat[1] = day
dateRightFormat[2] = year

newToday = "/".join(dateRightFormat)

finalDate = newToday[1]

if len(argv) < 3:
	print("Usage: iconconv <bitmap file> <output name>")
	raise TypeError("Usage: iconconv <bitmap file> <output name>")

bmp = argv[1]
output = argv[2]
outputFile = argv[2]

modTime = path.getmtime(bmp)
modDate = ctime(modTime)

image = imread(bmp, IMREAD_UNCHANGED)

tmp = output.split("/")
array = tmp[-1].split(".")

newPicture = array[0]
print(newPicture)
width = 0
height = 0
width = image.shape[1]
height = image.shape[0]
channel = image.shape[2]

test = modDate.split(" ")

newMonth = test[1]
newDay   = test[2]
newYear  = test[4]

IconLastModDate = ["", "", ""]

IconLastModDate[0] = newMonth
IconLastModDate[1] = newDay
IconLastModDate[2] = newYear

IconLastModDateFull = "/".join(IconLastModDate)
split = output.split(".")


print(tmp)

f = open(output, "wb")
# f.write(
# 	"/*****************************************\n"
# 	"                TechflashOS\n"
# 	"\n"
# 	"\n"
# 	"  This is a converted icon to embed into\n"
# 	"            the kernel image.\n"
# 	" * Converted File: " + bmp + "\n"
# 	" * Converted Date: " + str(newToday) + "\n"
# 	" * Icon Last Mod: " + str(IconLastModDateFull) + "\n"
# 	"*****************************************/\n"
# 	"#include <stdint.h>\n"
# 	"\n"
# )

x = 0
y = 0

# f = open(output, "at")

# f.write(
# 	"uint32_t g_" + newPicture + "_data[] = {\n"
# 		f"\t{width}, {height}, // width, height\n"
# 		 "\t/* data */ "
# )
widthList = []
heightList = []
def byte(number, i):
    return (number & (0xff << (i * 8))) >> (i * 8)

widthHi = byte(width, 1)
widthLo = byte(width, 0)
widthList.append(widthHi)
widthList.append(widthLo)
widthArr = bytearray(widthList)

heightHi = byte(width, 1)
heightLo = byte(width, 0)
heightList.append(widthHi)
heightList.append(widthLo)
heightArr = bytearray(widthList)

f.write(widthArr)
f.write(heightArr)
for y in range(height):
	for x in range(width):

		pixel = image[y, x]
		rgba = 0

		rgba |= (pixel[0] << 0)    #blue
		rgba |= (pixel[1] << 8)    #green
		rgba |= (pixel[2] << 16)   #red


		# toAdd = ["0x00", "", ", "]
		# toAdd[1] = hexWithout0x[1] 

		# newHex = "".join(toAdd)

		#if(pixel[2] == 0):
		#    newHex = "0xffffffff, "

		# if(newHex == "0x000, "):
			# newHex = "0x00000000, "
		f.write(rgba)
f.close()
