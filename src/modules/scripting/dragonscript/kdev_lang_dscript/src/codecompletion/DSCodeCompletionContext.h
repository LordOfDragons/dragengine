#ifndef DSCODECOMPLETIONCONTEXT_H
#define DSCODECOMPLETIONCONTEXT_H

#include <language/codecompletion/codecompletioncontext.h>
#include <language/duchain/duchainpointer.h>

#include "codecompletionexport.h"

using KDevelop::CodeCompletionContext;
using KDevelop::CompletionTreeItemPointer;
using KDevelop::CursorInRevision;
using KDevelop::DUContextPointer;

using KTextEditor::Range;
using KTextEditor::View;


namespace DragonScript {

class DSCodeCompletionWorker;

class KDEVDSCODECOMPLETION_EXPORT DSCodeCompletionContext : public CodeCompletionContext{
public:
	DSCodeCompletionContext(const DSCodeCompletionWorker &worker, DUContextPointer context,
		const QString& contextText, const QString& followingText,
		const CursorInRevision& position, int depth);
	
	QList<CompletionTreeItemPointer> completionItems(bool &abort, bool fullCompletion = true) override;
	
	
	
private:
	const DSCodeCompletionWorker &pWorker;
	const QString pContextText;
	const CursorInRevision pPosition;
};

}

#endif
