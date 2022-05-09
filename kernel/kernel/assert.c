void assert(const char *message, const char *file, int line) {
	printf("ASSERT: File: %s, Line: %d, Message:\r\n %s\r\n", file, line, message);
}
