find_package(Doxygen REQUIRED)

# Set Input and Output files
set(DOXYGEN_IN_DIR ${PROJECT_SOURCE_DIR}/src)
set(DOXYGEN_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUT_DIR}/xml/index.html)
set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# configure file
configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
message("Doxygen build started")

# create out dir
file(MAKE_DIRECTORY ${DOXYGEN_OUT_DIR})

ADD_CUSTOM_COMMAND(OUTPUT ${DOXYGEN_INDEX_FILE}
					COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
					MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
					WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
					COMMENT "Generating Doxygen documentation"
					VERBATIM)

# Create CMake Target for generating doc.
ADD_CUSTOM_TARGET(docs ALL DEPENDS ${DOXYGEN_INDEX_FILE})
