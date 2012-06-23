include_directories (
  ${KDE4_INCLUDES}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  .
  ..
)

set ( khipu_SRCS
  app/gplacs.cpp
  app/main.cpp

  widgets/dashboard.cpp
  widgets/gplacswidget.cpp
)


kde4_add_ui_files ( khipu_SRCS
  widgets/dashboardwidget.ui
)

kde4_add_executable ( khipu ${khipu_SRCS} )

target_link_libraries( khipu
  ${KDE4_KFILE_LIBS}
  ${ANALITZA_LIBRARY} ${ANALITZAGUI_LIBRARY} ${ANALITZAPLOT_LIBRARY}
  keomath
)

install (TARGETS khipu ${INSTALL_TARGETS_DEFAULT_ARGS} )
