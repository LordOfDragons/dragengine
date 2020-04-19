/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
