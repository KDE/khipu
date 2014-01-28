project ( khipu )

find_package ( KDE4 REQUIRED )
include(KDE4Defaults)

find_package ( OpenGL REQUIRED )
find_package ( Analitza REQUIRED )
find_package ( QJSON REQUIRED)

find_package(LibKdeEdu)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS}  ${QJSON_DEFINITIONS} ${qjson_DEFINITIONS} -DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

if(NOT OPENGL_GLU_FOUND)
    message(FATAL_ERROR "GLU was not found")
endif(NOT OPENGL_GLU_FOUND)

install( FILES khipu.knsrc DESTINATION  ${CONFIG_INSTALL_DIR} )

add_subdirectory (src) #app ... use the library
add_subdirectory (misc) #icons, pics, function library, etc
add_subdirectory (tests)
add_subdirectory (doc)
