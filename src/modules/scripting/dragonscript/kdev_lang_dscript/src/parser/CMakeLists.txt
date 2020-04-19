set(parser_STAT_SRCS
	ParseSession.cpp
	ParseSession.h
	DebugAst.cpp
	DebugAst.h
	dsp_lexer.h
	TokenText.cpp
)

# GENERATE_LEXER
kdevpgqt_generate(parser_SRCS dsp_ NAMESPACE DragonScript DEBUG_VISITOR TOKEN_TEXT
	"${CMAKE_CURRENT_SOURCE_DIR}/dragonscript.g"
)

# add_custom_target(fixGeneratedLexer
# 	#COMMAND sed -i -e "s/exit(-1);//" "${CMAKE_CURRENT_BINARY_DIR}/dsp_lexer.cpp"
# 	COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/CMakeFixLexer.txt"
# 	DEPENDS ${parser_SRCS}
# 	COMMENT "Patching generated lexer (prevent KDevelop Crash)"
# 	VERBATIM
# )

FLEX_TARGET(scanner "${CMAKE_CURRENT_SOURCE_DIR}/dragonscript.lex" "${CMAKE_CURRENT_BINARY_DIR}/dsp_lexer.cpp")

add_library(kdevdsparser SHARED ${parser_SRCS} ${parser_STAT_SRCS} ${FLEX_scanner_OUTPUTS})
# add_dependencies(kdevdsparser fixGeneratedLexer)

generate_export_header(kdevdsparser EXPORT_MACRO_NAME KDEVDSPARSER_EXPORT EXPORT_FILE_NAME parserexport.h)
target_link_libraries(kdevdsparser LINK_PRIVATE
	KDev::Language
	KF5::I18n
)
ecm_qt_declare_logging_category(parser_SRCS
	HEADER parserdebug.h
	IDENTIFIER PARSER
	CATEGORY_NAME "kdevelop.languages.dragonscript.parser"
)

# hack to make phpdebugvisitor.h (generated at compile time)
# use the correct EXPORT tags when compiling in MinGW
if (MINGW)
	add_definitions(-DMAKE_KDEVDSPPARSER_LIB)
endif (MINGW)

target_include_directories(kdevdsparser
	PUBLIC $<INSTALL_INTERFACE:${KDEVDS_PRIVATE_INCLUDE_DIR}/parser>
	PUBLIC $<INSTALL_INTERFACE:${KDEVDS_PRIVATE_INCLUDE_DIR}>
	PUBLIC $<INSTALL_INTERFACE:${KDEVDS_INCLUDE_DIR}>
)

install(TARGETS kdevdsparser EXPORT KDevDragonScriptTargets DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
# install(FILES
# 	ParseSession.h
# 	dsp_lexer.h
# 	DESTINATION ${KDEVDS_INCLUDE_DIR}/parser
# 	COMPONENT Devel
# )

# install(FILES
# 	${CMAKE_CURRENT_BINARY_DIR}/parserexport.h
# 	${CMAKE_CURRENT_BINARY_DIR}/dsp_ast-fwd.h
# 	${CMAKE_CURRENT_BINARY_DIR}/dsp_ast.h
# 	${CMAKE_CURRENT_BINARY_DIR}/dsp_debugvisitor.h
# 	${CMAKE_CURRENT_BINARY_DIR}/dsp_defaultvisitor.h
# 	${CMAKE_CURRENT_BINARY_DIR}/dsp_parser.h
# 	${CMAKE_CURRENT_BINARY_DIR}/dsp_tokentext.h
# 	${CMAKE_CURRENT_BINARY_DIR}/dsp_tokentype.h
# 	${CMAKE_CURRENT_BINARY_DIR}/dsp_visitor.h
# 	DESTINATION ${KDEVDS_PRIVATE_INCLUDE_DIR}/parser
# 	COMPONENT Devel
# )
