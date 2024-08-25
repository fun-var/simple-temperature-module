cmake_minimum_required(VERSION 3.12)
project(first_driver)

set(kerneldir "" CACHE STRING "Path to the kernel build directory")
if("${kerneldir}" STREQUAL "")
  execute_process(COMMAND uname -r OUTPUT_VARIABLE uname_r
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(kerneldir "/lib/modules/${uname_r}/build")
endif()

include_directories(include)

#file(GLOB_RECURSE SRC_FILES "src/*.c")

#add_library(${PROJECT_NAME} MODULE ${SRC_FILES})

function(compile_module obj)
  set(TARGET_NAME ${obj})
  # Add the target, OBJ is the module name. Since I build in the build directory, here chooses to copy the .c and .h file to this directory.
  add_custom_target(${TARGET_NAME} ALL cp -f ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c ${CMAKE_CURRENT_BINARY_DIR}/
			COMMAND cp -f ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_BINARY_DIR}/
  			COMMAND echo "compiling module ${obj}.ko..."
  		    )

  # Set dependence, equivalent to $ (module_name) -Objs + = DEMO_MAIN.O in Makefile
  set(depend_objlist "main.o") #список всех файлов .o(конвкпт)
  
  # Set the compile command
  add_custom_command(TARGET ${TARGET_NAME}
		     POST_BUILD
		     COMMAND echo "obj-m := ${obj}.o" > ${CMAKE_CURRENT_BINARY_DIR}/Makefile
		     COMMAND echo "${obj}-objs:=${depend_objlist}" >>${CMAKE_CURRENT_BINARY_DIR}/Makefile
		     COMMAND make -C ${kerneldir} M=${CMAKE_CURRENT_BINARY_DIR} modules
		     )
endfunction()

compile_module(temp_module)
# Call compilation function