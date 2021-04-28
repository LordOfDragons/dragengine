/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLRLIGHT_H_
#define _DEOGLRLIGHT_H_

#include "../component/deoglComponentList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/resources/light/deLight.h>

class deoglCollideList;
class deoglLightShader;
class deoglLightShaderConfig;
class deoglLightVolume;
class deoglOcclusionQuery;
class deoglOptimizerLight;
class deoglRCanvasView;
class deoglRComponent;
class deoglRenderPlan;
class deoglRenderThread;
class deoglRSkin;
class deoglRDynamicSkin;
class deoglRWorld;
class deoglSPBlockUBO;
class deoglShadowCaster;
class deoglSkinState;
class deoglSkinTexture;
class deoglWorldOctree;
class deoglOcclusionTest;

class decConvexVolumeList;
class deoglDCollisionVolume;
class decShapeBox;



/**
 * Render light.
 */
class deoglRLight : public deObject{
public:
	/** Shader Types. */
	enum eShaderTypes{
		/** No shadow casting. */
		estNoShadow,
		
		/** Single solid no transparent shadow. */
		estSolid1,
		
		/** Single solid and transparent shadow. */
		estSolid1Transp1,
		
		/** Single solid no transparent shadow without ambient lighting. */
		estSolid1NoAmbient,
		
		/** Single solid and transparent shadow without ambient lighting. */
		estSolid1Transp1NoAmbient,
		
		/** Double solid no transparent shadow. */
		estSolid2,
		
		/** Double solid and single transparent shadow. */
		estSolid2Transp1,
		
		/** Double solid and transparent shadow. */
		estSolid2Transp2,
		
		/** Luminance only single solid. */
		estLumSolid1,
		
		/** Luminance only single solid without ambient lighting. */
		estLumSolid1NoAmbient,
		
		/** Luminance only double solid. */
		estLumSolid2,
		
		/** GI rays no shadow casting. */
		estGIRayNoShadow,
		
		/** GI rays single solid shadow. */
		estGIRaySolid1,
		
		/** GI rays double solid shadow. */
		estGIRaySolid2,
		
		/** Number of shaders. */
		EST_COUNT
	};
	
	
	
public:
	deoglRenderThread &pRenderThread;
	
	deoglRWorld *pParentWorld;
	deoglWorldOctree *pOctreeNode;
	
	bool pActive;
	deLight::eLightTypes pLightType;
	bool pCastShadows;
	float pSpotAngle;
	float pSpotRatio;
	float pSpotSmoothness;
	float pSpotExponent;
	deLight::eMovementHints pHintMovement;
	int pHintShadowImportance;
	float pIntensity;
	float pAmbientRatio;
	decColor pColor;
	decLayerMask pLayerMask;
	decLayerMask pLayerMaskShadow;
	decObjectSet pShadowIgnoreComponents;
	
	deoglRSkin *pLightSkin;
	deoglRCanvasView *pLightCanvas;
	deoglRDynamicSkin *pDynamicSkin;
	decTexMatrix2 pTransform;
	
	deoglSkinState *pSkinState;
	deoglSkinTexture *pUseSkinTexture;
	bool pDirtyPrepareSkinStateRenderables;
	
	bool pDirtyPrepareLightCanvas;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	decDVector pFullMinExtend;
	decDVector pFullMaxExtend;
	bool pDirtyExtends;
	bool pDirtyFullExtends;
	
	float pAttenCoeff;
	float pRange;
	float pDampCoeff;
	float pDampThreshold;
	
	decConvexVolumeList *pConvexVolumeList;
	deoglOptimizerLight *pOptimizer;
	
	deoglShadowCaster *pShadowCaster;
	bool pDirtyStaticShadows;
	bool pDirtyDynamicShadows;
	
	deoglComponentList pStaticComponentList;
	deoglComponentList pDynamicComponentList;
	deoglCollideList *pStaticCollideList;
	deoglCollideList *pDynamicCollideList;
	bool pDirtyCollideLists;
	
	decDMatrix pMatrix;
	decDMatrix pInverseMatrix;
	
	deoglLightVolume *pLightVolume;
	decShapeBox *pLightVolumeCropBox;
	bool pDirtyConvexVolumeList;
	
	deoglOcclusionQuery *pOcclusionQuery;
	
	decVector2 pTrackAngles;
	
