add_custom_target(
  test_project
  DEPENDS ${USER_PROJECT_TARGET}
  COMMAND cmake -B _build_test -S ../ -G Ninja -D TESTING=ON
  COMMAND cmake --build _build_test --config Debug --target all
  COMMAND cd _build_test
  COMMAND ctest -j14 -C Debug -T test --output-on-failure
  COMMAND cd ..
  VERBATIM USES_TERMINAL
)
