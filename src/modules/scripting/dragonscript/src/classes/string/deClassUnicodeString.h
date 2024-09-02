/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// include only once
#ifndef _DECLASSUNICODESTRING_H_
#define _DECLASSUNICODESTRING_H_

// includes
#include <libdscript/libdscript.h>

class decString;
// predefinitions
class decUnicodeString;



// Unicode String Class
class deClassUnicodeString : public dsClass{
public:
	// constructor
	deClassUnicodeString();
	~deClassUnicodeString();
	// internal functions
	void CreateClassMembers( dsEngine *engine );
	const decUnicodeString &GetUnicodeString( dsRealObject *myself ) const;
	const decString GetUTF8( dsRealObject *myself ) const;
	void PushUnicodeString( dsRunTime *rt, const decUnicodeString &string );
	void PushUTF8( dsRunTime *rt, const char *utf8 );
	// for functions only
	void PushUnicodeAsUTF8( dsRunTime *rt, dsValue *myself );
	
private:
	struct sInitData{
		dsClass *clsUS, *clsVoid, *clsInt, *clsFlt, *clsStr;
		dsClass *clsObj, *clsBool, *clsByte;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewFrom );
	DEF_NATFUNC( nfNewFromCharacter );
	DEF_NATFUNC( nfNewFromUTF8 );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetLength );
	DEF_NATFUNC( nfGetCharacterAt );
	DEF_NATFUNC( nfGetLeft );
	DEF_NATFUNC( nfGetRight );
	DEF_NATFUNC( nfGetSubString );
	DEF_NATFUNC( nfReverse );
	
	DEF_NATFUNC( nfTrimLeft );
	DEF_NATFUNC( nfTrimRight );
	DEF_NATFUNC( nfTrimBoth );
	
	DEF_NATFUNC( nfFindChar );
	DEF_NATFUNC( nfFindCharReverse );
	
	DEF_NATFUNC( nfCompareTo );
	DEF_NATFUNC( nfCompareToNoCase );
	
	DEF_NATFUNC( nfToLower );
	DEF_NATFUNC( nfToUpper );
	DEF_NATFUNC( nfToInt );
	DEF_NATFUNC( nfToFloat );
	DEF_NATFUNC( nfToString );
	DEF_NATFUNC( nfToUTF8 );
	
	DEF_NATFUNC( nfOpAdd );
	DEF_NATFUNC( nfOpAddByte );
	DEF_NATFUNC( nfOpAddBool );
	DEF_NATFUNC( nfOpAddInt );
	DEF_NATFUNC( nfOpAddFloat );
	DEF_NATFUNC( nfOpAddObject );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfCompare );
#undef DEF_NATFUNC
};

// end of include only once
#endif
