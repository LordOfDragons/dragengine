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

#include "ExpressionVisitor.h"
#include "duchainexport.h"
#include "EditorIntegrator.h"
#include "Helpers.h"

using KDevelop::AliasDeclaration;
using KDevelop::ClassDeclaration;
using KDevelop::DeclarationPointer;
using KDevelop::IntegralType;
using KDevelop::IndexedType;
using KDevelop::StructureType;
using KDevelop::Problem;
using KDevelop::DocumentRange;

namespace DragonScript {

ExpressionVisitor::ExpressionVisitor( const EditorIntegrator &editorIntegrator, const DUContext *ctx ) :
DynamicLanguageExpressionVisitor( ctx ),
pEditorIntegrator( editorIntegrator )
{
	Q_ASSERT( context() );
	Q_ASSERT( context()->topContext() );
}



void ExpressionVisitor::setAllowVoid( bool allowVoid ){
	pAllowVoid = allowVoid;
}



void ExpressionVisitor::visitExpressionConstant( ExpressionConstantAst *node ){
	const int kind = pEditorIntegrator.session().tokenStream()->at( node->value ).kind;
	KDevelop::AbstractType::Ptr typePtr;
	ClassDeclaration *classDecl;
	DeclarationPointer declPtr;
	
	switch( kind ){
	case TokenTypeWrapper::TokenType::Token_LITERAL_BYTE:
		Helpers::getTypeByte( declPtr, typePtr );
		encounter( typePtr, declPtr );
		break;
		
	case TokenTypeWrapper::TokenType::Token_LITERAL_INTEGER:
		Helpers::getTypeInt( declPtr, typePtr );
		encounter( typePtr, declPtr );
		break;
		
	case TokenTypeWrapper::TokenType::Token_LITERAL_FLOAT:
		Helpers::getTypeFloat( declPtr, typePtr );
		encounter( typePtr, declPtr );
		break;
		
	case TokenTypeWrapper::TokenType::Token_LITERAL_STRING:
		Helpers::getTypeString( declPtr, typePtr );
		encounter( typePtr, declPtr );
		break;
		
	case TokenTypeWrapper::TokenType::Token_TRUE:
	case TokenTypeWrapper::TokenType::Token_FALSE:
		Helpers::getTypeBool( declPtr, typePtr );
		encounter( typePtr, declPtr );
		break;
		
	case TokenTypeWrapper::TokenType::Token_NULL:
		encounter( Helpers::getTypeNull() );
		break;
		
	case TokenTypeWrapper::TokenType::Token_THIS:
		classDecl = Helpers::thisClassDeclFor( DUChainPointer<const DUContext>( context() ) );
		if( classDecl ){
			encounter( classDecl->abstractType(), DeclarationPointer( classDecl ) );
		}
		break;
		
	case TokenTypeWrapper::TokenType::Token_SUPER:
		classDecl = Helpers::superClassDeclFor( DUChainPointer<const DUContext>( context() ) );
		if( classDecl ){
			encounter( classDecl->abstractType(), DeclarationPointer( classDecl ) );
		}
		break;
		
	default:
		// should never happen
		break;
	}
}

void ExpressionVisitor::visitFullyQualifiedClassname( FullyQualifiedClassnameAst *node ){
	if( ! node->nameSequence ){
		return;
	}
	if( node->nameSequence->count() == 0 ){
		qDebug() << "ExpressionVisitor::visitFullyQualifiedClassname: node->nameSequence->count() is 0!!!!!!";
		return;
	}
	
	const KDevPG::ListNode<IdentifierAst*> *iter = node->nameSequence->front();
	const KDevPG::ListNode<IdentifierAst*> *end = iter;
	DUChainPointer<const DUContext> searchContext( context() );
	CursorInRevision findNameBefore( pEditorIntegrator.findPosition( *iter->element, EditorIntegrator::BackEdge ) );
	bool checkForVoid = pAllowVoid;
	
	do{
		/*
		CursorInRevision findNameBefore;
		if( m_scanUntilCursor.isValid() ){
			findNameBefore = m_scanUntilCursor;
			
		}else if( m_forceGlobalSearching ){
			findNameBefore = CursorInRevision::invalid();
			
		}else{
			findNameBefore = pEditorIntegrator.findPosition( *iter->element, EditorIntegrator::BackEdge );
		}*/
		
		const QString name( pEditorIntegrator.tokenText( iter->element->name ) );
		
		Declaration *decl = nullptr;
		
		if( checkForVoid && name == "void" ){
			setLastIsAlias( false );
			encounter( Helpers::getTypeVoid() );
			searchContext = nullptr;
			
		}else{
			if( searchContext ){
				decl = Helpers::declarationForName( name, findNameBefore, searchContext );
			}
			
			if( ! decl ){
				if( m_reportUnknownNames ){
					addUnknownName( name );
				}
				encounterUnknown();
				return;
			}
			
			setLastIsAlias( dynamic_cast<AliasDeclaration*>( decl )
				|| dynamic_cast<ClassDeclaration*>( decl ) );
			encounter( decl->abstractType(), DeclarationPointer( decl ) );
			
			searchContext = decl->internalContext();
		}
		
		checkForVoid = false;
		findNameBefore = CursorInRevision( INT_MAX, INT_MAX );
		iter = iter->next;
	}while( iter != end );
}

void ExpressionVisitor::addUnknownName( const QString& name ){
	if( m_parentVisitor ){
		static_cast<ExpressionVisitor*>( m_parentVisitor )->addUnknownName( name );
		
	}else if( ! m_unknownNames.contains( name ) ){
		m_unknownNames.insert( name );
	}
}

}
