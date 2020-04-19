#include "Highlighting.h"

#include <language/duchain/declaration.h>
#include <language/duchain/types/abstracttype.h>
#include <language/duchain/declaration.h>


namespace DragonScript{

// class HighlightingInstance
///////////////////////////////

HighlightingInstance::HighlightingInstance( const Highlighting *highlighting ) :
CodeHighlightingInstance( highlighting ){
	qDebug() << "HighlightingInstance: constructor";
}

void HighlightingInstance::highlightUse( DUContext *context, int index, const QColor &color ){
	CodeHighlightingInstance::highlightUse( context, index, color );
}

bool HighlightingInstance::useRainbowColor( Declaration *dec ) const{
	return CodeHighlightingInstance::useRainbowColor( dec );
}



// class Highlighting
///////////////////////

Highlighting::Highlighting( QObject *parent ) :
CodeHighlighting( parent ){
}

HighlightingInstance *Highlighting::createInstance() const{
	return new HighlightingInstance( this );
}

}
