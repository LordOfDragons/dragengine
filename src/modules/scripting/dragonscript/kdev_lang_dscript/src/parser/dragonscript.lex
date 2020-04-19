/*** Definition section ***/
%{
#include <stdio.h>

#include "dsp_lexer.h"

#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) { \
	result = ((DragonScript::Lexer*)yyextra)->nextChar(buf[0]); \
	if( result ){ \
		/*yycolumn++;*/ \
	} \
	}

#undef yyterminate
#define yyterminate() return DragonScript::TokenType::Token_EOF;

#undef YY_USER_ACTION
#define YY_USER_ACTION { \
	yycolumn += yyleng; \
	((DragonScript::Lexer*)yyextra)->advancePosition(yyleng); \
	}

#define YY_NEXT_LINE { \
	yylineno++; \
	yycolumn = 0; \
	}
%}

%option noyywrap
%option reentrant

%x MULTILINE_COMMENT
%x DOC_MULTILINE_COMMENT
%x SINGLELINE_COMMENT
%x DOC_SINGLELINE_COMMENT
%x STRING
%%
	/*** Rules section ***/
"abstract"      { return DragonScript::TokenType::Token_ABSTRACT; }
"and"           { return DragonScript::TokenType::Token_LOGICAL_AND; }
"block"         { return DragonScript::TokenType::Token_BLOCK; }
"break"         { return DragonScript::TokenType::Token_BREAK; }
"case"          { return DragonScript::TokenType::Token_CASE; }
"castable"      { return DragonScript::TokenType::Token_CASTABLE; }
"cast"          { return DragonScript::TokenType::Token_CAST; }
"catch"         { return DragonScript::TokenType::Token_CATCH; }
"class"         { return DragonScript::TokenType::Token_CLASS; }
"continue"      { return DragonScript::TokenType::Token_CONTINUE; }
"downto"        { return DragonScript::TokenType::Token_DOWNTO; }
"elif"          { return DragonScript::TokenType::Token_ELIF; }
"else"          { return DragonScript::TokenType::Token_ELSE; }
"end"           { return DragonScript::TokenType::Token_END; }
"enum"          { return DragonScript::TokenType::Token_ENUM; }
"extends"       { return DragonScript::TokenType::Token_EXTENDS; }
"false"         { return DragonScript::TokenType::Token_FALSE; }
"fixed"         { return DragonScript::TokenType::Token_FIXED; }
"for"           { return DragonScript::TokenType::Token_FOR; }
"func"          { return DragonScript::TokenType::Token_FUNC; }
"if"            { return DragonScript::TokenType::Token_IF; }
"implements"    { return DragonScript::TokenType::Token_IMPLEMENTS; }
"interface"     { return DragonScript::TokenType::Token_INTERFACE; }
"namespace"     { return DragonScript::TokenType::Token_NAMESPACE; }
"native"        { return DragonScript::TokenType::Token_NATIVE; }
"null"          { return DragonScript::TokenType::Token_NULL; }
"not"           { return DragonScript::TokenType::Token_LOGICAL_NOT; }
"or"            { return DragonScript::TokenType::Token_LOGICAL_OR; }
"pin"           { return DragonScript::TokenType::Token_PIN; }
"private"       { return DragonScript::TokenType::Token_PRIVATE; }
"protected"     { return DragonScript::TokenType::Token_PROTECTED; }
"public"        { return DragonScript::TokenType::Token_PUBLIC; }
"requires"      { return DragonScript::TokenType::Token_REQUIRES; }
"return"        { return DragonScript::TokenType::Token_RETURN; }
"select"        { return DragonScript::TokenType::Token_SELECT; }
"static"        { return DragonScript::TokenType::Token_STATIC; }
"step"          { return DragonScript::TokenType::Token_STEP; }
"super"         { return DragonScript::TokenType::Token_SUPER; }
"this"          { return DragonScript::TokenType::Token_THIS; }
"throw"         { return DragonScript::TokenType::Token_THROW; }
"to"            { return DragonScript::TokenType::Token_TO; }
"true"          { return DragonScript::TokenType::Token_TRUE; }
"try"           { return DragonScript::TokenType::Token_TRY; }
"typeof"        { return DragonScript::TokenType::Token_TYPEOF; }
"var"           { return DragonScript::TokenType::Token_VAR; }
"while"         { return DragonScript::TokenType::Token_WHILE; }

