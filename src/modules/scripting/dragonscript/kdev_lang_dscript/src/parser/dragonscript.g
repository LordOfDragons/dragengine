[:
#include <QtCore/QRegularExpression>
#include <language/duchain/ducontext.h>
:]

%parser_declaration_header "QString"
%parser_declaration_header "QDebug"
%parser_declaration_header "language/duchain/problem.h"
%parser_declaration_header "dsp_lexer.h"
%parser_declaration_header "dsp_tokenstream.h"

%parser_bits_header "TokenText.h"
%parser_bits_header "parserdebug.h"

%export_macro "KDEVDSPARSER_EXPORT"
%export_macro_header "parserexport.h"

%token_stream TokenStream ;;
%input_encoding "utf8"
%input_stream "KDevPG::QByteArrayIterator"

%namespace
[:
	class Lexer;
	
	enum eTypeModifiers {
		etmNone = 0x0,
		etmPublic = 0x1,
		etmProtected = 0x2,
		etmPrivate = 0x4,
		etmNative = 0x8,
		etmStatic = 0x10,
		etmAbstract = 0x20,
		etmFixed = 0x40
	};
:]

%ast_extra_members
[:
	KDevelop::DUContext* ducontext;
:]


------------------
-- Parser Class --
------------------

%parserclass (public declaration)
[:
enum ProblemType{
	Error,
	Warning,
	Info,
	Todo
};

KDevelop::ProblemPointer reportProblem( Parser::ProblemType type, const QString &message, int tokenOffset = -1 );
inline QList<KDevelop::ProblemPointer> problems(){ return pProblems; }
void setContents( const QByteArray *contents );
void tokenize();
QString tokenText( qint64 begin, qint64 end );
void setDebug( bool debug );
void setCurrentDocument( KDevelop::IndexedString url );
void setTodoMarkers( const QStringList &markers );
void extractTodosFromComment( const QString &comment, qint64 offset );
:]

%parserclass (private declaration)
[:
	const QByteArray *pContents;
	bool pDebug;
	KDevelop::IndexedString pCurrentDocument;
	QList<KDevelop::ProblemPointer> pProblems;
	QRegularExpression pTodoMarkers;
	
	int pLastTypeModifiers;
:]

%parserclass (constructor)
[:
	pContents = nullptr;
:]



--------------------
-- Defined tokens --
--------------------

%token
	LITERAL_INTEGER ("literal-integer"),
	LITERAL_FLOAT ("literal-float"),
	LITERAL_STRING ("literal-string"),
	LITERAL_BYTE ("literal-byte")
	;;

%token
	LPAREN ("("),
	RPAREN (")"),
	ASSIGN ("="),
	ASSIGN_MULTIPLY ("*="),
	ASSIGN_DIVIDE ("/="),
	ASSIGN_MODULUS ("%="),
	ASSIGN_ADD ("+="),
	ASSIGN_SUBTRACT ("-="),
	ASSIGN_SHIFT_LEFT ("<<="),
	ASSIGN_SHIFT_RIGHT (">>="),
	ASSIGN_AND ("&="),
	ASSIGN_OR ("|="),
	ASSIGN_XOR ("^="),
	AND ("&"),
	OR ("|"),
	XOR ("^"),
	COMMAND_SEPARATOR (":"),
	SHIFT_LEFT ("<<"),
	SHIFT_RIGHT (">>"),
	LESS ("<"),
	GREATER (">"),
	LEQUAL ("<="),
	GEQUAL (">="),
	EQUALS ("=="),
	NEQUALS ("!="),
	MULTIPLY ("*"),
	DIVIDE ("/"),
	MODULUS ("%"),
	ADD ("+"),
	SUBTRACT ("-"),
	INCREMENT ("++"),
	DECREMENT ("--"),
	INVERSE ("~"),
	PERIOD ("."),
	COMMA (","),
	LINEBREAK ("line break"),
	LINESPLICE ("line splice"),
	IDENTIFIER ("identifier"),
	WHITESPACE ("whitespace")
	;;

%token
	COMMENT_MULTILINE ("multi line comment"),
	COMMENT_SINGLELINE ("single line comment"),
	DOC_COMMENT_MULTILINE ("doc multi line comment"),
	DOC_COMMENT_SINGLELINE ("doc single line comment")
	;;

