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

#ifndef _DECLASSSTRINGID_H_
#define _DECLASSSTRINGID_H_

#include <libdscript/libdscript.h>
#include <dragengine/common/collection/decPointerDictionary.h>
#include <dragengine/common/string/decStringList.h>

class deEngine;
class deScriptingDragonScript;



/**
 * \brief Unique string ID script class.
 * 
 * Stores table of unique strings mapped to position number. Each StringID instance stores
 * the table number. Testing strings is fast since it is only an integer comparison.
 * StringID instances are immutable like strings.
 */
class deClassStringID : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	decStringList pStrings;
	decPointerDictionary pLookUpTable;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassStringID(deScriptingDragonScript &ds);
	
	/** \brief Clean up class. */
	virtual ~deClassStringID();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief DragonScript module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	
	
	/** \brief Number of strings. */
	int GetStringCount() const;
	
	/** \brief String index. */
	int GetStringID(dsRealObject *myself);
	
	/** \brief Push string id with index. */
	void PushStringID(dsRunTime *rt, int index);
	
	/**
	 * \brief Get index for string.
	 * 
	 * If string is absent from mapping table inserts the string first. The returned
	 * index stays valid for the entire lifetime of the script module.
	 */
	int InsertString(const char *string);
	
	/** \brief String at index. */
	const decString &GetStringAt(int index) const;
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsStringID;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsBool;
		dsClass *clsObject;
		dsClass *clsString;
		
		dsClass *clsFileReader;
		dsClass *clsFileWriter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNew2);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetString);
	
	DEF_NATFUNC(nfReadFromFile);
	DEF_NATFUNC(nfWriteToFile);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
	DEF_NATFUNC(nfToString);
#undef DEF_NATFUNC
};

#endif
