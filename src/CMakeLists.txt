ADD_SUBDIRECTORY(lib)


ADD_EXECUTABLE(GAME main.cpp)

TARGET_LINK_LIBRARIES( GAME 
                       CLI_STATIC
)

TARGET_INCLUDE_DIRECTORIES( GAME PUBLIC
                            lib/cli
)