	deoglDCollisionVolume *pColVol;
	bool pDirtyColVol;
	bool pCameraInside;
	
	bool pDirtyTouching;
	bool pInsideCamera;
	
	bool pMarked;
	
	bool pUpdateOnRemoveComponent;
	
	deoglLightShader *pShaders[ EST_COUNT ];
	deoglSPBlockUBO *pParamBlockLight;
	deoglSPBlockUBO *pParamBlockInstance;
	
	bool pWorldMarkedRemove;
	
	deoglRLight *pLLWorldPrev;
	deoglRLight *pLLWorldNext;
	
	decPointerLinkedList::cListEntry pLLPrepareForRenderWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render light. */
	deoglRLight( deoglRenderThread &renderThread );
	
	/** Clean up render light. */
	virtual ~deoglRLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** Parent world or \em NULL if not in a world. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world or \em NULL if not in a world. */
	void SetParentWorld( deoglRWorld *parentWorld );
	
	/** Octree node or \em NULL if not inserted into the parent world octree. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/**
	 * Set octree node or \em NULL if not inserted into the parent world octree.
	 * \details This call is to be used only by the deoglWorldOctree only.
	 */
	void SetOctreeNode( deoglWorldOctree *octreeNode );
	
	/** Update octree node. */
	void UpdateOctreeNode();
	
	/** Update light skin if used. */
	void UpdateSkin( float elapsed );
	
	
	
	/** Light is active. */
	inline bool GetActive() const{ return pActive; }
	
	/** Set light is active. */
	void SetActive( bool active );
	
	/** Light type. */
	inline deLight::eLightTypes GetLightType() const{ return pLightType; }
	
	/** Set light type. */
	void SetLightType( deLight::eLightTypes type );
	
	/** Light casts shadows. */
	inline bool GetCastShadows() const{ return pCastShadows; }
	
	/** Set if light casts shadows. */
	void SetCastShadows( bool castShadows );
	
	/** Outer spot Angle. */
	inline float GetSpotAngle() const{ return pSpotAngle; }
	
	/** Set outer spot angle. */
	void SetSpotAngle( float angle );
	
	/** Spot ratio as height divided by width. */
	inline float GetSpotRatio() const{ return pSpotRatio; }
	
	/** Set spot ratio as height divided by width. */
	void SetSpotRatio( float ratio );
	
	/** Spot smoothness. */
	inline float GetSpotSmoothness() const{ return pSpotSmoothness; }
	
	/** Set spot smoothness. */
	void SetSpotSmoothness( float smoothness );
	
	/** Spot exponent. */
	inline float GetSpotExponent() const{ return pSpotExponent; }
	
	/** Set spot exponent. */
	void SetSpotExponent( float exponent );
	
	/** Movement hint. */
	inline deLight::eMovementHints GetHintMovement() const{ return pHintMovement; }
	
	/** Set movement hint. */
	void SetHintMovement( deLight::eMovementHints movement );
	
	/** Shadow importance. */
	inline int GetHintShadowImportance() const{ return pHintShadowImportance; }
	
	/** Set shadow importance. */
	void SetHintShadowImportance( int importance );
	
	/** Intensity. */
	inline float GetIntensity() const{ return pIntensity; }
	
	/** Set intensity. */
	void SetIntensity( float intensity );
	
	/** Ambient intensity ratio. */
	inline float GetAmbientRatio() const{ return pAmbientRatio; }
	
	/** Set ambient intensity ratio. */
	void SetAmbientRatio( float ratio );
	
	/** Light canvas or \em NULL if not used. */
	inline deoglRCanvasView *GetLightCanvas() const{ return pLightCanvas; }
	
	/** Set light canvas or \em NULL if not used. */
	void SetLightCanvas( deoglRCanvasView *canvas );
	
	/** Light color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** Set light color. */
	void SetColor( const decColor &color );
	
	/** Camera layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** Set camera layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** Shadow layer mask. */
	inline const decLayerMask &GetLayerMaskShadow() const{ return pLayerMaskShadow; }
	
	/** Set shadow layer mask. */
	void SetLayerMaskShadow( const decLayerMask &layerMask );
	
	/** Remove all shadow ignore components. */
	void RemoveAllShadowIgnoreComponents();
	
	/** Add shadow ignore component. */
	void AddShadowIgnoreComponent( deoglRComponent *component );
	
