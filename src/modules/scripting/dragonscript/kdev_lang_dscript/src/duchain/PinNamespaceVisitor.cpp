#include <language/duchain/types/containertypes.h>
#include <language/duchain/types/unsuretype.h>
#include <language/duchain/types/integraltype.h>
#include <language/duchain/types/typeregister.h>
#include <language/duchain/types/typesystemdata.h>
#include <language/duchain/types/functiontype.h>
#include <language/duchain/declaration.h>
#include <language/duchain/functiondeclaration.h>
#include <language/duchain/classdeclaration.h>
#include <language/duchain/aliasdeclaration.h>
#include <language/duchain/ducontext.h>

#include <QDebug>

#include <KLocalizedString>

#include <functional>

#include "PinNamespaceVisitor.h"
#include "duchainexport.h"
#include "EditorIntegrator.h"
#include "Helpers.h"

using KDevelop::AliasDeclaration;
using KDevelop::ClassDeclaration;
using KDevelop::IntegralType;
using KDevelop::IndexedType;
using KDevelop::StructureType;
using KDevelop::Problem;
using KDevelop::DocumentRange;
using KDevelop::DeclarationPointer;

namespace DragonScript {

PinNamespaceVisitor::PinNamespaceVisitor( const EditorIntegrator &editorIntegrator, const DUContext *ctx ) :
pEditorIntegrator( editorIntegrator ),
pContext( ctx )
{
	Q_ASSERT( ctx );
	Q_ASSERT( ctx->topContext() );
}



void PinNamespaceVisitor::visitFullyQualifiedClassname( FullyQualifiedClassnameAst *node ){
	if( ! node->nameSequence || node->nameSequence->count() == 0 ){
		return;
	}
	
	const KDevPG::ListNode<IdentifierAst*> *iter = node->nameSequence->front();
	const KDevPG::ListNode<IdentifierAst*> *end = iter;
	QVector<IdentifierAst*> nodes;
	QVector<QString> names;
	
	do{
		nodes.append( iter->element );
		names.append( pEditorIntegrator.tokenText( iter->element->name ) );
		iter = iter->next;
	}while( iter != end );
	
	qDebug() << "PinNamespaceVisitor::visitFullyQualifiedClassname:" << names;
	
	findNamespaceIn( pContext, nodes, names, 0 );
}

void PinNamespaceVisitor::findNamespaceIn( const DUContext *searchContext,
const QVector<IdentifierAst*> nodes, const QVector<QString> &names, int index ){
	const bool isLast = index == names.size() - 1;
	
// 	QList<Declaration*> declarations( searchContext->findLocalDeclarations(
// 		names.at( index ), CursorInRevision::invalid() ) );
	QVector<Declaration*> declarations( Helpers::declarationsForName( names.at( index ),
		CursorInRevision::invalid(), DUChainPointer<const DUContext>( searchContext ) ) );
	
	qDebug() << "PinNamespaceVisitor::findNamespaceIn:" << names.at( index ) << "found" << declarations;
	if( declarations.isEmpty() ){
		//qDebug() << "PinNamespaceVisitor::findNamespaceIn:" << names.at( index )
		//	<< "not found in" << searchContext;
		return;
	}
	
	foreach( Declaration *declaration, declarations ){
		DUContext * const context = declaration->internalContext();
		if( ! context ){
			continue;
		}
		
		if( isLast ){
			pNamespaces.append( DUChainPointer<const DUContext>( context ) );
			
		}else{
			findNamespaceIn( context, nodes, names, index + 1 );
		}
	}
}

}
