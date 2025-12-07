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

#ifndef _DECLASSMUTUABLEID_H_
#define _DECLASSMUTUABLEID_H_

#include <libdscript/libdscript.h>

class deEngine;
class decUniqueID;
class deClassUniqueID;
class deScriptingDragonScript;



/**
 * \brief Mutable unique identifier.
 */
class deClassMutableID : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassMutableID(deScriptingDragonScript &ds);
	
	/** \brief Clean up class. */
	virtual ~deClassMutableID();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Dragonscript module. */
	inline deScriptingDragonScript &GetDS() const{return pDS;}
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Unique id from real object. */
	decUniqueID &GetMutableID(dsRealObject *myself) const;
	
	/** \brief Push unique id onto stack. */
	void PushMutableID(dsRunTime *rt, const decUniqueID &id);
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsMUID, *clsVoid, *clsInt, *clsBool, *clsObj, *clsStr, *clsUID;
		dsClass *clsFileReader, *clsFileWriter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNewValue);
	DEF_NATFUNC(nfNewUniqueID);
	DEF_NATFUNC(nfNewCopy);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetBitCount);
	DEF_NATFUNC(nfSetBitCount);
	DEF_NATFUNC(nfGetByteAt);
	DEF_NATFUNC(nfSetByteAt);
	DEF_NATFUNC(nfIncrement);
	DEF_NATFUNC(nfIncrementByInt);
	DEF_NATFUNC(nfIncrementByUniqueID);
	DEF_NATFUNC(nfIncrementByMutableID);
	DEF_NATFUNC(nfDecrement);
	DEF_NATFUNC(nfDecrementByInt);
	DEF_NATFUNC(nfDecrementByUniqueID);
	DEF_NATFUNC(nfDecrementByMutableID);
	DEF_NATFUNC(nfReset);
	
	DEF_NATFUNC(nfSetFrom);
	DEF_NATFUNC(nfSetFromMutable);
	
	DEF_NATFUNC(nfToUniqueID);
	DEF_NATFUNC(nfNextID);
	
	DEF_NATFUNC(nfToHexString);
	DEF_NATFUNC(nfSetFromHexString);
	
	DEF_NATFUNC(nfReadFromFile);
	DEF_NATFUNC(nfWriteToFile);
	
	DEF_NATFUNC(nfOpLess);
	DEF_NATFUNC(nfOpLessEqual);
	DEF_NATFUNC(nfOpGreater);
	DEF_NATFUNC(nfOpGreaterEqual);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
	DEF_NATFUNC(nfToString);
#undef DEF_NATFUNC
};

#endif
