cmake_minimum_required (VERSION 3.0)

set(SOURCE ${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/osx_tts_api.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/osxtts_engine.mm
	${CMAKE_CURRENT_SOURCE_DIR}/osxtts_voice.mm
	PARENT_SCOPE
)

set(HEADERS ${HEADERS}
	${CMAKE_CURRENT_SOURCE_DIR}/osxtts_engine.h
	${CMAKE_CURRENT_SOURCE_DIR}/osxtts_voice.h
	PARENT_SCOPE
)