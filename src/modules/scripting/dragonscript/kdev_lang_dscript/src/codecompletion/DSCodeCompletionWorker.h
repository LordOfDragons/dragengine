#ifndef DSCODECOMPLETIONWORKER_H
#define DSCODECOMPLETIONWORKER_H

#include <language/codecompletion/codecompletionworker.h>
#include <language/codecompletion/codecompletionitem.h>

#include "DSCodeCompletionModel.h"
#include "codecompletionexport.h"


using KDevelop::CodeCompletionWorker;
using KDevelop::CodeCompletionContext;
using KDevelop::CursorInRevision;
using KDevelop::DUContextPointer;

using KTextEditor::Range;
using KTextEditor::View;


namespace DragonScript {

class KDEVDSCODECOMPLETION_EXPORT DSCodeCompletionWorker : public CodeCompletionWorker{
	Q_OBJECT
	
public:
	DSCodeCompletionWorker( DSCodeCompletionModel &model, const QUrl &document );
	
	CodeCompletionContext *createCompletionContext( const DUContextPointer context,
		const QString &contextText, const QString &followingText,
		const CursorInRevision &position ) const override;
	
	void updateContextRange( Range &contextRange, View *view,
		const DUContextPointer context ) const override;
	
	
	
private:
	DSCodeCompletionModel &pModel;
};

}

#endif
