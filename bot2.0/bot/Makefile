NAME = bot

CXX = g++

CPPFLAGS = -Wall -Werror -Wextra

SRCS = main \
	bot/bot \
	bot/commands \
	parser/parser \
	parser/config \
	parser/specs \
	log/log

OBJDIR = objects

OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(SRCS)))

$(OBJDIR)/%.o: srcs/%.cpp
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJDIR)/bot
	@mkdir -p $(OBJDIR)/helper
	@mkdir -p $(OBJDIR)/log
	@mkdir -p $(OBJDIR)/parser
	@mkdir -p $(OBJDIR)/commands
	$(CXX) $(CPPFLAGS) -c $< -o $@

all : $(NAME)

$(NAME) : $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean :
	rm -rf $(OBJDIR)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re