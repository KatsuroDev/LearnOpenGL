#TODO Authoritive Server, Basic Server Browser (Hearthbeat to web server)? (maybe steam support)?
#
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

$(info $(SHELL))
ifeq ($(shell uname -s), Linux)
TARGET_EXEC ?= a.out
endif
ifeq ($(OS),Windows_NT)
TARGET_EXEC ?= a.exe
endif
ifeq ($(UNAME_S), Darwin) # OS X
TARGET_EXEC ?= a.out
endif

BUILD_DIR ?= "build"
SRC_DIR ?= "src"

#( V)
#( ..)
#c(")(")
#Bunny

SRCS := $(shell find $(SRC_DIR) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

ifeq ($(shell uname -s), Linux)
CPPFLAGS ?= $(INC_FLAGS)  -std=c++14 -MMD -MP -g -Wall -Wextra -O3 -fpermissive #-Werror
endif
ifeq ($(OS),Windows_NT)
CPPFLAGS ?= $(INC_FLAGS) -g -std=c++14 -std=gnu++14 -MMD -MP -g -Wall -Wextra -O3 -fpermissive -static  -Wunused-parameter -Wstrict-aliasing -Wl,-subsystem,windows #-Werror
endif
ifeq ($(UNAME_S), Darwin)
CPPFLAGS ?= $(INC_FLAGS) -std=c++14 -MMD -MP -g -Wall -Wextra -03 -fpermissive
endif
# -g to debug  (gdb *.exe)

 #LINKER_FLAGS specifies the libraries we're linking against -L/usr/lib/nvidia-375/ -I./inc/ -w -lGLEW -lGL -lSLD2
ifeq ($(shell uname -s), Linux)
LINKER_FLAGS = -I./inc/ -L./lib/Linux/ -lGLEW -lGL -lglfw
endif
ifeq ($(OS),Windows_NT)
LINKER_FLAGS = -I"./inc/" -L"lib/Windows" -static  -std=c++14 -std=gnu++14 -Wunused-parameter -Wstrict-aliasing -lmingw32 -lglew32 -lglfw3 -lopengl32 -lglu32 -lgdi32 -dl #-I"./inc/steam/" -lsteam_api #-llua53 -dl
endif
ifeq ($(UNAME_S), Darwin)
LINKER_FLAGS = -I"./inc/"" -L"./lib/Mac" -lGLEW -lGL -lglfw
endif

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	g++ $(OBJS) -o $@ $(LDFLAGS) $(LINKER_FLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) $(LINKER_FLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	g++ -std=c++0x $(CPPFLAGS) $(CFLAGS) $(LINKER_FLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	g++ $(CPPFLAGS) $(CXXFLAGS) $(LINKER_FLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)/$(SRC_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
