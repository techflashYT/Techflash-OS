#include <stdint.h>
typedef struct {
	uint16_t serialPortAddrs[4];
	uint16_t parallelPortAddrs[3];
	// NOTE: this is bit shifted to the right by 4 *usually*
	uint16_t ebdaAddr;
	/*
	Flags:
		Bit  0:     is FDD available for booting?   0 = no; 1 = yes
		Bit  1:     is there an FPU present?        0 = no; 1 = yes
		Bit  2:     is there a mouse present?       0 = no; 1 = yes
		Bit  3:     Reserved
		Bits 4-5:   Init video mode                00 = EGA, VGA, PGA; 01 = Text mode 40x25 color; 10 = 80x25 color; 11 = 80x25 mono
		Bits 6-7:   Number of FDDs - 1             00 = 1 floppy drive; 01 = 2 floppy drives; 10 = 3 floppy drives; 11 = 4 floppy drives
		Bit  8:     Reserved
		Bits 9-11:  Number of serial devices
		Bit  12:    Reserved
		Bit  13:    Do we have an internal modem?   0 = no; 1 = yes
		Bits 14-15: Number of parallel devices
	*/
	uint16_t hwFlags;
	uint8_t  postStatus;
	// should always be 640, since it's EXTREMELY likely that we have more than 640kb of ram
	uint16_t lowMemorySize;
	uint8_t  junk1[2];
	/*
	Flags:
		Byte 0, Bit 0: Right shift
		Byte 0, Bit 1: Left shift
		Byte 0, Bit 2: Left or Right Control
		Byte 0, Bit 3: Left or Right Alt
		Byte 0, Bit 4: Scroll Lock active
		Byte 0, Bit 5: Num Lock active
		Byte 0, Bit 6: Caps Lock active
		Byte 0, Bit 7: Insert active
		Byte 1, Bit 0: Left Control
		Byte 1, Bit 1: Left Alt
		Byte 1, Bit 2: SysRQ
		Byte 1, Bit 3: Pause Break
		Byte 1, Bit 4: Scroll Lock pressed
		Byte 1, Bit 5: Num Lock pressed
		Byte 1, Bit 6: Caps Lock pressed
		Byte 1, Bit 7: Insert pressed
	*/
	uint8_t  kbdStateFlags[2];
	uint8_t  somethingForTheKeyboard1;
	uint16_t somethingForTheKeyboard2;
	uint16_t somethingForTheKeyboard3;
	uint8_t  kbdBuffer[32];
	uint8_t  fddRecalibrateStatus;
	uint8_t  fddMotorStatus;
	uint8_t  fddMotorTurnOffTimeOutCount;
	// 0 is good
	uint8_t  fddLastOpStatus;
	uint8_t  fddCommandBytes[7];
	uint8_t  displayMode;
	uint8_t  vgaTextModeColumns;
	uint16_t vgaPageSizeBytes;
	uint16_t vgaCurrentPageStartAddr;
	// 1 byte for x, 1 byte for y, 8 times, 1 for each page
	uint8_t  vgaCursorPos[2][8];
	// [0] = end line, [1] = start line
	uint8_t  vgaCursorType[2];
	uint8_t  vgaCurrentPageNum;
	// color should be 0x3D4, mono should be 0x3B4
	uint16_t vgaCRTControllerAddress;
	uint8_t  vgaCurrentModeSelectSetting;
	uint8_t  vgaCurrentCGAPaletteSetting;
	uint32_t realModeReEntryPoint;
	uint8_t  lastUnexpectedInterruptInPOST;
	uint32_t timerTicksSinceMidnight;
	uint8_t  timerOverflow;
	uint8_t  ctrlBreakFlag;
	/*
	FLAGS:
		0x64:  Burn in mode?
		0x1234 Skip memory test
		0x4321 Preserve memory
		0x5678 System suspended?
		0x9ABC POST loop
	*/
	uint16_t POSTResetFlag;
	/*
	FLAGS:
		0x00: OK
		0x01: Invalid request
		0x02: Address mark not found
		0x03: Write protected
		0x04: Sector not found
		0x05: Reset failed
		0x07: Drive parameter activity failed
		0x08: DMA Overrun
		0x09: DMA data boundary error
		0x0A: Bad Sector
		0x0B: Bad Track
		0x0D: Invalid number of sectors for format
		0x0E: Control data address mark detected
		0x0F: DMA Arbitration out of range
		0x10: Unrecoverable ECC/CRC error
		0x11: ECC corrected data error
		0x20: General controller failed
		0x40: Seek failed
		0x80: Timed out
		0xAA: Drive not ready
		0xBB: Undefined
		0xCC: Write fault
		0xE0: Status Error / Error register is 0
		0xFF: Sense failed
	*/
	uint8_t  fixedDiskLastOpStatus;
	uint8_t  numberOfFixedDisks;
	uint8_t  fixedDiskControlByte;
	uint8_t  fixedDiskIOPortOffset;
	uint8_t  parallelDevicesTimeOut[4];
	uint8_t  serialDevicesTimeOut[4];
	uint16_t kbdBufferStartOffsetFromSeg40h;
	uint16_t kbdBufferEndOffsetFromSeg40h;
	uint8_t  vgaTextModeRowsMinusOne;
	uint16_t vgaCharacterHeightInScanlines;
	/*
	Flags:
		Bit 0:    Convert CGA-style cursor requests to EGA/VGA?  0 = yes; 1 = no
		Bit 1:    Color monitor?                                 0 = color; 1 = mono
		Bit 2:    Wait for display enable?                       idk
		Bit 3:    Is EGA/VGA system active                       0 = active; 1 = inactive
		Bit 4:    Reserved
		Bits 5-6: VRAM size / 64KB
		Bit 7:    Clear VRAM?                                    0 = yes; 1 = no
	*/
	uint8_t  vgaControl;
	uint8_t  vgaSwitches;
	uint8_t  vgaModeSet;
	uint8_t  vgaIndexToDCCTable;
	/*
	Flags:
		Bits 0-1: Reserved
		Bits 2-3: Data rate at start of operation
		Bits 4-5: Last fdd step rate
		Bits 6-7: Last data rate set by controller   00 = 500kbps; 01 = 300kbps; 10 = 250kbps; 11=reserved
	*/
	uint8_t  fddMediaControl;
	uint8_t  fixedDiskControllerStatus;
	uint8_t  fixedDiskControllerErrorStatus;
	uint8_t  fixedDiskInterruptControl;
	// NOTE: I've given up on writing info for them from here on, write info for them eventually.
	uint8_t  fddControllerInfo;
	uint8_t  fdd0MediaState;
	uint8_t  fdd1MediaState;
	uint8_t  fdd0MediaStateAtStartOfOp;
	uint8_t  fdd1MediaStateAtStartOfOp;
	uint8_t  fdd0CurrentTrackNum;
	uint8_t  fdd1CurrentTrackNum;
	// TODO: info for this
	uint8_t  kbdStatusBytes[2];
	uint32_t timer2WaitCompleteFlagPtr;
	uint32_t timer2WaitCountMS;
	uint8_t  timer2WaitActiveFlag;
	uint8_t  reservedForNetworkAdapters[7];
	uint32_t fixedDiskInterruptVector;
	uint32_t vgaPtrToVideoSavePtrTable;
	uint8_t  reserved1[2];
	uint32_t ptrTo3363ODDEntryPoint;
	uint8_t  reserved2[2];
	uint8_t  reservedForPOST[3];
	uint8_t  unknown1[7];
	uint8_t  reserved3[14];
	uint16_t maybeDaysSinceLastBoot;
	uint8_t  reserved[31];
	uint8_t  reservedForUser[15];
	uint8_t  printScreenStatus;
} __attribute__((packed)) bda_t;

extern bda_t *bda;