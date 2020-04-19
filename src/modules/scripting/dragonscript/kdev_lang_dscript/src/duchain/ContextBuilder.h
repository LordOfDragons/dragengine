#ifndef _CONTEXTBUILDER_H
#define _CONTEXTBUILDER_H

#include <language/duchain/builders/abstractcontextbuilder.h>
#include <language/editor/rangeinrevision.h>
#include <language/duchain/topducontext.h>

#include "dsp_defaultvisitor.h"
#include "duchainexport.h"

using KDevelop::AbstractContextBuilder;
using KDevelop::DUContext;
using KDevelop::IndexedString;
using KDevelop::ParsingEnvironmentFile;
using KDevelop::QualifiedIdentifier;
using KDevelop::RangeInRevision;
using KDevelop::ReferencedTopDUContext;
using KDevelop::TopDUContext;

namespace DragonScript{

class EditorIntegrator;

typedef AbstractContextBuilder<AstNode, IdentifierAst> ContextBuilderBase;

/**
 * \brief Context builder calculating scopes in file.
 *
 * For practical reasons, some building of scopes also happens in the declaration builder.
 */
class KDEVDSDUCHAIN_EXPORT ContextBuilder : public ContextBuilderBase, public DefaultVisitor{
private:
	EditorIntegrator *pEditor = nullptr;
	int pNamespaceContextCount = 0;
	bool pRequiresReparsing = false;
	
	
	
public:
	/**
	 * \deprecated Class member required by older kdevelop template classes.
	 * 
	 * Is only used in one place as a boolean value so most probably a left-over.
	 * Has to be false to not trigger anything.
	 */
	bool m_mapAst = false;
	
	
	
public:
	ContextBuilder() = default;
	
	EditorIntegrator *editor() const{ return pEditor; }
	void setEditor( EditorIntegrator *editor );
	
	/** \brief Entry function called by KDevPlatform API. */
	ReferencedTopDUContext build( const IndexedString &url, AstNode *node,
		ReferencedTopDUContext updateContext = ReferencedTopDUContext() ) override;
	
	void startVisiting( AstNode *node ) override;
	
	/**
	 * \brief Requires reparsing.
	 * 
	 * Set after build() is called to signal some information is missing and a reparsing
	 * should be done to obtain correct results.
	 */
	inline bool requiresReparsing() const{ return pRequiresReparsing; }
	
	/** \brief Close namespace contexts. */
	void closeNamespaceContexts();
	
	/**
	 * \brief Set \p context as the context of \p node.
	 * The context is stored inside the AST itself.
	 */
	void setContextOnNode( AstNode *node, DUContext *context ) override;
	
	/**
	 * \brief Get the context set on \p node as previously set by \ref setContextOnNode.
	 */
	DUContext *contextFromNode( AstNode *node ) override;
	
	RangeInRevision editorFindRange( AstNode *fromNode, AstNode *toNode ) override;
	RangeInRevision editorFindRangeNode( AstNode *node );
	
	QualifiedIdentifier identifierForNode( IdentifierAst *node ) override;
	QualifiedIdentifier identifierForToken( qint64 token );
	
	TopDUContext *newTopContext( const RangeInRevision &range, ParsingEnvironmentFile *file ) override;
	
	
	void openContextClass( ClassAst *node );
	void openContextInterface( InterfaceAst *node );
	void openContextEnumeration( EnumerationAst *node );
	void openContextClassFunction( ClassFunctionDeclareAst *node );
	
	/*
	void visitNamespace( NamespaceAst *node ) override;
	void visitClass( ClassAst *node ) override;
	void visitInterface( InterfaceAst *node ) override;
	void visitEnumeration( EnumerationAst *node ) override;
	*/
};

}

#endif
