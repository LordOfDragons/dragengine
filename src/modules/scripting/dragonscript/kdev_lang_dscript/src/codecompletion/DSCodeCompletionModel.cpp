#include <QDebug>
#include <KTextEditor/View>
#include <KTextEditor/Document>
#include <KTextEditor/CodeCompletionModelControllerInterface>

#include "DSCodeCompletionModel.h"
#include "DSCodeCompletionWorker.h"


using KDevelop::CodeCompletionModel;

using KTextEditor::CodeCompletionModelControllerInterface;
using KTextEditor::Cursor;
using KTextEditor::Range;
using KTextEditor::View;


namespace DragonScript {

DSCodeCompletionModel::DSCodeCompletionModel( QObject *parent ) :
CodeCompletionModel( parent )
{
	// avoid flickering of completion-list when full code-completion mode is used
	setForceWaitForModel( true );
}

DSCodeCompletionModel::~DSCodeCompletionModel(){
}

bool DSCodeCompletionModel::shouldStartCompletion( View *view, const QString &inserted,
bool userInsertion, const Cursor &position ){
	/*
	QList<QString> words;
	words << "for" << "raise" << "except" << "in";
	foreach ( const QString& word, words ) {
		if ( view->document()->line(position.line()).mid(0, position.column()).endsWith(word + " ") ) {
			return true;
		}
	}
	// shebang / encoding lines
	if ( view->document()->line(position.line()).mid(0, position.column()).endsWith("#") && 
		position.line() < 2 )
	{
		return true;
	}
	
	// we're probably dealing with string formatting completion
	// is there any other case where this condition is true?
	if ( ! userInsertion && inserted.startsWith('{') ) {
		return true;
	}
	*/
	
	return CodeCompletionModel::shouldStartCompletion( view, inserted, userInsertion, position );
}

bool DSCodeCompletionModel::shouldAbortCompletion( View *view, const Range &range,
const QString &currentCompletion ){
	/*
	const QString text( view->document()->text( range ) );
	if( completionContext() ){
		auto context = static_cast<CodeCompletionContext*>( completionContext().data() );
		if( context->completionContextType() == CodeCompletionContext::StringFormattingCompletion ){
			if( text.endsWith( '"' ) || text.endsWith( "'" ) || text.endsWith( ' ' ) ){
				return true;
			}
		}
	}
	*/
	
	return CodeCompletionModelControllerInterface::shouldAbortCompletion( view, range, currentCompletion );
}

QString DSCodeCompletionModel::filterString( View *view, const Range &range, const Cursor &position ){
	// TODO The completion context may be null, so we need to check it first. This might a bug.
	/*
	if( completionContext() ){
		auto context = static_cast<CodeCompletionContext*>( completionContext().data() );
		if( context->completionContextType() == CodeCompletionContext::StringFormattingCompletion ){
			return QString();
		}
	}
	*/
	
	return CodeCompletionModel::filterString( view, range, position );
}

Range DSCodeCompletionModel::completionRange( View *view, const Cursor &position ){
	pDocument = view->document()->url();
	return CodeCompletionModelControllerInterface::completionRange( view, position );
}

KDevelop::CodeCompletionWorker *DSCodeCompletionModel::createCompletionWorker(){
	return new DSCodeCompletionWorker( *this, pDocument );
}

}
