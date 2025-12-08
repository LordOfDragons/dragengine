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

#ifndef _DECLASSMEMORYFILE_H_
#define _DECLASSMEMORYFILE_H_

#include <libdscript/libdscript.h>

class decMemoryFile;
class deScriptingDragonScript;



/**
 * \brief Memory file script class.
 */
class deClassMemoryFile : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassMemoryFile(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassMemoryFile();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Memory file or \em NULL if myself is \em NULL. */
	decMemoryFile *GetMemoryFile(dsRealObject *myself) const;
	
	/**
	 * \brief Pushe memory file on stack.
	 * \details Memory file can be \em NULL in which case a null object is pushed. */
	void PushMemoryFile(dsRunTime *rt, decMemoryFile *memoryFile);
	
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsMemFile;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsStr;
		dsClass *clsObj;
		dsClass *clsInt;
		dsClass *clsFlt;
		
		dsClass *clsFRead;
		dsClass *clsFWrite;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNewCopy);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetFilename);
	DEF_NATFUNC(nfGetSize);
	DEF_NATFUNC(nfSetSize);
	
	DEF_NATFUNC(nfGetReader);
	DEF_NATFUNC(nfGetWriter);
	DEF_NATFUNC(nfGetReaderZCompressed);
	DEF_NATFUNC(nfGetWriterZCompressed);
#undef DEF_NATFUNC
};

#endif
