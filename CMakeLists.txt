project ( khipu )

find_package ( KDE4 REQUIRED )
include(KDE4Defaults)

find_package ( OpenGL REQUIRED )
find_package ( Gettext REQUIRED )
find_package ( Analitza REQUIRED )
find_package (qjson REQUIRED)

find_package(LibKdeEdu)
find_library(QTMML_LIBRARY qtmmlwidget)

macro_log_feature(QTMML_LIBRARY "QtMathML Widget" "Supports displaying some formulas in a mathematic form" "http://doc.qt.nokia.com/solutions/4/qtmmlwidget/qtmmlwidget.html" FALSE "" "")

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} ${qjson_DEFINITIONS} -DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

if(NOT OPENGL_GLU_FOUND)
    message(FATAL_ERROR "GLU was not found")
endif(NOT OPENGL_GLU_FOUND)

add_subdirectory ( src ) #app ... use the library
add_subdirectory ( misc ) #icons, pics, function library, etc
