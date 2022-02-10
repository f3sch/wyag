# Library here
add_library(libwyag libwyag.cpp)
target_link_libraries(libwyag PRIVATE Boost::boost Boost::program_options cryptopp-static)

# main here
add_executable(wyag main.cpp)
target_link_libraries(wyag PRIVATE libwyag)
