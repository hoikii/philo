NAME		= philo
HEADER		= philo.h
SRCS		= main.c init.c simulate.c utils.c
OBJS		= $(SRCS:.c=.o)
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
LDLIBS		= -lpthread
RM			= rm -rf

CCBLUE		= \033[34m
CCBLUE_BOLD	= \033[1;34m
CCEND		= \033[0m

.PHONY: all clean fclean re test

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDLIBS) -o $(NAME)
	@echo "$(CCBLUE_BOLD) >>> make $(NAME) done!  <<< $(CCEND)"

%.o: %.c $(HEADER)
	$(CC) -I $(HEADERS) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(CCBLUE) >>> clean main projects object files. <<< $(CCEND)"
	$(RM) $(OBJS)

fclean: clean
	@echo "$(CCBLUE) >>> clean main project's executable. <<< $(CCEND)"
	$(RM) $(NAME)

re: fclean all

test: CFLAGS += -g3 -fsanitize=address
test: clean all

