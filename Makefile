SRC=main.c
LIB_SRC=king_mysql.c
LINK_LIB= -lmysqlclient -lking_mysql

all:
	gcc -o a.out $(SRC) -L. $(LINK_LIB)

lib:
	gcc -o libking_mysql.so $(LIB_SRC) -fPIC -shared
install:
	mkdir -p /usr/local/include/cjson
	cp -a king_mysql.h /usr/local/include/cjson
	cp -a libking_mysql.so /usr/local/lib