"("             { return DragonScript::TokenType::Token_LPAREN; }
")"             { return DragonScript::TokenType::Token_RPAREN; }
"="             { return DragonScript::TokenType::Token_ASSIGN; }
"*="            { return DragonScript::TokenType::Token_ASSIGN_MULTIPLY; }
"/="            { return DragonScript::TokenType::Token_ASSIGN_DIVIDE; }
"%="            { return DragonScript::TokenType::Token_ASSIGN_MODULUS; }
"+="            { return DragonScript::TokenType::Token_ASSIGN_ADD; }
"-="            { return DragonScript::TokenType::Token_ASSIGN_SUBTRACT; }
"<<="           { return DragonScript::TokenType::Token_ASSIGN_SHIFT_LEFT; }
">>="           { return DragonScript::TokenType::Token_ASSIGN_SHIFT_RIGHT; }
"&="            { return DragonScript::TokenType::Token_ASSIGN_AND; }
"|="            { return DragonScript::TokenType::Token_ASSIGN_OR; }
"^="            { return DragonScript::TokenType::Token_ASSIGN_XOR; }
"&"             { return DragonScript::TokenType::Token_AND; }
"|"             { return DragonScript::TokenType::Token_OR; }
"^"             { return DragonScript::TokenType::Token_XOR; }
":"             { return DragonScript::TokenType::Token_COMMAND_SEPARATOR; }
"<<"            { return DragonScript::TokenType::Token_SHIFT_LEFT; }
">>"            { return DragonScript::TokenType::Token_SHIFT_RIGHT; }
"<"             { return DragonScript::TokenType::Token_LESS; }
">"             { return DragonScript::TokenType::Token_GREATER; }
"<="            { return DragonScript::TokenType::Token_LEQUAL; }
">="            { return DragonScript::TokenType::Token_GEQUAL; }
"=="            { return DragonScript::TokenType::Token_EQUALS; }
"!="            { return DragonScript::TokenType::Token_NEQUALS; }
"*"             { return DragonScript::TokenType::Token_MULTIPLY; }
"/"             { return DragonScript::TokenType::Token_DIVIDE; }
"%"             { return DragonScript::TokenType::Token_MODULUS; }
"+"             { return DragonScript::TokenType::Token_ADD; }
"-"             { return DragonScript::TokenType::Token_SUBTRACT; }
"++"            { return DragonScript::TokenType::Token_INCREMENT; }
"--"            { return DragonScript::TokenType::Token_DECREMENT; }
"~"             { return DragonScript::TokenType::Token_INVERSE; }
"."             { return DragonScript::TokenType::Token_PERIOD; }
","             { return DragonScript::TokenType::Token_COMMA; }

[A-Za-z_][A-Za-z0-9_]*   { return DragonScript::TokenType::Token_IDENTIFIER; }

	/*** multine comment ***/
<INITIAL>{
"/*"            BEGIN(MULTILINE_COMMENT);
}
<MULTILINE_COMMENT>{
"*/"            {
                BEGIN(INITIAL);
                return DragonScript::TokenType::Token_COMMENT_MULTILINE;
                }
[^*\n\r]+       // consume comment in chunks
"*"             // consume lone star
\\(\n|\r|\r\n)  { YY_NEXT_LINE; }  // line splicing
(\n|\r|\r\n)    { YY_NEXT_LINE; }
}

	/*** documentation multine comment ***/
<INITIAL>{
"/**"            BEGIN(DOC_MULTILINE_COMMENT);
}
<DOC_MULTILINE_COMMENT>{
"*/"            {
                BEGIN(INITIAL);
                return DragonScript::TokenType::Token_DOC_COMMENT_MULTILINE;
                }
[^*\n\r]+       // consume comment in chunks
"*"             // consume lone star
\\(\n|\r|\r\n)  { YY_NEXT_LINE; }  // line splicing
(\n|\r|\r\n)    { YY_NEXT_LINE; }
}

	/*** single line comment ***/
