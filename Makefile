
test:
	clib install --dev
	@$(CC) test.c src/cstring_array.c deps/char_array/char_array.c deps/utf8/utf8.c deps/utf8proc/utf8proc.c -std=c99 -I src -I deps -o $@
	@./$@

.PHONY: test
