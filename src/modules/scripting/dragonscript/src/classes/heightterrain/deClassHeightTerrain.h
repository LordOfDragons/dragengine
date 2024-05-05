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
