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

#ifndef _DECLASSHEIGHTTERRAIN_H_
#define _DECLASSHEIGHTTERRAIN_H_

#include <libdscript/libdscript.h>

class deEngine;
class deHeightTerrain;
class deScriptingDragonScript;



/**
 * \brief Height Terrain Script Class.
 *
 * Height Terrain Script Class.
 */
class deClassHeightTerrain : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	dsClass *pClsNavigationSpaceType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassHeightTerrain( deScriptingDragonScript &ds );
	
	/** \brief Clean up script class. */
	virtual ~deClassHeightTerrain();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Height terrain from object or NULL. */
	deHeightTerrain *GetHeightTerrain( dsRealObject *myself ) const;
	
	/** \brief Push height terrain or NULL. */
	void PushHeightTerrain( dsRunTime *rt, deHeightTerrain *hterrain );
	
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	inline dsClass *GetClassNavigationSpaceType() const{ return pClsNavigationSpaceType; }
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsHT;
		dsClass *clsVoid;
		dsClass *clsInt;
		dsClass *clsStr;
		dsClass *clsObj;
		dsClass *clsBool;
		dsClass *clsFlt;
		dsClass *clsVec;
		dsClass *clsVec2;
		dsClass *clsQuat;
		dsClass *clsSkin;
		dsClass *clsDec;
		dsClass *clsDVec;
		dsClass *clsImg;
		dsClass *clsPt;
		dsClass *clsCF;
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
	
	DEF_NATFUNC( nfGetSectorSize );
	DEF_NATFUNC( nfGetSectorResolution );
	
	DEF_NATFUNC( nfGetBaseHeight );
	DEF_NATFUNC( nfSetBaseHeight );
	DEF_NATFUNC( nfGetHeightScaling );
	DEF_NATFUNC( nfSetHeightScaling );
	DEF_NATFUNC( nfGetCollisionFilter );
	DEF_NATFUNC( nfSetCollisionFilter );
	
	// sectors
	DEF_NATFUNC( nfGetSectorCount );
	DEF_NATFUNC( nfHasSector );
	DEF_NATFUNC( nfIndexOfSector );
	DEF_NATFUNC( nfAddSector );
	DEF_NATFUNC( nfRemoveSector );
	DEF_NATFUNC( nfRemoveAllSectors );
	
	DEF_NATFUNC( nfGetHeightImage );
	DEF_NATFUNC( nfSetHeightImage );
	
	DEF_NATFUNC( nfSetVisibilityFromImage );
	
	// textures
	DEF_NATFUNC( nfGetTextureCount );
	DEF_NATFUNC( nfAddTexture );
	DEF_NATFUNC( nfTextureGetSkin );
	DEF_NATFUNC( nfTextureSetSkin );
	DEF_NATFUNC( nfTextureGetUVOffset );
	DEF_NATFUNC( nfTextureSetUVOffset );
	DEF_NATFUNC( nfTextureGetUVScaling );
	DEF_NATFUNC( nfTextureSetUVScaling );
	DEF_NATFUNC( nfTextureGetUVRotation );
	DEF_NATFUNC( nfTextureSetUVRotation );
	DEF_NATFUNC( nfTextureGetMask );
	DEF_NATFUNC( nfTextureSetMask );
	
	// decals
	DEF_NATFUNC( nfGetDecalCount );
	DEF_NATFUNC( nfAddDecal );
	DEF_NATFUNC( nfRemoveDecal );
	DEF_NATFUNC( nfRemoveAllDecals );
	
	// navspaces
	DEF_NATFUNC( nfGetNavSpaceCount );
	DEF_NATFUNC( nfAddNavSpace );
	DEF_NATFUNC( nfRemoveNavSpaceAt );
	DEF_NATFUNC( nfRemoveAllNavSpaces );
	
	DEF_NATFUNC( nfNavSpaceGetType );
	DEF_NATFUNC( nfNavSpaceSetType );
	DEF_NATFUNC( nfNavSpaceGetLayer );
	DEF_NATFUNC( nfNavSpaceSetLayer );
	DEF_NATFUNC( nfNavSpaceGetSnapDistance );
	DEF_NATFUNC( nfNavSpaceSetSnapDistance );
	DEF_NATFUNC( nfNavSpaceGetSnapAngle );
	DEF_NATFUNC( nfNavSpaceSetSnapAngle );
	
	DEF_NATFUNC( nfNavSpaceGetCornerCount );
	DEF_NATFUNC( nfNavSpaceSetCornerCount );
	DEF_NATFUNC( nfNavSpaceGetCornerAt );
	DEF_NATFUNC( nfNavSpaceSetCornerAt );
	
	DEF_NATFUNC( nfNavSpaceGetEdgeCount );
	DEF_NATFUNC( nfNavSpaceSetEdgeCount );
	DEF_NATFUNC( nfNavSpaceEdgeGetPoint1 );
	DEF_NATFUNC( nfNavSpaceEdgeGetPoint2 );
	DEF_NATFUNC( nfNavSpaceEdgeGetType1 );
	DEF_NATFUNC( nfNavSpaceEdgeGetType2 );
	DEF_NATFUNC( nfNavSpaceSetEdgeAt );
	
	DEF_NATFUNC( nfNavSpaceGetFaceCount );
	DEF_NATFUNC( nfNavSpaceSetFaceCount );
	DEF_NATFUNC( nfNavSpaceFaceGetCornerCount );
	DEF_NATFUNC( nfNavSpaceFaceGetType );
	DEF_NATFUNC( nfNavSpaceSetFaceAt );
	DEF_NATFUNC( nfNavSpaceLayoutChanged );
	
	// special
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
