#ifndef DSCODECOMPLETIONMODEL_H
#define DSCODECOMPLETIONMODEL_H

#include "codecompletionexport.h"

#include <language/codecompletion/codecompletionmodel.h>
#include <language/duchain/duchainpointer.h>

using KDevelop::CodeCompletionModel;
using KDevelop::CodeCompletionWorker;

using KTextEditor::Cursor;
using KTextEditor::Range;
using KTextEditor::View;


namespace DragonScript {

class KDEVDSCODECOMPLETION_EXPORT DSCodeCompletionModel : public CodeCompletionModel {
	Q_OBJECT
	
public:
	DSCodeCompletionModel( QObject *parent );
	~DSCodeCompletionModel() override;
	
	CodeCompletionWorker *createCompletionWorker() override;
	
	Range completionRange( View *view, const Cursor &position ) override;
	
	bool shouldStartCompletion( View *view, const QString &inserted,
		bool userInsertion, const Cursor &position ) override;
	
	bool shouldAbortCompletion( View *view, const Range &range,
		const QString &currentCompletion ) override;
	
	QString filterString( View *view, const Range &range, const Cursor &position ) override;
	
private:
	QUrl pDocument;
};

}

#endif
