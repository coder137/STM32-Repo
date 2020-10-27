# Includes a .cmake script similar to a subdirectory
# Expects the same .cmake script name as the folder
# Does NOT work for nested paths ex. third_party/FreeRTOS
# Use ess_include_script instead
macro(ess_include relative_script_path)
  message("In ${CMAKE_CURRENT_LIST_DIR}/${relative_script_path}/${relative_script_path}.cmake")
  include("${CMAKE_CURRENT_LIST_DIR}/${relative_script_path}/${relative_script_path}.cmake")
endmacro()

# Expects the script path name relative to your current .cmake or 
# CMakeLists.txt file
macro(ess_include_script relative_script_path)
  message("In ${CMAKE_CURRENT_LIST_DIR}/${relative_script_path}")
  include("${CMAKE_CURRENT_LIST_DIR}/${relative_script_path}")
endmacro()

# Converts list of relative path names to absolute path
# by appending the CMAKE_CURRENT_LIST_DIR to variables
macro(ess_data variable)
  set(list_var "${ARGN}")
  list(TRANSFORM list_var PREPEND "${CMAKE_CURRENT_LIST_DIR}/")
  list(APPEND ${variable} ${list_var})
endmacro()


# Register a unit test
function(ess_test test_name test_file actual_file)
  add_executable(${test_name} ${test_file})
  target_sources(${test_name} PRIVATE ${actual_file})
  target_include_directories(${test_name} PRIVATE ${USER_TEST_DIRS})
  target_link_libraries(${test_name} PRIVATE unity fff)
  # Remove unnecessary warnings
  target_compile_options(${test_name} PRIVATE -Wno-int-to-pointer-cast)
  add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()
