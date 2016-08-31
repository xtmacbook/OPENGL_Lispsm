SET(NAME LispSM2-ShaderX)
PROJECT(${NAME})

SET(COMMON ${CMAKE_CURRENT_SOURCE_DIR}/../_common)
SET(MATH ${COMMON}/Mathematic)
SET(ENGINE ${COMMON}/Engine)

INCLUDE_DIRECTORIES(${COMMON})

ADD_LIBRARY(${NAME}.math STATIC 
	${MATH}/Mathematic.cpp 
	${MATH}/MathTools.cpp 
	${MATH}/Perlin.cpp 
	${MATH}/Geometry/Converter.cpp 
)

ADD_LIBRARY(${NAME}.engine STATIC 
	${ENGINE}/EngineBase/KeyState.cpp

	${ENGINE}/OS/OSwin.cpp
	${ENGINE}/GLInterface/ToolsGL.cpp
	${ENGINE}/GLInterface/TextureObjectGL.cpp
	${ENGINE}/Renderer/State.cpp
	${ENGINE}/WindowManager/GlutKeyState.cpp
	${ENGINE}/EngineLevel1/Shader.cpp
	${ENGINE}/EngineLevel1/ShaderManager.cpp
	Occlusion.cpp
	${ENGINE}/Algo/LispSM.h
	${ENGINE}/Algo/LispSM.cpp 
)

ADD_EXECUTABLE(${NAME} 
	Main.cpp 
	DataTypes.h
	MathStuff.h MathStuff.cpp 
	SceneData.h SceneData.cpp 
	FrameState.h FrameState.cpp 
	ActionStateHandler.h ActionStateHandler.cpp
	StateGeometry.h StateGeometry.cpp 
	GeometryObjects.h
	Text.h Text.cpp 
	Materials.h 
	FBODepth.h 

	sky.frag 
	depthOnly.vert depthOnly.frag 
	shadowNdiffuse.frag shadowNdiffuse.vert
)

TARGET_LINK_LIBRARIES(${NAME} ${NAME}.math)
TARGET_LINK_LIBRARIES(${NAME} ${NAME}.engine)

INCLUDE(${COMMON}/cmake.exe.txt)
INCLUDE(${COMMON}/cmake.gl.lib.dll.txt)
INCLUDE(${COMMON}/cmake.shader.txt)
