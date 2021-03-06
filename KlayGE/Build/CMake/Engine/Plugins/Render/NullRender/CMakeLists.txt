SET(LIB_NAME KlayGE_RenderEngine_NullRender)

SET(NULL_RE_SOURCE_FILES
	${KLAYGE_PROJECT_DIR}/Plugins/Src/Render/NullRender/NullRenderEngine.cpp
	${KLAYGE_PROJECT_DIR}/Plugins/Src/Render/NullRender/NullRenderFactory.cpp
	${KLAYGE_PROJECT_DIR}/Plugins/Src/Render/NullRender/NullRenderStateObject.cpp
	${KLAYGE_PROJECT_DIR}/Plugins/Src/Render/NullRender/NullShaderObject.cpp
	${KLAYGE_PROJECT_DIR}/Plugins/Src/Render/NullRender/NullTexture.cpp
)

SET(NULL_RE_HEADER_FILES
	${KLAYGE_PROJECT_DIR}/Plugins/Include/KlayGE/NullRender/NullRenderEngine.hpp
	${KLAYGE_PROJECT_DIR}/Plugins/Include/KlayGE/NullRender/NullRenderFactory.hpp
	${KLAYGE_PROJECT_DIR}/Plugins/Include/KlayGE/NullRender/NullRenderStateObject.hpp
	${KLAYGE_PROJECT_DIR}/Plugins/Include/KlayGE/NullRender/NullShaderObject.hpp
	${KLAYGE_PROJECT_DIR}/Plugins/Include/KlayGE/NullRender/NullTexture.hpp
)

SOURCE_GROUP("Source Files" FILES ${NULL_RE_SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${NULL_RE_HEADER_FILES})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${KLAYGE_PROJECT_DIR}/../External/GSL/include)
INCLUDE_DIRECTORIES(${KLAYGE_PROJECT_DIR}/../External/UniversalDXSDK/Include)
INCLUDE_DIRECTORIES(${KLAYGE_PROJECT_DIR}/../glloader/include)
if(KLAYGE_IS_DEV_PLATFORM)
	INCLUDE_DIRECTORIES(${KLAYGE_PROJECT_DIR}/../DXBC2GLSL/Include)
endif()
INCLUDE_DIRECTORIES(${KLAYGE_PROJECT_DIR}/../KFL/include)
INCLUDE_DIRECTORIES(${KLAYGE_PROJECT_DIR}/Core/Include)
INCLUDE_DIRECTORIES(${KLAYGE_PROJECT_DIR}/Plugins/Include)
if(KLAYGE_IS_DEV_PLATFORM)
	LINK_DIRECTORIES(${KLAYGE_PROJECT_DIR}/../DXBC2GLSL/lib/${KLAYGE_PLATFORM_NAME})
endif()
LINK_DIRECTORIES(${KLAYGE_PROJECT_DIR}/../KFL/lib/${KLAYGE_PLATFORM_NAME})
IF(KLAYGE_PLATFORM_DARWIN OR KLAYGE_PLATFORM_LINUX)
	LINK_DIRECTORIES(${KLAYGE_BIN_DIR})
ELSE()
	LINK_DIRECTORIES(${KLAYGE_OUTPUT_DIR})
ENDIF()

ADD_LIBRARY(${LIB_NAME} SHARED
	${NULL_RE_SOURCE_FILES} ${NULL_RE_HEADER_FILES}
)
ADD_DEPENDENCIES(${LIB_NAME} ${KLAYGE_CORELIB_NAME})
if(KLAYGE_IS_DEV_PLATFORM)
	add_dependencies(${LIB_NAME} glloader DXBC2GLSLLib)
endif()

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY ${KLAYGE_OUTPUT_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${KLAYGE_OUTPUT_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${KLAYGE_OUTPUT_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${KLAYGE_OUTPUT_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${KLAYGE_OUTPUT_DIR}
	RUNTIME_OUTPUT_DIRECTORY ${KLAYGE_BIN_DIR}/Render
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${KLAYGE_BIN_DIR}/Render
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${KLAYGE_BIN_DIR}/Render
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${KLAYGE_BIN_DIR}/Render
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${KLAYGE_BIN_DIR}/Render
	LIBRARY_OUTPUT_DIRECTORY ${KLAYGE_BIN_DIR}/Render
	LIBRARY_OUTPUT_DIRECTORY_DEBUG ${KLAYGE_BIN_DIR}/Render
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${KLAYGE_BIN_DIR}/Render
	LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${KLAYGE_BIN_DIR}/Render
	LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${KLAYGE_BIN_DIR}/Render
	PROJECT_LABEL ${LIB_NAME}
	DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
	OUTPUT_NAME ${LIB_NAME}${KLAYGE_OUTPUT_SUFFIX}
	FOLDER "KlayGE/Engine/Plugins/Render"
)

KLAYGE_ADD_PRECOMPILED_HEADER(${LIB_NAME} "${KLAYGE_PROJECT_DIR}/Core/Include/KlayGE/KlayGE.hpp")

if(KLAYGE_IS_DEV_PLATFORM)
	target_link_libraries(${LIB_NAME}
		debug DXBC2GLSLLib${KLAYGE_OUTPUT_SUFFIX}_d optimized DXBC2GLSLLib${KLAYGE_OUTPUT_SUFFIX}
	)
endif()
TARGET_LINK_LIBRARIES(${LIB_NAME}
	debug KlayGE_Core${KLAYGE_OUTPUT_SUFFIX}_d optimized KlayGE_Core${KLAYGE_OUTPUT_SUFFIX}
	debug KFL${KLAYGE_OUTPUT_SUFFIX}_d optimized KFL${KLAYGE_OUTPUT_SUFFIX}
)

ADD_DEPENDENCIES(AllInEngine ${LIB_NAME})