	/** Has shadow ignore component. */
	bool HasShadowIgnoreComponent( deoglRComponent *component ) const;
	
	/**
	 * Check if camera layer mask matches light/shadow layer masks.
	 * 
	 * Static shadow maps are only valid if this method returns true.
	 */
	bool StaticMatchesCamera( const decLayerMask &layerMask ) const;
	
	
	
	/** Light skin or \em NULL if not used. */
	inline deoglRSkin *GetLightSkin() const{ return pLightSkin; }
	
	/** Set light skin or \em NULL if not used. */
	void SetLightSkin( deoglRSkin *skin );
	
	/** Dynamic skin or \em NULL if not used. */
	inline deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Set dynamic skin or \em NULL if not used. */
	void SetDynamicSkin( deoglRDynamicSkin *dynamicSkin );
	
	/** Light texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** Set light texture coordinate transformation matrix. */
	void SetTransform( const decTexMatrix2 &matrix );
	
	/** Skin state or \em NULL if not used. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/** Skin texture to use or \em NULL. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	void InitSkinStateCalculatedProperties();
	void UpdateSkinStateCalculatedProperties();
	
	void DirtyPrepareSkinStateRenderables();
	void PrepareSkinStateRenderables();
	
	void DynamicSkinRenderablesChanged();
	void UpdateRenderableMapping();
	
	void DirtyPrepareLightCanvas();
	
	
	
	/** Transformation matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** Inverse transformation matrix. */
	inline const decDMatrix &GetInverseMatrix() const{ return pInverseMatrix; }
	
	/** Set matrices. */
	void SetMatrix( const decDMatrix &matrix );
	
	
	
	/** Full minimum extend. */
	const decDVector &GetFullMinExtend();
	
	/** Full maximum extend. */
	const decDVector &GetFullMaxExtend();
	
	/** Mark full extends dirty. */
	void SetDirtyFullExtends();
	
	/** Minimum extend. */
	const decDVector &GetMinimumExtend();
	
	/** Maximum extend. */
	const decDVector &GetMaximumExtend();
	
	/** Mark extends dirty. */
	void SetDirtyExtends();
	
	
	
	/** Marked. */
	inline bool GetMarked() const{ return pMarked; }
	
	/** Set marked. */
	inline void SetMarked( bool marked ){ pMarked = marked; }
	
	
	
	/** Convex light volume. */
	inline decConvexVolumeList *GetConvexVolumeList() const{ return pConvexVolumeList; }
	
	/** Update convex volume. */
	void UpdateConvexVolumeList();
	
	/** Has extends. */
	bool HasExtends() const;
	
	/** Collision volume. */
	deoglDCollisionVolume *GetCollisionVolume();
	
	/** Mark collision volume dirty. */
	void SetDirtyCollisionVolume();
	
	
	
	/** Camera is inside light volume. */
	inline bool GetCameraInside() const{ return pCameraInside; }
	
	/** Test if camera is inside light volume. */
	void TestCameraInside( deoglRenderPlan &plan );
	
	
	
	/** Static component list. */
	inline deoglComponentList &GetStaticComponentList(){ return pStaticComponentList; }
	inline const deoglComponentList &GetStaticComponentList() const{ return pStaticComponentList; }
	
	/** Dynamic component list. */
	inline deoglComponentList &GetDynamicComponentList(){ return pDynamicComponentList; }
	inline const deoglComponentList &GetDynamicComponentList() const{ return pDynamicComponentList; }
	
	/**
	 * Static collide list.
	 * \details Updates it if not updated already.
	 */
	const deoglCollideList *GetStaticCollideList();
	
	/**
	 * Retrieves the dynamic collide list.
	 * \details Updates it if not updated already.
	 */
	const deoglCollideList *GetDynamicCollideList();
	
	/** Mark collide lists dirty. */
	void SetDirtyCollideLists();
	
	
	
	/** Mark light volume dirty. */
	void SetLightVolumeDirty();
	
	/** 
	 * Update convex volume list and light volume.
	 * \note Can be called indirectly from main thread during synchronization.
	 */
	void UpdateLightVolume();
	
	/** Light volume. */
	inline deoglLightVolume *GetLightVolume() const{ return pLightVolume; }
	
	/** Light volume crop box or \em NULL if not set. */
	inline decShapeBox *GetLightVolumeCropBox() const{ return pLightVolumeCropBox; }
	
