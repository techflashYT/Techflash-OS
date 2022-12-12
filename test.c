/*asm("\
.global _start\r\n\
.type _start, @function\r\n\
_start:\r\n\
	mov $0x0, %rax\r\n\
	ret\r\n\
");
*/
struct {
	unsigned long long int test1;
	unsigned long long int test2;
	unsigned long long int test3;
	unsigned long long int test4;
	unsigned long long int test5;
	unsigned long long int test6;
	unsigned long long int test7;
	unsigned long long int test8;
	unsigned long long int test9;
	unsigned long long int testa;
} randomVariable;
int _start() {
		randomVariable.test1 = 0;
		randomVariable.test2 = 0;
		randomVariable.test3 = 0;
		randomVariable.test4 = 0;
		randomVariable.test5 = 0;
		randomVariable.test6 = 0;
		randomVariable.test7 = 0;
		randomVariable.test8 = 0;
		randomVariable.test9 = 0;
		randomVariable.testa = 0;
		return 0;
}
