NAME    		= philo
CURRENT_PATH	= .

SRCDIR 		= ./src
SRC_PATHS		= $(CURRENT_PATH)
SRCS			= $(foreach path, $(SRC_PATHS), $(wildcard $(path)/*.c))

VPATH		= $(CURRENT_PATH)

OBJDIR  	= ./obj
OBJS    	= $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))

CC      	= gcc
#CFLAGS   	= -Wall -Wextra -Werror -fsanitize=thread
#CFLAGS   	= -fsanitize=thread
CFLAGS   	= -fsanitize=address

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
	norminette $(SRC_PATHS) | grep Error

.PHONY: all bonus clean fclean re