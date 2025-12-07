#include <ktexteditor/document.h>
#include <interfaces/icore.h>
#include <interfaces/ilanguagecontroller.h>
#include <interfaces/iprojectcontroller.h>
#include <language/backgroundparser/backgroundparser.h>
#include <KF5/KConfigCore/kconfiggroup.h>

#include "ContextBuilder.h"
#include "EditorIntegrator.h"
#include "Helpers.h"

using KDevelop::ICore;
using KDevelop::DUChain;
using KDevelop::DUChainReadLocker;
using KDevelop::DUChainWriteLocker;

namespace DragonScript{

ReferencedTopDUContext ContextBuilder::build(const IndexedString &url,
AstNode* node, ReferencedTopDUContext updateContext){
	if(!updateContext){
		DUChainReadLocker lock;
		updateContext = DUChain::self()->chainForDocument(url);
		if(updateContext){
			Q_ASSERT(updateContext->type() == DUContext::Global);
		}
	}
	
	if(updateContext){
		qDebug() << "KDevDScript: ContextBuilder::build: rebuilding duchain for" << url.str() << "(was built before)";
		DUChainWriteLocker lock;
		Q_ASSERT(updateContext->type() == DUContext::Global);
		updateContext->clearImportedParentContexts();
		updateContext->parsingEnvironmentFile()->clearModificationRevisions();
		updateContext->clearProblems();
		
	}else{
		qDebug() << "KDevDScript: ContextBuilder::build: building duchain for" << url.str();
	}
	
	return ContextBuilderBase::build(url, node, updateContext);
}


void ContextBuilder::setEditor(EditorIntegrator *editor){
	pEditor = editor;
}

void ContextBuilder::startVisiting(AstNode *node){
	qDebug() << "KDevDScript: ContextBuilder::startVisiting";
	
	TopDUContext * const top = topContext();
	
	// if this file is not a documentation file make it import the documentation file contexts
	if(!Helpers::getDocumentationFiles().contains(document())){
		DUChainWriteLocker lock;
		pRequiresReparsing = !Helpers::addImportsDocumentationFileContexts(top);
	}
	
	// also add all include directory files as imports for project files
	if(!pRequiresReparsing){
		IProject * const project = ICore::self()->projectController()->findProjectForUrl(document().toUrl());
		if(project){
			KConfigGroup config(project->projectConfiguration()->group("dragonscriptsupport"));
			const QStringList list(config.readEntry("pathInclude", QStringList()));
			
			foreach(const QString &dirname, list){
				Helpers::addImportsIncludePath(top, dirname);
			}
		}
	}
	
	// visit node to start building
	visitNode(node);
	closeNamespaceContexts();
	qDebug() << "KDevDScript: ContextBuilder::startVisiting finished";
}

void ContextBuilder::setContextOnNode(AstNode *node, DUContext *context){
	node->ducontext = context;
}

DUContext *ContextBuilder::contextFromNode(AstNode *node){
	return node->ducontext;
}

RangeInRevision ContextBuilder::editorFindRange(AstNode *fromNode, AstNode *toNode){
	return pEditor->findRange(*fromNode, *toNode);
}

RangeInRevision ContextBuilder::editorFindRangeNode(AstNode *node){
	return pEditor->findRange(*node, *node);
}

QualifiedIdentifier ContextBuilder::identifierForNode(IdentifierAst *node){
	return QualifiedIdentifier(pEditor->tokenText(*node));
}

QualifiedIdentifier ContextBuilder::identifierForToken(qint64 token){
	return QualifiedIdentifier(pEditor->tokenText(token));
}

TopDUContext *ContextBuilder::newTopContext(const RangeInRevision &range,
ParsingEnvironmentFile *file){
	// file is allowed to be null but some code in kdevelop does not seem to know this causing
	// segfaults if run. ensure the file is present
	if(!file){
		file = new ParsingEnvironmentFile(document());
		file->setLanguage(IndexedString("DragonScript"));
	}
	
	return AbstractContextBuilder::newTopContext(range, file);
}



void ContextBuilder::closeNamespaceContexts(){
	qDebug() << "KDevDScript: ContextBuilder::closeNamespaceContexts" << pNamespaceContextCount;
	while(pNamespaceContextCount > 0){
		pNamespaceContextCount--;
		closeContext();
	}
}

void ContextBuilder::openContextClass(ClassAst *node){
	if(!node->end){
		qDebug() << "ContextBuilder::openContextClass: node->end is NULL in" << document()
			<< "at" << pEditor->findPosition(*node->begin);
	}
	// context starts at the end of the declaration
	const CursorInRevision cursorBegin(pEditor->findPosition(*node->begin, EditorIntegrator::BackEdge));
	const CursorInRevision cursorEnd(node->end
		? pEditor->findPosition(*node->end, EditorIntegrator::FrontEdge)
		: CursorInRevision::invalid());
	const RangeInRevision range(cursorBegin, cursorEnd);
	
	{
	DUChainWriteLocker lock;
	openContext(node, range, DUContext::Class, node->begin->name);
	currentContext()->setLocalScopeIdentifier(identifierForNode(node->begin->name));
	}
	
// 	addImportedContexts();
}

void ContextBuilder::openContextInterface(InterfaceAst *node){
	if(!node->end){
		qDebug() << "ContextBuilder::openContextInterface: node->end is NULL in" << document()
			<< "at" << pEditor->findPosition(*node->begin);
	}
	// context starts at the end of the declaration
	const CursorInRevision cursorBegin(pEditor->findPosition(*node->begin, EditorIntegrator::BackEdge));
	const CursorInRevision cursorEnd(node->end
		? pEditor->findPosition(*node->end, EditorIntegrator::FrontEdge)
		: CursorInRevision::invalid());
	const RangeInRevision range(cursorBegin, cursorEnd);
	
	DUChainWriteLocker lock;
	openContext(node, range, DUContext::Class, node->begin->name);
	currentContext()->setLocalScopeIdentifier(identifierForNode(node->begin->name));
// 	addImportedContexts();
}

void ContextBuilder::openContextEnumeration(EnumerationAst *node){
	if(!node->end){
		qDebug() << "ContextBuilder::openContextEnumeration: node->end is NULL in" << document()
			<< "at" << pEditor->findPosition(*node->begin);
	}
	// context starts at the end of the declaration
	const CursorInRevision cursorBegin(pEditor->findPosition(*node->begin, EditorIntegrator::BackEdge));
	const CursorInRevision cursorEnd(node->end
		? pEditor->findPosition(*node->end, EditorIntegrator::FrontEdge)
		: CursorInRevision::invalid());
	const RangeInRevision range(cursorBegin, cursorEnd);
	
	DUChainWriteLocker lock;
	openContext(node, range, DUContext::Enum, node->begin->name);
	currentContext()->setLocalScopeIdentifier(identifierForNode(node->begin->name));
// 	addImportedContexts();
}

void ContextBuilder::openContextClassFunction(ClassFunctionDeclareAst *node){
	if(!node->end){
		qDebug() << "ContextBuilder::openContextClassFunction: node->end is NULL in" << document()
			<< "at" << pEditor->findPosition(*node->begin);
	}
	// context starts at the end of the declaration
	const CursorInRevision cursorBegin(node->begin->name
		? pEditor->findPosition(*node->begin->name, EditorIntegrator::BackEdge)
		: pEditor->findPosition(*node->begin->op, EditorIntegrator::BackEdge));
	const CursorInRevision cursorEnd(node->end
		? pEditor->findPosition(*node->end, EditorIntegrator::FrontEdge)
		: CursorInRevision::invalid());
	const RangeInRevision range(cursorBegin, cursorEnd);
	
	DUChainWriteLocker lock;
	openContext(node, range, DUContext::Function, node->begin->name);
	
	if(node->begin->name){
		currentContext()->setLocalScopeIdentifier(identifierForNode(node->begin->name));
		
	}else if(node->begin->op){
		currentContext()->setLocalScopeIdentifier(identifierForToken(node->begin->op->op));
	}
// 	addImportedContexts();
}

/*
void ContextBuilder::visitNamespace(NamespaceAst *node){
	qDebug() << "KDevDScript: ContextBuilder::visitNamespace";
	closeNamespaceContexts();
	
	// for each namespace component add a context
	const KDevPG::ListNode<qint64> *iter = node->name->nameSequence->front();
	const KDevPG::ListNode<qint64> *end = iter;
	{
	DUChainWriteLocker lock;
	do{
		const RangeInRevision range(pEditor->findRange(iter->element));
		
		openContext(node, range, DUContext::Namespace, &iter->element);
		currentContext()->setLocalScopeIdentifier(identifierForNode(&iter->element));
		
		iter = iter->next;
		pNamespaceContextCount++;
		
	}while(iter != end);
	}
	
// 	addImportedContexts();
}

void ContextBuilder::visitClass(ClassAst *node){
	qDebug() << "KDevDScript: ContextBuilder::visitClass";
	openContextClass(node);
	DefaultVisitor::visitClass(node);
	closeContext();
}

void ContextBuilder::visitInterface(InterfaceAst *node){
	qDebug() << "KDevDScript: ContextBuilder::visitInterface";
	openContextInterface(node);
	DefaultVisitor::visitInterface(node);
	closeContext();
}

void ContextBuilder::visitEnumeration(EnumerationAst *node){
	qDebug() << "KDevDScript: ContextBuilder::visitEnumeration";
	openContextEnumeration(node);
	DefaultVisitor::visitEnumeration(node);
	closeContext();
}
*/

}
