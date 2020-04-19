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
	deClassMutableID( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassMutableID();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Dragonscript module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Unique id from real object. */
	decUniqueID &GetMutableID( dsRealObject *myself ) const;
	
	/** \brief Push unique id onto stack. */
	void PushMutableID( dsRunTime *rt, const decUniqueID &id );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsMUID, *clsVoid, *clsInt, *clsBool, *clsObj, *clsStr, *clsUID;
		dsClass *clsFileReader, *clsFileWriter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewValue );
	DEF_NATFUNC( nfNewUniqueID );
	DEF_NATFUNC( nfNewCopy );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetBitCount );
	DEF_NATFUNC( nfSetBitCount );
	DEF_NATFUNC( nfGetByteAt );
	DEF_NATFUNC( nfSetByteAt );
	DEF_NATFUNC( nfIncrement );
	DEF_NATFUNC( nfIncrementByInt );
	DEF_NATFUNC( nfIncrementByUniqueID );
	DEF_NATFUNC( nfIncrementByMutableID );
	DEF_NATFUNC( nfDecrement );
	DEF_NATFUNC( nfDecrementByInt );
	DEF_NATFUNC( nfDecrementByUniqueID );
	DEF_NATFUNC( nfDecrementByMutableID );
	DEF_NATFUNC( nfReset );
	
	DEF_NATFUNC( nfSetFrom );
	DEF_NATFUNC( nfSetFromMutable );
	
	DEF_NATFUNC( nfToUniqueID );
	DEF_NATFUNC( nfNextID );
	
	DEF_NATFUNC( nfToHexString );
	DEF_NATFUNC( nfSetFromHexString );
	
	DEF_NATFUNC( nfReadFromFile );
	DEF_NATFUNC( nfWriteToFile );
	
	DEF_NATFUNC( nfOpLess );
	DEF_NATFUNC( nfOpLessEqual );
	DEF_NATFUNC( nfOpGreater );
	DEF_NATFUNC( nfOpGreaterEqual );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfToString );
#undef DEF_NATFUNC
};

#endif