%token
	ABSTRACT ("abstract"),
	BLOCK ("block"),
	BREAK ("break"),
	CASE ("case"),
	CASTABLE ("castable"),
	CAST ("cast"),
	CATCH ("catch"),
	CLASS ("class"),
	CONTINUE ("continue"),
	DOWNTO ("downto"),
	ELIF ("elif"),
	ELSE ("else"),
	END ("end"),
	ENUM ("enum"),
	EXTENDS ("extends"),
	FALSE ("false"),
	FIXED ("fixed"),
	FOR ("for"),
	FUNC ("func"),
	IF ("if"),
	IMPLEMENTS ("implements"),
	INTERFACE ("interface"),
	LOGICAL_AND ("and"),
	LOGICAL_NOT ("not"),
	LOGICAL_OR ("or"),
	NAMESPACE ("namespace"),
	NATIVE ("native"),
	NULL ("null"),
	PIN ("pin"),
	PRIVATE ("private"),
	PROTECTED ("protected"),
	PUBLIC ("public"),
	REQUIRES ("requires"),
	RETURN ("return"),
	SELECT ("select"),
	STATIC ("static"),
	STEP ("step"),
	SUPER ("super"),
	THIS ("this"),
	THROW ("throw"),
	TO ("to"),
	TRUE ("true"),
	TRY ("try"),
	TYPEOF ("typeof"),
	VAR ("var"),
	WHILE ("while")
	;;

%token INVALID ("invalid token") ;;


------------
-- Gramar --
------------

-- FIRST/FIRST conflict in classFunctionDeclareBegin:
-- Terminals [
--   IDENTIFIER
-- ]
-- 
-- Happens due to the special function handling ("new", "destructor"). Not a problem due
-- to the manual conditional code.


#script=script*
-> start ;;


-- common stuff --
------------------

PUBLIC      [: pLastTypeModifiers |= ( (*yynode)->modifiers = etmPublic ); :]
| PROTECTED [: pLastTypeModifiers |= ( (*yynode)->modifiers = etmProtected ); :]
| PRIVATE   [: pLastTypeModifiers |= ( (*yynode)->modifiers = etmPrivate ); :]
| ABSTRACT  [: pLastTypeModifiers |= ( (*yynode)->modifiers = etmAbstract ); :]
| FIXED     [: pLastTypeModifiers |= ( (*yynode)->modifiers = etmFixed ); :]
| STATIC    [: pLastTypeModifiers |= ( (*yynode)->modifiers = etmStatic ); :]
| NATIVE    [: pLastTypeModifiers |= ( (*yynode)->modifiers = etmNative ); :]
-> typeModifier [
	member variable modifiers: int;
] ;;

name=IDENTIFIER
-> identifier ;;

#name=identifier @ PERIOD
-> fullyQualifiedClassname ;;

COMMAND_SEPARATOR | ?comment=COMMENT_SINGLELINE LINEBREAK
-> endOfCommand ;;


-- script level --
------------------

#modifiers=typeModifier* ( aclass=class | interface=interface )
-> scriptDeclaration ;;

requires=requires | anamespace=namespace | pin=pin | declaration=scriptDeclaration | empty=endOfCommand
-> script ;;

REQUIRES name=LITERAL_STRING endOfCommand
-> requires ;;

NAMESPACE name=fullyQualifiedClassname endOfCommand
-> namespace ;;

PIN name=fullyQualifiedClassname endOfCommand
-> pin ;;


-- class level --
-----------------

