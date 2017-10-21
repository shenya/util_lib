SRC=main.c king_mysql.c

LIB=-lmysqlclient

all:
	gcc -o a.out $(SRC) $(LIB)
