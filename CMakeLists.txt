project ( GPLACS )

find_package ( KDE4 REQUIRED )
find_package ( OpenGL REQUIRED )
find_package ( Gettext REQUIRED )
find_package ( Analitza REQUIRED )

macro_optional_find_package(OpenGL)

include(KDE4Defaults)
include(MacroLibrary)
include(MacroOptionalAddSubdirectory)
include(MacroBoolTo01)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

if(NOT OPENGL_GLU_FOUND)
    message(FATAL_ERROR "GLU was not found")
endif(NOT OPENGL_GLU_FOUND)

include_directories (
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
)

add_subdirectory ( 3rdparty ) #dependencies
add_subdirectory ( keomath ) #framework library
add_subdirectory ( src ) #app ... use the library
add_subdirectory ( misc ) #icons, pics, function library, etc
add_subdirectory ( po ) #spanish lang support goes to GPLACS ... :)

