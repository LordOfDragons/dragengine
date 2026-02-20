#ifndef _DUMPCHAIN_H
#define _DUMPCHAIN_H

#include "duchainexport.h"

#include <QTextStream>
#include <language/duchain/ducontext.h>

using KDevelop::DUContext;

namespace DragonScript{

class KDEVDSDUCHAIN_EXPORT DumpChain{
public:
	DumpChain();
	virtual ~DumpChain();
	void dump(DUContext* context, bool imported = false);

private:
    int indent;
};

}

#endif
