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
#ifndef _DECLASSWORLD_H_
#define _DECLASSWORLD_H_

// includes
#include <libdscript/libdscript.h>
#include "dragengine/common/math/decMath.h"

// predefinitions
class deEngine;
class deRenderTarget;
class deScriptingDragonScript;
class deWorld;
class deClassBillboard;
class deClassCamera;
class deClassColor;
class deClassCollider;
class deClassCollisionInfo;
class deClassComponent;
class deClassDVector;
class deClassLight;
class deClassMatrix;
class deClassMicrophone;
class deClassNetworkState;
class deClassSkyInstance;
class deClassSpeaker;
class deClassVector;
class deClassTouchSensor;
class deClassHeightTerrain;
class deClassNavigationSpace;
class deClassEnvMapProbe;



// scene script class
class deClassWorld : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pDS;
	
	deClassCamera *pClsCam;
	deClassColor *pClsClr;
	deClassCollider *pClsCol;
	deClassCollisionInfo *pClsCI;
	deClassComponent *pClsComp;
	deClassDVector *pClsDVec;
	deClassHeightTerrain *pClsHT;
	deClassLight *pClsLig;
	deClassMatrix *pClsMat;
	deClassMicrophone *pClsMic;
	deClassNetworkState *pClsNS;
	deClassSkyInstance *pClsSkyInst;
	deClassSpeaker *pClsSpk;
	deClassTouchSensor *pClsTS;
	deClassVector *pClsVec;
	
public:
	// constructor
	deClassWorld(deEngine *GameEngine, deScriptingDragonScript *ScrMgr);
	~deClassWorld();
	
	// internal functions
	void CreateClassMembers(dsEngine *engine);
	
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	inline deScriptingDragonScript *GetDS() const{ return pDS; }
	
	inline deClassCamera *GetClassCamera() const{ return pClsCam; }
	inline deClassColor *GetClassColor() const{ return pClsClr; }
	inline deClassCollider *GetClassCollider() const{ return pClsCol; }
	inline deClassCollisionInfo *GetClassCollisionInfo() const{ return pClsCI; }
	inline deClassComponent *GetClassComponent() const{ return pClsComp; }
	inline deClassDVector *GetClassDVector() const{ return pClsDVec; }
	inline deClassHeightTerrain *GetClassHeightTerrain() const{ return pClsHT; }
	inline deClassLight *GetClassLight() const{ return pClsLig; }
	inline deClassMatrix *GetClassMatrix() const{ return pClsMat; }
	inline deClassMicrophone *GetClassMicrophone() const{ return pClsMic; }
	inline deClassNetworkState *GetClassNetworkState() const{ return pClsNS; }
	inline deClassSkyInstance *GetClassSkyInstance() const{ return pClsSkyInst; }
	inline deClassSpeaker *GetClassSpeaker() const{ return pClsSpk; }
	inline deClassTouchSensor *GetClassTouchSensor() const{ return pClsTS; }
	inline deClassVector *GetClassVector() const{ return pClsVec; }
	
	/** Retrieves the world or NULL if myself is NULL. */
	deWorld *GetWorld( dsRealObject *myself ) const;
	/** Pushes the world which can be NULL. */
	void PushWorld( dsRunTime *rt, deWorld *world );

