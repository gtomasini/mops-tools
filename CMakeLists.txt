
set(prjname mops_tools)
set(version 1)

project(${prjname} CXX)
add_subdirectory(avoidance_suscriber)
add_subdirectory(bin2txt)
add_subdirectory(bin2udp)
add_subdirectory(mops2kml)
add_subdirectory(remixer)
add_subdirectory(remixerLib)
add_subdirectory(tp_server)
add_subdirectory(trackLib)
add_subdirectory(udpServer)
add_subdirectory(ut_remixer)
