#ifndef _PINNAMESPACEVISITOR_H
#define _PINNAMESPACEVISITOR_H

#include <QHash>
#include <QVector>
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
using KDevelop::TypePtr;
using KDevelop::QualifiedIdentifier;
using KDevelop::DUContextPointer;

namespace DragonScript {

class EditorIntegrator;

typedef DUChainPointer<FunctionDeclaration> FunctionDeclarationPointer;

/**
 * \brief Pin namespace visitor.
 * 
 * \note DUChainReadLocker required.
 */
class KDEVDSDUCHAIN_EXPORT PinNamespaceVisitor : public DefaultVisitor {
private:
	const EditorIntegrator &pEditorIntegrator;
	const DUContext *pContext;
	
	QVector<DUChainPointer<const DUContext>> pNamespaces;
	
	
	
public:
	PinNamespaceVisitor( const EditorIntegrator &editorIntegrator, const DUContext *ctx );
	
	void visitFullyQualifiedClassname( FullyQualifiedClassnameAst *node ) override;
	
	const QVector<DUChainPointer<const DUContext>> &namespaces() const{
		return pNamespaces;
	}
	
	
	
protected:
	void reportSemanticHint( const RangeInRevision &range, const QString &hint );
	
	void findNamespaceIn( const DUContext *searchContext, const QVector<IdentifierAst*> nodes,
		const QVector<QString> &names, int index );
};

}

#endif // EXPRESSIONVISITOR_H
