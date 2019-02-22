install ( PROGRAMS org.kde.khipu.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install ( FILES khipu.rc  DESTINATION  ${KXMLGUI_INSTALL_DIR}/khipu )
install(FILES org.kde.khipu.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
