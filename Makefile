CFLAGS		+= -O2 -Wall -Werror -Wextra
CFLAGS		+= -I inc/ -I libft/inc/
LDFLAGS		+= -O2 -Wall -Werror -Wextra -L libft/ -lft
NM			= ft_nm
NM_FILES	= helpers.c main.c nm_print.c nm_segments.c sort.c sym_array.c symbol.c
OTOOL		= ft_otool
OTOOL_FILES	= main.c otool_print.c otool_segments.c otool_sections.c
SHAREDFILES	= archive.c fat.c fat_cpu.c fat_file.c lcmd.c is.c loader.c msg.c sizeof.c swap.c swapx.c
LIBFT		= libft/libft.a
SRC_DIR		= src/
OBJ_DIR		= obj/
SHARED_OBJS	= $(addprefix $(OBJ_DIR), $(SHAREDFILES:.c=.o))
OBJFOLDERS	:= $(OBJ_DIR) $(OBJ_DIR)$(NM)/ $(OBJ_DIR)$(OTOOL)
NM_OBJS		= $(addprefix $(OBJ_DIR)$(NM)/, $(NM_FILES:.c=.o))
OTOOL_OBJS	= $(addprefix $(OBJ_DIR)$(OTOOL)/, $(OTOOL_FILES:.c=.o))


all: $(NM) $(OTOOL)

$(NM): $(NM_OBJS) $(SHARED_OBJS) | $(LIBFT)
	$(CC) $(LDFLAGS) -o $@ $^

$(OTOOL): $(OTOOL_OBJS) $(SHARED_OBJS) | $(LIBFT)
	$(CC) $(LDFLAGS) -o $@ $^

$(NM_OBJS): $(OBJ_DIR)$(NM)/%.o : $(SRC_DIR)$(NM)/%.c | $(OBJ_DIR)$(NM)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OTOOL_OBJS): $(OBJ_DIR)$(OTOOL)/%.o : $(SRC_DIR)$(OTOOL)/%.c | $(OBJ_DIR)$(OTOOL)
	$(CC) $(CFLAGS) -c -o $@ $<

$(SHARED_OBJS): $(OBJ_DIR)%.o : $(SRC_DIR)%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIBFT):
	make -C libft/

$(OBJ_DIR)$(NM): | $(OBJ_DIR)
	mkdir -p $@

$(OBJ_DIR)$(OTOOL): | $(OBJ_DIR)
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf libft/obj/
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NM) $(OTOOL) $(LIBFT)

re: fclean all


again:
	rm -rf $(OBJ_DIR)
	make all

norminette:
	norminette inc/ src/ libft/inc/ libft/src/

norm: norminette

samples:
	git clone https://github.com/JonathanSalwan/binary-samples
