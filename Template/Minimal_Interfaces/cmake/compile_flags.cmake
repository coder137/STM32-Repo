# ~~~
# https://gcc.gnu.org/onlinedocs/gcc-3.2/gcc/Warning-Options.html#Warning%20Options
# ~~~
set(WARNING_FLAGS -Wall -Werror)

# Project Variables
set(COMMON_C_FLAGS
    -Os
    -mcpu=cortex-m4
    -mthumb
    -mfloat-abi=hard
    -mfpu=auto
    -g
    -pipe
    -Wl,--print-memory-usage
    ${WARNING_FLAGS})
set(USER_C_FLAGS -std=c99 -fmessage-length=0 -ffunction-sections
                 -fdata-sections -fstack-usage -nostdlib)

set(USER_LINK_FLAGS
    -nostartfiles
    -lc
    -lm
    # "SHELL:-u _printf_float" "SHELL:-u _scanf_float"
    -static
    -Wl,--gc-sections
    -Wl,-Map,${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map
    -T${PROJECT_SOURCE_DIR}/l0_lowlevel/linker/${CONTROLLER_NAME}/gcc_arm.ld
)

set (USER_NO_SEMIHOSTING_FLAGS
    -specs=nano.specs
    -specs=nosys.specs
)

# Make sure you do not add your syscalls overrides
# This will be provided by rdimon.specs
set (USER_SEMIHOSTING_FLAGS 
    -specs=nano.specs
    -specs=rdimon.specs 
)
