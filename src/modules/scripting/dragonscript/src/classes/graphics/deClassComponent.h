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
#ifndef _DECLASSCOMPONENT_H_
#define _DECLASSCOMPONENT_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deScriptingDragonScript;
class deComponent;
class deClassCamera;
class deClassColor;
class deClassDecal;
class deClassDVector;
class deClassImage;
class deClassModel;
class deClassQuaternion;
class deClassRig;
class deClassSkin;
class deClassVector;

// scene element component script class
class deClassComponent : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
	
	deClassModel *pClsMdl;
	deClassRig *pClsRig;
	deClassSkin *pClsSkin;
	deClassImage *pClsImg;
	deClassDecal *pClsDec;
	deClassColor *pClsClr;
	deClassCamera *pClsCam;
	deClassVector *pClsVec;
	deClassDVector *pClsDVec;
	deClassQuaternion *pClsQuat;
	dsClass *pClsComponentHintMovement;
	
public:
	// constructor
	deClassComponent( deEngine *GameEngine, deScriptingDragonScript *ScrMgr );
	~deClassComponent();
	
	// internal functions
	void CreateClassMembers( dsEngine *engine );
	deComponent *GetComponent( dsRealObject *This ) const;
	void PushComponent( dsRunTime *rt, deComponent *component );
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	
	inline deClassModel *GetClassModel() const{ return pClsMdl; }
	inline deClassRig *GetClassRig() const{ return pClsRig; }
	inline deClassSkin *GetClassSkin() const{ return pClsSkin; }
	inline deClassImage *GetClassImage() const{ return pClsImg; }
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	inline deClassDecal *GetClassDecal() const{ return pClsDec; }
	inline deClassColor *GetClassColor() const{ return pClsClr; }
	inline deClassCamera *GetClassCamera() const{ return pClsCam; }
	inline deClassVector *GetClassVector() const{ return pClsVec; }
	inline deClassDVector *GetClassDVector() const{ return pClsDVec; }
	inline deClassQuaternion *GetClassQuaternion() const{ return pClsQuat; }
	inline dsClass *GetClassComponentHintMovement() const{ return pClsComponentHintMovement; }

private:
	struct sInitData{
		dsClass *clsCom, *clsVoid, *clsInt, *clsFlt, *clsStr;
		dsClass *clsMdl, *clsSkin, *clsImg, *clsRig;
		dsClass *clsVec, *clsMat, *clsDMat, *clsObj, *clsBool, *clsCL;
		dsClass *clsDec, *clsClr, *clsCam, *clsQuat, *clsDVec;
		dsClass *clsDSkin, *clsTexMat2, *clsOccM;
		dsClass *clsLayerMask;
		dsClass *clsBlock;
		dsClass *clsComponentHintMovement;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfNew2 );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetModel );
	DEF_NATFUNC( nfGetRig );
	DEF_NATFUNC( nfGetSkin );
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfGetScaling );
	DEF_NATFUNC( nfGetMatrix );
	DEF_NATFUNC( nfGetInverseMatrix );
	
	DEF_NATFUNC( nfGetBoneCount );
	DEF_NATFUNC( nfIndexOfBoneNamed );
	DEF_NATFUNC( nfBoneGetName );
	DEF_NATFUNC( nfBoneGetPosition );
	DEF_NATFUNC( nfBoneSetPosition );
	DEF_NATFUNC( nfBoneGetRotation );
	DEF_NATFUNC( nfBoneSetRotation );
	DEF_NATFUNC( nfBoneGetMatrix );
	DEF_NATFUNC( nfBoneGetInverseMatrix );
	DEF_NATFUNC( nfGetBonePosition );
	DEF_NATFUNC( nfGetBoneRotation );
	DEF_NATFUNC( nfGetBoneMatrix );
	DEF_NATFUNC( nfGetBoneInverseMatrix );
	DEF_NATFUNC( nfGetBoneOriginMatrix );
	DEF_NATFUNC( nfGetBoneOriginInverseMatrix );
	DEF_NATFUNC( nfSetBonePosition );
	DEF_NATFUNC( nfSetBoneRotation );
	
	DEF_NATFUNC( nfGetVertexPositionSetCount );
	DEF_NATFUNC( nfIndexOfVertexPositionSetNamed );
	DEF_NATFUNC( nfVertexPositionSetGetNameAt );
	DEF_NATFUNC( nfVertexPositionSetGetWeightAt );
	DEF_NATFUNC( nfVertexPositionSetGetWeightNamed );
	DEF_NATFUNC( nfVertexPositionSetSetWeightAt );
	DEF_NATFUNC( nfVertexPositionSetSetWeightNamed );
	
	DEF_NATFUNC( nfGetVisible );
	DEF_NATFUNC( nfSetVisible );
	
	DEF_NATFUNC( nfSetModel );
	DEF_NATFUNC( nfSetModelKeepTextures );
	DEF_NATFUNC( nfSetSkin );
	DEF_NATFUNC( nfSetRig );
	DEF_NATFUNC( nfSetModelAndSkin );
	DEF_NATFUNC( nfGetOcclusionMesh );
	DEF_NATFUNC( nfSetOcclusionMesh );
	DEF_NATFUNC( nfGetAudioModel );
	DEF_NATFUNC( nfSetAudioModel );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfSetOrientation );
	DEF_NATFUNC( nfSetScaling );
	
	DEF_NATFUNC( nfGetDynamicSkin );
	DEF_NATFUNC( nfSetDynamicSkin );
	
	DEF_NATFUNC( nfGetLayerMask );
	DEF_NATFUNC( nfSetLayerMask );
	
	DEF_NATFUNC( nfUpdateBones );
	DEF_NATFUNC( nfCopyBonesToComponent );
	
	DEF_NATFUNC( nfIndexOfTextureNamed );
	DEF_NATFUNC( nfIndexOfTextureClosedTo );
	DEF_NATFUNC( nfGetTextureCount );
	DEF_NATFUNC( nfGetTextureNameAt );
	DEF_NATFUNC( nfGetTextureSkinAt );
	DEF_NATFUNC( nfGetTextureTextureAt );
	DEF_NATFUNC( nfSetTextureSkinAt );
	DEF_NATFUNC( nfGetTextureTransformAt );
	DEF_NATFUNC( nfSetTextureTransformAt );
	DEF_NATFUNC( nfGetTextureDynamicSkinAt );
	DEF_NATFUNC( nfSetTextureDynamicSkinAt );
	
	DEF_NATFUNC( nfAddDecal );
	DEF_NATFUNC( nfRemoveDecal );
	DEF_NATFUNC( nfRemoveAllDecals );
	DEF_NATFUNC( nfGetDecalCount );
	DEF_NATFUNC( nfForEachDecal );
	
	DEF_NATFUNC( nfGetHintMovement );
	DEF_NATFUNC( nfSetHintMovement );
	DEF_NATFUNC( nfGetEnableGI );
	DEF_NATFUNC( nfSetEnableGI );
	DEF_NATFUNC( nfGetHintGIImportance );
	DEF_NATFUNC( nfSetHintGIImportance );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

// end of include only once
#endif
