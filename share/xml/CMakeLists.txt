file(GLOB_RECURSE VOTCA_CTP_XML *.xml)
install(FILES ${VOTCA_CTP_XML} DESTINATION ${CMAKE_INSTALL_DATADIR}/votca/ctp/xml)
