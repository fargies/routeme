
set(CMAKE_MAN_DIR share/man)
set(CMAKE_ETC_DIR /etc)
install(FILES routeme.1
    DESTINATION ${CMAKE_MAN_DIR}/man1)
install(FILES routeme.conf.5
    DESTINATION ${CMAKE_MAN_DIR}/man5)
install(FILES routeme.conf
    DESTINATION ${CMAKE_ETC_DIR})
install(PROGRAMS routeme.initd
    DESTINATION ${CMAKE_ETC_DIR}/init.d
    RENAME routeme)

