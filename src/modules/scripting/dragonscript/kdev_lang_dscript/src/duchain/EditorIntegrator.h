#ifndef _EDITORINTEGRATOR_H
#define _EDITORINTEGRATOR_H

#include <language/editor/documentrange.h>

#include "duchainexport.h"
#include "../parser/ParseSession.h"
#include "dsp_ast-fwd.h"

using KDevelop::CursorInRevision;
using KDevelop::RangeInRevision;

namespace DragonScript{

class KDEVDSDUCHAIN_EXPORT EditorIntegrator{
public:
	EditorIntegrator(ParseSession &session);
	
	inline ParseSession *parseSession() const{ return &pSession; }
	
	enum Edge {
		FrontEdge,
		BackEdge
	};
	
	enum RangeEdge {
		InnerEdge,
		OuterEdge
	};
	
	inline ParseSession &session() const{ return pSession; }
	
	QString tokenText(qint64 token) const;
	QString tokenText(const AstNode &node) const;
	
	CursorInRevision findPosition(qint64 token, Edge edge=BackEdge) const;
	CursorInRevision findPosition(const AstNode &node, Edge edge=BackEdge) const;
	
	RangeInRevision findRange(qint64 token) const;
	RangeInRevision findRange(const AstNode &node, RangeEdge edge=OuterEdge) const;
	RangeInRevision findRange(const AstNode &from, const AstNode &to) const;
	
private:
	ParseSession &pSession;
};

}

#endif
