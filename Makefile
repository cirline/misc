CC	:= cc
CFLAGS	+= -lctutils

objs	:=
objs	+= main.o
objs	+= fsloop.o


target	:= fsdeamon.out

$(target):$(objs)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY:clean

clean:
	rm -rf *.o $(target)

