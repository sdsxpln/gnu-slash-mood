APP=tests
SRC_FILES = $(filter-out ../src/gnu-slash-mood.c, $(wildcard ../src/*.c))

all:
	gcc -Wall -std=gnu99 -pedantic -O3 -o ${APP} ${SRC_FILES} *.c -I/usr/local/include/libbbb -I/usr/local/include/libgnusocial -lm -lcurl -lbbb -lgnusocial
debug:
	gcc -Wall -std=gnu99 -pedantic -g -o ${APP} ${SRC_FILES} *.c -Isrc -I/usr/local/include/libbbb -I/usr/local/include/libgnusocial -lm -lcurl -lbbb -lgnusocial
clean:
	rm -f ${APP} \#* \.#* *.plist
