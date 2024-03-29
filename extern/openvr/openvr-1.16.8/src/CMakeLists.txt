# Project name.
project(openvr_api)

set( LIBNAME "openvr_api" )
set(OPENVR_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../headers)

# Set some properies for specific files.
if(APPLE)
  set(CMAKE_MACOSX_RPATH 1)
  if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set_source_files_properties(vrcommon/pathtools_public.cpp vrcommon/vrpathregistry_public.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
  endif()
  if(BUILD_SHARED OR BUILD_FRAMEWORK)
    find_library(FOUNDATION_FRAMEWORK Foundation)
    mark_as_advanced(FOUNDATION_FRAMEWORK)
    set(EXTRA_LIBS ${EXTRA_LIBS} ${FOUNDATION_FRAMEWORK})
  endif(BUILD_SHARED OR BUILD_FRAMEWORK)
elseif(WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_definitions( -DWIN64 )
    set( LIBNAME "openvr_api64" )
  endif()
endif()

# Add include folders.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../headers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vrcommon)

if(USE_CUSTOM_LIBCXX)
	link_directories(
		${LIBCXX_LIB_DIR}
	)
endif()

# Set the source group and files.
set(CORE_FILES
	openvr_api_public.cpp
	jsoncpp.cpp
)
set(VRCOMMON_FILES
	vrcommon/dirtools_public.cpp
	vrcommon/envvartools_public.cpp
	vrcommon/pathtools_public.cpp
	vrcommon/sharedlibtools_public.cpp
	vrcommon/hmderrors_public.cpp
	vrcommon/vrpathregistry_public.cpp
	vrcommon/strtools_public.cpp
)

set(SOURCE_FILES
	${CORE_FILES}
	${VRCOMMON_FILES}
)

set(PUBLIC_HEADER_FILES
	${OPENVR_HEADER_DIR}/openvr_driver.h
	${OPENVR_HEADER_DIR}/openvr_capi.h
	${OPENVR_HEADER_DIR}/openvr.h
)

source_group("Src" FILES
	${CORE_FILES}
)

source_group("VRCommon" FILES
	${VRCOMMON_FILES}
)

# Build the library.
if(BUILD_SHARED)
	add_library(${LIBNAME} SHARED ${SOURCE_FILES})
elseif(BUILD_FRAMEWORK)
	set( LIBNAME "OpenVR" )
	add_library( ${LIBNAME} 
            SHARED ${SOURCE_FILES}
            ${CMAKE_SOURCE_DIR}/headers/openvr.h
            ${CMAKE_SOURCE_DIR}/headers/openvr_api.cs
            ${CMAKE_SOURCE_DIR}/headers/openvr_api.json
            ${CMAKE_SOURCE_DIR}/headers/openvr_capi.h
            ${CMAKE_SOURCE_DIR}/headers/openvr_driver.h
	)
	set_target_properties(OpenVR PROPERTIES
		FRAMEWORK TRUE
		FRAMEWORK_VERSION A
		MACOSX_FRAMEWORK_IDENTIFIER com.valvesoftware.OpenVR.framework
		MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_SOURCE_DIR}/src/Info.plist
		# "current version" in semantic format in Mach-O binary file
		VERSION 1.0.6
		# "compatibility version" in semantic format in Mach-O binary file
		SOVERSION 1.0.0
		PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/headers/openvr.h;${CMAKE_SOURCE_DIR}/headers/openvr_api.cs;${CMAKE_SOURCE_DIR}/headers/openvr_api.json;${CMAKE_SOURCE_DIR}/headers/openvr_capi.h;${CMAKE_SOURCE_DIR}/headers/openvr_driver.h"
		LINKER_LANGUAGE CXX
	)
else()
	add_library(${LIBNAME} STATIC ${SOURCE_FILES})
endif()

if(USE_CUSTOM_LIBCXX)
	set(EXTRA_LIBS ${EXTRA_LIBS} c++ c++abi)
endif()

if(MINGW_STDTHREADS_GENERATE_STDHEADERS)
	set(EXTRA_LIBS ${EXTRA_LIBS} mingw_stdthreads)
endif()

target_link_libraries(${LIBNAME} ${EXTRA_LIBS} ${CMAKE_DL_LIBS})
target_include_directories(${LIBNAME} PUBLIC ${OPENVR_HEADER_DIR})

install(TARGETS ${LIBNAME} DESTINATION lib)
install(FILES ${PUBLIC_HEADER_FILES} DESTINATION include/openvr)

# Generate a .pc file for linux environments
if(PLATFORM_NAME MATCHES "linux")
    set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")
    CONFIGURE_FILE("openvr.pc.in" "openvr.pc" @ONLY)

    set(OPENVR_PC ${CMAKE_CURRENT_BINARY_DIR}/openvr.pc)
    if(NOT SKIP_INSTALL_FILES AND NOT SKIP_INSTALL_ALL )
        install(FILES ${OPENVR_PC} DESTINATION "${INSTALL_PKGCONFIG_DIR}")
    endif()
endif()
