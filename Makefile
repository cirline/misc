CC	:= cc
CFLAGS	+= -shared -fPIC

objs	:=
objs	+= log.o
objs	+= ustring.o

target	:= ctutils

target_lib	:= lib$(target).so

.PHONY:all
all: $(target_lib)
	make install
	make test.out

$(target_lib):$(objs)
	$(CC) -o $@ $^ $(CFLAGS)

test.out:test.c
	$(CC) -o $@ $^ -l$(target)

.PHONY:clean

clean_objs	:=
clean_objs	+= *.o *.so *.out
clean_objs	+= *.tmp.log
clean_objs	+= *.orig

clean:
	rm -rf $(clean_objs)

.PHONY:install

install:
	sudo cp $(target_lib) /lib/
