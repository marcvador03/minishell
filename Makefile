#Directory definition
LIBFT_DIR := libft
SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include
LIB_DIR := lib
BUILT_INS := $(SRC_DIR)/built_ins

#Filenames definition
NAME := minishell

BUILTINS = ft_cd.c \
		   ft_echo.c \
		   ft_env.c \
		   ft_export.c \
		   ft_pwd.c \
		   built_utils.c \
		   #ft_unset.c \

PARSE = prompt_creation.c \
		tokens.c \
		tokens_utils.c \
		pipes.c \
		quotes.c \
		redirections.c \
		redirections_utils.c \
		heredoc.c

EXECUTE = commands.c

UTILS = core_utils.c \
		free_utils.c \
		list_utils.c \
		str_utils.c

MAIN = minishell.c \
	   shell.c \
	   subshell.c \
	   subshell_utils.c \
	   signals.c


SRC_NAMES =  $(addprefix $(SRC_DIR)/parse/, $(PARSE)) \
			 $(addprefix $(SRC_DIR)/execute/, $(EXECUTE)) \
			 $(addprefix $(SRC_DIR)/utils/, $(UTILS)) \
			 $(addprefix $(SRC_DIR)/main/, $(MAIN)) \
			$(addprefix $(SRC_DIR)/built_ins/, $(BUILTINS))
INC_NAMES := minishell.h

#SOURCES := $((SRC_NAMES): %.c=$(SRC_DIR)/%.c)

#OBJECTS := $(SRC_NAMES:.c=.o)
OBJECTS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRC_NAMES)))

INCLUDES := $(patsubst %.h, $(INC_DIR)/%.h, $(INC_NAMES))

DEPS := $(OBJECTS:.o=.d)

CFLAGS += -MMD -MP -I $(INC_DIR)

LIB_NAMES := libft.a 
LIBS_TAG := $(patsubst lib%.a, -l%, $(LIB_NAMES))
LIBS_TAG += -lreadline -ltermcap

DEBUG ?=

CUR_DIR := $(shell pwd)

#TARGETS
all: libft $(OBJECTS) $(NAME) 

$(NAME): libft/libft.a Makefile $(INCLUDES) $(OBJECTS)
	cc $(CFLAGS) -L libft $(DEBUG) -g $(OBJECTS) -o $@ $(LIBS_TAG) $(LIBS_TAG)

$(OBJ_DIR)/%.o: $(SRC_DIR)/*/%.c | $(OBJ_DIR)
	cc $(CFLAGS) $(DEBUG) -g -c $< -o $@ 

libft: 
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

#$(LIB_DIR):
#	@mkdir $(LIB_DIR)

#$(INC_DIR):
#	@mkdir $(INC_DIR)

flags:
	@echo $(CFLAGS)

show:
	@echo $(OBJECTS)


clean: 
	@$(MAKE) clean -C $(LIBFT_DIR)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@rm -rf $(LIB_DIR)/*

re: fclean all
ifneq ($(DEPS), )
-include $(DEPS)
endif
.PHONY: all flags clean fclean re show libft bonus
