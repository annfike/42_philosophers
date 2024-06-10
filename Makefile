NAME			=	philo

SRCS			=	main.c utils.c init.c process.c
				
OBJS = ${SRCS:.c=.o}

CC = gcc

#CFLAGS = -Wall -Wextra -Werror -fsanitize=thread
CFLAGS = -Wall -Wextra -Werror -pthread

RM = rm -f

all: ${NAME}

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(NAME)


clean:
	${RM} ${OBJS} 

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re