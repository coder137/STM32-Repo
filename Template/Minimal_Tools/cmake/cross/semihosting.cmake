if(USE_SEMIHOSTING)
  message("Configuring Project with Semihosting")
  target_link_options(${USER_PROJECT_TARGET} PRIVATE 
    ${USER_SEMIHOSTING_FLAGS}
  )
  target_compile_definitions(${USER_PROJECT_TARGET} PRIVATE SEMIHOSTING=1)
else()
  message("Turning Semihosting off")
  target_link_options(${USER_PROJECT_TARGET} PRIVATE 
    ${USER_NO_SEMIHOSTING_FLAGS}
  )
endif()
