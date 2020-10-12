# https://github.com/vikaig/stm32-libopencm3-meson-blink/blob/master/meson.build
project('minimal_unit_testing_meson', ['c', 'cpp'])

# TODO, Add an option for SEMIHOSTING

warning_flags = [
  '-Wall',
  '-Werror'
]

commonflags = [
  '-Os',
  '-mcpu=cortex-m4',
  '-mthumb',
  '-mfloat-abi=hard',
  '-mfpu=auto',
  '-Wall',
  '-g',
  '-pipe'
]

cflags = [
  '-fmessage-length=0', 
  '-ffunction-sections',
  '-fdata-sections', 
  '-fstack-usage'
]

ldflags = [
  '-nostartfiles',
  '-specs=nano.specs', 
  '-specs=nosys.specs',
  '-lc',
  '-lm',
  # "SHELL:-u _printf_float" "SHELL:-u _scanf_float"
  '-nostdlib',
  '-static',
  '-Wl,--gc-sections',
  '-Wl,-Map,'+meson.project_name()+'.map',
  '-T'+meson.source_root()+'/l0_lowlevel/linker/gcc_arm.ld'
]

subdir('l0_lowlevel')
subdir('driver')
subdir('modules')
subdir('HAL')
subdir('third_party/FreeRTOS')
subdir('application')

sources = l0_sources + driver_sources + modules_sources + freertos_sources + HAL_sources + application_sources

# Summation of two include directories
# https://github.com/mesonbuild/meson/issues/5748
inc_dirs = [l0_inc_dirs] + [driver_inc_dirs] + [modules_inc_dirs] + [freertos_inc_dirs] + [HAL_inc_dirs] + [application_inc_dirs]

c_preprocessor_args = ['-DSTM32L475xx']

project_elf = executable(meson.project_name()+'.elf', 
  sources, 
  c_args : c_preprocessor_args + commonflags + cflags + warning_flags, 
  link_args : commonflags + ldflags, 
  include_directories: inc_dirs, 
  native: false
)

# Custom Target
bin = custom_target('bin', 
    input: project_elf,
    output: meson.project_name()+'.bin',
    command: [ find_program('objcopy'), '-O', 'binary', '@INPUT@', '@OUTPUT@'],
    build_by_default: true,
    depends: project_elf
)

hex = custom_target('hex', 
    input: project_elf,
    output: meson.project_name()+'.hex',
    command: [ find_program('objcopy'), '-O', 'ihex', '@INPUT@', '@OUTPUT@'],
    build_by_default: true,
    depends: project_elf
)

lst = custom_target('lst',
    input: project_elf,
    output: meson.project_name()+'.lst',
    command: [ find_program('objdump'), '--source', '--all-headers', '--demangle', '--line-numbers', '--wide', '@INPUT@'],
    capture: true,
    build_by_default: true,
    depends: project_elf
)

# Just used to print the size of the project
size = custom_target('size',
    input: project_elf,
    output: 'unused',
    command: [ find_program('size'), '--format=berkeley', '@INPUT@'],
    build_by_default: true,
    depends: project_elf
)

# Find and Flash
stm32_programmer_cli = find_program('stm32_programmer_cli')
if not stm32_programmer_cli.found()
  error('Please install the STM32_Programmer_CLI tool')
endif

find_devices = run_target('find_devices',
    command: [ stm32_programmer_cli, '-l', 'stlink']
)

# For flashing we want to set the stlink_sn number of the board dynamically
stlink_sn = get_option('stlink_sn')
if stlink_sn == ''
  warning('Configure your STLINK_SN
  `meson configure -D stlink_sn=<your_stlink_sn>`
  Run `ninja find_devices -c build_directory` to find your STM32 software number')
else
  flash = run_target('flash',
    command: [ stm32_programmer_cli, '-c', 'port=SWD', 'sn='+stlink_sn, '-w', bin, '0x08000000'],
    depends: bin
)
endif
