cmake_minimum_required(VERSION 2.8.12)

set(KDEVPYTHON_VERSION_MAJOR 5)
set(KDEVPYTHON_VERSION_MINOR 1)
set(KDEVPYTHON_VERSION_PATCH 2)
set(KDEVPLATFORM_VERSION "${KDEVPYTHON_VERSION_MAJOR}.${KDEVPYTHON_VERSION_MINOR}")

project(kdev_lang_dscript)

find_package(ECM "5.14.0" REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
include(FindSharedMimeInfo)

include(ECMQtDeclareLoggingCategory)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(GenerateExportHeader)

find_package(Qt5Core)
include_directories(${Qt5Core_INCLUDE_DIR})

find_package(KDevelop-PG-Qt REQUIRED)
find_package(FLEX)

set(KF5_DEP_VERSION "5.15.0")
find_package(KF5 ${KF5_DEP_VERSION} REQUIRED
    I18n
    ThreadWeaver
    TextEditor
    ItemModels
)

find_package(KDevPlatform ${KDEVPLATFORM_VERSION} CONFIG REQUIRED)
find_package(KDevelop ${KDEVPLATFORM_VERSION} REQUIRED)

include_directories(
	${KDEVPGQT_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR}
)

add_subdirectory(src)
