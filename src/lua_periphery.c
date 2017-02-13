/*
 * lua-periphery by vsergeev
 * https://github.com/vsergeev/lua-periphery
 * License: MIT
 */

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <time.h>
#include <unistd.h>

#include "lua_periphery.h"
#include "lua_compat.h"

#ifdef GPIO
LUALIB_API int luaopen_periphery_gpio(lua_State *L);
#endif
#ifdef SPI
LUALIB_API int luaopen_periphery_spi(lua_State *L);
#endif
#ifdef MMIO
LUALIB_API int luaopen_periphery_mmio(lua_State *L);
#endif
#ifdef I2C
LUALIB_API int luaopen_periphery_i2c(lua_State *L);
#endif
#ifdef SERIAL
LUALIB_API int luaopen_periphery_serial(lua_State *L);
#endif

static int periphery_error_tostring(lua_State *L) {
    lua_getfield(L, -1, "message");
    return 1;
}

#ifdef SLEEP
static int periphery_sleep(lua_State *L) {
    unsigned int duration;

    duration = luaL_checkunsigned(L, 1);

    sleep(duration);

    return 0;
}

static int periphery_sleep_ms(lua_State *L) {
    unsigned int duration;
    struct timespec ts;

    duration = luaL_checkunsigned(L, 1);

    ts.tv_sec = duration / 1000;
    ts.tv_nsec = (duration - ts.tv_sec*1000)*1000000;
    nanosleep(&ts, NULL);

    return 0;
}

static int periphery_sleep_us(lua_State *L) {
    unsigned int duration;
    struct timespec ts;

    duration = luaL_checkunsigned(L, 1);

    ts.tv_sec = duration / 1000000;
    ts.tv_nsec = (duration - ts.tv_sec*1000000)*1000;
    nanosleep(&ts, NULL);

    return 0;
}
#endif

LUALIB_API int luaopen_periphery(lua_State *L) {
    /* Create error metatable with __tostring */
    luaL_newmetatable(L, "periphery.error");
    lua_pushcclosure(L, periphery_error_tostring, 0);
    lua_setfield(L, -2, "__tostring");
    lua_pop(L, 1);

    /* Create table of sub-modules */
    lua_newtable(L);

#ifdef GPIO
    luaopen_periphery_gpio(L);
    lua_setfield(L, -2, "GPIO");
#endif

#ifdef SPI
    luaopen_periphery_spi(L);
    lua_setfield(L, -2, "SPI");
#endif

#ifdef I2C
    luaopen_periphery_i2c(L);
    lua_setfield(L, -2, "I2C");
#endif

#ifdef SERIAL
    luaopen_periphery_serial(L);
    lua_setfield(L, -2, "Serial");
#endif

#ifdef MMIO
    luaopen_periphery_mmio(L);
    lua_setfield(L, -2, "MMIO");
#endif

    lua_pushstring(L, LUA_PERIPHERY_VERSION);
    lua_setfield(L, -2, "version");

#ifdef SLEEP
    lua_pushcclosure(L, periphery_sleep, 0);
    lua_setfield(L, -2, "sleep");
    lua_pushcclosure(L, periphery_sleep_ms, 0);
    lua_setfield(L, -2, "sleep_ms");
    lua_pushcclosure(L, periphery_sleep_us, 0);
    lua_setfield(L, -2, "sleep_us");
#endif

    return 1;
}