	/** Set light volume crop box or NULL if not set. */
	void SetLightVolumeCropBox( decShapeBox *box );
	
	/**
	 * Set if the light is inside the camera.
	 * \details This affects the occlusion test.
	 */
	void SetInsideCamera( bool inside );
	
	
	
	/** Shadow caster. Update first if dirty. */
	deoglShadowCaster *GetShadowCaster();
	
	/** Set static and dynamic shadows dirty. */
	void SetDirtyShadows();
	
	/** Set shadow parameters. */
	void SetShadowParameters( const decVector &shadowOrigin, float shadowGap );
	
	/** Shadow caster requires neeed future preparing. */
	void ShadowCasterRequiresPrepare();
	
	
	
	
	/** Range. */
	inline float GetRange() const{ return pRange; }
	
	/** Attenuation coefficient. */
	inline float GetAttenuationCoefficient() const{ return pAttenCoeff; }
	
	/** Damping coefficient. */
	inline float GetDampingCoefficient() const{ return pDampCoeff; }
	
	/** Damping threshold. */
	inline float GetDampingThreshold() const{ return pDampThreshold; }
	
	/** Update attenuation parameters. */
	void UpdateAttenuation( float range, float halfIntensityDistance );
	
	
	
	/** Clear registered optimizer. */
	void ClearOptimizer();
	
	
	
	/** Prepare for rendering. */
	void PrepareForRender( deoglRenderPlan &plan );
	
	
	
	/** Shader for a shader type. */
	deoglLightShader *GetShaderFor( eShaderTypes shaderType );
	
	/** Shader configuration for a shader type. */
	bool GetShaderConfigFor( eShaderTypes shaderType, deoglLightShaderConfig &config );
	
	/** Light parameter block. */
	deoglSPBlockUBO *GetLightParameterBlock();
	
	/** Instance parameter block. */
	deoglSPBlockUBO *GetInstanceParameterBlock();
	
	/** Drop all shaders and parameter blocks. */
	void DropShaders();
	
	
	
	/** Set touching dirty. */
	void SetDirtyTouching();
	
	/** Notifiy env maps in the world about light changes. */
	void EnvMapNotifyLightChanged();
	
	
	
	/** Prepare for quick disposal of light. */
	void PrepareQuickDispose();
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/** Add a component. */
	void AddComponent( deoglRComponent *component );
	
	/** Remove a component. */
	void RemoveComponent( deoglRComponent *component );
	
	/** Remove all components. */
	void RemoveAllComponents();
	
	/**
	 * Test component against this light.
	 * \details If the component is in contact with the bounding box it is added to the
	 *          list of touching component if not already listed. If not touching the
	 *          component is removed if listed.
	 */
	void TestComponent( deoglRComponent *oglComponent );
	/*@}*/
	
	
	
	/** \name Culling */
	/*@{*/
	/** Occlusion query. */
	deoglOcclusionQuery &GetOcclusionQuery();
	
	/**
	 * Light is hidden according to the last time the occlusion query has been run.
	 * \details Deals with various special cases preventing the caller to know or care about them.
	 */
	bool IsHiddenByOccQuery();
	/*@}*/
	
	
	
	/** \name Optimizations */
	/*@{*/
	/** Light volume has been improved. */
	void LightVolumeImproved();
	
	/** Replace light volume with an improved version. */
	void ReplaceLightVolume( decConvexVolumeList *list );
	
	/** Replace shadow caster with an improved version. */
	void ReplaceShadowCaster( deoglShadowCaster *shadowCaster );
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove( bool marked );
	
	
	
	inline deoglRLight *GetLLWorldPrev() const{ return pLLWorldPrev; }
	void SetLLWorldPrev( deoglRLight *light );
	inline deoglRLight *GetLLWorldNext() const{ return pLLWorldNext; }
	void SetLLWorldNext( deoglRLight *light );
	
	inline decPointerLinkedList::cListEntry &GetLLPrepareForRenderWorld(){ return pLLPrepareForRenderWorld; }
	inline const decPointerLinkedList::cListEntry &GetLLPrepareForRenderWorld() const{ return pLLPrepareForRenderWorld; }
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateFullExtends();
	void pUpdateExtends();
	void pUpdateCollideLists();
	void pCheckTouching();
	
	void pRequiresPrepareForRender();
};

#endif
