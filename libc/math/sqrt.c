float sqrt(int num) {
	float tmp, root;

	root = (float)(num / 2);
	tmp = 0;

	while (root != tmp) {
		tmp = root;
		root = ((float)num / tmp + tmp) / 2;
	}
	return root;
}