<INITIAL>{
"//"            BEGIN(SINGLELINE_COMMENT);
}
<SINGLELINE_COMMENT>{
[^\n\r]+       // consume comment in chunks
\\(\n|\r|\r\n)  { YY_NEXT_LINE; }  // line splicing
(\n|\r|\r\n)    {
                YY_NEXT_LINE;
                BEGIN(INITIAL);
                return DragonScript::TokenType::Token_COMMENT_SINGLELINE;
                }
}

	/*** documentation single line comment ***/
<INITIAL>{
"//<<!"         BEGIN(DOC_SINGLELINE_COMMENT);
}
<DOC_SINGLELINE_COMMENT>{
[^\n\r]+       // consume comment in chunks
\\(\n|\r|\r\n)  { YY_NEXT_LINE; }  // line splicing
(\n|\r|\r\n)    {
                YY_NEXT_LINE;
                BEGIN(INITIAL);
                return DragonScript::TokenType::Token_DOC_COMMENT_SINGLELINE;
                }
}

	/*** string ***/
<INITIAL>{
"\""            BEGIN(STRING);
}
<STRING>{
"\""            {
                BEGIN(INITIAL);
                return DragonScript::TokenType::Token_LITERAL_STRING;
                }
\\.             // escaped character
\\x[0-9a-fA-F]{1,2}   // hexadecimal byte
\\[0-7]{1,3}    // octal byte
\\(\n|\r|\r\n)  { YY_NEXT_LINE; }  // line splicing
(\n|\r|\r\n)    { YY_NEXT_LINE; }
}

	/*** character ***/
"'"(.|\\(.|h[0-9a-fA-F]{1,2}|[0-7]{1,3}))"'"   { return DragonScript::TokenType::Token_LITERAL_BYTE; }

	/*** other literals ***/
-?"0b"[01]{1,8}        { return DragonScript::TokenType::Token_LITERAL_INTEGER; }
-?"0o"[0-7]{1,3}       { return DragonScript::TokenType::Token_LITERAL_INTEGER; }
-?"0h"[0-9A-Fa-f]{1,8}    { return DragonScript::TokenType::Token_LITERAL_INTEGER; }
-?[0-9]+           { return DragonScript::TokenType::Token_LITERAL_INTEGER; }
-?"'"(.|\\(.|h[0-9a-fA-F]{1,2}|[0-7]{1,3})){2,4}"'"     { return DragonScript::TokenType::Token_LITERAL_INTEGER; }

-?[0-9]+(\.[0-9]+|e[+\-]?[0-9]+)   { return DragonScript::TokenType::Token_LITERAL_FLOAT; }

[ \t\f]+         { return DragonScript::TokenType::Token_WHITESPACE; }
(\n|\r|\r\n)     { YY_NEXT_LINE; return DragonScript::TokenType::Token_LINEBREAK; }
\\(\n|\r|\r\n)   { YY_NEXT_LINE; return DragonScript::TokenType::Token_LINESPLICE; }

	/*** [^\n]+  { return DragonScript::TokenType::Token_INVALID; } ***/

%%

/*** C Code section ***/
namespace DragonScript {

Lexer::Lexer( Iterator iter ) :
DragonScript::TokenStream(),
Iterator( iter ),
pScanner( nullptr ),
pPosition( 0 )
{
	yylex_init_extra( this, &pScanner );
	
	Q_UNUSED(yyunput)
}

Lexer::~Lexer(){
	if( pScanner ){
		yylex_destroy( pScanner );
	}
}

DragonScript::Token& Lexer::read(){
	const int begin = pPosition;
	const int line = yyget_lineno( pScanner );
	const int column = yyget_column( pScanner );
	
	const int kind = yylex( pScanner );
	
	DragonScript::Token& token( DragonScript::TokenStream::push() );
	token.kind = kind;
	token.begin = begin;
	token.end = pPosition - 1;
	token.line = line - 1;  // flex starts at line 1 but kdevelop at 0
	token.column = column;
	return DragonScript::TokenStream::read();
}

int Lexer::nextChar( char &character ){
	if( Iterator::hasNext() ){
		character = ( char )Iterator::next();
		return 1;
		
	}else{
		return 0;
	}
}

void Lexer::advancePosition( int amount ){
	pPosition += amount;
}

}
