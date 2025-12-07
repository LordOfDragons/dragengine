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
#ifndef _DECLASSUTF8DECODER_H_
#define _DECLASSUTF8DECODER_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class decUTF8Decoder;



// UTF8 Decoder Class
class deClassUTF8Decoder : public dsClass{
public:
	// constructor
	deClassUTF8Decoder();
	~deClassUTF8Decoder();
	// internal functions
	void CreateClassMembers(dsEngine *engine);
	decUTF8Decoder &GetUTF8Decoder(dsRealObject *myself) const;
	
private:
	struct sInitData{
		dsClass *clsU8D, *clsVoid, *clsInt, *clsStr, *clsObj, *clsBool;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetString);
	DEF_NATFUNC(nfGetLength);
	DEF_NATFUNC(nfGetPosition);
	DEF_NATFUNC(nfHasReachedEnd);
	DEF_NATFUNC(nfSetString);
	DEF_NATFUNC(nfSetPosition);
	DEF_NATFUNC(nfDecodeNextCharacter);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

// end of include only once
#endif
