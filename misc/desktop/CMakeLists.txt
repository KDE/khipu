install ( PROGRAMS org.kde.khipu.desktop DESTINATION  ${KDE_INSTALL_APPDIR} )
install ( FILES khipu.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/khipu )
install(FILES org.kde.khipu.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
