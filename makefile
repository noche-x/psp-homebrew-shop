TARGET = psp-homebrew-shop
OBJS = 	source/main.o \
 		source/state_manager.o \
 		source/app_logic.o \
 		source/global_variables.o \
 		source/states/loading_state.o \
 		source/states/splash_state.o \
 		source/states/shop_state.o \

INCDIR = $(PSPPATH)/include lib/Stardust-Engine/Stardust/include lib/Stardust-Engine/Deps/include

CFLAGS = -O3 -G0 -mpreferred-stack-boundary=4 -ffast-math -fno-rtti -fno-exceptions -DCURRENT_PLATFORM=PLATFORM_PSP
CXXFLAGS = -std=gnu++17
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PSP_FW_VERSION=500
PSP_LARGE_MEMORY = 0

LDFLAGS =
# -losl -lmmio -lpng

LIBDIR = lib/Stardust-Engine/lib lib/Stardust-Engine/Stardust/lib/psp lib/Stardust-Engine/lib lib/Stardust-Engine/Deps/lib/psp
LIBS    = -lstardust -ljson -lintraFont -lpng -lpspmath -lpspwlan -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lpspusb -lpspusbstor -lpspmp3 -lpspreg -lpspumd -lpsphprm -lpspaudiolib -lpspaudio -lpspaudiocodec -lpng -lz -lpspgum -lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpspsdk -lpsppower -lpsphttp -lpspssl
# LIBS = -lpspaudiolib -lpspaudio -lmikmod -lpspaudiocodec -lpng -lz -lpspgum -lpspgu -lstdc++ -lpsprtc -lpspctrl -lm -lpspvfpu -lpspsdk -lpsppower -lpsphttp

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_ICON= ICON0.png
PSP_EBOOT_TITLE = PSP-Homebrew-Shop

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
