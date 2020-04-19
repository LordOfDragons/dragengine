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

#ifndef _DEDSCLASSNAVIGATIONSPACE_H_
#define _DEDSCLASSNAVIGATIONSPACE_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deNavigationSpace;



/**
 * @brief Navigation Mesh Script Class.
 */
class deClassNavigationSpace : public dsClass{
private:
	deScriptingDragonScript *pDS;
	dsClass *pClsNavigationSpaceType;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new class. */
	deClassNavigationSpace( deScriptingDragonScript *ds );
	/** Cleans up the class. */
	virtual ~deClassNavigationSpace();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	/** Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Retrieves the navigation space or NULL if myself is NULL. */
	deNavigationSpace *GetNavigationSpace( dsRealObject *myself ) const;
	/** Pushes the navigation space which can be NULL. */
	void PushNavigationSpace( dsRunTime *rt, deNavigationSpace *navspace );
	
	inline dsClass *GetClassNavigationSpaceType() const{ return pClsNavigationSpaceType; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsNavSpace, *clsVoid, *clsBool, *clsInt, *clsFlt, *clsStr, *clsObj;
		dsClass *clsDVec, *clsVec, *clsQuat, *clsShaList;
		dsClass *clsNavigationSpaceType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNewCopy );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfSetOrientation );
	DEF_NATFUNC( nfGetLayer );
	DEF_NATFUNC( nfSetLayer );
	DEF_NATFUNC( nfGetType );
	DEF_NATFUNC( nfSetType );
	DEF_NATFUNC( nfGetBlockingPriority );
	DEF_NATFUNC( nfSetBlockingPriority );
	
	DEF_NATFUNC( nfGetVertexCount );
	DEF_NATFUNC( nfSetVertexCount );
	DEF_NATFUNC( nfGetVertexAt );
	DEF_NATFUNC( nfSetVertexAt );
	
	DEF_NATFUNC( nfGetEdgeCount );
	DEF_NATFUNC( nfSetEdgeCount );
	DEF_NATFUNC( nfGetEdgeVertex1 );
	DEF_NATFUNC( nfGetEdgeVertex2 );
	DEF_NATFUNC( nfGetEdgeType1 );
	DEF_NATFUNC( nfGetEdgeType2 );
	DEF_NATFUNC( nfSetEdgeAt );
	
	DEF_NATFUNC( nfGetCornerCount );
	DEF_NATFUNC( nfSetCornerCount );
	DEF_NATFUNC( nfGetCornerVertex );
	DEF_NATFUNC( nfGetCornerType );
	DEF_NATFUNC( nfSetCornerAt );
	
	DEF_NATFUNC( nfGetFaceCount );
	DEF_NATFUNC( nfSetFaceCount );
	DEF_NATFUNC( nfGetFaceCornerCount );
	DEF_NATFUNC( nfGetFaceType );
	DEF_NATFUNC( nfSetFaceAt );
	
	DEF_NATFUNC( nfGetWallCount );
	DEF_NATFUNC( nfSetWallCount );
	DEF_NATFUNC( nfGetWallFace );
	DEF_NATFUNC( nfGetWallType );
	DEF_NATFUNC( nfSetWallAt );
	
	DEF_NATFUNC( nfGetRoomCount );
	DEF_NATFUNC( nfSetRoomCount );
	DEF_NATFUNC( nfGetRoomFrontWallCount );
	DEF_NATFUNC( nfGetRoomBackWallCount );
	DEF_NATFUNC( nfGetRoomType );
	DEF_NATFUNC( nfSetRoomAt );
	
	DEF_NATFUNC( nfGetSnapDistance );
	DEF_NATFUNC( nfSetSnapDistance );
	DEF_NATFUNC( nfGetSnapAngle );
	DEF_NATFUNC( nfSetSnapAngle );
	
	DEF_NATFUNC( nfLayoutChanged );
	
	DEF_NATFUNC( nfGetBlockerShapeList );
	DEF_NATFUNC( nfSetBlockerShapeList );
	
	DEF_NATFUNC( nfHashCode );
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
