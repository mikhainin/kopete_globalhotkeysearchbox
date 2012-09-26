
project(kopete_globalhotkeysearchbox)

cmake_minimum_required(VERSION 2.6)


find_package(KDE4 REQUIRED)
find_package(Kopete REQUIRED)
find_package(KdepimLibs REQUIRED)
include (KDE4Defaults)
include (MacroLibrary)

if (Kopete_FOUND)
    message(STATUS "Kopete include dir is: ${KOPETE_INCLUDE_DIR}")
    include_directories(${KOPETE_INCLUDE_DIR})
    include_directories("${KOPETE_INCLUDE_DIR}/kopete")
else(Kopete_FOUND)
    message(ERROR "Kopete devel libraries do not found")
endif(Kopete_FOUND)



include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})


set(
    kopete_globalsearchhotkey_PART_SRCS

    globalsearchhotkeyplugin.cpp
    selectnthcontactaction.cpp
)

kde4_add_plugin(kopete_globalsearchhotkey ${kopete_globalsearchhotkey_PART_SRCS})

target_link_libraries(kopete_globalsearchhotkey  ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} kopete )

install(TARGETS kopete_globalsearchhotkey DESTINATION ${PLUGIN_INSTALL_DIR})


# install files

install( FILES kopete_globalsearchhotkey.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
kde4_install_icons( ${DATA_INSTALL_DIR}/kopete/icons  )

