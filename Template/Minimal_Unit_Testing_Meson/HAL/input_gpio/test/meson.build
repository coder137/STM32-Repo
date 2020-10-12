input_gpio_test_exe = executable(
  'input_gpio_test',
  ['input_gpio_test.c', '../input_gpio.c'],
  c_args: test_c_args,
  include_directories: inc_dirs + [mock_inc_dirs] + [test_inc_dirs],
  link_with: [unity_lib],
  native: true
)

test('input_gpio_test', input_gpio_test_exe)
