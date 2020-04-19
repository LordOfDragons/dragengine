#ifndef _HIGHLIGHTING_H
#define _HIGHLIGHTING_H

#include <QObject>
#include <QHash>
#include <QModelIndex>

#include <language/highlighting/codehighlighting.h>
#include <language/duchain/topducontext.h>

using KDevelop::CodeHighlighting;
using KDevelop::CodeHighlightingInstance;
using KDevelop::Declaration;
using KDevelop::DUContext;
using KDevelop::TopDUContext;

namespace DragonScript{

class Highlighting;

class HighlightingInstance : public CodeHighlightingInstance {
public:
	HighlightingInstance( const Highlighting *highlighting );
	
	/**
	 * \brief Highlight use.
	 */
	void highlightUse( DUContext *context, int index, const QColor &color ) override;
	
	/**
	 * Decides whether to apply auto-generated rainbow colors to @p dec.
	 * Default implementation only applies that to local variables in functions.
	 */
	bool useRainbowColor( Declaration *dec ) const override;
	
private:
};


class Highlighting : public CodeHighlighting{
Q_OBJECT
public:
	Highlighting( QObject *parent );
	HighlightingInstance *createInstance() const override;
};

}
#endif
