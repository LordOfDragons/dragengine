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

#ifndef _DECLASSMATH_H_
#define _DECLASSMATH_H_

#include <libdscript/libdscript.h>
#include <dragengine/common/math/decMath.h>

class deScriptingDragonScript;



/**
 * @brief Math Routines.
 * Script class providing math routines. This class provides all the functions the DragonScript
 * Math class does but differs in that they operate using degrees instead of radians. In
 * addition some helper functions are provided for common operations to reduce the scripting work.
 */
class deClassMath : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new script class. */
	deClassMath( deScriptingDragonScript *ds );
	/** Cleans up the script class. */
	virtual ~deClassMath();
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
		dsClass *clsMath, *clsVoid, *clsBool, *clsStr, *clsInt, *clsFlt, *clsObj;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC( nfacos );
	DEF_NATFUNC( nfasin );
	DEF_NATFUNC( nfatan );
	DEF_NATFUNC( nfatan2 );
	DEF_NATFUNC( nfcos );
	DEF_NATFUNC( nfcosh );
	DEF_NATFUNC( nfsin );
	DEF_NATFUNC( nfsinh );
	DEF_NATFUNC( nftan );
	DEF_NATFUNC( nftanh );
	
	DEF_NATFUNC( nfDegToRad );
	DEF_NATFUNC( nfRadToDeg );
	DEF_NATFUNC( nfLinearStep );
	DEF_NATFUNC( nfLinearStep2 );
	DEF_NATFUNC( nfSmoothStep );
	DEF_NATFUNC( nfSmoothStep2 );
	DEF_NATFUNC( nfNormalize );
	DEF_NATFUNC( nfNormalize2 );
	DEF_NATFUNC( nfMix );
	DEF_NATFUNC( nfRandom );
	DEF_NATFUNC( nfRandomInt );
	DEF_NATFUNC( nfRandomFloat );
	DEF_NATFUNC( nfProbability );
#undef DEF_NATFUNC
};

#endif
