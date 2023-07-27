


config: bin/configure
	@bin/configure

bin/configure: util/configure.c
	@$(info CC    ==> $<)
	@$(HOSTCC) $(HOSTCLFAGS) $< -o $@