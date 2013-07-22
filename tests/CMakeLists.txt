set(testLibs ${QT_QTXML_LIBRARY} ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY} ${KDE4_KDECORE_LIBS})

include_directories (
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${ANALITZA_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ../src
)

kde4_add_unit_test(datastoretest datastoretest.cpp)

kde4_add_unit_test(persistancefiletest persistancefiletest.cpp)

kde4_add_unit_test(dictionarycollectiontest dictionarycollectiontest.cpp)

target_link_libraries(datastoretest
  ${testLibs}
  ${ANALITZA_LIBRARY} ${ANALITZAGUI_LIBRARY} ${ANALITZAPLOT_LIBRARY}
  khipuLib
)

target_link_libraries(persistancefiletest
  ${testLibs}
  ${ANALITZA_LIBRARY} ${ANALITZAGUI_LIBRARY} ${ANALITZAPLOT_LIBRARY}
  khipuLib
)

target_link_libraries(dictionarycollectiontest
  ${testLibs}
  ${ANALITZA_LIBRARY} ${ANALITZAGUI_LIBRARY} ${ANALITZAPLOT_LIBRARY}
  khipuLib
)
