CC	:= cc
CFLAGS	+= -shared -fPIC

objs	:=
objs	+= log.o

target	:= ctutils

target_lib	:= lib$(target).so

.PHONY:all
all: $(target_lib) test.out

$(target_lib):$(objs)
	$(CC) -o $@ $^ $(CFLAGS)

test.out:test.c
	$(CC) -o $@ $^ -l$(target)

.PHONY:clean

clean:
	rm -rf *.o *.so *.out *.tmp.log

.PHONY:install

install:
	sudo cp $(target_lib) /lib/
