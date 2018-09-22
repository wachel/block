APP_STL := gnustl_static
APP_CPPFLAGS := -fexceptions -std=c++11 -Wno-deprecated

APP_OPTIM := release

OPT_CFLAGS := -O2 -fno-exceptions -fno-rtti
OPT_CPPFLAGS := $(OPT_CLFAGS)
APP_CFLAGS += $(OPT_CFLAGS)
APP_CPPFLAGS += $(OPT_CPPFLAGS)