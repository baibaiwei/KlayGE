SET(LIB_NAME filesystem)

IF(KLAYGE_PLATFORM_ANDROID AND KLAYGE_COMPILER_CLANG)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unusable-partial-specialization")
ENDIF()

SET(SOURCE_FILES
	${BOOST_PROJECT_DIR}/libs/filesystem/src/codecvt_error_category.cpp
	${BOOST_PROJECT_DIR}/libs/filesystem/src/directory.cpp
	${BOOST_PROJECT_DIR}/libs/filesystem/src/exception.cpp
	${BOOST_PROJECT_DIR}/libs/filesystem/src/operations.cpp
	${BOOST_PROJECT_DIR}/libs/filesystem/src/path.cpp
	${BOOST_PROJECT_DIR}/libs/filesystem/src/path_traits.cpp
	${BOOST_PROJECT_DIR}/libs/filesystem/src/portability.cpp
	${BOOST_PROJECT_DIR}/libs/filesystem/src/unique_path.cpp
	${BOOST_PROJECT_DIR}/libs/filesystem/src/utf8_codecvt_facet.cpp
	${BOOST_PROJECT_DIR}/libs/filesystem/src/windows_file_codecvt.cpp
)

add_definitions(-DBOOST_FILESYSTEM_SOURCE)

IF(KLAYGE_PREFERRED_LIB_TYPE STREQUAL "SHARED")
	ADD_DEFINITIONS(-DBOOST_FILESYSTEM_DYN_LINK=1)
ELSE()
	ADD_DEFINITIONS(-DBOOST_FILESYSTEM_STATIC_LINK=1)
ENDIF()

IF(NOT MSVC)
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -frtti" )
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -frtti")
	SET(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -frtti")
ENDIF()

SETUP_BOOST_LIBRARY(${LIB_NAME})
