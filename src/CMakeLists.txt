include_directories (
  ${KDE4_INCLUDES}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  .
  ..
  ../3rdparty
)

set ( gplacs_SRCS
  app/gplacs.cpp
  app/main.cpp

  widgets/dashboard.cpp
  widgets/gplacswidget.cpp
)


kde4_add_ui_files ( gplacs_SRCS
  widgets/dashboardwidget.ui
)

kde4_add_executable ( gplacs ${gplacs_SRCS} )

target_link_libraries( gplacs
  ${KDE4_KFILE_LIBS}
  QtSolutions
  ${ANALITZA_LIBRARY} ${ANALITZAGUI_LIBRARY}
  keomath
)

install (TARGETS gplacs ${INSTALL_TARGETS_DEFAULT_ARGS} )
