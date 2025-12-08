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

#ifndef _DECLASSFILEWRITER_H_
#define _DECLASSFILEWRITER_H_

#include <libdscript/libdscript.h>

class decBaseFileWriter;
class deScriptingDragonScript;



/**
 * @brief File Writer Script Class.
 */
class deClassFileWriter : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new script class. */
	deClassFileWriter(deScriptingDragonScript *ds);
	/** Cleans up the script class. */
	virtual ~deClassFileWriter();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Create class members. */
	void CreateClassMembers(dsEngine *engine);
	/** Retrieves the fileWriter or NULL if myself is NULL. */
	decBaseFileWriter *GetFileWriter(dsRealObject *myself) const;
	/** Retrieves the stream version. */
	int GetStreamVersion(dsRealObject *myself) const;
	/** Pushes the file writer on the stack. File writer can be NULL to push a null object. */
	void PushFileWriter(dsRunTime *rt, decBaseFileWriter::Ref fileWriter);
	/** Retrieves the dragon script module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsFileWriter;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsString;
		dsClass *clsObject;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsTimeDate;
		
		dsClass *clsFileReader;
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
	
	DEF_NATFUNC(nfGetStreamVersion);
	DEF_NATFUNC(nfSetStreamVersion);
	
	DEF_NATFUNC(nfGetPosition);
	DEF_NATFUNC(nfSetPosition);
	DEF_NATFUNC(nfMovePosition);
	DEF_NATFUNC(nfSetPositionEnd);
	
	DEF_NATFUNC(nfWriteChar);
	DEF_NATFUNC(nfWriteByte);
	DEF_NATFUNC(nfWriteShort);
	DEF_NATFUNC(nfWriteUShort);
	DEF_NATFUNC(nfWriteInt);
	DEF_NATFUNC(nfWriteUInt);
	DEF_NATFUNC(nfWriteVarUInt);
	DEF_NATFUNC(nfWriteFloat);
	DEF_NATFUNC(nfWriteString8);
	DEF_NATFUNC(nfWriteString16);
	DEF_NATFUNC(nfWriteString32);
	DEF_NATFUNC(nfWriteVarString);
	DEF_NATFUNC(nfWriteString);
	DEF_NATFUNC(nfWriteData);
	DEF_NATFUNC(nfWriteData2);
	DEF_NATFUNC(nfWriteTimeDate);
#undef DEF_NATFUNC
};

#endif
