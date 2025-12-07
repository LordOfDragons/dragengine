#include <QDebug>

#include <language/duchain/declaration.h>
#include <language/duchain/use.h>
#include <language/duchain/topducontext.h>
#include <language/duchain/duchain.h>
#include <language/duchain/duchainlock.h>
#include <language/duchain/types/structuretype.h>
#include <language/duchain/classfunctiondeclaration.h>
#include <language/duchain/types/functiontype.h>

#include "UseBuilder.h"
#include "ParseSession.h"
#include "EditorIntegrator.h"
#include "ExpressionVisitor.h"
#include "Helpers.h"

using KDevelop::DUChainReadLocker;
using KDevelop::Problem;
using KDevelop::DocumentRange;
using KDevelop::DUChainWriteLocker;
using KDevelop::DUChain;
using KDevelop::DeclarationPointer;
using KDevelop::ClassMemberDeclaration;
using KDevelop::ClassFunctionDeclaration;
using KDevelop::FunctionType;
using KDevelop::StructureType;

namespace DragonScript{

UseBuilder::UseBuilder(EditorIntegrator &editor) :
UseBuilderBase(),
pParseSession(*editor.parseSession()),
pEnableErrorReporting(true),
pAllowVoidType(false)
{
	setEditor(&editor);
}



void UseBuilder::setEnableErrorReporting(bool enable){
	pEnableErrorReporting = enable;
}



DUContext *UseBuilder::contextAtOrCurrent(const CursorInRevision &pos){
	DUContext * const context = topContext()->findContextAt(pos, true);
	
	if(context){
		return context;
		
	}else{
		return currentContext();
	}
}



void UseBuilder::visitFullyQualifiedClassname(FullyQualifiedClassnameAst *node){
	if(!node->nameSequence){
		return;
	}
	
	DUChainReadLocker lock;
	
	DUChainPointer<const DUContext> context(contextAtOrCurrent(editor()->findPosition(*node)));
	
	const KDevPG::ListNode<IdentifierAst*> *iter = node->nameSequence->front();
	const KDevPG::ListNode<IdentifierAst*> *end = iter;
	bool checkForVoid = pAllowVoidType;
	Declaration *decl = nullptr;
	
	do{
		const QString name(editor()->tokenText(*iter->element));
		RangeInRevision useRange(editor()->findRange(*iter->element));
		
		if(checkForVoid && name == "void"){
			context = nullptr;
			
		}else{
			if(context){
				decl = Helpers::declarationForName(name, CursorInRevision(INT_MAX, INT_MAX), context);
				context = nullptr;
			}
			
			if(!decl){
				reportSemanticError(lock, useRange, i18n("Unknown type: %1", name));
			}
			
			lock.unlock();
			UseBuilderBase::newUse(iter->element, useRange, DeclarationPointer(decl));
			lock.lock();
			if(decl){
				context = decl->internalContext();
			}
		}
		
		checkForVoid = false;
		
		iter = iter->next;
	}while(iter != end);
}

void UseBuilder::visitClassFunctionDeclareBegin(ClassFunctionDeclareBeginAst *node){
	DUChainPointer<const DUContext> context(contextAtOrCurrent(editor()->findPosition(*node)));
	
	// overwritten to allow void type in FullyQualifiedClassnameAst
	pAllowVoidType = true;
	visitNode(node->type);
	pAllowVoidType = false;
	
	if(node->argumentsSequence){
		const KDevPG::ListNode<ClassFunctionDeclareArgumentAst*> *iter = node->argumentsSequence->front();
		const KDevPG::ListNode<ClassFunctionDeclareArgumentAst*> *end = iter;
		do{
			visitNode(iter->element);
			iter = iter->next;
		}while(iter != end);
	}
	
	// process this/super call
	if(node->super == -1){
		return;
	}
	
	const bool isSuper = pParseSession.tokenStream()->at(node->super).kind == TokenTypeWrapper::Token_SUPER;
	
	// find this/super class constructors
	RangeInRevision useRange(editor()->findRange(node->super));
	DUChainPointer<const DUContext> superContext(context);
	const TopDUContext * const top = topContext();
	QVector<Declaration*> declarations;
	
	if(superContext && isSuper){
		const ClassDeclaration *classDecl = Helpers::classDeclFor(
			DUChainPointer<const DUContext>(superContext->parentContext()));
		superContext = nullptr;
		
		if(classDecl){
			classDecl = Helpers::superClassOf(top, classDecl);
			if(classDecl){
				superContext = classDecl->internalContext();
			}
		}
	}
	
	if(superContext){
		declarations = Helpers::constructorsInClass(superContext);
	}
	
	// process super arguments building signature at the same time
	QVector<KDevelop::AbstractType::Ptr> signature;
	DUChainReadLocker lock;
	
	if(node->superArgumentsSequence){
		const KDevPG::ListNode<ExpressionAst*> *iter = node->superArgumentsSequence->front();
		const KDevPG::ListNode<ExpressionAst*> *end = iter;
		do{
			lock.unlock();
			visitNode(iter->element);
			lock.lock();
			
			if(pCurExprType){
				signature.append(pCurExprType);
				
			}else{
				signature.append(Helpers::getTypeVoid()); // error
			}
			
			iter = iter->next;
		}while(iter != end);
	}
	
	/*
	qDebug() << "KDevDScript: UseBuilder::visitClassFunctionDeclareBegin found functions";
	foreach(Declaration* declaration, declarations){
		qDebug() << "KDevDScript: UseBuilder::visitClassFunctionDeclareBegin"
			<< declaration << ": "
			<< declaration->context()->owner()->toString() << " -> "
			<< declaration->toString();
	}
	qDebug() << "KDevDScript: UseBuilder::visitClassFunctionDeclareBegin match signature";
	foreach(KDevelop::AbstractType::Ptr type, signature){
		qDebug() << "KDevDScript: UseBuilder::visitClassFunctionDeclareBegin" << type->toString();
	}
	*/
	
	// find best matching function
	ClassFunctionDeclaration *useFunction = Helpers::bestMatchingFunction(top, signature, declarations);
	
	if(!useFunction){
		// find functions matching with auto-casting
		const QVector<ClassFunctionDeclaration*> possibleFunctions(
			Helpers::autoCastableFunctions(top, signature, declarations));
		
		if(possibleFunctions.size() == 1){
			useFunction = possibleFunctions.at(0);
			
		}else if(possibleFunctions.size() > 1){
			useFunction = possibleFunctions.at(0);
			
			QVector<KDevelop::IProblem::Ptr> diagnostics;
			foreach(ClassFunctionDeclaration* each, possibleFunctions){
				Problem * const problem = new Problem();
				problem->setFinalLocation(DocumentRange(each->url(), each->range().castToSimpleRange()));
				problem->setSource(KDevelop::IProblem::SemanticAnalysis);
				problem->setSeverity(KDevelop::IProblem::Hint);
				
				const ClassDeclaration * const classDecl = Helpers::classDeclFor(
					DUChainPointer<const DUContext>(each->context()));
				if(classDecl){
					problem->setDescription(i18n("Candidate: %1.%2",
						classDecl->identifier().toString(), each->toString()));
					
				}else{
					problem->setDescription(i18n("Candidate: %1", each->toString()));
				}
				
				diagnostics.append(KDevelop::IProblem::Ptr(problem));
			}
			
			const char *separator = "";
			QString sig(isSuper ? "super(" : "this(");
			foreach(const KDevelop::AbstractType::Ptr &each, signature){
				sig.append(separator).append(each->toString());
				separator = ", ";
			}
			sig.append(")");
			
			const ClassDeclaration * const classDecl = Helpers::classDeclFor(context);
			if(classDecl){
				reportSemanticError(lock, useRange, i18n("Ambiguous constructor call: found %1.%2",
					classDecl->identifier().toString(), sig), diagnostics);
				
			}else{
				reportSemanticError(lock, useRange,
					i18n("Ambiguous constructor call: found %1", sig), diagnostics);
			}
		}
	}
	
	// use function if found. no context required since this is outside function block
	if(useFunction){
		lock.unlock();
		UseBuilderBase::newUse(node->name, useRange, DeclarationPointer(useFunction));
		lock.lock();
		
	}else{
		const char *separator = "";
		QString sig(isSuper ? "super(" : "this(");
		foreach(const KDevelop::AbstractType::Ptr &each, signature){
			sig.append(separator).append(each->toString());
			separator = ", ";
		}
		sig.append(")");
		
		const ClassDeclaration * const classDecl = Helpers::classDeclFor(context);
		if(classDecl){
			reportSemanticError(lock, useRange, i18n("No suitable constructor found. expected %1",
				classDecl->identifier().toString(), sig));
			
		}else{
			reportSemanticError(lock, useRange, i18n("No suitable constructor found. expected %1", sig));
		}
	}
}



void UseBuilder::visitExpression(ExpressionAst *node){
	DUChainReadLocker lock;
	pCurExprContext = contextAtOrCurrent(editor()->findPosition(*node));
	pCurExprType = nullptr;
	lock.unlock();
	
	UseBuilderBase::visitExpression(node);
}

void UseBuilder::visitExpressionConstant(ExpressionConstantAst *node){
	DUChainReadLocker lock;
	
	DUContext * const context = contextAtOrCurrent(editor()->findPosition(*node));
	
	const QString name(editor()->tokenText(*node));
	RangeInRevision useRange(editor()->findRange(*node));
	
	ExpressionVisitor exprValue(*editor(), context);
	exprValue.visitNode(node);
	DeclarationPointer declaration(exprValue.lastDeclaration());
	KDevelop::AbstractType::Ptr type(exprValue.lastType());
	
	if(declaration) {
		// add a use only for "this" and "super". the rest only updated the expr-context
		switch(pParseSession.tokenStream()->at(node->value).kind){
		case TokenTypeWrapper::TokenType::Token_THIS:
		case TokenTypeWrapper::TokenType::Token_SUPER:
			lock.unlock();
			UseBuilderBase::newUse(node, useRange, declaration);
			lock.lock();
			break;
		}
	}
	
	if(declaration){
		pCurExprContext = declaration->internalContext();
		
	}else{
		pCurExprContext = nullptr;
	}
	pCurExprType = type;
}

void UseBuilder::visitExpressionMember(ExpressionMemberAst *node){
	DUChainReadLocker lock;
	DUChainPointer<const DUContext> context(pCurExprContext);
	
	if(node->funcCall != -1){
		/*
		qDebug() << "KDevDScript: UseBuilder::visitExpressionMember found functions for" << name;
		foreach(Declaration* declaration, declarations){
			qDebug() << "KDevDScript: UseBuilder::visitExpressionMember"
				<< declaration << ": "
				<< declaration->context()->owner()->toString() << " -> "
				<< declaration->toString();
		}
		*/
		
		// process function arguments building signature at the same time
		QVector<KDevelop::AbstractType::Ptr> signature;
		
		if(node->argumentsSequence){
			const KDevPG::ListNode<ExpressionAst*> *iter = node->argumentsSequence->front();
			const KDevPG::ListNode<ExpressionAst*> *end = iter;
			do{
				lock.unlock();
				visitNode(iter->element);
				lock.lock();
				
				if(pCurExprType){
					signature.append(pCurExprType);
					
				}else{
					signature.append(Helpers::getTypeInvalid());
				}
				
				iter = iter->next;
			}while(iter != end);
		}
		
		/*
		qDebug() << "KDevDScript: UseBuilder::visitExpressionMember match signature";
		foreach(KDevelop::AbstractType::Ptr type, signature){
			qDebug() << "KDevDScript: UseBuilder::visitExpressionMember" << type->toString();
		}
		*/
		
		lock.unlock();
		checkFunctionCall(node->name, context, signature);
		
	}else{
		const QString name(editor()->tokenText(*node->name));
		QVector<Declaration*> declarations;
		if(context){
			declarations = Helpers::declarationsForName(name, editor()->findPosition(*node), context);
		}
		
		if(declarations.isEmpty()){
			// if the context is not a class context we are at the beginning of an expression
			// and auto-this has to be used. find the this-context and try again
			if(context && !dynamic_cast<ClassDeclaration*>(context->owner())){
				const ClassDeclaration * const classDecl = Helpers::thisClassDeclFor(
					DUChainPointer<const DUContext>(context));
				if(classDecl){
					context = classDecl->internalContext();
				}
				
				if(context){
					declarations = Helpers::declarationsForName(name, editor()->findPosition(*node), context);
				}
			}
		}
		
		RangeInRevision useRange(editor()->findRange(*node->name));
		
		if(declarations.isEmpty()){
			const ClassDeclaration * const classDecl = Helpers::classDeclFor(context);
			lock.unlock();
			if(classDecl){
				reportSemanticError(lock, useRange, i18n("Unknown object: %1.%2",
					classDecl->identifier().toString(), name));
				
			}else{
				reportSemanticError(lock, useRange, i18n("Unknown object: %1", name));
			}
			UseBuilderBase::visitExpressionMember(node);
			pCurExprContext = nullptr;
			pCurExprType = nullptr;
			return;
		}
		
		Declaration *declaration = declarations.at(0);
		if(dynamic_cast<ClassFunctionDeclaration*>(declaration)){
			const ClassDeclaration * const classDecl = Helpers::classDeclFor(
				DUChainPointer<const DUContext>(declaration->context()));
			if(classDecl){
				reportSemanticError(lock, useRange, i18n("Expected object but found function: %1.%2",
					classDecl->identifier().toString(), declaration->toString()));
				
			}else{
				reportSemanticError(lock, useRange, i18n("Expected object but found function: %1",
					declaration->toString()));
			}
			
			pCurExprContext = nullptr;
			pCurExprType = nullptr;
			return;
		}
		
		lock.unlock();
		UseBuilderBase::newUse(node->name, useRange, DeclarationPointer(declaration));
		lock.lock();
		
		const StructureType::Ptr structType = declaration->type<StructureType>();
		if(structType){
			pCurExprContext = structType->internalContext(topContext());
			
		}else{
			pCurExprContext = nullptr;
		}
		pCurExprType = declaration->abstractType();
	}
}

void UseBuilder::visitExpressionBlock(ExpressionBlockAst *node){
	UseBuilderBase::visitExpressionBlock(node);
	
	DUChainReadLocker lock;
	
	DeclarationPointer declBlock;
	KDevelop::AbstractType::Ptr typeBlock;
	Helpers::getTypeBlock(declBlock, typeBlock);
	
	if(declBlock){
		pCurExprContext = declBlock->internalContext();
		
	}else{
		pCurExprContext = nullptr;
	}
	pCurExprType = typeBlock;
}

void UseBuilder::visitExpressionAddition(ExpressionAdditionAst *node){
	DUChainReadLocker lock;
	DUChainPointer<const DUContext> context(contextAtOrCurrent(editor()->findPosition(*node)));
	lock.unlock();
	
	DUChainPointer<const DUContext> contextLeft(functionGetContext(node->left, context));
	
	if(!node->moreSequence){
		return;
	}
	
	const KDevPG::ListNode<ExpressionAdditionMoreAst*> *iter = node->moreSequence->front();
	const KDevPG::ListNode<ExpressionAdditionMoreAst*> *end = iter;
	do{
		checkFunctionCall(iter->element->op, contextLeft, typeOfNode(iter->element->right, context));
		contextLeft = pCurExprContext;
		iter = iter->next;
	}while(iter != end);
}

void UseBuilder::visitExpressionAssign(ExpressionAssignAst *node){
	DUChainReadLocker lock;
	DUChainPointer<const DUContext> context(contextAtOrCurrent(editor()->findPosition(*node)));
	lock.unlock();
	
	DUChainPointer<const DUContext> contextLeft(functionGetContext(node->left, context));
	
	if(!node->moreSequence){
		return;
	}
	
	const KDevPG::ListNode<ExpressionAssignMoreAst*> *iter = node->moreSequence->front();
	const KDevPG::ListNode<ExpressionAssignMoreAst*> *end = iter;
	const TopDUContext * const top = topContext();
	
	do{
		bool isAssign = false;
		if(iter->element->op && iter->element->op->op != -1){
			isAssign = pParseSession.tokenStream()->at(iter->element->op->op).kind == TokenType::Token_ASSIGN;
		}
		
		if(isAssign){
			const KDevelop::AbstractType::Ptr typeLeft(typeOfNode(node->left, context));
			const KDevelop::AbstractType::Ptr typeRight(typeOfNode(iter->element->right, context));
			
			if(!Helpers::castable(top, typeRight, typeLeft)){
				RangeInRevision useRange(editor()->findRange(*iter->element->op));
				reportSemanticError(lock, useRange, i18n("Cannot assign object of type %1 to %2",
					typeRight ? typeRight->toString() : "??", typeLeft ? typeLeft->toString() : "??"));
			}
			
			contextLeft = Helpers::contextForType(top, typeLeft);
			
		}else{
			checkFunctionCall(iter->element->op, contextLeft, typeOfNode(iter->element->right, context));
			contextLeft = pCurExprContext;
		}
		
		iter = iter->next;
	}while(iter != end);
}

void UseBuilder::visitExpressionBitOperation(ExpressionBitOperationAst *node){
	DUChainReadLocker lock;
	DUChainPointer<const DUContext> context(contextAtOrCurrent(editor()->findPosition(*node)));
	lock.unlock();
	
	DUChainPointer<const DUContext> contextLeft(functionGetContext(node->left, context));
	
	if(!node->moreSequence){
		return;
	}
	
	const KDevPG::ListNode<ExpressionBitOperationMoreAst*> *iter = node->moreSequence->front();
	const KDevPG::ListNode<ExpressionBitOperationMoreAst*> *end = iter;
	do{
		checkFunctionCall(iter->element->op, contextLeft, typeOfNode(iter->element->right, context));
		contextLeft = pCurExprContext;
		iter = iter->next;
	}while(iter != end);
}

void UseBuilder::visitExpressionCompare(ExpressionCompareAst *node){
	DUChainReadLocker lock;
	DUChainPointer<const DUContext> context(contextAtOrCurrent(editor()->findPosition(*node)));
	lock.unlock();
	
	DUChainPointer<const DUContext> contextLeft(functionGetContext(node->left, context));
	
	if(!node->moreSequence){
		return;
	}
	
	const KDevPG::ListNode<ExpressionCompareMoreAst*> *iter = node->moreSequence->front();
	const KDevPG::ListNode<ExpressionCompareMoreAst*> *end = iter;
	do{
		checkFunctionCall(iter->element->op, contextLeft, typeOfNode(iter->element->right, context));
		contextLeft = pCurExprContext;
		iter = iter->next;
	}while(iter != end);
}

void UseBuilder::visitExpressionLogic(ExpressionLogicAst *node){
	DUChainReadLocker lock;
	DUChainPointer<const DUContext> context(contextAtOrCurrent(editor()->findPosition(*node)));
	lock.unlock();
	
	DUChainPointer<const DUContext> contextLeft(functionGetContext(node->left, context));
	
	if(!node->moreSequence){
		return;
	}
	
	const KDevPG::ListNode<ExpressionLogicMoreAst*> *iter = node->moreSequence->front();
	const KDevPG::ListNode<ExpressionLogicMoreAst*> *end = iter;
	do{
		checkFunctionCall(iter->element->op, contextLeft, typeOfNode(iter->element->right, context));
		contextLeft = pCurExprContext;
		iter = iter->next;
	}while(iter != end);
}

void UseBuilder::visitExpressionMultiply(ExpressionMultiplyAst *node){
	DUChainReadLocker lock;
	DUChainPointer<const DUContext> context(contextAtOrCurrent(editor()->findPosition(*node)));
	lock.unlock();
	
	DUChainPointer<const DUContext> contextLeft(functionGetContext(node->left, context));
	
	if(!node->moreSequence){
		return;
	}
	
	const KDevPG::ListNode<ExpressionMultiplyMoreAst*> *iter = node->moreSequence->front();
	const KDevPG::ListNode<ExpressionMultiplyMoreAst*> *end = iter;
	do{
		checkFunctionCall(iter->element->op, contextLeft, typeOfNode(iter->element->right, context));
		contextLeft = pCurExprContext;
		iter = iter->next;
	}while(iter != end);
}

void UseBuilder::visitExpressionPostfix(ExpressionPostfixAst *node){
	DUChainReadLocker lock;
	DUChainPointer<const DUContext> context(contextAtOrCurrent(editor()->findPosition(*node)));
	lock.unlock();
	
	DUChainPointer<const DUContext> contextLeft(functionGetContext(node->left, context));
	
	if(!node->opSequence){
		return;
	}
	
	const KDevPG::ListNode<ExpressionPostfixOpAst*> *iter = node->opSequence->front();
	const KDevPG::ListNode<ExpressionPostfixOpAst*> *end = iter;
	do{
		checkFunctionCall(iter->element, contextLeft, QVector<KDevelop::AbstractType::Ptr>());
		contextLeft = pCurExprContext;
		iter = iter->next;
	}while(iter != end);
}

void UseBuilder::visitExpressionSpecial(ExpressionSpecialAst *node){
	DUChainReadLocker lock;
	DUChainPointer<const DUContext> context(contextAtOrCurrent(editor()->findPosition(*node)));
	
	lock.unlock();
	pCurExprContext = context;
	pCurExprType = nullptr;
	visitNode(node->left);
	
	if(!node->moreSequence){
		return;
	}
	
	DeclarationPointer classDeclBool;
	KDevelop::AbstractType::Ptr typeBool;
	lock.lock();
	Helpers::getTypeBool(classDeclBool, typeBool);
	
	const KDevPG::ListNode<ExpressionSpecialMoreAst*> *iter = node->moreSequence->front();
	const KDevPG::ListNode<ExpressionSpecialMoreAst*> *end = iter;
	
	do{
		pCurExprContext = context;
		pCurExprContext = nullptr;
		lock.unlock();
		visitNode(iter->element->type);
		lock.lock();
		
		pCurExprContext = nullptr;
		pCurExprType = nullptr;
		
		if(iter->element->op && iter->element->op->op != -1){
			if(pParseSession.tokenStream()->at(iter->element->op->op).kind == TokenType::Token_CAST){
				if(context){
					ExpressionVisitor exprValue(*editor(), context.data());
					exprValue.visitNode(iter->element->type);
					const DeclarationPointer declaration(exprValue.lastDeclaration());
					if(declaration){
						ClassDeclaration * const classDeclRight = dynamic_cast<ClassDeclaration*>(declaration.data());
						if(classDeclRight){
							pCurExprContext = classDeclRight->internalContext();
						}
						pCurExprType = declaration->abstractType();
					}
				}
				
			}else{
				if(classDeclBool){
					pCurExprContext = classDeclBool->internalContext();
				}
				pCurExprType = typeBool;
			}
		}
		
		iter = iter->next;
	}while(iter != end);
}

void UseBuilder::visitExpressionUnary(ExpressionUnaryAst *node){
	DUChainReadLocker lock;
	DUChainPointer<const DUContext> context(contextAtOrCurrent(editor()->findPosition(*node)));
	lock.unlock();
	
	DUChainPointer<const DUContext> contextRight(functionGetContext(node->right, context));
	
	if(!node->opSequence){
		return;
	}
	
	QVector<ExpressionUnaryOpAst*> sequence;
	const KDevPG::ListNode<ExpressionUnaryOpAst*> *iter = node->opSequence->front();
	const KDevPG::ListNode<ExpressionUnaryOpAst*> *end = iter;
	do{
		sequence.push_front(iter->element);
		iter = iter->next;
	}while(iter != end);
	
	foreach(ExpressionUnaryOpAst *each, sequence){
		checkFunctionCall(each, contextRight, QVector<KDevelop::AbstractType::Ptr>());
		contextRight = pCurExprContext;
	}
}

void UseBuilder::visitExpressionInlineIfElse(ExpressionInlineIfElseAst *node){
	DUChainReadLocker lock;
	DUChainPointer<const DUContext> context(contextAtOrCurrent(editor()->findPosition(*node)));
	lock.unlock();
	
	KDevelop::AbstractType::Ptr typeCondition(typeOfNode(node->condition, context));
	
	if(!node->more){
		return;
	}
	
	KDevelop::AbstractType::Ptr typeIf(typeOfNode(node->more->expressionIf, context));
	KDevelop::AbstractType::Ptr typeElse(typeOfNode(node->more->expressionElse, context));
	const TopDUContext * const top = topContext();
	
	if(!Helpers::castable(top, typeCondition, Helpers::getTypeBool())){
		RangeInRevision useRange(editor()->findRange(*node->condition));
		reportSemanticError(lock, useRange, i18n("Cannot assign object of type %1 to bool",
			typeCondition ? typeCondition->toString() : "??"));
	}
	
	if(node->more->expressionIf && node->more->expressionElse
	&& !Helpers::castable(top, typeElse, typeIf)){
		RangeInRevision useRange(editor()->findRange(*node->more->expressionElse));
		reportSemanticError(lock, useRange, i18n("Cannot assign object of type %1 to %2",
			typeElse ? typeElse->toString() : "??", typeIf ? typeIf->toString() : "??"));
	}
	
	pCurExprContext = Helpers::contextForType(top, typeIf);
	pCurExprType = typeIf;
}

void UseBuilder::visitStatementFor(StatementForAst *node){
	// variable is ExpressionObjectAst. we need to manually clear the search context
	// as if ExpressionAst would be used.
	DUChainReadLocker lock;
	pCurExprContext = contextAtOrCurrent(editor()->findPosition(*node));
	pCurExprType = nullptr;
	lock.unlock();
	
	UseBuilderBase::visitStatementFor(node);
}



DUChainPointer<const DUContext> UseBuilder::functionGetContext(AstNode *node,
DUChainPointer<const DUContext> context){
	DUChainPointer<const DUContext> contextLeft;
	if(!node){
		return contextLeft;
	}
	
	pCurExprContext = context;
	pCurExprType = nullptr;
	visitNode(node);
	
	if(!pCurExprContext){
		return contextLeft;
	}
	
	DUChainReadLocker lock;
	const ClassDeclaration * const classDecl = Helpers::classDeclFor(pCurExprContext);
	if(classDecl){
		contextLeft = classDecl->internalContext();
	}
	return contextLeft;
}

KDevelop::AbstractType::Ptr UseBuilder::typeOfNode(AstNode *node,
DUChainPointer<const DUContext> context){
	if(node){
		pCurExprContext = context;
		pCurExprType = nullptr;
		visitNode(node);
	}
	
	KDevelop::AbstractType::Ptr type(pCurExprType);
	if(!type){
		DUChainReadLocker lock;
		type = Helpers::getTypeInvalid();
	}
	return type;
}

void UseBuilder::checkFunctionCall(AstNode *node, DUChainPointer<const DUContext> context,
const KDevelop::AbstractType::Ptr &argument){
	QVector<KDevelop::AbstractType::Ptr> signature;
	signature.append(argument);
	checkFunctionCall(node, context, signature);
}

void UseBuilder::checkFunctionCall(AstNode *node, DUChainPointer<const DUContext> context,
const QVector<KDevelop::AbstractType::Ptr> &signature){
	DUChainReadLocker lock;
	
	const QString name(editor()->tokenText(*node));
	RangeInRevision useRange(editor()->findRange(*node));
	
	QVector<Declaration*> declarations;
	if(context){
		declarations = Helpers::declarationsForName(name, editor()->findPosition(*node), context);
	}
	
	const TopDUContext * const top = topContext();
	
	if(declarations.isEmpty()){
		lock.unlock();
		
		const ClassDeclaration * const classDecl = Helpers::classDeclFor(context);
		if(classDecl){
			reportSemanticError(lock, useRange, i18n("Unknown function: %1.%2",
				classDecl->identifier().toString(), name));
			
		}else{
			reportSemanticError(lock, useRange, i18n("Unknown function: %1", name));
		}
		
		pCurExprContext = nullptr;
		pCurExprType = nullptr;
		return;
	}
	
	// if the first found declaration is not a function definition something is wrong
	if(!dynamic_cast<ClassFunctionDeclaration*>(declarations.at(0))){
		const ClassDeclaration * const classDecl = Helpers::classDeclFor(context);
		
		if(classDecl){
			reportSemanticError(lock, useRange, i18n("Expected function but found object: %1.%2",
				classDecl->identifier().toString(), name));
			
		}else{
			reportSemanticError(lock, useRange, i18n("Expected function but found object: %1", name));
		}
		
		pCurExprContext = nullptr;
		pCurExprType = nullptr;
		return;
	}
	
	// find best matching function
	ClassFunctionDeclaration *useFunction = Helpers::bestMatchingFunction(top, signature, declarations);
	
	if(!useFunction){
		// find functions matching with auto-casting
		const QVector<ClassFunctionDeclaration*> possibleFunctions(
			Helpers::autoCastableFunctions(top, signature, declarations));
		
		if(possibleFunctions.size() == 1){
			useFunction = possibleFunctions.at(0);
			
		}else if(possibleFunctions.size() > 1){
			useFunction = possibleFunctions.at(0);
			
			QVector<KDevelop::IProblem::Ptr> diagnostics;
			foreach(ClassFunctionDeclaration* each, possibleFunctions){
				Problem * const problem = new Problem();
				problem->setFinalLocation(DocumentRange(each->url(), each->range().castToSimpleRange()));
				problem->setSource(KDevelop::IProblem::SemanticAnalysis);
				problem->setSeverity(KDevelop::IProblem::Hint);
				
				const ClassDeclaration * const classDecl = dynamic_cast<ClassDeclaration*>(each->context());
				if(classDecl){
					problem->setDescription(i18n("Candidate: %1.%2",
						classDecl->identifier().toString(), each->toString()));
					
				}else{
					problem->setDescription(i18n("Candidate: %1", each->toString()));
				}
				
				diagnostics.append(KDevelop::IProblem::Ptr(problem));
			}
			
			const char *separator = "";
			QString sig(name + "(");
			foreach(const KDevelop::AbstractType::Ptr &each, signature){
				sig.append(separator).append(each->toString());
				separator = ", ";
			}
			sig.append(")");
			
			const ClassDeclaration * const classDecl = Helpers::classDeclFor(context);
			if(classDecl){
				reportSemanticError(lock, useRange, i18n("Ambiguous function call: found %1.%2",
					classDecl->identifier().toString(), sig), diagnostics);
				
			}else{
				reportSemanticError(lock, useRange, i18n("Ambiguous function call: found %1",
					sig), diagnostics);
			}
		}
	}
	
	// use function if found
	if(useFunction){
		lock.unlock();
		UseBuilderBase::newUse(node, useRange, DeclarationPointer(useFunction));
		lock.lock();
		
		const StructureType::Ptr structType = TypePtr<StructureType>::dynamicCast(
			useFunction->type<FunctionType>()->returnType());
		if(structType){
			pCurExprContext = structType->internalContext(top);
		}
		pCurExprType = useFunction->type<FunctionType>()->returnType();
		
		return;
		
	}else{
		const char *separator = "";
		QString sig(name + "(");
		foreach(const KDevelop::AbstractType::Ptr &each, signature){
			sig.append(separator).append(each->toString());
			separator = ", ";
		}
		sig.append(")");
		
		const ClassDeclaration * const classDecl = Helpers::classDeclFor(context);
		if(classDecl){
			reportSemanticError(lock, useRange, i18n("No suitable function found: expected %1.%2",
				classDecl->identifier().toString(), sig));
			
		}else{
			reportSemanticError(lock, useRange, i18n("No suitable function found: expected %1", sig));
		}
	}
	
	pCurExprContext = nullptr;
	pCurExprType = nullptr;
}



void UseBuilder::reportSemanticError(const RangeInRevision &range, const QString &hint){
	reportSemanticError(range, hint, QVector<KDevelop::IProblem::Ptr>());
}

void UseBuilder::reportSemanticError(const RangeInRevision &range, const QString &hint,
const QVector<KDevelop::IProblem::Ptr> &diagnostics){
	if(!pEnableErrorReporting){
		return;
	}
	
	Problem * const problem = new Problem();
	problem->setFinalLocation(DocumentRange(pParseSession.currentDocument(), range.castToSimpleRange()));
	problem->setSource(KDevelop::IProblem::SemanticAnalysis);
	problem->setSeverity(KDevelop::IProblem::Error);
	problem->setDescription(hint);
	problem->setDiagnostics(diagnostics);
	
	DUChainWriteLocker lock;
	topContext()->addProblem(ProblemPointer(problem));
}

void UseBuilder::reportSemanticError(DUChainReadLocker &locker, const RangeInRevision &range,
const QString &hint){
	reportSemanticError(locker, range, hint, QVector<KDevelop::IProblem::Ptr>());
}

void UseBuilder::reportSemanticError(DUChainReadLocker &locker, const RangeInRevision &range,
const QString &hint, const QVector<KDevelop::IProblem::Ptr> &diagnostics){
	if(!pEnableErrorReporting){
		return;
	}
	
	locker.unlock();
	reportSemanticError(range, hint, diagnostics);
	locker.lock();
}

void UseBuilder::reportSemanticHint(const RangeInRevision &range, const QString &hint){
	if(!pEnableErrorReporting){
		return;
	}
	
// 	qDebug() << "KDevDScript: UseBuilder::reportSemanticHint:" << hint;
	
	Problem * const problem = new Problem();
	problem->setFinalLocation(DocumentRange(pParseSession.currentDocument(), range.castToSimpleRange()));
	problem->setSource(KDevelop::IProblem::SemanticAnalysis);
	problem->setSeverity(KDevelop::IProblem::Hint);
	problem->setDescription(hint);
	
	DUChainWriteLocker lock;
	topContext()->addProblem(ProblemPointer(problem));
}

void UseBuilder::reportSemanticHint(DUChainReadLocker &locker,
const RangeInRevision &range, const QString &hint){
	if(!pEnableErrorReporting){
		return;
	}
	
	locker.unlock();
	reportSemanticHint(range, hint);
	locker.lock();
}

}
