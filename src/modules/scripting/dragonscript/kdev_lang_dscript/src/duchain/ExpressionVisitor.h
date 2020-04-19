#ifndef _EXPRESSIONVISITOR_H
#define _EXPRESSIONVISITOR_H

#include <QHash>
#include <KLocalizedString>

#include <language/duchain/types/abstracttype.h>
#include <language/duchain/types/integraltype.h>
#include <language/duchain/types/typesystemdata.h>
#include <language/duchain/types/typeregister.h>
#include <language/duchain/types/containertypes.h>
#include <language/duchain/duchainpointer.h>
#include <language/duchain/declaration.h>
#include <language/duchain/types/structuretype.h>
#include <language/duchain/classdeclaration.h>
#include <language/duchain/functiondeclaration.h>
#include <language/duchain/builders/dynamiclanguageexpressionvisitor.h>

#include "dsp_defaultvisitor.h"
#include "duchainexport.h"
#include "EditorIntegrator.h"
#include "Helpers.h"

using KDevelop::Identifier;
using KDevelop::DUContext;
using KDevelop::DUChainPointer;
using KDevelop::FunctionDeclaration;
using KDevelop::DynamicLanguageExpressionVisitor;
using KDevelop::TypePtr;
using KDevelop::QualifiedIdentifier;

namespace DragonScript {

class EditorIntegrator;

typedef DUChainPointer<FunctionDeclaration> FunctionDeclarationPointer;

/**
 * \brief Expression visitor.
 * 
 * \note DUChainReadLocker required.
 */
class KDEVDSDUCHAIN_EXPORT ExpressionVisitor : public DefaultVisitor,
public DynamicLanguageExpressionVisitor
{
private:
	const EditorIntegrator &pEditorIntegrator;
	
	/** \brief Void type is allowed. */
	bool pAllowVoid = false;
	
	/** \brief tells whether the returned declaration is an alias. */
	bool m_isAlias = false;
	
	/** \brief used by code completion to disable range checks on declaration searches. */
	bool m_forceGlobalSearching = false;
	
	/** \brief used by code completion to detect unknown NameAst elements in expressions. */
	bool m_reportUnknownNames = false;
	
	CursorInRevision m_scanUntilCursor = CursorInRevision::invalid();
	
	QSet<QString> m_unknownNames;
	
	
	
public:
	ExpressionVisitor( const EditorIntegrator &editorIntegrator, const DUContext *ctx );
	
	void visitExpressionConstant( ExpressionConstantAst *node ) override;
	void visitFullyQualifiedClassname( FullyQualifiedClassnameAst *node ) override;
	
	inline bool isAlias() const { return m_isAlias; }
	
	/** \brief Void type is allowed. */
	inline bool getAllowVoid() const{ return pAllowVoid; }
	
	/** \brief Set if void type is allowed. */
	void setAllowVoid( bool allowVoid );
	
	void enableGlobalSearching(){
		m_forceGlobalSearching = true;
	}

	void enableUnknownNameReporting(){
		m_reportUnknownNames = true;
	}

	void scanUntil( const CursorInRevision &end ){
		m_scanUntilCursor = end;
	}

	QSet<QString> unknownNames() const{
		return m_unknownNames;
	}
	
	
	
protected:
	/** \brief Report semantic hint if reporting is enabled. */
	void reportSemanticHint( const RangeInRevision &range, const QString &hint );
	
	
	
private:
	void addUnknownName( const QString &name );
	
	void setLastIsAlias(bool alias) {
		m_isAlias = alias;
	}
};

}

#endif // EXPRESSIONVISITOR_H
