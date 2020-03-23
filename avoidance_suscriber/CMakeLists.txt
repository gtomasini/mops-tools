#=================================================================================================================
# Welcome to avoidance_suscriber
#=================================================================================================================
set(prjname avoidance_suscriber)
#=================================================================================================================
# Headers
#=================================================================================================================
file (GLOB dir_16_headers *.h *.hpp *.inl *.vert *.frag *.geom *.tes *.tcs)
SOURCE_GROUP("" FILES ${dir_16_headers})
set(all_headers ${dir_16_headers} )
#=================================================================================================================
# Sources
#=================================================================================================================
file (GLOB dir_17_sources *.cpp *.c *.qrc *.vert *.frag)
SOURCE_GROUP("" FILES ${dir_17_sources})
set(all_sources ${dir_17_sources} )
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
add_executable(${prjname} ${all_headers} ${all_sources})
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
include_directories(${PROJECTS_SRC_ROOT}/cppiso)
include_directories(${PROJECTS_SRC_ROOT}/daa_icd)
include_directories(${PROJECTS_SRC_ROOT}/yaswim)
#=================================================================================================================
# Libraries - 1st Party Dependencies
#=================================================================================================================
target_link_libraries(${prjname} debug pococonf${DEBUG_TARGET_SUFFIX}${BUILD_ARCH_SUFFIX} optimized pococonf)
target_link_libraries(${prjname} debug pocogger${DEBUG_TARGET_SUFFIX}${BUILD_ARCH_SUFFIX} optimized pocogger)
target_link_libraries(${prjname} debug avoidance_idl${DEBUG_TARGET_SUFFIX}${BUILD_ARCH_SUFFIX} optimized avoidance_idl)
target_link_libraries(${prjname} debug daa_datatypes_icd${DEBUG_TARGET_SUFFIX}${BUILD_ARCH_SUFFIX} optimized daa_datatypes_icd)
target_link_libraries(${prjname} debug swim_opendds${DEBUG_TARGET_SUFFIX}${BUILD_ARCH_SUFFIX} optimized swim_opendds)
#=================================================================================================================
# External Libraries - 3rd Party Dependencies
#=================================================================================================================
#=================================================================================================================
# Link Snippet for boost
#=================================================================================================================
target_link_libraries(${prjname} ${Boost_LIBRARIES})

#=================================================================================================================
# Link Snippet for opendds
#=================================================================================================================
FOREACH(A_LIB ${ACE_LIBS_RELEASE}) 
  target_link_libraries(${prjname} optimized ${A_LIB})
ENDFOREACH()

FOREACH(A_LIB ${ACE_LIBS_DEBUG}) 
  target_link_libraries(${prjname} debug ${A_LIB})
ENDFOREACH()

FOREACH(A_LIB ${OPENDDS_LIBS_RELEASE}) 
  target_link_libraries(${prjname} optimized ${A_LIB})
ENDFOREACH()

FOREACH(A_LIB ${OPENDDS_LIBS_DEBUG}) 
  target_link_libraries(${prjname} debug ${A_LIB})
ENDFOREACH()

#=================================================================================================================
# Link Snippet for opendds_monitor
#=================================================================================================================
FOREACH(A_LIB ${ACE_LIBS_RELEASE}) 
  target_link_libraries(${prjname} optimized ${A_LIB})
ENDFOREACH()

FOREACH(A_LIB ${ACE_LIBS_DEBUG}) 
  target_link_libraries(${prjname} debug ${A_LIB})
ENDFOREACH()

FOREACH(A_LIB ${OPENDDS_LIBS_RELEASE}) 
  target_link_libraries(${prjname} optimized ${A_LIB})
ENDFOREACH()

FOREACH(A_LIB ${OPENDDS_LIBS_DEBUG}) 
  target_link_libraries(${prjname} debug ${A_LIB})
ENDFOREACH()

FOREACH(A_LIB ${OPENDDS_MONITOR_LIBS_RELEASE}) 
  target_link_libraries(${prjname} optimized ${A_LIB})
ENDFOREACH()

FOREACH(A_LIB ${OPENDDS_MONITOR_LIBS_DEBUG}) 
  target_link_libraries(${prjname} debug ${A_LIB})
ENDFOREACH()

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

#=================================================================================================================
# Link Snippet for poco_xml
#=================================================================================================================
FOREACH(A_LIB ${POCO_XML_LIBS_RELEASE}) 
  target_link_libraries(${prjname} optimized ${A_LIB})
ENDFOREACH()

FOREACH(A_LIB ${POCO_XML_LIBS_DEBUG}) 
  target_link_libraries(${prjname} debug ${A_LIB})
ENDFOREACH()

