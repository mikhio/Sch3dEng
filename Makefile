LDFLAGS := -L/opt/local/lib -lglfw -lglew -framework OpenGL
INFLAGS := -g -std=c++11
WARNING := -Wall

SRCPATH := ./src
APPPATH := ./build/app
CCHPATH := ./cpp_cache

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SOURCES := $(call rwildcard,$(SRCPATH),*.cpp)
OBJECTS := $(addprefix $(CCHPATH)/, $(patsubst %.cpp, %.o, $(SOURCES)))
DEPENDS := $(addprefix $(CCHPATH)/, $(patsubst %.cpp, %.d, $(SOURCES)))

.PHONY: all build

# test:
# 	echo $(OBJECTS)

all:
	clear
	make build
	$(APPPATH)

build: $(APPPATH)

clean:
	$(RM) $(OBJECTS) $(DEPENDS)

# Linking the executable from the object files
$(APPPATH): $(OBJECTS)
	$(CXX) $(WARNING) $(LDFLAGS) $(INFLAGS) $(CXXFLAGS) $^ -o $(APPPATH)

-include $(DEPENDS)

$(CCHPATH)/$(SRCPATH)/%.o: $(SRCPATH)/%.cpp Makefile
	$(CXX) $(WARNING) $(INFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@
