#ifndef _DSP_LEXER_H_
#define _DSP_LEXER_H_

#include "dsp_tokentype.h"
#include "dsp_tokenstream.h"

#include <kdev-pg-char-sets.h>

#include "QDebug"

namespace DragonScript{

class KDEVDSPARSER_EXPORT Lexer :
public DragonScript::TokenStream,
public KDevPG::QByteArrayIterator,
public TokenTypeWrapper
{
public:
	typedef KDevPG::QByteArrayIterator Iterator;
	
private:
	void *pScanner;
	int pPosition;
	
public:
	Lexer( Iterator iter );
	~Lexer();
	DragonScript::Token& read();
	
	// for flex use only
	int nextChar( char &character );
	void advancePosition( int amount );
};

}

#endif
