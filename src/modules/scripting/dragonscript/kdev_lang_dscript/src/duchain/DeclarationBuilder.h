#ifndef _DECLARATIONBUILDER_H
#define _DECLARATIONBUILDER_H

#include <language/duchain/builders/abstracttypebuilder.h>
#include <language/duchain/builders/abstractdeclarationbuilder.h>

#include <QList>

#include "ContextBuilder.h"
#include "dsp_defaultvisitor.h"
#include "duchainexport.h"

using KDevelop::AbstractTypeBuilder;
using KDevelop::AbstractDeclarationBuilder;
using KDevelop::ReferencedTopDUContext;
using KDevelop::IndexedString;
using KDevelop::DUContextPointer;
using KDevelop::DUContext;
using KDevelop::DUChainPointer;
using KDevelop::Declaration;

namespace DragonScript{

class CorrectionHelper;
class TokenStream;
class EditorIntegrator;
class ParseSession;

typedef AbstractTypeBuilder<AstNode, IdentifierAst, ContextBuilder> TypeBuilderBase;
typedef AbstractDeclarationBuilder<AstNode, IdentifierAst, TypeBuilderBase> DeclarationBuilderBase;

class KDEVDSDUCHAIN_EXPORT DeclarationBuilder : public DeclarationBuilderBase{

private:
	const ParseSession &pParseSession;
	QList<bool> pNamespaceContexts;
	QVector<DUChainPointer<const DUContext>> pPinned;
	int pEnumNextValue;
	int pLastModifiers;
	
	
	
public:
	DeclarationBuilder( EditorIntegrator &editor, int ownPriority, const ParseSession &parseSession );
	~DeclarationBuilder() override;
	
	/** \brief Entry function, called by KDevPlatform. */
	ReferencedTopDUContext build( const IndexedString &url, AstNode *node,
		ReferencedTopDUContext updateContext = ReferencedTopDUContext() ) override;
	
	/** \brief Close namespace contexts. */
	void closeNamespaceContexts();
	
	/** \brief Pinned namespaces. */
	inline const QVector<DUChainPointer<const DUContext>> &GetPinned(){ return pPinned; }
	
	/** \brief Find all existing declarations for the identifier \p node. */
	QList<Declaration*> existingDeclarationsForNode( IdentifierAst *node );
	
	/** \brief Get documentation for node. */
	QString getDocumentationForNode( const AstNode &node ) const;
	
	void visitScript( ScriptAst *node ) override;
    void visitScriptDeclaration( ScriptDeclarationAst *node ) override;
	void visitPin( PinAst *node ) override;
	void visitRequires( RequiresAst *node ) override;
	void visitNamespace( NamespaceAst *node ) override;
	void visitClass( ClassAst *node ) override;
	void visitClassBodyDeclaration( ClassBodyDeclarationAst *node ) override;
	void visitClassVariablesDeclare( ClassVariablesDeclareAst *node ) override;
	void visitClassFunctionDeclare( ClassFunctionDeclareAst *node ) override;
	void visitInterface( InterfaceAst *node ) override;
	void visitEnumeration( EnumerationAst *node ) override;
	void visitEnumerationBody( EnumerationBodyAst *node ) override;
	void visitExpressionBlock( ExpressionBlockAst *node ) override;
	void visitStatementIf( StatementIfAst *node ) override;
	void visitStatementElif( StatementElifAst *node ) override;
	void visitStatementSelect( StatementSelectAst *node ) override;
	void visitStatementCase( StatementCaseAst *node ) override;
	void visitStatementFor( StatementForAst *node ) override;
	void visitStatementWhile( StatementWhileAst *node ) override;
	void visitStatementTry( StatementTryAst *node ) override;
	void visitStatementCatch( StatementCatchAst *node ) override;
	void visitStatementVariableDefinitions( StatementVariableDefinitionsAst *node ) override;
	void visitTypeModifier( TypeModifierAst *node ) override;
	
	
	
protected:
	/** \brief Access policy from last modifiers. */
	KDevelop::ClassMemberDeclaration::AccessPolicy accessPolicyFromLastModifiers() const;
};

}

#endif
