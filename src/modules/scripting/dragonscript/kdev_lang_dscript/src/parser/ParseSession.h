#ifndef _PARSESESSION_H_
#define _PARSESESSION_H_

#include <QtCore/QString>

#include "dsp_parser.h"
#include "parserexport.h"

using KDevelop::CursorInRevision;
using KDevelop::DUContextPointer;
using KDevelop::IndexedString;
using KDevelop::ProblemPointer;
using KDevelop::RangeInRevision;

namespace DragonScript{

class MemoryPool;
class TokenStream;
struct StartAst;

typedef QPair<DUContextPointer, RangeInRevision> SimpleUse;

class KDEVDSPARSER_EXPORT ParseSession{
public:
	ParseSession(const IndexedString &filename, const QByteArray &contents);
	~ParseSession();
	
	void setDebug(bool);
	TokenStream *tokenStream() const;
	
	inline const IndexedString &currentDocument() const{return pCurrentDocument;}
	inline const QByteArray &contents() const{return pContents;}
	
	bool parse(StartAst **);
	
	QString symbol(qint64 token) const;
	QString symbol(const AstNode &node) const;
	
	CursorInRevision positionAt(qint64 token) const;
	
	CursorInRevision tokenBegin(qint64 token) const;
	CursorInRevision tokenEnd(qint64 token) const;
	
	QString documentation(qint64 token) const;
	QString documentation(const AstNode &node) const;
	
	QList<ProblemPointer> problems();
	
	/// @TODO implement this
	void mapAstUse(AstNode *node, const SimpleUse &use){
		Q_UNUSED(node);
		Q_UNUSED(use);
	}
	
private:
	const IndexedString pCurrentDocument;
	const QByteArray &pContents;
	bool pDebug;
	KDevPG::MemoryPool* pPool;
	TokenStream* pTokenStream;
	QList<ProblemPointer> pProblems;
};

}

#endif
