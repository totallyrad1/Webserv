SRC = srcs/config/Config.cpp srcs/request/Request.cpp srcs/response/ResponseHeader.cpp srcs/server/Webserver.cpp  srcs/server/ss_request.cpp srcs/server/Client.cpp srcs/utils/files.cpp \
	srcs/autoIdx/AutoIndex.cpp srcs/main.cpp srcs/response/ResMembers.cpp srcs/route/Route.cpp srcs/server/init_sockets.cpp srcs/utils/errors.cpp srcs/utils/strings.cpp \
	srcs/cgi/CgiHandler.cpp srcs/request/ReqMembers.cpp srcs/response/Response.cpp srcs/server/Server.cpp srcs/server/parsing.cpp srcs/utils/extension.cpp srcs/utils/time.cpp
OBJ = ${SRC:.cpp=.o}
INC = inc/AutoIndex.hpp inc/CgiHandler.hpp inc/Client.hpp inc/Config.hpp inc/Request.hpp inc/Response.hpp inc/ResponseHeader.hpp inc/Route.hpp inc/Server.hpp inc/WebServer.hpp inc/Webserv.hpp inc/ss_request.hpp inc/utils.hpp
CC = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -I./inc -g -D DEBUG=1 #-fsanitize=address
NAME = webserv

all : $(NAME)
	@echo "Webserv compiled"

%.o : %.cpp $(INC)
	@echo "Compiling $<"
	@$(CC) $(CPPFLAGS) -c $< -o $@

$(NAME) : $(OBJ)
	@echo "Linking $@"
	@$(CC) $(CPPFLAGS) -o $@ $^

clean :
	@echo "Cleaning"
	@rm -f $(OBJ)

fclean : clean
	@echo "Full cleaning"
	@rm -f $(NAME)

re : clean all
	@echo "Recompiled"

.PHONY : all re clean fclean