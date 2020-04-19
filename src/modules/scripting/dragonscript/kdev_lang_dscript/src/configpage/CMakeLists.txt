set(configpage_STAT_SRCS
	ProjectConfigPage.cpp
	ProjectConfigPage.h
	projectConfigPage.ui
)

add_library(kdevdsconfigpage SHARED ${configpage_SRCS} ${configpage_STAT_SRCS})

ki18n_wrap_ui(configpage_STAT_SRCS
	projectConfigPage.ui
)

generate_export_header(kdevdsconfigpage EXPORT_MACRO_NAME KDEVDSCONFIGPAGE_EXPORT EXPORT_FILE_NAME configpageexport.h)
target_link_libraries(kdevdsconfigpage LINK_PRIVATE
	KDev::Language
	KDev::Interfaces
	KDev::Project
	kdevdsduchain
	kdevdsparser
)
ecm_qt_declare_logging_category(configpage_SRCS
	HEADER configpagedebug.h
	IDENTIFIER CONFIGPAGE
	CATEGORY_NAME "kdevelop.languages.dragonscript.configpage"
)

target_include_directories(kdevdsconfigpage
	PUBLIC $<INSTALL_INTERFACE:${KDEVDS_PRIVATE_INCLUDE_DIR}/configpage>
	PUBLIC $<INSTALL_INTERFACE:${KDEVDS_PRIVATE_INCLUDE_DIR}>
	PUBLIC $<INSTALL_INTERFACE:${KDEVDS_INCLUDE_DIR}>
)

install(TARGETS kdevdsconfigpage EXPORT KDevDragonScriptTargets DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
