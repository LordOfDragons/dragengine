#include <QDebug>
#include <language/duchain/declaration.h>
#include <KLocalizedString>
#include <KTextEditor/View>

#include "DSCodeCompletionWorker.h"
#include "DSCodeCompletionModel.h"
#include "DSCodeCompletionContext.h"


using KDevelop::CodeCompletionWorker;
using KDevelop::CodeCompletionContext;
using KDevelop::CursorInRevision;
using KDevelop::DUContextPointer;

using KTextEditor::Range;
using KTextEditor::View;


namespace DragonScript {

DSCodeCompletionWorker::DSCodeCompletionWorker(DSCodeCompletionModel &model, const QUrl &document) :
CodeCompletionWorker(&model),
pModel(model)
{
	Q_UNUSED(document);
}

CodeCompletionContext *DSCodeCompletionWorker::createCompletionContext(
const DUContextPointer context, const QString &contextText, const QString &followingText,
const CursorInRevision &position) const{
	if(! context){
		return nullptr;
	}
	
	return new DSCodeCompletionContext(*this, context, contextText, followingText, position, 0);
}

void DSCodeCompletionWorker::updateContextRange(Range &contextRange, View *view,
const DUContextPointer context) const{
	if(! context){
		return;
	}
	if(! contextRange.start().isValid()){
		contextRange.setStart({0, 0});
	}
	
	Q_UNUSED(view);
	/*
	if(CodeHelpers::endsInside(view->document()->text(contextRange)) == CodeHelpers::String){
		qDebug() << "we're dealing with string completion. extend the range";
		contextRange = context->rangeInCurrentRevision();
	}
	*/
}

}
