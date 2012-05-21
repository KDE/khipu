project(QtSolutions)

include_directories(
  ${KDE4_INCLUDES}
  ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ..
)
set(QtSolutions_SRCS
	QtMmlWidget/qtmmlwidget.cpp
	QtThumbWheel/qtthumbwheel.cpp
)

kde4_add_library(QtSolutions SHARED ${QtSolutions_SRCS})
target_link_libraries(QtSolutions ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY})

set_target_properties(QtSolutions PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install(TARGETS QtSolutions ${INSTALL_TARGETS_DEFAULT_ARGS})

