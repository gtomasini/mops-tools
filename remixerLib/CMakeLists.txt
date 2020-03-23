#=================================================================================================================
# Welcome to remixerLib
#=================================================================================================================
set(prjname remixerLib)
#=================================================================================================================
# Headers
#=================================================================================================================
file (GLOB dir_23_headers *.h *.hpp *.inl *.vert *.frag *.geom *.tes *.tcs)
SOURCE_GROUP("" FILES ${dir_23_headers})
set(all_headers ${dir_23_headers} )
#=================================================================================================================
# Sources
#=================================================================================================================
file (GLOB dir_24_sources *.cpp *.c *.qrc *.vert *.frag)
SOURCE_GROUP("" FILES ${dir_24_sources})
set(all_sources ${dir_24_sources} )
#=================================================================================================================
# Helper
#=================================================================================================================
set(all_helpers )
#=================================================================================================================
# User's Definitions
#=================================================================================================================
#=================================================================================================================
# Target Properties
#=================================================================================================================
STRING(TOUPPER ${prjname} PRJNAME)
add_definitions(-D${PRJNAME}_STATIC) 

add_library(${prjname} STATIC ${all_headers} ${all_sources} ${all_helpers})
#=================================================================================================================
# Build Properties
#=================================================================================================================
set_target_properties(${prjname} PROPERTIES DEBUG_OUTPUT_NAME ${prjname}${DEBUG_TARGET_SUFFIX}${BUILD_ARCH_SUFFIX})
set_target_properties(${prjname} PROPERTIES RELEASE_OUTPUT_NAME ${prjname}${BUILD_ARCH_SUFFIX})
#=================================================================================================================
# Include - Current Project
#=================================================================================================================
include_directories(${PROJECTS_SRC_ROOT}/mops_tools)
#=================================================================================================================
# Includes - 1st Party Dependencies
#=================================================================================================================
include_directories(${PROJECTS_SRC_ROOT}/daa_icd)
include_directories(${PROJECTS_SRC_ROOT}/mops_tools)
#=================================================================================================================
# Libraries - 1st Party Dependencies
#=================================================================================================================
add_definitions(-DDAA_DATATYPES_ICD_STATIC) #static library
target_link_libraries(${prjname} debug daa_datatypes_icd${DEBUG_TARGET_SUFFIX}${BUILD_ARCH_SUFFIX} optimized daa_datatypes_icd)
target_link_libraries(${prjname} debug trackLib${DEBUG_TARGET_SUFFIX}${BUILD_ARCH_SUFFIX} optimized trackLib)
#=================================================================================================================
# External Libraries - 3rd Party Dependencies
#=================================================================================================================
#=================================================================================================================
# Link Snippet for poco_foundation
#=================================================================================================================
FOREACH(A_LIB ${POCO_FOUNDATION_LIBS_RELEASE}) 
  target_link_libraries(${prjname} optimized ${A_LIB})
ENDFOREACH()

FOREACH(A_LIB ${POCO_FOUNDATION_LIBS_DEBUG}) 
  target_link_libraries(${prjname} debug ${A_LIB})
ENDFOREACH()

#=================================================================================================================
# Link Snippet for poco_net
#=================================================================================================================
FOREACH(A_LIB ${POCO_NET_LIBS_RELEASE}) 
  target_link_libraries(${prjname} optimized ${A_LIB})
ENDFOREACH()

FOREACH(A_LIB ${POCO_NET_LIBS_DEBUG}) 
  target_link_libraries(${prjname} debug ${A_LIB})
ENDFOREACH()

#=================================================================================================================
# Link Snippet for poco_util
#=================================================================================================================
FOREACH(A_LIB ${POCO_UTIL_LIBS_RELEASE}) 
  target_link_libraries(${prjname} optimized ${A_LIB})
ENDFOREACH()

FOREACH(A_LIB ${POCO_UTIL_LIBS_DEBUG}) 
  target_link_libraries(${prjname} debug ${A_LIB})
ENDFOREACH()

