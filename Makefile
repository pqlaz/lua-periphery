LIB = periphery.so
SRCS = src/lua_periphery.c
OPTIONS =
ifndef NOMMIO
SRCS += src/lua_mmio.c
OPTIONS += -DMMIO
endif
ifndef NOGPIO
SRCS += src/lua_gpio.c
OPTIONS += -DGPIO
endif
ifndef NOSPI
SRCS += src/lua_spi.c
OPTIONS += -DSPI
endif
ifndef NOI2C
SRCS += src/lua_i2c.c
OPTIONS += -DI2C
endif
ifndef NOSERIAL
SRCS += src/lua_serial.c
OPTIONS += -DSERIAL
endif
ifndef NOSLEEP
OPTIONS += -DSLEEP
endif

C_PERIPHERY = c-periphery
C_PERIPHERY_LIB = $(C_PERIPHERY)/periphery.a

ifndef LUA
LUA = lua
endif

ifndef LUA_INCDIR
LUA_CFLAGS = $(shell pkg-config --cflags $(LUA))
else
LUA_CFLAGS = -I$(LUA_INCDIR)
endif

###########################################################################

MOD_CFLAGS = $(CFLAGS)
MOD_CFLAGS += -std=c99 -pedantic -D_XOPEN_SOURCE=700
MOD_CFLAGS += -Wall -Wextra -Wno-unused-parameter $(DEBUG) -fPIC -I. $(LUA_CFLAGS)

MOD_LDFLAGS = $(LDFLAGS)
MOD_LDFLAGS += -shared

###########################################################################

.PHONY: all
all: $(LIB)

.PHONY: clean
clean:
	cd $(C_PERIPHERY) && $(MAKE) clean
	rm -rf $(LIB)

.PHONY: install
install:
	mkdir -p $(LUA_LIBDIR)
	cp $(LIB) $(LUA_LIBDIR)/$(LIB)

###########################################################################

$(LIB): $(C_PERIPHERY_LIB) $(SRCS)
	$(CC) $(OPTIONS) $(MOD_CFLAGS) $(MOD_LDFLAGS) $(SRCS) $(C_PERIPHERY_LIB) -o $@

$(C_PERIPHERY_LIB): $(C_PERIPHERY)/Makefile
	cd $(C_PERIPHERY); $(MAKE)

