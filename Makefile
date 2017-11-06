all: mymod.so

%.so: %.o
	gcc -shared $(LDFLAGS) -o $@ $<

%.o: %.c
	gcc -std=gnu99 -ggdb3 -Wall -fPIC -I/mnt/c/home/src/github.com/emacs-mirror/emacs/src -c $<
