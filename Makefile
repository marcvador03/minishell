#Directory definition
LIBFT_DIR := libft
SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include
LIB_DIR := lib

#Filenames definition
NAME := minishell

SRC_NAMES := minishell.c

INC_NAMES := minishell.h

SOURCES := $(patsubst %.c, $(SRC_DIR)/%.c, $(SRC_NAMES))

OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

INCLUDES := $(patsubst %.h, $(INC_DIR)/%.h, $(INC_NAMES))

DEPS := $(OBJECTS:.o=.d)

CFLAGS += -Wall -Werror -Wextra -MMD -MP

LIB_NAMES := libft.a 
LIBS_TAG := $(patsubst lib%.a, -l%, $(LIB_NAMES)) 

DEBUG ?=

CUR_DIR := $(shell pwd)

#TARGETS
all: libft $(OBJECTS) $(NAME) 

$(NAME): libft/libft.a Makefile $(INCLUDES) $(OBJECTS)
	cc $(CFLAGS) -L libft $(DEBUG) $(OBJECTS) -o $@ $(LIBS_TAG) $(LIBS_TAG)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	cc $(CFLAGS) $(DEBUG) -c $< -o $@ 

libft: | $(LIB_DIR) $(INC_DIR) 
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(LIB_DIR):
	@mkdir $(LIB_DIR)

$(INC_DIR):
	@mkdir $(INC_DIR)

flags:
	@echo $(CFLAGS)

show:
	@echo $(SOURCES)

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