CLASS name=identifier
	?( EXTENDS extends=fullyQualifiedClassname )
	?( IMPLEMENTS #implements=fullyQualifiedClassname
		( COMMA #implements=fullyQualifiedClassname )* )
	endOfCommand
-> classBegin ;;

[: pLastTypeModifiers = 0; :] #modifiers=typeModifier*
	( aclass=class
	| interface=interface
	| enumeration=enumeration
	| function=classFunctionDeclare
	| variable=classVariablesDeclare
	)
-> classBodyDeclaration ;;

FUNC begin=classFunctionDeclareBegin
	-- for non-abstract functions only
	[: if( ( pLastTypeModifiers & etmAbstract ) != etmAbstract ){ :]
	#body=statement*
	end=classFunctionEnd
	[: } :]
-> classFunctionDeclare ;;

-- NOTE endOfCommand has to be duplicated with all cases. this is due to the grammar compiler
--      having some bug. if the endOfCommand is located at the end of the rule so it affects
--      all cases it is not matched by the grammar compiler. this results in the rule being
--      accepted but the endOfCommand is nullptr.
-- 
-- NOTE the grammar compiler fucks up major league if conditional user code is used with a
--      fixed token in front of it. so for example this never works:
--      FUNC ( (condition part1) | (condition part2) | part3)
--      you need to move the FUNC to the outer rule and start with a full OR-part right at
--      the root of the rule. what a mess

-- constructor
( ?[: (LA(1).kind == Token_IDENTIFIER && tokenText(LA(1).begin, LA(1).end) == "new") :]
	name=identifier
	LPAREN ?( #arguments=classFunctionDeclareArgument @ COMMA ) RPAREN
	?(
		( super=THIS | super=SUPER ) LPAREN ?( #superArguments=expression @ COMMA ) RPAREN
	)
	endOfCommand
)

-- destructor
| ( ?[: (LA(1).kind == Token_IDENTIFIER && tokenText(LA(1).begin, LA(1).end) == "destructor") :]
	name=identifier LPAREN RPAREN endOfCommand
)

-- regular function
| (
	type=fullyQualifiedClassname
	( name=identifier | op=operatorFunctionName )
	LPAREN ?( #arguments=classFunctionDeclareArgument @ COMMA ) RPAREN
	endOfCommand
)
-> classFunctionDeclareBegin ;;

op=ASSIGN_MULTIPLY | op=ASSIGN_DIVIDE | op=ASSIGN_MODULUS
| op=ASSIGN_ADD | op=ASSIGN_SUBTRACT | op=ASSIGN_SHIFT_LEFT | op=ASSIGN_SHIFT_RIGHT
| op=ASSIGN_AND | op=ASSIGN_OR | op=ASSIGN_XOR | op=AND | op=OR | op=XOR
| op=SHIFT_LEFT | op=SHIFT_RIGHT | op=LESS | op=GREATER | op=LEQUAL | op=GEQUAL
| op=MULTIPLY | op=DIVIDE | op=MODULUS | op=ADD | op=SUBTRACT | op=INCREMENT
| op=DECREMENT | op=INVERSE
-> operatorFunctionName ;;

type=fullyQualifiedClassname name=identifier
-> classFunctionDeclareArgument ;;

END endOfCommand
-> classFunctionEnd ;;

VAR type=fullyQualifiedClassname
	( #variables=classVariableDeclare @ COMMA )
	endOfCommand
-> classVariablesDeclare ;;

name=identifier ?( ASSIGN value=expression )
-> classVariableDeclare ;;

declaration=classBodyDeclaration | empty=endOfCommand
-> classBody ;;

END endOfCommand
-> classEnd ;;

begin=classBegin #body=classBody* end=classEnd
-> class ;;


-- interface level --
---------------------

INTERFACE name=identifier
	?( IMPLEMENTS #implements=fullyQualifiedClassname
		( COMMA #implements=fullyQualifiedClassname )* )
	endOfCommand
-> interfaceBegin ;;

#modifiers=typeModifier* ( aclass=class | interface=interface | enumeration=enumeration )
-> interfaceBodyDeclaration ;;

interfaceBodyDeclaration | empty=endOfCommand
-> interfaceBody ;;

END endOfCommand
-> interfaceEnd ;;

begin=interfaceBegin #body=interfaceBody* end=interfaceEnd
-> interface ;;


-- enumeration level --
-----------------------

ENUM name=identifier endOfCommand
-> enumerationBegin ;;

name=identifier ?( ASSIGN value=expression ) endOfCommand
-> enumerationBody ;;

END endOfCommand
-> enumerationEnd ;;

begin=enumerationBegin #body=enumerationBody* end=enumerationEnd
-> enumeration ;;


-- statement level --
---------------------

sif=statementIf
| sreturn=statementReturn
| sselect=statementSelect
| swhile=statementWhile
| sfor=statementFor
| sbreak=BREAK
| scontinue=CONTINUE
| sthrow=statementThrow
| stry=statementTry
| vardef=statementVariableDefinitions
| ?expression=expression endOfCommand
-> statement ;;

IF condition=expression endOfCommand #body=statement*
	#elif=statementElif*
	?( ELSE #else=statement* )
	END endOfCommand
-> statementIf ;;

ELIF condition=expression endOfCommand #body=statement*
-> statementElif ;;

RETURN ?value=expression endOfCommand
-> statementReturn ;;

SELECT value=expression endOfCommand
	#case=statementCase*
	?( ELSE #else=statement* )
	END endOfCommand
-> statementSelect ;;

CASE ( #matches=expression @ COMMA ) endOfCommand #body=statement*
-> statementCase ;;

WHILE condition=expression endOfCommand #body=statement* END endOfCommand
-> statementWhile ;;

FOR variable=expressionObject ASSIGN from=expression
	( TO to=expression | DOWNTO downto=expression )
	?( STEP step=expression ) endOfCommand
	#body=statement*
	END endOfCommand
-> statementFor ;;

THROW ?exception=expression endOfCommand
-> statementThrow ;;

TRY endOfCommand
	#body=statement*
	#catches=statementCatch*
	END endOfCommand
-> statementTry ;;

CATCH type=fullyQualifiedClassname variable=identifier endOfCommand
	#body=statement*
-> statementCatch ;;

VAR type=fullyQualifiedClassname ( #variables=statementVariableDefinition @ COMMA ) endOfCommand
-> statementVariableDefinitions ;;

name=identifier ?( ASSIGN value=expression )
-> statementVariableDefinition ;;


-- expression level --
----------------------
--
-- operator precedence (lowest to highest):
-- right:             =, *=, /=, %=, +=, -=, <<=, >>=, &=, |=, ^=
-- left:              inline if-else
-- left:              and, or
-- left:              <, >, <=, >=, ==, !=
-- left:              <<, >>, &, |, ^
-- left:              +, -
-- left:              *, /, %
-- non-associative:   cast <type>, castable <type>, typeof <type>
-- left:              postfix ++, postfix --
-- right:             prefix ++, prefix --, prefix -, prefix ~ (bitwise negate), not (logical not)
-- left:              . (class member)

expression=expressionAssign
-> expression ;;

left=expressionInlineIfElse #more=expressionAssignMore*
-> expressionAssign ;;

op=expressionAssignOp right=expressionInlineIfElse
-> expressionAssignMore ;;

op=ASSIGN | op=ASSIGN_MULTIPLY | op=ASSIGN_DIVIDE | op=ASSIGN_MODULUS
	| op=ASSIGN_ADD | op=ASSIGN_SUBTRACT | op=ASSIGN_SHIFT_LEFT
	| op=ASSIGN_SHIFT_RIGHT | op=ASSIGN_AND | op=ASSIGN_OR | Op=ASSIGN_XOR
-> expressionAssignOp ;;

condition=expressionLogic ?more=expressionInlineIfElseMore
-> expressionInlineIfElse ;;

IF expressionIf=expressionLogic ELSE expressionElse=expressionLogic
-> expressionInlineIfElseMore ;;

left=expressionCompare #more=expressionLogicMore*
-> expressionLogic ;;

op=expressionLogicOp right=expressionCompare
-> expressionLogicMore ;;

op=LOGICAL_AND | op=LOGICAL_OR
-> expressionLogicOp ;;

left=expressionBitOperation #more=expressionCompareMore*
-> expressionCompare ;;

op=expressionCompareOp right=expressionBitOperation
-> expressionCompareMore ;;

op=LESS | op=GREATER | op=LEQUAL | op=GEQUAL | op=EQUALS | op=NEQUALS
-> expressionCompareOp ;;

left=expressionAddition #more=expressionBitOperationMore*
-> expressionBitOperation ;;

op=expressionBitOperationOp right=expressionAddition
-> expressionBitOperationMore ;;

op=SHIFT_LEFT | op=SHIFT_RIGHT | op=AND | op=OR | op=XOR
-> expressionBitOperationOp ;;

left=expressionMultiply #more=expressionAdditionMore*
-> expressionAddition ;;

op=expressionAdditionOp right=expressionMultiply
-> expressionAdditionMore ;;

op=ADD | op=SUBTRACT
-> expressionAdditionOp ;;

left=expressionSpecial #more=expressionMultiplyMore*
-> expressionMultiply ;;

op=expressionMultiplyOp right=expressionSpecial
-> expressionMultiplyMore ;;

op=MULTIPLY | op=DIVIDE | op=MODULUS
-> expressionMultiplyOp ;;

left=expressionPostfix #more=expressionSpecialMore*
-> expressionSpecial ;;

op=expressionSpecialOp type=fullyQualifiedClassname
-> expressionSpecialMore ;;

op=CAST | op=CASTABLE | op=TYPEOF
-> expressionSpecialOp ;;

left=expressionUnary #op=expressionPostfixOp*
-> expressionPostfix ;;

op=INCREMENT | op=DECREMENT
-> expressionPostfixOp ;;

#op=expressionUnaryOp* right=expressionObject
-> expressionUnary ;;

op=INCREMENT | op=DECREMENT | op=SUBTRACT | op=INVERSE | op=LOGICAL_NOT
-> expressionUnaryOp ;;

object=expressionBaseObject ( PERIOD #member=expressionMember )*
-> expressionObject ;;

group=expressionGroup | constant=expressionConstant | member=expressionMember | block=expressionBlock
-> expressionBaseObject ;;

LPAREN expression=expression RPAREN
-> expressionGroup ;;

value=LITERAL_BYTE | value=LITERAL_INTEGER | value=LITERAL_FLOAT | value=LITERAL_STRING
| value=TRUE | value=FALSE | value=NULL | value=THIS | value=SUPER
-> expressionConstant ;;

name=identifier ?( funcCall=LPAREN ?( #arguments=expression @ COMMA ) RPAREN )
-> expressionMember ;;



-- block level --
-----------------

begin=expressionBlockBegin #body=statement* end=expressionBlockEnd
-> expressionBlock ;;

BLOCK ?( #arguments=expressionBlockArgument @ COMMA )
	endOfCommand
-> expressionBlockBegin ;;

END
-> expressionBlockEnd ;;

type=fullyQualifiedClassname name=identifier
-> expressionBlockArgument ;;


-----------------------------------------------------------------
-- Code segments copied to the implementation (.cpp) file.
-- If existent, kdevelop-pg's current syntax requires this block
-- to occur at the end of the file.
-----------------------------------------------------------------

[:
#include <QtCore/QDebug>
#include <language/editor/documentrange.h>

namespace DragonScript{

void Parser::setContents( const QByteArray *contents ){
	pContents = contents;
}

void Parser::tokenize(){
	const DragonScript::TokenStream::Token *token;
	Lexer lexer( *pContents );
	
	int lastDocCommentBegin = -1;
	int lastDocCommentEnd = -1;
	
	while( true ){
		bool skipToken = true;
		
		token = &lexer.read();
		
		while( skipToken ){
			switch( token->kind ){
			case TokenType::Token_WHITESPACE:
			case TokenType::Token_LINESPLICE:
				break;
				
			case TokenType::Token_COMMENT_MULTILINE:
			case TokenType::Token_COMMENT_SINGLELINE:
				extractTodosFromComment( tokenText( token->begin, token->end ), token->begin );
				lastDocCommentBegin = -1;
				lastDocCommentEnd = -1;
				break;
				
			case TokenType::Token_DOC_COMMENT_MULTILINE:
			case TokenType::Token_DOC_COMMENT_SINGLELINE:
				extractTodosFromComment( tokenText( token->begin, token->end ), token->begin );
				lastDocCommentBegin = token->begin;
				lastDocCommentEnd = token->end;
				break;
				
			default:
				skipToken = false;
				break;
			}
			
			if( skipToken ){
				token = &lexer.read();
			}
		}
		
		Parser::Token &t = tokenStream->push();
		t.begin = token->begin;
		t.end = token->end;
		t.kind = token->kind;
		t.line = token->line;
		t.column = token->column;
		t.docCommentBegin = lastDocCommentBegin;
		t.docCommentEnd = lastDocCommentEnd;
		
		if( pDebug ){
			qDebug() << "KDevDScript: Token kind="
				<< TokenText( t.kind ) << t.kind
				<< "begin=" << t.begin << "end=" << t.end
				<< QString::fromLatin1( pContents->mid( t.begin, t.end - t.begin + 1 ) );
		}
		
		if( t.kind == TokenType::Token_EOF ){
			break;
		}
	};
	
	yylex(); // produce the look ahead token
}

void Parser::extractTodosFromComment( const QString &comment, qint64 startPosition ){
	// this seems to be total crap. needs to be verified with the source plugin why this
	// mess should work at all. disabled until then
	#if 0
	if( startPosition < 0 ){
		qDebug() << "extractTodosFromComment" << startPosition;
	}
	auto i = pTodoMarkers.globalMatch( comment );
	while( i.hasNext() ){
		auto match = i.next();
		auto p = reportProblem( Todo, match.captured( 1 ), 0 );
		if( ! p ){
			continue;
		}
		
		const Lexer::Token& tok = tokenStream->at( startPosition );
		
		auto location = p->finalLocation();
		location.setStart(KTextEditor::Cursor( tok.line, tok.column + match.capturedStart( 1 ) ) );
		location.setEnd( KTextEditor::Cursor( tok.line, tok.column + match.capturedEnd( 1 ) ) );
		p->setFinalLocation( location );
	};
	#endif
}

void Parser::setTodoMarkers( const QStringList &markers ){
	QString pattern = QStringLiteral( "^(?:[/\\*\\s]*)(.*(?:" );
	bool first = true;
	foreach( const QString &marker, markers ){
		if( ! first ){
			pattern += '|';
		}
		pattern += QRegularExpression::escape( marker );
		first = false;
	}
	pattern += QStringLiteral( ").*?)(?:[/\\*\\s]*)$" );
	pTodoMarkers.setPatternOptions( QRegularExpression::MultilineOption );
	pTodoMarkers.setPattern( pattern );
}

QString Parser::tokenText( qint64 begin, qint64 end ){
	if( pContents ){
		return QString::fromUtf8( pContents->mid( begin, end - begin + 1 ) );
		
	}else{
		return "";
	}
}

KDevelop::ProblemPointer Parser::reportProblem( Parser::ProblemType type, const QString &message, int offset ){
	qint64 sLine;
	qint64 sCol;
	qint64 index = tokenStream->index() + offset;
	
	if( index >= tokenStream->size() ){
		return {};
	}
	
	tokenStream->startPosition( index, &sLine, &sCol );
	qint64 eLine;
	qint64 eCol;
	tokenStream->endPosition( index, &eLine, &eCol );
	
	auto p = KDevelop::ProblemPointer( new KDevelop::Problem() );
	p->setSource( KDevelop::IProblem::Parser );
	switch( type ){
	case Error:
		p->setSeverity( KDevelop::IProblem::Error );
		break;
		
	case Warning:
		p->setSeverity( KDevelop::IProblem::Warning );
		break;
		
	case Info:
		p->setSeverity( KDevelop::IProblem::Hint );
		break;
		
	case Todo:
		p->setSeverity( KDevelop::IProblem::Hint );
		p->setSource( KDevelop::IProblem::ToDo );
		break;
	}
	
	p->setDescription( message );
	KTextEditor::Range range( sLine, sCol, eLine, eCol + 1 );
	p->setFinalLocation( KDevelop::DocumentRange( pCurrentDocument, range ) );
	pProblems << p;
	return p;
}

// custom error recovery
void Parser::expectedToken( int expected, qint64 where, const QString &name ){
	Q_UNUSED(expected)
	Q_UNUSED(where)
	reportProblem( Parser::Error, QStringLiteral( "Expected token \"%1\"" ).arg( name ) );
}

void Parser::expectedSymbol( int expectedSymbol, const QString &name ){
	Q_UNUSED(expectedSymbol)
	
	qint64 line;
	qint64 col;
	qint64 index = tokenStream->index() - 1;
	if( index == -1 ){
		qDebug() << "KDevDScript: Parser::expectedSymbol token" << name << "but index is -1!";
		return;
	}
	Token &token = tokenStream->at( index );
	
	qDebug() << "KDevDScript: Parser::expectedSymbol token " << name << "starts at:" << token.begin << "index is:" << index;
	tokenStream->startPosition( index, &line, &col );
	QString tokenValue = tokenText( token.begin, token.end );
	qint64 eLine;
	qint64 eCol;
	tokenStream->endPosition( index, &eLine, &eCol );
	reportProblem( Parser::Error,
		QStringLiteral( "Expected symbol \"%1\" (current token: \"%2\" [%3] at %4:%5 - %6:%7)" )
			.arg( name, token.kind != 0 ? tokenValue : QStringLiteral( "EOF" ) )
			.arg( token.kind )
			.arg( line )
			.arg( col )
			.arg( eLine )
			.arg( eCol ) );
}

void Parser::setDebug( bool debug ){
	pDebug = debug;
}

void Parser::setCurrentDocument( KDevelop::IndexedString url ){
	pCurrentDocument = url;
}

}

:]
