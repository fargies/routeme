#############################
## GObjectBuilder commands ##
#############################
add_custom_command(OUTPUT rme-rule.h rme-rule.c rme-rule-private.h
    DEPENDS rme-rule.gob
    COMMAND ${GOB} ${CMAKE_CURRENT_SOURCE_DIR}/rme-rule.gob)

add_custom_command(OUTPUT rme-rule-manager.h rme-rule-manager.c
    DEPENDS rme-rule-manager.gob
    COMMAND ${GOB} ${CMAKE_CURRENT_SOURCE_DIR}/rme-rule-manager.gob)

add_custom_command(OUTPUT rme-config-loader.h rme-config-loader.c
    DEPENDS rme-config-loader.gob
    COMMAND ${GOB} ${CMAKE_CURRENT_SOURCE_DIR}/rme-config-loader.gob)

add_custom_command(OUTPUT rme-controlpoint.h rme-controlpoint.c
    DEPENDS rme-controlpoint.gob
    COMMAND ${GOB} ${CMAKE_CURRENT_SOURCE_DIR}/rme-controlpoint.gob)

add_custom_command(OUTPUT rme-service.h rme-service.c
    DEPENDS rme-service.gob
    COMMAND ${GOB} ${CMAKE_CURRENT_SOURCE_DIR}/rme-service.gob)

add_custom_command(OUTPUT rme-service-wan.h rme-service-wan.c
    DEPENDS rme-service-wan.gob
    COMMAND ${GOB} ${CMAKE_CURRENT_SOURCE_DIR}/rme-service-wan.gob)

#######################
## Lex/Yacc commands ##
#######################
bison_target(config_parser config_parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/config_parser.c
    COMPILE_FLAGS "${BISON_FLAGS}")
flex_target(config_lexer config_lexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/config_lexer.c
    COMPILE_FLAGS "${FLEX_FLAGS}")
add_flex_bison_dependency(config_lexer config_parser)

####################
## GUPnP commands ##
####################
add_custom_command(OUTPUT WANConnection_proxy.h
    DEPENDS WANConnection1.xml
    COMMAND ${GUPNP_BINDING_TOOL} -m client
    ${CMAKE_CURRENT_SOURCE_DIR}/WANConnection1.xml >
    WANConnection_proxy.h)

######################
## Internal library ##
######################
set(routeme_SRCS
    rme-rule.h rme-rule.c rme-rule-private.h
    rme-rule-manager.c rme-rule-manager.h
    rme-config-loader.c rme-config-loader.h
    rme-controlpoint.h rme-controlpoint.c
    rme-service.c rme-service.h
    rme-service-wan.c rme-service-wan.h
    WANConnection_proxy.h
    ${BISON_config_parser_OUTPUTS}
    ${FLEX_config_lexer_OUTPUTS}
    logger.h logger.c)
add_library(rme STATIC ${routeme_SRCS})
target_link_libraries(rme
    ${GOBJECT_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${GUPNP_LIBRARIES})

#####################
## Main executable ##
#####################
add_executable(routeme main.c)
target_link_libraries(routeme
    rme
    ${GOBJECT_LIBRARIES}
    ${GLIB_LIBRARIES})
include_directories(
    ${GOBJECT_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS}
    ${GUPNP_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS routeme
    RUNTIME DESTINATION sbin)

