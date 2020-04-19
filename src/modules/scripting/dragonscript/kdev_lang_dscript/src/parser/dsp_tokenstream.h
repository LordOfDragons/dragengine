#ifndef _DSP_TOKENSTREAM_H_
#define _DSP_TOKENSTREAM_H_

#include <kdev-pg-token-stream.h>

namespace DragonScript{

class Token : public KDevPG::Token{
public:
	qint64 docCommentBegin;
	qint64 docCommentEnd;
	int line;
	int column;
};

class TokenStream : public KDevPG::TokenStreamBase<Token>{
};

}
#endif
