# Find the native FFTW3 includes and library
#
#  FFTW3_INCLUDE_DIR - where to find fftw3.h, etc.
#  FFTW3_LIBRARIES   - List of libraries
#  FFTW3_FOUND       - True if FFTW3 found.
set(FFTW3_DEBUG 1)
if(FFTW3_DEBUG)
  MESSAGE(STATUS "Finding FFTW3")
endif()

# Only set FFTW3_INSTALL to the environment variable if it is blank
if("${FFTW3_INSTALL}" STREQUAL "")
    set(FFTW3_INSTALL  $ENV{FFTW3_INSTALL})
endif()

IF (FFTW3_INCLUDE_DIR)
  # Already in cache, be silent
  SET(FFTW3_LIB_FIND_QUIETLY TRUE)
ENDIF (FFTW3_INCLUDE_DIR)

FIND_PATH(FFTW3_INCLUDE_DIR fftw3.h
  ${FFTW3_INSTALL}/include
  /usr/local/include
  /usr/include
  NO_DEFAULT_PATH
)

SET(FFTW3_LIB_NAMES fftw3)
FIND_LIBRARY(FFTW3_LIBRARY
  NAMES ${FFTW3_LIB_NAMES}
  PATHS
  ${FFTW3_INSTALL}/lib /usr/lib /usr/local/lib
  NO_DEFAULT_PATH
)

if(FFTW3_DEBUG)
  MESSAGE(STATUS "FFTW3_LIBRARY: ${FFTW3_LIBRARY}")
  MESSAGE(STATUS "FFTW3_INCLUDE_DIR: ${FFTW3_INCLUDE_DIR}")
ENDIF()

IF (FFTW3_INCLUDE_DIR AND FFTW3_LIBRARY)
   SET(FFTW3_FOUND TRUE)
   SET( FFTW3_LIBRARIES ${FFTW3_LIBRARY} )
ELSE (FFTW3_INCLUDE_DIR AND FFTW3_LIBRARY)
   SET(FFTW3_FOUND FALSE)
   SET( FFTW3_LIBRARIES )
ENDIF (FFTW3_INCLUDE_DIR AND FFTW3_LIBRARY)

IF (FFTW3_FOUND)
  message(STATUS "FFTW3 Location: ${FFTW3_INSTALL}")
  message(STATUS "FFTW3 Version: ${FFTW3_VERSION}")
  message(STATUS "FFTW3 LIBRARY: ${FFTW3_LIBRARY}")
ELSE (FFTW3_FOUND)
  IF (FFTW3_LIB_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for FFTW3 libraries named ${FFTW3_LIBS_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find FFTW3 library")
  ENDIF (FFTW3_LIB_FIND_REQUIRED)
ENDIF (FFTW3_FOUND)

MARK_AS_ADVANCED(
  FFTW3_LIBRARY
  FFTW3_INCLUDE_DIR
  )
