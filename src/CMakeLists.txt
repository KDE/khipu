include_directories (
  ${KDE4_INCLUDES}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  .
  ..
)

set ( khipu_SRCS

  functionsmodel.cpp
  functionsview.cpp
  functioneditor.cpp
  functionlibraryedit.cpp
  functionlibrarymodel.cpp
  #use analitzaplot now :) 

  space.cpp
  spacesmodel.cpp
  spacesview.cpp

  viewer2d.cpp
  viewer3d.cpp

  mainwindow.cpp
  main.cpp

  dashboard.cpp
)


kde4_add_ui_files ( khipu_SRCS
  functioneditorwidget.ui
  viewer2dwidget.ui
  viewer3dwidget.ui

  dashboardwidget.ui
)



kde4_add_executable ( khipu ${khipu_SRCS} )

target_link_libraries( khipu
  ${KDE4_KFILE_LIBS}
  ${ANALITZA_LIBRARY} ${ANALITZAGUI_LIBRARY} ${ANALITZAPLOT_LIBRARY}
    #OpenGL
  ${OPENGL_gl_LIBRARY}
  ${OPENGL_glu_LIBRARY}
  ${QT_QTOPENGL_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QGLVIEWER_LIBRARY}
#   keomath # remove this for next iters
)

install (TARGETS khipu ${INSTALL_TARGETS_DEFAULT_ARGS} )
