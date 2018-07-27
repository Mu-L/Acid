if(APPLE)
	FIND_PATH(OPENAL_INCLUDES al.h)
else()
	set(OPENAL_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/openal/include")
endif()

find_library(OPENAL_LIBRARY NAMES openal OpenAL32 OpenAL PATHS "${PROJECT_SOURCE_DIR}/Libraries/openal/lib/")

set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${OPENAL_INCLUDES})
set(LIBRARIES_LINKS ${LIBRARIES_LINKS} "${OPENAL_LIBRARY}")
message(STATUS "OpenAL: ${OPENAL_LIBRARY}")

if(NOT OPENAL_LIBRARY)
	message(FATAL_ERROR "OpenAL library not found!")
endif()

# Install
if (ACID_INSTALL)
	file(COPY "${OPENAL_INCLUDES}/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/")
    file(COPY "${OPENAL_LIBRARY}" DESTINATION "${CMAKE_INSTALL_BINDIR}/")
    file(COPY "${PROJECT_SOURCE_DIR}/Libraries/openal/lib/OpenAL32.lib" DESTINATION "${CMAKE_INSTALL_LIBDIR}/")
endif()
