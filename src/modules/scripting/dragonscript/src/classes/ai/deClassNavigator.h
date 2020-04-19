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

#ifndef _DEDSCLASSNAVIGATOR_H_
#define _DEDSCLASSNAVIGATOR_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deNavigator;



/**
 * @brief Navigator Script Class.
 */
class deClassNavigator : public dsClass{
private:
	deScriptingDragonScript *pDS;
	dsClass *pClsNavigationSpaceType;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassNavigator( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	virtual ~deClassNavigator();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Retrieves the navigator or NULL if myself is NULL. */
	deNavigator *GetNavigator( dsRealObject *myself ) const;
	/** Pushes the navigator which can be NULL. */
	void PushNavigator( dsRunTime *rt, deNavigator *navigator );
	
	inline dsClass *GetClassNavigationSpaceType() const{ return pClsNavigationSpaceType; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsNavigator;
		
		dsClass *clsBool;
		dsClass *clsFloat;
		dsClass *clsInteger;
		dsClass *clsObject;
		dsClass *clsString;
		dsClass *clsVoid;
		
		dsClass *clsCollider;
		dsClass *clsDVector;
		dsClass *clsNavInfo;
		dsClass *clsQuaternion;
		dsClass *clsVector;
		dsClass *clsNavPath;
		dsClass *clsNavigationSpaceType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetLayer );
	DEF_NATFUNC( nfSetLayer );
	DEF_NATFUNC( nfGetSpaceType );
	DEF_NATFUNC( nfSetSpaceType );
	DEF_NATFUNC( nfGetMaxOutsideDistance );
	DEF_NATFUNC( nfSetMaxOutsideDistance );
	
	DEF_NATFUNC( nfGetDefaultFixCost );
	DEF_NATFUNC( nfSetDefaultFixCost );
	DEF_NATFUNC( nfGetDefaultCostPerMeter );
	DEF_NATFUNC( nfSetDefaultCostPerMeter );
	DEF_NATFUNC( nfGetBlockingCost );
	DEF_NATFUNC( nfSetBlockingCost );
	
	DEF_NATFUNC( nfGetTypeFixCost );
	DEF_NATFUNC( nfSetTypeFixCost );
	DEF_NATFUNC( nfGetTypeCostPerMeter );
	DEF_NATFUNC( nfSetTypeCostPerMeter );
	DEF_NATFUNC( nfRemoveType );
	DEF_NATFUNC( nfRemoveAllTypes );
	
	DEF_NATFUNC( nfFindPath );
	
	DEF_NATFUNC( nfNearestPoint );
	DEF_NATFUNC( nfLineCollide );
	
	DEF_NATFUNC( nfPathCollideRay );
	DEF_NATFUNC( nfPathCollideRay2 );
	DEF_NATFUNC( nfPathCollideShape );
	DEF_NATFUNC( nfPathCollideShape2 );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
