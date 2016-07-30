TARGET	= libsins.so

CFLAGS	= -I include -shared -fPIC

libsins.so: base/rbtree.c
	gcc $(CFLAGS) $^ -o $@
