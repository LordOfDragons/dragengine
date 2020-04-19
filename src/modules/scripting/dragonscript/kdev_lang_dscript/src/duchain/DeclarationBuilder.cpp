#include <language/duchain/namespacealiasdeclaration.h>
#include <language/duchain/aliasdeclaration.h>
#include <language/duchain/classdeclaration.h>
#include <language/duchain/types/functiontype.h>

#include <QByteArray>
#include <QtGlobal>
#include <QDebug>

#include <functional>

#include "DeclarationBuilder.h"
#include "EditorIntegrator.h"
#include "ExpressionVisitor.h"
#include "PinNamespaceVisitor.h"
#include "Helpers.h"

#include "../parser/ParseSession.h"

using KDevelop::AbstractType;
using KDevelop::AliasDeclaration;
using KDevelop::ClassDeclaration;
using KDevelop::DUChainWriteLocker;
using KDevelop::FunctionType;
using KDevelop::Identifier;
using KDevelop::NamespaceAliasDeclaration;
using KDevelop::StructureType;
using KDevelop::BaseClassInstance;
using KDevelop::DUChainReadLocker;
using KDevelop::ClassMemberDeclaration;
using KDevelop::ClassFunctionDeclaration;


namespace DragonScript{

DeclarationBuilder::DeclarationBuilder( EditorIntegrator &editor,
	int ownPriority, const ParseSession &parseSession ) :
DeclarationBuilderBase(),
pParseSession( parseSession )
{
	Q_UNUSED( ownPriority );
	
	setEditor( &editor );
}

DeclarationBuilder::~DeclarationBuilder(){
	/*
	if ( ! m_scheduledForDeletion.isEmpty() ){
		DUChainWriteLocker lock;
		foreach( DUChainBase *d, m_scheduledForDeletion ){
			delete d;
		}
		m_scheduledForDeletion.clear();
	}
	*/
}

ReferencedTopDUContext DeclarationBuilder::build( const IndexedString &url,
AstNode *node, ReferencedTopDUContext updateContext ){
	qDebug() << "KDevDScript: DeclarationBuilder::build";
	pPinned.clear();
	return DeclarationBuilderBase::build( url, node, updateContext );
}

void DeclarationBuilder::closeNamespaceContexts(){
// 	qDebug() << "KDevDScript: DeclarationBuilder::closeNamespaceContexts" << pNamespaceContexts.size();
	// for each namespace component add a context
	DUChainWriteLocker lock;
	
	while( ! pNamespaceContexts.isEmpty() ){
		if( pNamespaceContexts.last() ){
			closeContext();
			//m_currentClassTypes.removeLast();
			closeType();
		}
		closeDeclaration();
		pNamespaceContexts.removeLast();
	}
}

QList<Declaration*> DeclarationBuilder::existingDeclarationsForNode( IdentifierAst *node ){
	return currentContext()->findDeclarations( identifierForNode( node ).last(),
		CursorInRevision::invalid(), nullptr, ( DUContext::SearchFlag )
			( DUContext::DontSearchInParent | DUContext::DontResolveAliases ) );
}

QString DeclarationBuilder::getDocumentationForNode( const AstNode &node ) const{
	// TODO process the documentation string to convert it to a user readable form
	return pParseSession.documentation( node );
}



void DeclarationBuilder::visitScript( ScriptAst *node ){
	pLastModifiers = etmPublic;
	DeclarationBuilderBase::visitScript( node );
	closeNamespaceContexts();
}

void DeclarationBuilder::visitScriptDeclaration( ScriptDeclarationAst *node ){
	pLastModifiers = etmNone;
	DeclarationBuilderBase::visitScriptDeclaration( node );
}

void DeclarationBuilder::visitPin( PinAst *node ){
	DUChainReadLocker lock;
	
	PinNamespaceVisitor pinvisitor( *editor(), currentContext() );
	if( node->name ){
		pinvisitor.visitFullyQualifiedClassname( node->name );
		
		const QVector<DUChainPointer<const DUContext>> &namespaces = pinvisitor.namespaces();
		foreach( const DUChainPointer<const DUContext> &each, namespaces ){
			qDebug() << "KDevDScript: DeclarationBuilder::visitPin add" << each.operator->();
			pPinned.append( each );
		}
	}
}

void DeclarationBuilder::visitRequires( RequiresAst *node ){
	// this would trigger loading additional scripts but how to handle?
}

void DeclarationBuilder::visitNamespace( NamespaceAst *node ){
// 	qDebug() << "KDevDScript: DeclarationBuilder::visitNamespace";
	closeNamespaceContexts();
	
	// for each namespace component add a context
	const KDevPG::ListNode<IdentifierAst*> *iter = node->name->nameSequence->front();
	const KDevPG::ListNode<IdentifierAst*> *end = iter;
	
	DUChainWriteLocker lock;
	do{
		const RangeInRevision range( editorFindRangeNode( iter->element ) );
		QualifiedIdentifier identifier( identifierForNode( iter->element ) );
		
		StructureType::Ptr type( new StructureType() );
		
// 		AliasDeclaration *aliasDecl = nullptr;
		ClassDeclaration *decl = nullptr;
		bool newDecl = false;
		
#if 0
		foreach( Declaration* each, existingDeclarationsForNode( iter->element ) ){
			decl = dynamic_cast<ClassDeclaration*>( each );
			if( ! decl ){
				continue;
			}
			
			if( decl->topContext() != currentContext()->topContext() ){
				qDebug() << "KDevDScript: DeclarationBuilder::visitNamespace: "
					<< "Not opening previously existing declaration "
					<< "because it's in another top context";
				decl = nullptr;
				continue;
			}
			
			qDebug() << "KDevDScript: DeclarationBuilder::visitNamespace: "
				<< "Re-open context" << decl->toString();
			
				/*
			aliasDecl = openDeclaration<AliasDeclaration>(
				iter->element, iter->element, DeclarationFlags::NoFlags );
			aliasDecl->setAliasedDeclaration( decl );
			aliasDecl->setAlwaysForceDirect( true );
			
			eventuallyAssignInternalContext();
			
			aliasDecl->setKind( KDevelop::Declaration::Namespace );
			//aliasDecl->setComment(docstring);
			
			openAbstractType( decl->abstractType() );
			//m_currentClassTypes.append( type );
			
			openContext( iter->element, range, DUContext::Namespace, iter->element );
			currentContext()->setLocalScopeIdentifier( identifier );
			aliasDecl->setInternalContext( currentContext() );
			*/
			
			break;
			
			/*
			if( decl->topContext() == currentContext()->topContext() ){
				qDebug() << "KDevDScript: DeclarationBuilder::visitNamespace: "
					<< "Re-open context" << decl->toString();
				openDeclarationInternal( decl );
				decl->setRange( range );
				setEncountered( decl );
				
				openAbstractType( decl->abstractType() );
				
				openContext( iter->element, range, DUContext::Namespace, iter->element );
				currentContext()->setLocalScopeIdentifier( identifier );
				//decl->setInternalContext( currentContext() );
				
				break;
				
			}else{
				qDebug() << "KDevDScript: DeclarationBuilder::visitNamespace: "
					<< "Not opening previously existing declaration "
					<< "because it's in another top context";
				decl = nullptr;
			}
			*/
		}
		
		if( decl ){
			continue;
		}
#endif
		
		if( ! decl ){
			decl = openDeclaration<ClassDeclaration>( iter->element, iter->element,
				DeclarationFlags::NoFlags );
// 			decl->setAlwaysForceDirect( true );
			newDecl = true;
			
			eventuallyAssignInternalContext();
			
			decl->setKind( KDevelop::Declaration::Namespace );
			decl->setComment( getDocumentationForNode( *node ) );
			
			type->setDeclaration( decl );
			decl->setType( type );
			
			openType( type );
			//m_currentClassTypes.append( type );
			
			openContext( iter->element, range, DUContext::Namespace, iter->element );
			currentContext()->setLocalScopeIdentifier( identifier );
			decl->setInternalContext( currentContext() );
		}
		
		iter = iter->next;
// 		qDebug() << "KDevDScript: DeclarationBuilder::visitNamespace push back" << newDecl;
		pNamespaceContexts.push_back( newDecl );
		
	}while( iter != end );
}

void DeclarationBuilder::visitClass( ClassAst *node ){
// 	qDebug() << "KDevDScript: DeclarationBuilder::visitClass";
	
	DUChainWriteLocker lock;
	
	StructureType::Ptr type( new StructureType() );
	
	ClassDeclaration * const decl = openDeclaration<ClassDeclaration>( node->begin->name,
		node->begin->name, DeclarationFlags::NoFlags );
	decl->setAlwaysForceDirect( true );
	
	eventuallyAssignInternalContext();
	
	decl->setKind( KDevelop::Declaration::Type );
	decl->setComment( getDocumentationForNode( *node ) );
	decl->clearBaseClasses();
	decl->setClassType( KDevelop::ClassDeclarationData::Class );
	decl->setAccessPolicy( accessPolicyFromLastModifiers() );
	
	if( ( pLastModifiers & etmFixed ) == etmFixed ){
		decl->setClassModifier( KDevelop::ClassDeclarationData::Final );
		
	}else if( ( pLastModifiers & etmAbstract ) == etmAbstract ){
		decl->setClassModifier( KDevelop::ClassDeclarationData::Abstract );
		
	}else{
		decl->setClassModifier( KDevelop::ClassDeclarationData::None );
	}
	
	// add base class and interfaces
	if( node->begin->extends ){
		ExpressionVisitor exprvisitor( *editor(), currentContext() );
		exprvisitor.visitNode( node->begin->extends );
		
		if( exprvisitor.lastType()
		&& exprvisitor.lastType()->whichType() == KDevelop::AbstractType::TypeStructure ){
			const StructureType::Ptr baseType = exprvisitor.lastType().cast<StructureType>();
			BaseClassInstance base;
			base.baseClass = baseType->indexed();
			base.access = KDevelop::Declaration::Public;
			decl->addBaseClass( base );
		}
		
	}else if( document() != Helpers::getDocumentationFileObject() ){
		// set object as base class. this is only done if this is not the object class from
		// the documentation being parsed
		const AbstractType::Ptr typeObject = Helpers::getTypeObject();
		if( typeObject ){
			BaseClassInstance base;
			base.baseClass = typeObject->indexed();
			base.access = KDevelop::Declaration::Public;
			decl->addBaseClass( base );
		}
	}
	
	if( node->begin->implementsSequence ){
		const KDevPG::ListNode<FullyQualifiedClassnameAst*> *iter = node->begin->implementsSequence->front();
		const KDevPG::ListNode<FullyQualifiedClassnameAst*> *end = iter;
		do{
			ExpressionVisitor exprvisitor( *editor(), currentContext() );
			exprvisitor.visitNode( iter->element );
			if( exprvisitor.lastType()
			&& exprvisitor.lastType()->whichType() == KDevelop::AbstractType::TypeStructure ){
				StructureType::Ptr baseType = exprvisitor.lastType().cast<StructureType>();
				BaseClassInstance base;
				base.baseClass = baseType->indexed();
				base.access = KDevelop::Declaration::Public;
				decl->addBaseClass( base );
			}
			iter = iter->next;
		}while( iter != end );
	}
	
	// continue
	type->setDeclaration( decl );
	decl->setType( type );
	
	openType( type );
	
	openContextClass( node );
	decl->setInternalContext( currentContext() );
	
	lock.unlock();
	DeclarationBuilderBase::visitClass( node );
	lock.lock();
	
	closeContext();
	closeType();
	closeDeclaration();
}

void DeclarationBuilder::visitClassBodyDeclaration( ClassBodyDeclarationAst *node ){
	pLastModifiers = 0;
	DeclarationBuilderBase::visitClassBodyDeclaration( node );
}

void DeclarationBuilder::visitClassVariablesDeclare( ClassVariablesDeclareAst *node ){
	if( ! node->variablesSequence ){
		return;
	}
	
	DUChainWriteLocker lock;
	
	ExpressionVisitor exprType( *editor(), currentContext() );
	exprType.visitNode( node->type );
	AbstractType::Ptr type( exprType.lastType() );
	
	KDevelop::ClassMemberDeclaration::StorageSpecifiers storageSpecifiers;
	if( ( pLastModifiers & etmStatic ) == etmStatic ){
		storageSpecifiers |= KDevelop::ClassMemberDeclaration::StaticSpecifier;
	}
	if( ( pLastModifiers & etmFixed ) == etmFixed ){
		// NOTE existing in an older version of KDevelop but now gone... damn it!
// 		storageSpecifiers |= KDevelop::ClassMemberDeclaration::FinalSpecifier;
	}
	
	const KDevPG::ListNode<ClassVariableDeclareAst*> *iter = node->variablesSequence->front();
	const KDevPG::ListNode<ClassVariableDeclareAst*> *end = iter;
	do{
		ClassMemberDeclaration * const decl = openDeclaration<ClassMemberDeclaration>(
			iter->element->name, iter->element->name, DeclarationFlags::NoFlags );
		decl->setAlwaysForceDirect( true );
		decl->setType( type );
		decl->setKind( KDevelop::Declaration::Instance );
		decl->setComment( getDocumentationForNode( *node ) );
		decl->setAccessPolicy( accessPolicyFromLastModifiers() );
		decl->setStorageSpecifiers( storageSpecifiers );
		
		if( iter->element->value ){
			lock.unlock();
			visitNode( iter->element->value );
			lock.lock();
		}
		
		closeDeclaration();
		
		iter = iter->next;
	}while( iter != end );
}

void DeclarationBuilder::visitClassFunctionDeclare( ClassFunctionDeclareAst *node ){
	DUChainWriteLocker lock;
	
	ClassFunctionDeclaration *decl;
	
	if( node->begin->name ){
		decl = openDeclaration<ClassFunctionDeclaration>( node->begin->name,
			node->begin->name, DeclarationFlags::NoFlags );
		
	}else if( node->begin->op ){
		// WARNING according to documentation of openDeclaration (abstracttypebuilder.h:68)
		//         if name is nullptr range is used. unfortunately this method calls
		//         identifierForNode with name which is nullptr. we could change our
		//         implementation of this method to deal with this but with a nullptr we can
		//         not return the correct qualified identifier. thus we resort to the
		//         Identifier version since the QualifiedIdentifier version seems deprecated
		
		// decl = openDeclaration<ClassFunctionDeclaration>(
		//	nullptr, node->begin->op, DeclarationFlags::NoFlags );
		
		// this is now a huge problem. on some systems the function expects QualifiedIdentifier
		// as argument while on others this is deprecated and Identifier is required. We stick
		// for the time being with the QualifiedIdentifier version until this mess is solved
		/*decl = openDeclaration<ClassFunctionDeclaration>(
			Identifier( editor()->tokenText( *node->begin->op ) ),
			editorFindRangeNode( node->begin->op ), DeclarationFlags::NoFlags );*/
		
		decl = openDeclaration<ClassFunctionDeclaration>(
			QualifiedIdentifier( editor()->tokenText( *node->begin->op ) ),
			editorFindRangeNode( node->begin->op ), DeclarationFlags::NoFlags );
	}
	
	decl->setAlwaysForceDirect( true );
	decl->setKind( KDevelop::Declaration::Instance );
	decl->setStatic( false );  // TODO check type modifiers
	decl->setComment( getDocumentationForNode( *node ) );
	decl->setAccessPolicy( accessPolicyFromLastModifiers() );
	
	KDevelop::ClassMemberDeclaration::StorageSpecifiers storageSpecifiers;
	//storageSpecifiers |= KDevelop::ClassMemberDeclaration::AbstractSpecifier; // virtual?
	if( ( pLastModifiers & etmNative ) == etmNative ){
		// NOTE existing in an older version of KDevelop but now gone... damn it!
// 		storageSpecifiers |= KDevelop::ClassMemberDeclaration::NativeSpecifier;
	}
	if( ( pLastModifiers & etmStatic ) == etmStatic ){
		storageSpecifiers |= KDevelop::ClassMemberDeclaration::StaticSpecifier;
	}
	if( ( pLastModifiers & etmFixed ) == etmFixed ){
		// NOTE existing in an older version of KDevelop but now gone... damn it!
// 		storageSpecifiers |= KDevelop::ClassMemberDeclaration::FinalSpecifier;
	}
	if( ( pLastModifiers & etmAbstract ) == etmAbstract ){
		// NOTE existing in an older version of KDevelop but now gone... damn it!
// 		storageSpecifiers |= KDevelop::ClassMemberDeclaration::AbstractSpecifier; // pure virtual ?
	}
	decl->setStorageSpecifiers( storageSpecifiers );
	
	KDevelop::FunctionDeclaration::FunctionSpecifiers functionSpecifiers;
	functionSpecifiers |= KDevelop::FunctionDeclaration::VirtualSpecifier;
	decl->setFunctionSpecifiers( functionSpecifiers );
	
	FunctionType::Ptr funcType( new FunctionType() );
	
	if( node->begin->type ){
		ExpressionVisitor exprRetType( *editor(), currentContext() );
		exprRetType.setAllowVoid( true );
		exprRetType.visitNode( node->begin->type );
		funcType->setReturnType( exprRetType.lastType() );
		
	}else{
		// used only for constructors. return type is the object class type
		const Declaration * const classDecl = Helpers::classDeclFor(
			DUChainPointer<const DUContext>( decl->context() ) );
		if( classDecl ){
			funcType->setReturnType( classDecl->abstractType() );
		}
	}
	
	openContextClassFunction( node );
	decl->setInternalContext( currentContext() );
	
	if( node->begin->argumentsSequence ){
		const KDevPG::ListNode<ClassFunctionDeclareArgumentAst*> *iter = node->begin->argumentsSequence->front();
		const KDevPG::ListNode<ClassFunctionDeclareArgumentAst*> *end = iter;
		do{
			ExpressionVisitor exprArgType( *editor(), currentContext() );
			exprArgType.visitNode( iter->element->type );
			AbstractType::Ptr argType( exprArgType.lastType() );
			
			funcType->addArgument( argType );
			
			Declaration * const declArg = openDeclaration<Declaration>( iter->element->name,
				iter->element->name, DeclarationFlags::NoFlags );
			declArg->setAlwaysForceDirect( true );
			declArg->setAbstractType( argType );
			declArg->setKind( KDevelop::Declaration::Instance );
			closeDeclaration();
			
			iter = iter->next;
		}while( iter != end );
	}
	
	// assign function type to function declaration. this has to be done after setting return
	// type and arguments in function type otherwise the information will not show up in browsing
	decl->setType( funcType );
	
	lock.unlock();
	DeclarationBuilderBase::visitClassFunctionDeclare( node );
	lock.lock();
	
	closeContext();
	closeDeclaration();
}

void DeclarationBuilder::visitInterface( InterfaceAst *node ){
// 	qDebug() << "KDevDScript: DeclarationBuilder::visitInterface";
	
	DUChainWriteLocker lock;
	
	StructureType::Ptr type( new StructureType() );
	
	ClassDeclaration * const decl = openDeclaration<ClassDeclaration>( node->begin->name,
		node->begin->name, DeclarationFlags::NoFlags );
	decl->setAlwaysForceDirect( true );
	
	eventuallyAssignInternalContext();
	
	decl->setKind( KDevelop::Declaration::Type );
	decl->setComment( getDocumentationForNode( *node ) );
	decl->clearBaseClasses();
	decl->setClassType( KDevelop::ClassDeclarationData::Interface );
	decl->setClassModifier( KDevelop::ClassDeclarationData::Abstract );
	decl->setAccessPolicy( accessPolicyFromLastModifiers() );
	
	// add interfaces
	if( node->begin->implementsSequence ){
		const KDevPG::ListNode<FullyQualifiedClassnameAst*> *iter = node->begin->implementsSequence->front();
		const KDevPG::ListNode<FullyQualifiedClassnameAst*> *end = iter;
		do{
			ExpressionVisitor exprvisitor( *editor(), currentContext() );
			exprvisitor.visitNode( iter->element );
			
			if( exprvisitor.lastType()
			&& exprvisitor.lastType()->whichType() == KDevelop::AbstractType::TypeStructure ){
				const StructureType::Ptr baseType = exprvisitor.lastType().cast<StructureType>();
				BaseClassInstance base;
				base.baseClass = baseType->indexed();
				base.access = KDevelop::Declaration::Public;
				decl->addBaseClass( base );
			}
			iter = iter->next;
		}while( iter != end );
	}
	
	// body
	type->setDeclaration( decl );
	decl->setType( type );
	
	openType( type );
	
	openContextInterface( node );
	decl->setInternalContext( currentContext() );
	
	lock.unlock();
	DeclarationBuilderBase::visitInterface( node );
	lock.lock();
	
	closeContext();
	closeType();
	closeDeclaration();
}

void DeclarationBuilder::visitEnumeration( EnumerationAst *node ){
// 	qDebug() << "KDevDScript: DeclarationBuilder::visitEnumeration";
// 	openContextEnumeration( node );
	pEnumNextValue = 0;
	DefaultVisitor::visitEnumeration( node );
// 	closeContext();
}

void DeclarationBuilder::visitEnumerationBody( EnumerationBodyAst *node ){
	DUChainWriteLocker lock;
	
	ClassMemberDeclaration * const decl = openDeclaration<ClassMemberDeclaration>(
		node->name, node->name, DeclarationFlags::NoFlags );
	decl->setAlwaysForceDirect( true );
	decl->setType( Helpers::getTypeInt() );
	decl->setKind( KDevelop::Declaration::Instance );
	decl->setComment( getDocumentationForNode( *node ) );
	decl->setAccessPolicy( accessPolicyFromLastModifiers() );
	decl->setStorageSpecifiers( KDevelop::ClassMemberDeclaration::StorageSpecifiers()
		| KDevelop::ClassMemberDeclaration::StaticSpecifier );
	// NOTE existing in an older version of KDevelop but now gone... damn it!
// 		| KDevelop::ClassMemberDeclaration::FinalSpecifier );
	
	if( node->value ){
		lock.unlock();
		visitNode( node->value );
		lock.lock();
		
	}else{
		pEnumNextValue++;
	}
	
	closeDeclaration();
}

void DeclarationBuilder::visitExpressionBlock( ExpressionBlockAst *node ){
	DUChainWriteLocker lock;
	
	openContext( node, DUContext::Other, QualifiedIdentifier( "{block}" ) );
	
	if( node->begin->argumentsSequence ){
		const KDevPG::ListNode<ExpressionBlockArgumentAst*> *iter = node->begin->argumentsSequence->front();
		const KDevPG::ListNode<ExpressionBlockArgumentAst*> *end = iter;
		do{
			ExpressionVisitor exprArgType( *editor(), currentContext() );
			exprArgType.visitNode( iter->element->type );
			const AbstractType::Ptr argType( exprArgType.lastType() );
			
			Declaration * const declArg = openDeclaration<Declaration>( iter->element->name,
				iter->element->name, DeclarationFlags::NoFlags );
			declArg->setAlwaysForceDirect( true );
			declArg->setAbstractType( argType );
			declArg->setKind( KDevelop::Declaration::Instance );
			closeDeclaration();
			
			iter = iter->next;
		}while( iter != end );
	}
	
	lock.unlock();
	DefaultVisitor::visitExpressionBlock( node );
	lock.lock();
	
	closeContext();
}

void DeclarationBuilder::visitStatementIf( StatementIfAst *node ){
	// if
	visitNode( node->condition );
	
	if( node->bodySequence ){
		DUChainWriteLocker lock;
		openContext( node, DUContext::Other, QualifiedIdentifier( "{if}" ) );
		lock.unlock();
		
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		lock.lock();
		closeContext();
	}
	
	// elif
	if( node->elifSequence ){
		const KDevPG::ListNode<StatementElifAst*> *iter = node->elifSequence->front();
		const KDevPG::ListNode<StatementElifAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
	
	// else
	if( node->elseSequence ){
		DUChainWriteLocker lock;
		openContext( node, DUContext::Other, QualifiedIdentifier( "{else}" ) );
		lock.unlock();
		
		const KDevPG::ListNode<StatementAst*> *iter = node->elseSequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		lock.lock();
		closeContext();
	}
}

void DeclarationBuilder::visitStatementElif( StatementElifAst *node ){
	visitNode( node->condition );
	
	if( node->bodySequence ){
		DUChainWriteLocker lock;
		openContext( node, DUContext::Other, QualifiedIdentifier( "{elif}" ) );
		lock.unlock();
		
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		lock.lock();
		closeContext();
	}
}

void DeclarationBuilder::visitStatementSelect( StatementSelectAst *node ){
	visitNode( node->value );
	
	// cases
	if( node->caseSequence ){
		const KDevPG::ListNode<StatementCaseAst*> *iter = node->caseSequence->front();
		const KDevPG::ListNode<StatementCaseAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
	
	// else
	if( node->elseSequence ){
		DUChainWriteLocker lock;
		openContext( node, DUContext::Other, QualifiedIdentifier( "{else}" ) );
		lock.unlock();
		
		const KDevPG::ListNode<StatementAst*> *iter = node->elseSequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		lock.lock();
		closeContext();
	}
}

void DeclarationBuilder::visitStatementCase( StatementCaseAst *node ){
	if( node->matchesSequence ){
		const KDevPG::ListNode<ExpressionAst*> *iter = node->matchesSequence->front();
		const KDevPG::ListNode<ExpressionAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
	
	if( node->bodySequence ){
		DUChainWriteLocker lock;
		openContext( node, DUContext::Other, QualifiedIdentifier( "{case}" ) );
		lock.unlock();
		
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		lock.lock();
		closeContext();
	}
}

void DeclarationBuilder::visitStatementFor( StatementForAst *node ){
	visitNode( node->variable );
	visitNode( node->from );
	visitNode( node->to );
	visitNode( node->downto );
	visitNode( node->step );
	
	if( node->bodySequence ){
		DUChainWriteLocker lock;
		openContext( node, DUContext::Other, QualifiedIdentifier( "{for}" ) );
		lock.unlock();
		
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		lock.lock();
		closeContext();
	}
}

void DeclarationBuilder::visitStatementWhile( StatementWhileAst *node ){
	visitNode( node->condition );
	
	if( node->bodySequence ){
		DUChainWriteLocker lock;
		openContext( node, DUContext::Other, QualifiedIdentifier( "{while}" ) );
		lock.unlock();
		
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		lock.lock();
		closeContext();
	}
}

void DeclarationBuilder::visitStatementTry( StatementTryAst *node ){
	if( node->bodySequence ){
		DUChainWriteLocker lock;
		openContext( node, DUContext::Other, QualifiedIdentifier( "{try}" ) );
		lock.unlock();
		
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		lock.lock();
		closeContext();
	}
	
	if( node->catchesSequence ){
		const KDevPG::ListNode<StatementCatchAst*> *iter = node->catchesSequence->front();
		const KDevPG::ListNode<StatementCatchAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
	}
}

void DeclarationBuilder::visitStatementCatch( StatementCatchAst *node ){
	DUChainWriteLocker lock;
	openContext( node, DUContext::Other, QualifiedIdentifier( "{catch}" ) );
	
	if( node->variable ){
		ExpressionVisitor exprType( *editor(), currentContext() );
		exprType.visitNode( node->type );
		const AbstractType::Ptr type( exprType.lastType() );
		
		Declaration * const decl = openDeclaration<Declaration>(
			node->variable, node->variable, DeclarationFlags::NoFlags );
		decl->setAlwaysForceDirect( true );
		decl->setType( type );
		decl->setKind( KDevelop::Declaration::Instance );
		
		closeDeclaration();
	}
	
	if( node->bodySequence ){
		lock.unlock();
		
		const KDevPG::ListNode<StatementAst*> *iter = node->bodySequence->front();
		const KDevPG::ListNode<StatementAst*> *end = iter;
		do{
			visitNode( iter->element );
			iter = iter->next;
		}while( iter != end );
		
		lock.lock();
	}
	
	closeContext();
}

void DeclarationBuilder::visitStatementVariableDefinitions( StatementVariableDefinitionsAst *node ){
	if( ! node->variablesSequence ){
		return;
	}
	
	DUChainWriteLocker lock;
	
	ExpressionVisitor exprType( *editor(), currentContext() );
	exprType.visitNode( node->type );
	AbstractType::Ptr type( exprType.lastType() );
	
	const KDevPG::ListNode<StatementVariableDefinitionAst*> *iter = node->variablesSequence->front();
	const KDevPG::ListNode<StatementVariableDefinitionAst*> *end = iter;
	do{
		Declaration * const decl = openDeclaration<Declaration>(
			iter->element->name, iter->element->name, DeclarationFlags::NoFlags );
		decl->setAlwaysForceDirect( true );
		decl->setType( type );
		decl->setKind( KDevelop::Declaration::Instance );
		
		if( iter->element->value ){
			visitNode( iter->element->value );
		}
		
		closeDeclaration();
		
		iter = iter->next;
	}while( iter != end );
}

void DeclarationBuilder::visitTypeModifier( TypeModifierAst *node ){
	pLastModifiers |= node->modifiers;
}



// Protected Functions
////////////////////////

KDevelop::ClassMemberDeclaration::AccessPolicy DeclarationBuilder::accessPolicyFromLastModifiers() const{
	if( ( pLastModifiers & etmPrivate ) == etmPrivate ){
		return KDevelop::ClassMemberDeclaration::Private;
		
	}else if( ( pLastModifiers & etmProtected ) == etmProtected ){
		return KDevelop::ClassMemberDeclaration::Protected;
		
	}else{
		return KDevelop::ClassMemberDeclaration::Public;
	}
}

}
