NAME    		= philo_bonus
CURRENT_PATH	= .

SRCDIR 		= ./src
SRC_PATHS	= $(CURRENT_PATH)
SRCS		= destroy.c init.c main.c monitor.c ph_grab_forks.c philo.c philo_getset.c philo_utils.c unwrap.c utils.c

VPATH		= $(CURRENT_PATH)

OBJDIR  	= ./obj
OBJS    	= $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))

CC      	= gcc -pthread
CFLAGS   	= -Wall -Wextra -Werror
#CFLAGS   	= -Wall -Wextra -Werror -fsanitize=thread
#CFLAGS   	= -fsanitize=address

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDE) $(LIB)

$(OBJDIR)/%.o: %.c
	@if [ ! -d $(dir $@) ];then mkdir $(dir $@); fi
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME) $(CHECKER)

re: fclean all

run:
	./$(NAME)

norm:
	norminette $(SRC_PATHS) | grep -v Header

bonus: $(NAME)

.PHONY: all bonus clean fclean re bonus