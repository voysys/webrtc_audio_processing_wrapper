cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(
  webrtc_audio_processing_wrapper
  VERSION 1.0
  LANGUAGES CXX)

add_library(${CMAKE_PROJECT_NAME} SHARED webrtc_audio_processing_wrapper.h
                                         webrtc_audio_processing_wrapper.cpp)

set_target_properties(
  ${CMAKE_PROJECT_NAME} PROPERTIES CXX_STANDARD 14 CMAKE_CXX_STANDARD_REQUIRED
                                                   TRUE)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_definitions(
    ${CMAKE_PROJECT_NAME}
    PRIVATE _CRT_SECURE_NO_WARNINGS
            UNICODE
            NOMINMAX
            WIN32_LEAN_AND_MEAN
            VC_EXTRALEAN
            WEBRTC_WIN
            _WINSOCKAPI_
            USE_MATH_DEFINES)

  target_compile_options(
    ${CMAKE_PROJECT_NAME}
    PRIVATE -nologo
            -Zc:wchar_t
            -Zc:rvalueCast
            -Zc:inline
            -Zc:referenceBinding
            -W4
            -WX
            -w44996
            -w34189
            -w44456
            -w44457
            -w44458
            -wd4577
            -wd4467
            -MT
            -sdl
            -fp:fast
            -fp:except-
            -EHa
            -std:c++14
            -Zi
            -FS)

  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

  target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    PUBLIC Winmm
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/artifacts/webrtc/lib/libwebrtc-audio-processing.a
  )

endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE WEBRTC_LINUX
                                                           WEBRTC_THREAD_RR)

  target_compile_options(
    ${CMAKE_PROJECT_NAME}
    PRIVATE -Weverything
            -Werror
            -Wfatal-errors
            -Wno-gnu-anonymous-struct
            -Wno-nested-anon-types
            -Wno-missing-braces
            -Wno-c++98-compat-pedantic
            -Wno-padded
            -Wno-missing-braces
            -Wno-covered-switch-default
            -Wno-gnu-anonymous-struct
            -Wno-nested-anon-types
            -Wno-weak-vtables
            -Wno-switch-enum
            -Wno-c++2a-compat
            -Wno-extra-semi-stmt
            -ffast-math
            -isystem
            ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/artifacts/absl/include
            ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/artifacts/webrtc/include
            -std=c++14
            -fPIC
            -fno-strict-aliasing
            -funsigned-char
            -ftrapv)

  target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/artifacts/webrtc/lib/libwebrtc-audio-processing.a
  )
endif()

target_include_directories(
  ${CMAKE_PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/artifacts/absl/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/artifacts/webrtc/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/artifacts/webrtc/include/webrtc-audio-processing
)

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION "")
