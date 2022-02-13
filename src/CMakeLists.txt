# Submodules here
add_library(cli cli.cpp)
target_link_libraries(cli PRIVATE Boost::boost Boost::program_options)
add_library(config config.cpp)
target_link_libraries(config PRIVATE Boost::boost Boost::program_options)
add_library(repo repo.cpp)
target_link_libraries(repo PRIVATE Boost::boost Boost::program_options config)

# Library here
add_library(libwyag libwyag.cpp)
target_link_libraries(libwyag PRIVATE Boost::boost Boost::program_options
                                      cryptopp-static repo cli)

# main here
add_executable(wyag main.cpp)
target_link_libraries(wyag PRIVATE libwyag)
