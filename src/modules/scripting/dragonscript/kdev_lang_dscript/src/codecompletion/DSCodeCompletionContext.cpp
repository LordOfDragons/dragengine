#include <QDebug>
#include <language/duchain/declaration.h>
#include <language/duchain/ducontext.h>
#include <language/duchain/use.h>
#include <KLocalizedString>
#include <KTextEditor/View>

#include "DSCodeCompletionContext.h"
#include "DSCodeCompletionModel.h"
#include "ExpressionVisitor.h"


using KDevelop::CodeCompletionWorker;
using KDevelop::CodeCompletionContext;
using KDevelop::CompletionTreeItemPointer;
using KDevelop::CursorInRevision;
using KDevelop::DUContext;
using KDevelop::DUContextPointer;
using KDevelop::DUChainReadLocker;
using KDevelop::Use;

using KTextEditor::Range;
using KTextEditor::View;


namespace DragonScript {

DSCodeCompletionContext::DSCodeCompletionContext( const DSCodeCompletionWorker &worker,
DUContextPointer context, const QString& contextText, const QString& followingText,
const CursorInRevision& position, int depth ) :
CodeCompletionContext( context, contextText, position, depth ),
pWorker( worker ),
pContextText( contextText ),
pPosition( position )
{
	Q_UNUSED( followingText );
	qDebug() << "KDevDScript: DSCodeCompletionContext():" << contextText << position
		<< context->localScopeIdentifier().toString() << context->range();
}

QList<CompletionTreeItemPointer> DSCodeCompletionContext::completionItems(
bool &abort, bool fullCompletion ){
	Q_UNUSED( fullCompletion );
	Q_UNUSED( abort );
	
	DUChainReadLocker lock();
	
	const DUContext * const context = duContext();
	const Use * const uses = context->uses();
	const int usesCount = context->usesCount();
	const Use *lastUse = nullptr;
	int i;
	
	qDebug() << "KDevDScript: DSCodeCompletionContext(): uses" << usesCount;
	
	for( i=0; i<usesCount; i++ ){
		qDebug() << "KDevDScript: DSCodeCompletionContext(): test" << uses[ i ].m_range;
		if( uses[ i ].m_range.start >= pPosition ){
			break;
		}
		lastUse = uses + i;
	}
	
	if( lastUse ){
		TopDUContext * const top = context->topContext();
		const Declaration * const decl = lastUse->usedDeclaration( top );
		if( decl ){
			qDebug() << "KDevDScript: DSCodeCompletionContext(): lastUse" << decl->toString();
		}
	}
	
	QList<CompletionTreeItemPointer> list;
	return list;
}

}
