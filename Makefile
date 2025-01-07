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
		   ft_exit.c \
		   ft_export.c \
		   ft_pwd.c \
		   ft_unset.c \
		   built_utils.c

PARSE =	args.c \
		brackets.c \
		env_var.c \
		heredoc.c \
		pipes.c \
		quotes.c \
		quotes_utils.c \
		redirections.c \
		redirections_utils.c \
		tokens.c \
		tokens_utils.c

EXECUTE = exec.c \
		  pre_exec.c

UTILS = core_utils.c \
		core_utils2.c \
		free_utils.c \
		list_utils.c \
		list_utils2.c \
		str_utils.c

MAIN = environment.c \
	   environment_utils.c \
	   minishell.c \
	   shell.c \
	   shell_utils.c \
	   signals.c \
	   subshell.c \
	   subshell_utils.c \
	   termcaps.c


SRC_NAMES =  $(addprefix $(SRC_DIR)/parse/, $(PARSE)) \
			 $(addprefix $(SRC_DIR)/execute/, $(EXECUTE)) \
			 $(addprefix $(SRC_DIR)/utils/, $(UTILS)) \
			 $(addprefix $(SRC_DIR)/main/, $(MAIN)) \
			$(addprefix $(SRC_DIR)/built_ins/, $(BUILTINS))

INC_NAMES := minishell.h

OBJECTS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRC_NAMES)))

INCLUDES := $(patsubst %.h, $(INC_DIR)/%.h, $(INC_NAMES))

DEPS := $(OBJECTS:.o=.d)

CFLAGS += -Wall -Werror -Wextra -g -MMD -MP -I $(INC_DIR)

LIB_NAMES := libft.a 
LIBS_TAG := $(patsubst lib%.a, -l%, $(LIB_NAMES))
LIBS_TAG += -lreadline -ltermcap

DEBUG ?=

CUR_DIR := $(shell pwd)

#TARGETS
all: libft $(OBJECTS) $(NAME) 

$(NAME): libft/libft.a Makefile $(INCLUDES) $(OBJECTS)
	cc $(CFLAGS) -L libft $(DEBUG) $(OBJECTS) -o $@ $(LIBS_TAG) $(LIBS_TAG)

$(OBJ_DIR)/%.o: $(SRC_DIR)/parse/%.c Makefile | $(OBJ_DIR)
	cc $(CFLAGS) $(DEBUG) -c $< -o $@ 
$(OBJ_DIR)/%.o: $(SRC_DIR)/execute/%.c Makefile | $(OBJ_DIR)
	cc $(CFLAGS) $(DEBUG) -c $< -o $@ 
$(OBJ_DIR)/%.o: $(SRC_DIR)/utils/%.c Makefile | $(OBJ_DIR)
	cc $(CFLAGS) $(DEBUG) -c $< -o $@ 
$(OBJ_DIR)/%.o: $(SRC_DIR)/main/%.c Makefile | $(OBJ_DIR)
	cc $(CFLAGS) $(DEBUG) -c $< -o $@ 
$(OBJ_DIR)/%.o: $(SRC_DIR)/built_ins/%.c Makefile | $(OBJ_DIR)
	cc $(CFLAGS) $(DEBUG) -c $< -o $@ 

libft: 
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

flags:
	@echo $(CFLAGS)

show:
	@echo $(OBJECTS)
	@echo $(SRC_NAMES)


clean: 
	@$(MAKE) clean -C $(LIBFT_DIR)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all
ifneq ($(DEPS), )
-include $(DEPS)
endif
.PHONY: all flags clean fclean re show libft bonus
