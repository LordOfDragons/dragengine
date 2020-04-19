#include <ktexteditor/document.h>
#include <language/editor/documentrange.h>

#include "EditorIntegrator.h"
#include "dsp_ast.h"


using namespace KTextEditor;

namespace DragonScript{

EditorIntegrator::EditorIntegrator( ParseSession &session ) :
pSession( session ){
}



QString EditorIntegrator::tokenText( qint64 token ) const{
	return pSession.symbol( token );
}

QString EditorIntegrator::tokenText( const AstNode &node ) const{
	return pSession.symbol( node );
}

CursorInRevision EditorIntegrator::findPosition( qint64 token, Edge edge ) const{
	if( edge == BackEdge ){
		return pSession.tokenEnd( token );
		
	}else{
		return pSession.tokenBegin( token );
	}
}

CursorInRevision EditorIntegrator::findPosition( const AstNode &node , Edge edge ) const{
	if( edge == BackEdge ){
		return pSession.tokenEnd( node.endToken );
		
	}else{
		return pSession.tokenBegin( node.startToken );
	}
}

RangeInRevision EditorIntegrator::findRange( qint64 token ) const{
	return RangeInRevision( findPosition( token, FrontEdge ), findPosition( token, BackEdge ) );
}

RangeInRevision EditorIntegrator::findRange( const AstNode &node, RangeEdge edge ) const{
	Q_UNUSED( edge );
	return RangeInRevision( findPosition( node, FrontEdge ), findPosition( node, BackEdge ) );
}

RangeInRevision EditorIntegrator::findRange( const AstNode &from, const AstNode &to ) const{
    return RangeInRevision( findPosition( from, FrontEdge ), findPosition( to, BackEdge ) );
}

}
