CXX = g++

SRC_DIR =  src

OBJ_DIR =  objs

DEPS_DIR = deps

OUT_DIR = out

SRC  = $(wildcard $(SRC_DIR)/*.cpp)

OBJS = $(addprefix $(OBJ_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SRC))))

DEPS =  $(addprefix $(DEPS_DIR)/,$(patsubst %.cpp,%.d,$(notdir $(SRC))))

BOOST_INCLUDE_DIR = /home/mjf/lib/include

BOOST_LIB_DIR = /home/mjf/lib/lib

LIBS =  -lpthread  -lboost_system -lboost_thread  -lpthread

HEADER_DIR =  -I ./include   -I/usr/include/mysql  -I$(BOOST_INCLUDE_DIR)

CXXFLAG = $(HEADER_DIR)  -o2  -g  -Wall  -std=c++11

SQL_DEPENDS =  `mysql_config  --cflags  --libs`

TARGET = myserver

$(TARGET):$(OBJS)
	$(CXX)   $^  -g $(SQL_DEPENDS)  -o $(TARGET) $(LIBS)  -L$(BOOST_LIB_DIR)


#@在makefile里面表达的是解析shell命令 fi表示else吧
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	@if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi; \
	$(CXX)  -c  -g $(HEADER_DIR)  $(SQL_DEPENDS)  $(LIBS)  -o   $@   $<



$(DEPS_DIR)/%.d:$(SRC_DIR)/%.cpp
	@if [ ! -d $(DEFS_DIR) ]; then mkdir -p $(DEFS_DIR); fi; \
	set  -e;rm -f $@;\
	$(CXX)  -MM  $(CXXFLAG)   $<  > $@.$$$$ ;\
	sed  's,\($(notdir$*)\)\.o[ :]*,$(OBJ_DIR$*)/\1.o$@ : ,g' <  $@.$$$$  >  $@;\
	rm -f $@.$$$$

-include $(DEPS)

$(OBJS):
.PHONY:clean
clean:
	rm -f $(TARGET)
	rm -f $(OBJ_DIR)/*
