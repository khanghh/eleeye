cmake_minimum_required(VERSION 3.6)
# set(ANDROID_NDK "$ENV{ANDROID_HOME}/ndk-bundle")
# set(CMAKE_TOOLCHAIN_FILE "${ANDROID_NDK}/build/cmake/android.toolchain.cmake")
# set(ANDROID_ABI "arm64-v8a")
# set(ANDROID_PLATFORM android-28)
# set(ANDROID_TOOLCHAIN clang)

set(PROJECT_NAME eleeye)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -O4 -Wall")
set(CMAKE_BUILD_TYPE Release)

set(SRC_FILES 
	base/pipe.cpp
	ucci.cpp
	pregen.cpp
	position.cpp
	genmoves.cpp 
	hash.cpp 
	book.cpp 
	movesort.cpp 
	preeval.cpp 
	evaluate.cpp 
	search.cpp 
	eleeye.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
