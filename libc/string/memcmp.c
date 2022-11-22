int memcmp(const void *s1, const void *s2, int len) {
	const unsigned char *p = s1;
	const unsigned char *q = s2;
	int charCompareStatus = 0;
	// If both pointer pointing same memory block
	if (s1 == s2) {
		return charCompareStatus;
	}
	while (len > 0) {
		if (*p != *q) {
			// compare the mismatching character
			charCompareStatus = (*p > *q) ? 1 : -1;
			break;
		}
		len--;
		p++;
		q++;
	}
	return charCompareStatus;
}