private:
	struct sInitData{
		dsClass *clsWorld, *clsVoid, *clsInt, *clsFlt, *clsBool;
		dsClass *clsObject;
		dsClass *clsCam, *clsClr, *clsCol, *clsCI, *clsComp;
		dsClass *clsDVec, *clsHT, *clsLig, *clsCF, *clsMat;
		dsClass *clsMic, *clsNS, *clsSkyInst, *clsSpk, *clsTer;
		dsClass *clsTS, *clsVec, *clsPF, *clsFF, *clsDD;
		dsClass *clsNavSpace, *clsNavBlocker, *clsNavigator;
		dsClass *clsPEI, *clsEmp, *clsCLL, *clsLum;
		dsClass *clsBillboard;
		
		dsClass *clsSoundLevelMeter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfUpdate );
	
	DEF_NATFUNC( nfDetectCollisions );
	
	DEF_NATFUNC( nfGetGravity );
	DEF_NATFUNC( nfSetGravity );
	
	DEF_NATFUNC( nfGetDisableLights );
	DEF_NATFUNC( nfSetDisableLights );
	DEF_NATFUNC( nfGetAmbientLight );
	DEF_NATFUNC( nfSetAmbientLight );
	DEF_NATFUNC( nfGetLightColorMatrix );
	DEF_NATFUNC( nfSetLightColorMatrix );
	
	DEF_NATFUNC( nfGetSpeakerGain );
	DEF_NATFUNC( nfSetSpeakerGain);
	
	DEF_NATFUNC( nfGetHeightTerrain );
	DEF_NATFUNC( nfSetHeightTerrain );
	
	DEF_NATFUNC( nfGetSize );
	DEF_NATFUNC( nfSetSize );
	
	DEF_NATFUNC( nfGetSkyCount );
	DEF_NATFUNC( nfAddSky );
	DEF_NATFUNC( nfRemoveSky );
	DEF_NATFUNC( nfRemoveAllSkies );
	
	DEF_NATFUNC( nfGetCameraCount );
	DEF_NATFUNC( nfAddCamera );
	DEF_NATFUNC( nfRemoveCamera );
	DEF_NATFUNC( nfRemoveAllCameras );
	
	DEF_NATFUNC( nfGetSpeakerCount );
	DEF_NATFUNC( nfAddSpeaker );
	DEF_NATFUNC( nfRemoveSpeaker );
	DEF_NATFUNC( nfRemoveAllSpeakers );
	
	DEF_NATFUNC( nfGetMicrophoneCount );
	DEF_NATFUNC( nfAddMicrophone );
	DEF_NATFUNC( nfRemoveMicrophone );
	DEF_NATFUNC( nfRemoveAllMicrophones );
	
	DEF_NATFUNC( nfGetNetworkStateCount );
	DEF_NATFUNC( nfAddNetworkState );
	DEF_NATFUNC( nfRemoveNetworkState );
	DEF_NATFUNC( nfRemoveAllNetworkStates );
	
	DEF_NATFUNC( nfGetComponentCount );
	DEF_NATFUNC( nfAddComponent );
	DEF_NATFUNC( nfRemoveComponent );
	DEF_NATFUNC( nfRemoveAllComponents );
	
	DEF_NATFUNC( nfGetLightCount );
	DEF_NATFUNC( nfAddLight );
	DEF_NATFUNC( nfRemoveLight );
	DEF_NATFUNC( nfRemoveAllLights );
	
	DEF_NATFUNC( nfGetColliderCount );
	DEF_NATFUNC( nfAddCollider );
	DEF_NATFUNC( nfRemoveCollider );
	DEF_NATFUNC( nfRemoveAllColliders );
	
	DEF_NATFUNC( nfGetLumimeterCount );
	DEF_NATFUNC( nfAddLumimeter );
	DEF_NATFUNC( nfRemoveLumimeter );
	DEF_NATFUNC( nfRemoveAllLumimeters );
	
	DEF_NATFUNC( nfGetTouchSensorCount );
	DEF_NATFUNC( nfAddTouchSensor );
	DEF_NATFUNC( nfRemoveTouchSensor );
	DEF_NATFUNC( nfRemoveAllTouchSensors );
	
	DEF_NATFUNC( nfGetPropFieldCount );
	DEF_NATFUNC( nfAddPropField );
	DEF_NATFUNC( nfRemovePropField );
	DEF_NATFUNC( nfRemoveAllPropFields );
	
	DEF_NATFUNC( nfGetForceFieldCount );
	DEF_NATFUNC( nfAddForceField );
	DEF_NATFUNC( nfRemoveForceField );
	DEF_NATFUNC( nfRemoveAllForceFields );
	
	DEF_NATFUNC( nfGetParticleEmitterCount );
	DEF_NATFUNC( nfAddParticleEmitter );
	DEF_NATFUNC( nfRemoveParticleEmitter );
	DEF_NATFUNC( nfRemoveAllParticleEmitters );
	
	DEF_NATFUNC( nfGetDebugDrawerCount );
	DEF_NATFUNC( nfAddDebugDrawer );
	DEF_NATFUNC( nfRemoveDebugDrawer );
	DEF_NATFUNC( nfRemoveAllDebugDrawers );
	
	DEF_NATFUNC( nfGetNavigationSpaceCount );
	DEF_NATFUNC( nfAddNavigationSpace );
	DEF_NATFUNC( nfRemoveNavigationSpace );
	DEF_NATFUNC( nfRemoveAllNavigationSpaces );
	
	DEF_NATFUNC( nfGetNavigationBlockerCount );
	DEF_NATFUNC( nfAddNavigationBlocker );
	DEF_NATFUNC( nfRemoveNavigationBlocker );
	DEF_NATFUNC( nfRemoveAllNavigationBlockers );
	
	DEF_NATFUNC( nfGetNavigatorCount );
	DEF_NATFUNC( nfAddNavigator );
	DEF_NATFUNC( nfRemoveNavigator );
	DEF_NATFUNC( nfRemoveAllNavigators );
	
	DEF_NATFUNC( nfGetEnvMapProbeCount );
	DEF_NATFUNC( nfAddEnvMapProbe );
	DEF_NATFUNC( nfRemoveEnvMapProbe );
	DEF_NATFUNC( nfRemoveAllEnvMapProbes );
	
	DEF_NATFUNC( nfGetSoundLevelMeterCount );
	DEF_NATFUNC( nfAddSoundLevelMeter );
	DEF_NATFUNC( nfRemoveSoundLevelMeter );
	DEF_NATFUNC( nfRemoveAllSoundLevelMeters );
	
	DEF_NATFUNC( nfGetBillboardCount );
	DEF_NATFUNC( nfAddBillboard );
	DEF_NATFUNC( nfRemoveBillboard );
	DEF_NATFUNC( nfRemoveAllBillboards );
	
	DEF_NATFUNC( nfPointHits );
	DEF_NATFUNC( nfColliderHits );
	DEF_NATFUNC( nfColliderMoveHits );
	DEF_NATFUNC( nfColliderMoveHitsClosest );
	DEF_NATFUNC( nfRayHits );
	DEF_NATFUNC( nfRayHitsClosest );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

// end of include only once
#endif
