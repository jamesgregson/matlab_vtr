cmake_minimum_required( VERSION 2.6 )

project( vtr_loader )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY         ${CMAKE_SOURCE_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY         ${CMAKE_SOURCE_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin )

find_package( VTK REQUIRED )

include_directories( 
 source
 ${VTK_INCLUDE_DIRS}
)

add_library( vtr_loader SHARED source/vtr_loader.cpp source/vtr_loader.h )
target_link_libraries( vtr_loader ${VTK_LIBRARIES} )

FILE( WRITE mex/build_mex.sh 
  "#!/bin/bash\n"
  "mex -I${CMAKE_SOURCE_DIR}/source -L${CMAKE_SOURCE_DIR}/bin -lvtr_loader load_vtr.cpp\n"
)