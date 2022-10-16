cmake_minimum_required(VERSION 3.0)

project(saf7) 

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake") 

find_package(Gloox REQUIRED)
find_package(Boost COMPONENTS program_options CONFIG REQUIRED)
find_package(DBusCpp REQUIRED)

add_executable(saf7 
    dbus_saf_adaptor.hpp  dbus_saf_proxy.hpp
    DBusSafWorkers.cpp  main.cpp  SAF.cpp)

target_link_libraries(saf7 ${GLOOX_LIBRARIES} ${Boost_LIBRARIES} ${DBUSCPP_LIBRARIES})

install(TARGETS saf7 RUNTIME DESTINATION bin)
install(FILES
        saf7_SendMsg.sh saf7_Quit.sh
        DESTINATION bin)
install(FILES
        saf7-default.rc.template
        DESTINATION etc/saf7/)
install(FILES
        saf7-default.conf
        DESTINATION /etc/dbus-1/system.d/)
install(FILES
        saf7-default.service
        DESTINATION /etc/systemd/system/)
