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

// include only once
#ifndef _DECLASSCACHEDVEGETATION_H_
#define _DECLASSCACHEDVEGETATION_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deScriptingDragonScript;



/**
 * @brief Class Cached Vegetation.
 *
 * Manages vegetation in a world using a grid of prop fields  per sector and
 * a prop field cache file.
 */
class deClassCachedVegetation : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new CLASS. */
	deClassCachedVegetation( deScriptingDragonScript *ds );
	/** Cleans up the CLASS. */
	virtual ~deClassCachedVegetation();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsCVeg, *clsVoid, *clsBool, *clsInt, *clsFlt, *clsStr, *clsObj;
		dsClass *clsCF, *clsPt, *clsWorld;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfSetCollisionFilter );
	
	DEF_NATFUNC( nfHasSector );
	DEF_NATFUNC( nfAddSector );
	DEF_NATFUNC( nfRemoveSector );
	DEF_NATFUNC( nfRemoveAllSectors );
	
	DEF_NATFUNC( nfSetWorld );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

// end of include only once
#endif
