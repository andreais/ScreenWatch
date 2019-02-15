##
## makefile fed in the abyss
## rudreais
##
##

CC		=	gcc

CFLAGS		+=	-I $(DIRINC)	\
			-Wall -Wextra -W	\
			-g

LDFLAGS		+=	-lbluetooth

DIRSRC		+=	src/

DIRINC		+=	include/

SRC		+=	$(DIRSRC)main.c

OBJ		=	$(SRC:.c=.o)

EXEC		=	watcher

all:		$(EXEC)

$(EXEC):	$(OBJ)
		$(CC) -o $(EXEC) $(OBJ) $(LDFLAGS)

clean:
		rm -f $(OBJ)

fclean:		clean
		rm -f $(EXEC)

re:		fclean all

.PHONY:		all clean fclean re
