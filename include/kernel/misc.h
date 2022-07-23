#define BREAK \
asm volatile (\
	"xchgw %bx, %bx\n"\
);\
