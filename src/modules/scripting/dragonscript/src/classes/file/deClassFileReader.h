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

#ifndef _DECLASSFILEREADER_H_
#define _DECLASSFILEREADER_H_

#include <libdscript/libdscript.h>

class decBaseFileReader;
class deScriptingDragonScript;



/**
 * @brief File Reader Script Class.
 */
class deClassFileReader : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new script class. */
	deClassFileReader(deScriptingDragonScript *ds);
	/** Cleans up the script class. */
	virtual ~deClassFileReader();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Create class members. */
	void CreateClassMembers(dsEngine *engine);
	/** Retrieves the fileReader or NULL if myself is NULL. */
	decBaseFileReader *GetFileReader(dsRealObject *myself) const;
	/** Retrieves the stream version. */
	int GetStreamVersion(dsRealObject *myself) const;
	/** Pushes the file reader on the stack. File reader can be NULL to push a null object. */
	void PushFileReader(dsRunTime *rt, decBaseFileReader::Ref fileReader);
	/** Retrieves the dragon script module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsFRead, *clsVoid, *clsBool, *clsStr, *clsObj, *clsInt, *clsFlt;
		dsClass *clsTimeDate;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNewZCompressed);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetFilename);
	DEF_NATFUNC(nfGetLength);
	DEF_NATFUNC(nfGetPosition);
	DEF_NATFUNC(nfSetPosition);
	DEF_NATFUNC(nfMovePosition);
	DEF_NATFUNC(nfSetPositionEnd);
	
	DEF_NATFUNC(nfGetStreamVersion);
	DEF_NATFUNC(nfSetStreamVersion);
	
	DEF_NATFUNC(nfEOF);
	DEF_NATFUNC(nfReadChar);
	DEF_NATFUNC(nfReadByte);
	DEF_NATFUNC(nfReadShort);
	DEF_NATFUNC(nfReadUShort);
	DEF_NATFUNC(nfReadInt);
	DEF_NATFUNC(nfReadUInt);
	DEF_NATFUNC(nfReadVarUInt);
	DEF_NATFUNC(nfReadFloat);
	DEF_NATFUNC(nfReadString8);
	DEF_NATFUNC(nfReadString16);
	DEF_NATFUNC(nfReadString32);
	DEF_NATFUNC(nfReadVarString);
	DEF_NATFUNC(nfReadString);
	DEF_NATFUNC(nfSkipString8);
	DEF_NATFUNC(nfSkipString16);
	DEF_NATFUNC(nfSkipString32);
	DEF_NATFUNC(nfSkipVarString);
	DEF_NATFUNC(nfReadTimeDate);
#undef DEF_NATFUNC
};

// end of include only once
#endif
