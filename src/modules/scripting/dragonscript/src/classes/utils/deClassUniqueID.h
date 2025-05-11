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

#ifndef _DECLASSUNIQUEID_H_
#define _DECLASSUNIQUEID_H_

#include <libdscript/libdscript.h>

class deEngine;
class decUniqueID;
class deScriptingDragonScript;



/**
 * \brief Immutable unique identifier.
 */
class deClassUniqueID : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassUniqueID( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassUniqueID();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Dragonscript module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Unique id from real object. */
	decUniqueID &GetUniqueID( dsRealObject *myself ) const;
	
	/** \brief Push unique id onto stack. */
	void PushUniqueID( dsRunTime *rt, const decUniqueID &id );
	
	/** \brief Push unique id onto stack. */
	void PushUniqueID( dsRunTime *rt, int byteCount, unsigned char *bytes );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsUID, *clsVoid, *clsInt, *clsBool, *clsObj, *clsStr;
		dsClass *clsFileReader, *clsFileWriter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewInt );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetBitCount );
	DEF_NATFUNC( nfGetByteAt );
	
	DEF_NATFUNC( nfToHexString );
	DEF_NATFUNC( nfNewFromHexString );
	DEF_NATFUNC(nfZero);
	DEF_NATFUNC(nfNotZero);
	
	DEF_NATFUNC( nfReadFromFile );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfOpAddInt );
	DEF_NATFUNC( nfOpAddID );
	DEF_NATFUNC( nfOpSubInt );
	DEF_NATFUNC( nfOpSubID );
	DEF_NATFUNC( nfOpLess );
	DEF_NATFUNC( nfOpLessEqual );
	DEF_NATFUNC( nfOpGreater );
	DEF_NATFUNC( nfOpGreaterEqual );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfCompare );
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfToString );
#undef DEF_NATFUNC
};

#endif
