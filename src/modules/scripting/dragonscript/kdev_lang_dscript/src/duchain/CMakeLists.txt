set(duchain_STAT_SRCS
	ContextBuilder.cpp
	ContextBuilder.h
	DeclarationBuilder.cpp
	DeclarationBuilder.h
	EditorIntegrator.cpp
	EditorIntegrator.h
	Helpers.cpp
	Helpers.h
	ExpressionVisitor.cpp
	ExpressionVisitor.h
	PinNamespaceVisitor.cpp
	PinNamespaceVisitor.h
	UseBuilder.cpp
	UseBuilder.h
	DumpChain.cpp
	DumpChain.h
)

add_library(kdevdsduchain SHARED ${duchain_SRCS} ${duchain_STAT_SRCS})

generate_export_header(kdevdsduchain EXPORT_MACRO_NAME KDEVDSDUCHAIN_EXPORT EXPORT_FILE_NAME duchainexport.h)
target_link_libraries(kdevdsduchain LINK_PRIVATE
	KDev::Language
	KF5::I18n
	kdevdsparser
)
ecm_qt_declare_logging_category(duchain_SRCS
	HEADER parserdebug.h
	IDENTIFIER PARSER
	CATEGORY_NAME "kdevelop.languages.dragonscript.duchain"
)

target_include_directories(kdevdsduchain
	PUBLIC $<INSTALL_INTERFACE:${KDEVDS_PRIVATE_INCLUDE_DIR}/duchain>
	PUBLIC $<INSTALL_INTERFACE:${KDEVDS_PRIVATE_INCLUDE_DIR}>
	PUBLIC $<INSTALL_INTERFACE:${KDEVDS_INCLUDE_DIR}>
)

install(TARGETS kdevdsduchain EXPORT KDevDragonScriptTargets DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
