output_gpio_test_exe = executable(
  'output_gpio_test',
  ['output_gpio_test.c', '../output_gpio.c'],
  c_args: test_c_args,
  include_directories: inc_dirs + [mock_inc_dirs] + [test_inc_dirs],
  link_with: [unity_lib],
  native: true
)

test('output_gpio_test', output_gpio_test_exe)
