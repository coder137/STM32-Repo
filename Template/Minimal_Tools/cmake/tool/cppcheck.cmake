set(CPPCHECK_ENABLE "--enable=all" "--force")
set(CPPCHECK_STD "--std=c11")

# unmatchedSuppression: When an error doesn't occur we get an error called unmatchedSuppression
# unusedFunction: functions that are not used. Throws false positives when interrupt functions are defined
# missingIncludeSystem: We do not require the system includes to be passed into cppcheck
set(CPPCHECK_SUPPRESS 
    "--suppress=unmatchedSuppression"
    "--suppress=unusedFunction"
    "--suppress=missingIncludeSystem"

    # Third party library suppressions
    "--suppress=*:*l0_lowlevel/arm/cmsis/core/*"
    "--suppress=*:*l1_third_party/FreeRTOS/*"
)

set(CPPCHECK_ADDITIONAL_OPTIONS 
    "--error-exitcode=1"
    "-q"
)

# Cppcheck directory for intermediate files
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/cppcheck_build")
set(CPPCHECK_BUILDDIR "--cppcheck-build-dir=${CMAKE_CURRENT_BINARY_DIR}/cppcheck_build")

# CMake generates compile_commands.json which can be used by cppcheck
set(CPPCHECK_PROJECT "--project=${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")

# Final command
set(CMAKE_C_CPPCHECK "cppcheck"
    ${CPPCHECK_BUILDDIR}
    ${CPPCHECK_ENABLE}
    ${CPPCHECK_SUPPRESS}
    ${CPPCHECK_ADDITIONAL_OPTIONS}
)
