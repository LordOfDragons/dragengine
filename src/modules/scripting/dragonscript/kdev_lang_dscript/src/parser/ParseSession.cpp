#include "ParseSession.h"
#include "DebugAst.h"
#include "TokenText.h"

#include "dsp_ast.h"
#include "dsp_debugvisitor.h"
#include "dsp_tokenstream.h"

#include "kdev-pg-memory-pool.h"
#include "kdev-pg-token-stream.h"
// #include "parserdebug.h"

#include <QFile>
#include <QTextCodec>

#include <KLocalizedString>

#include <interfaces/icore.h>
#include <interfaces/ilanguagecontroller.h>
#include <interfaces/icompletionsettings.h>
#include <language/editor/documentrange.h>

using KDevelop::ICore;

namespace DragonScript{

ParseSession::ParseSession( const IndexedString &filename, const QByteArray &contents ) :
pCurrentDocument( filename ),
pContents( contents ),
pDebug( false ),
pPool( new KDevPG::MemoryPool() ),
pTokenStream( nullptr ){
}

ParseSession::~ParseSession(){
	delete pPool;
	if( pTokenStream ){
		delete pTokenStream;
	}
}

void ParseSession::setDebug( bool debug ){
	pDebug = debug;
}

TokenStream *ParseSession::tokenStream() const{
	return pTokenStream;
}

bool ParseSession::parse( StartAst **ast ){
	if( pTokenStream ){
		delete pTokenStream;
		pTokenStream = nullptr;
	}
	pTokenStream = new TokenStream;
	
	Parser parser;
	parser.setContents( &pContents );
	parser.setTokenStream( pTokenStream );
	parser.setMemoryPool( pPool );
	parser.setDebug( pDebug );
	parser.setCurrentDocument( pCurrentDocument );
	parser.setTodoMarkers( ICore::self()->languageController()->completionSettings()->todoMarkerWords() );
	
	parser.tokenize();
	
	StartAst *dspAst;
	bool matched = parser.parseStart( &dspAst );
	if( matched ) {
		qDebug() << "KDevDScript: ParseSession::parse Successfully parsed";
// 		DebugVisitor( pTokenStream, QString::fromLatin1( pContents ) ).visitStart( dspAst );
// 		DebugAst( *pTokenStream, pContents ).visitStart( dspAst );
		*ast = dspAst;
		
	}else{
		*ast = nullptr;
		parser.expectedSymbol( AstNode::StartKind, QStringLiteral( "start" ) );
		qDebug() << "KDevDScript: ParseSession::parse Couldn't parse content";
	}
	pProblems << parser.problems();
	return matched;
}

CursorInRevision ParseSession::positionAt( qint64 token ) const{
	const Lexer::Token& tok = pTokenStream->at( token );
	return CursorInRevision( tok.line, tok.column );
}

CursorInRevision ParseSession::tokenBegin( qint64 token ) const{
	const Lexer::Token& tok = pTokenStream->at( token );
	return CursorInRevision( tok.line, tok.column );
}

CursorInRevision ParseSession::tokenEnd( qint64 token ) const{
	const Lexer::Token& tok = pTokenStream->at( token );
	return CursorInRevision( tok.line, tok.column + tok.end - tok.begin + 1 );
}

QString ParseSession::documentation( qint64 token ) const{
	const Lexer::Token& tok = pTokenStream->at( token );
	if( tok.docCommentBegin == -1 || tok.docCommentEnd == -1 ){
		return QString();
	}
	
	return pContents.mid( tok.docCommentBegin, tok.docCommentEnd - tok.docCommentBegin + 1 );
}

QString ParseSession::documentation( const AstNode &node ) const{
	return documentation( node.startToken );
}

QString ParseSession::symbol( qint64 token ) const{
	const Lexer::Token& tok = pTokenStream->at(token);
	return pContents.mid( tok.begin, tok.end - tok.begin + 1 );
}

QString ParseSession::symbol( const AstNode &node ) const{
	const Lexer::Token& startTok = pTokenStream->at( node.startToken );
	const Lexer::Token& endTok = pTokenStream->at( node.endToken );
	return pContents.mid( startTok.begin, endTok.end - startTok.begin + 1 );
}

QList<ProblemPointer> ParseSession::problems(){
	return pProblems;
}

}
