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
#include "../occlusiontest/deoglOcclusionTestListener.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectSet.h>
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

class decConvexVolumeList;
class deoglDCollisionVolume;
class decShapeBox;



/**
 * \brief Render light.
 */
class deoglRLight : public deObject, deoglOcclusionTestListener{
public:
	/** \brief Shader Types. */
	enum eShaderTypes{
		/** \brief No shadow casting. */
		estNoShadow,
		
		/** \brief Single solid no transparent shadow. */
		estSolid1,
		
		/** \brief Single solid and transparent shadow. */
		estSolid1Transp1,
		
		/** \brief Single solid no transparent shadow without ambient lighting. */
		estSolid1NoAmbient,
		
		/** \brief Single solid and transparent shadow without ambient lighting. */
		estSolid1Transp1NoAmbient,
		
		/** \brief Double solid no transparent shadow. */
		estSolid2,
		
		/** \brief Double solid and single transparent shadow. */
		estSolid2Transp1,
		
		/** \brief Double solid and transparent shadow. */
		estSolid2Transp2,
		
		/** \brief Luminance only single solid. */
		estLumSolid1,
		
		/** \brief Luminance only single solid without ambient lighting. */
		estLumSolid1NoAmbient,
		
		/** \brief Luminance only double solid. */
		estLumSolid2,
		
		/** \brief Number of shaders. */
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
	bool pDirtyRenderables;
	
	deoglSkinState *pSkinState;
	deoglSkinTexture *pUseSkinTexture;
	
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
	
	bool pVisible;
	bool pDirtyTouching;
	bool pInsideCamera;
	
	bool pMarked;
	
	bool pUpdateOnRemoveComponent;
	
	deoglLightShader *pShaders[ EST_COUNT ];
	deoglSPBlockUBO *pParamBlockLight;
	deoglSPBlockUBO *pParamBlockInstance;
	
	bool pWorldMarkedRemove;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render light. */
	deoglRLight( deoglRenderThread &renderThread );
	
	/** \brief Clean up render light. */
	virtual ~deoglRLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** \brief Parent world or \em NULL if not in a world. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or \em NULL if not in a world. */
	void SetParentWorld( deoglRWorld *parentWorld );
	
	/** \brief Octree node or \em NULL if not inserted into the parent world octree. */
	inline deoglWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/**
	 * \brief Set octree node or \em NULL if not inserted into the parent world octree.
	 * \details This call is to be used only by the deoglWorldOctree only.
	 */
	void SetOctreeNode( deoglWorldOctree *octreeNode );
	
	/** \brief Update octree node. */
	void UpdateOctreeNode();
	
	/** \brief Update light skin if used. */
	void UpdateSkin( float elapsed );
	
	
	
	/** \brief Light is active. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set light is active. */
	void SetActive( bool active );
	
	/** \brief Light type. */
	inline deLight::eLightTypes GetLightType() const{ return pLightType; }
	
	/** \brief Set light type. */
	void SetLightType( deLight::eLightTypes type );
	
	/** \brief Light casts shadows. */
	inline bool GetCastShadows() const{ return pCastShadows; }
	
	/** \brief Set if light casts shadows. */
	void SetCastShadows( bool castShadows );
	
	/** \brief Outer spot Angle. */
	inline float GetSpotAngle() const{ return pSpotAngle; }
	
	/** \brief Set outer spot angle. */
	void SetSpotAngle( float angle );
	
	/** \brief Spot ratio as height divided by width. */
	inline float GetSpotRatio() const{ return pSpotRatio; }
	
	/** \brief Set spot ratio as height divided by width. */
	void SetSpotRatio( float ratio );
	
	/** \brief Spot smoothness. */
	inline float GetSpotSmoothness() const{ return pSpotSmoothness; }
	
	/** \brief Set spot smoothness. */
	void SetSpotSmoothness( float smoothness );
	
	/** \brief Spot exponent. */
	inline float GetSpotExponent() const{ return pSpotExponent; }
	
	/** \brief Set spot exponent. */
	void SetSpotExponent( float exponent );
	
	/** \brief Movement hint. */
	inline deLight::eMovementHints GetHintMovement() const{ return pHintMovement; }
	
	/** \brief Set movement hint. */
	void SetHintMovement( deLight::eMovementHints movement );
	
	/** \brief Shadow importance. */
	inline int GetHintShadowImportance() const{ return pHintShadowImportance; }
	
	/** \brief Set shadow importance. */
	void SetHintShadowImportance( int importance );
	
	/** \brief Intensity. */
	inline float GetIntensity() const{ return pIntensity; }
	
	/** \brief Set intensity. */
	void SetIntensity( float intensity );
	
	/** \brief Ambient intensity ratio. */
	inline float GetAmbientRatio() const{ return pAmbientRatio; }
	
	/** \brief Set ambient intensity ratio. */
	void SetAmbientRatio( float ratio );
	
	/** \brief Light canvas or \em NULL if not used. */
	inline deoglRCanvasView *GetLightCanvas() const{ return pLightCanvas; }
	
	/** \brief Set light canvas or \em NULL if not used. */
	void SetLightCanvas( deoglRCanvasView *canvas );
	
	/** \brief Light color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set light color. */
	void SetColor( const decColor &color );
	
	/** \brief Camera layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set camera layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** \brief Shadow layer mask. */
	inline const decLayerMask &GetLayerMaskShadow() const{ return pLayerMaskShadow; }
	
	/** \brief Set shadow layer mask. */
	void SetLayerMaskShadow( const decLayerMask &layerMask );
	
	/** \brief Remove all shadow ignore components. */
	void RemoveAllShadowIgnoreComponents();
	
	/** \brief Add shadow ignore component. */
	void AddShadowIgnoreComponent( deoglRComponent *component );
	
	/** \brief Has shadow ignore component. */
	bool HasShadowIgnoreComponent( deoglRComponent *component ) const;
	
	
	
	/** \brief Light skin or \em NULL if not used. */
	inline deoglRSkin *GetLightSkin() const{ return pLightSkin; }
	
	/** \brief Set light skin or \em NULL if not used. */
	void SetLightSkin( deoglRSkin *skin );
	
	/** \brief Dynamic skin or \em NULL if not used. */
	inline deoglRDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Set dynamic skin or \em NULL if not used. */
	void SetDynamicSkin( deoglRDynamicSkin *dynamicSkin );
	
	/** \brief Light texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** \brief Set light texture coordinate transformation matrix. */
	void SetTransform( const decTexMatrix2 &matrix );
	
	/** \brief Skin state or \em NULL if not used. */
	inline deoglSkinState *GetSkinState() const{ return pSkinState; }
	
	/** \brief Skin texture to use or \em NULL. */
	inline deoglSkinTexture *GetUseSkinTexture() const{ return pUseSkinTexture; }
	
	/** \brief Mark renderables dirty. */
	void SetRenderablesDirty();
	
	
	
	/** \brief Transformation matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Inverse transformation matrix. */
	inline const decDMatrix &GetInverseMatrix() const{ return pInverseMatrix; }
	
	/** \brief Set matrices. */
	void SetMatrix( const decDMatrix &matrix );
	
	
	
	/** \brief Full minimum extend. */
	const decDVector &GetFullMinExtend();
	
	/** \brief Full maximum extend. */
	const decDVector &GetFullMaxExtend();
	
	/** \brief Mark full extends dirty. */
	void SetDirtyFullExtends();
	
	/** \brief Minimum extend. */
	const decDVector &GetMinimumExtend();
	
	/** \brief Maximum extend. */
	const decDVector &GetMaximumExtend();
	
	/** \brief Mark extends dirty. */
	void SetDirtyExtends();
	
	
	
	/** \brief Marked. */
	inline bool GetMarked() const{ return pMarked; }
	
	/** \brief Set marked. */
	inline void SetMarked( bool marked ){ pMarked = marked; }
	
	
	
	/** \brief Convex light volume. */
	inline decConvexVolumeList *GetConvexVolumeList() const{ return pConvexVolumeList; }
	
	/** \brief Update convex volume. */
	void UpdateConvexVolumeList();
	
	/** \brief Has extends. */
	bool HasExtends() const;
	
	/** \brief Collision volume. */
	deoglDCollisionVolume *GetCollisionVolume();
	
	/** \brief Mark collision volume dirty. */
	void SetDirtyCollisionVolume();
	
	
	
	/** \brief Camera is inside light volume. */
	inline bool GetCameraInside() const{ return pCameraInside; }
	
	/** \brief Test if camera is inside light volume. */
	void TestCameraInside( deoglRenderPlan &plan );
	
	
	
	/** \brief Static component list. */
	inline deoglComponentList &GetStaticComponentList(){ return pStaticComponentList; }
	inline const deoglComponentList &GetStaticComponentList() const{ return pStaticComponentList; }
	
	/** \brief Dynamic component list. */
	inline deoglComponentList &GetDynamicComponentList(){ return pDynamicComponentList; }
	inline const deoglComponentList &GetDynamicComponentList() const{ return pDynamicComponentList; }
	
	/**
	 * \brief Static collide list.
	 * \details Updates it if not updated already.
	 */
	const deoglCollideList *GetStaticCollideList();
	
	/**
	 * \brief Retrieves the dynamic collide list.
	 * \details Updates it if not updated already.
	 */
	const deoglCollideList *GetDynamicCollideList();
	
	/** \brief Mark collide lists dirty. */
	void SetDirtyCollideLists();
	
	
	
	/** \brief Mark light volume dirty. */
	void SetLightVolumeDirty();
	
	/** \brief Update convex volume list and light volume. */
	void UpdateLightVolume();
	
	/** \brief Light volume. */
	inline deoglLightVolume *GetLightVolume() const{ return pLightVolume; }
	
	/** \brief Light volume crop box or \em NULL if not set. */
	inline decShapeBox *GetLightVolumeCropBox() const{ return pLightVolumeCropBox; }
	
	/** \brief Set light volume crop box or NULL if not set. */
	void SetLightVolumeCropBox( decShapeBox *box );
	
	/**
	 * \brief Set if the light is inside the camera.
	 * \details This affects the occlusion test.
	 */
	void SetInsideCamera( bool inside );
	
	
	
	/** \brief Shadow caster. Update first if dirty. */
	deoglShadowCaster *GetShadowCaster();
	
	/** \brief Set static and dynamic shadows dirty. */
	void SetDirtyShadows();
	
	/** \brief Set shadow parameters. */
	void SetShadowParameters( const decVector &shadowOrigin, float shadowGap );
	
	
	
	/** \brief Range. */
	inline float GetRange() const{ return pRange; }
	
	/** \brief Attenuation coefficient. */
	inline float GetAttenuationCoefficient() const{ return pAttenCoeff; }
	
	/** \brief Damping coefficient. */
	inline float GetDampingCoefficient() const{ return pDampCoeff; }
	
	/** \brief Damping threshold. */
	inline float GetDampingThreshold() const{ return pDampThreshold; }
	
	/** \brief Update attenuation parameters. */
	void UpdateAttenuation( float range, float halfIntensityDistance );
	
	
	
	/** \brief Clear registered optimizer. */
	void ClearOptimizer();
	
	
	
	/** \brief Prepare for rendering. */
	void PrepareForRender( deoglRenderPlan &plan );
	
	/** \brief Prepare for shadow casting. */
	void PrepareForShadowCasting( deoglRenderPlan &plan );
	
	
	
	/** \brief Shader for a shader type. */
	deoglLightShader *GetShaderFor( eShaderTypes shaderType );
	
	/** \brief Shader configuration for a shader type. */
	bool GetShaderConfigFor( eShaderTypes shaderType, deoglLightShaderConfig &config );
	
	/** \brief Light parameter block. */
	deoglSPBlockUBO *GetLightParameterBlock();
	
	/** \brief Instance parameter block. */
	deoglSPBlockUBO *GetInstanceParameterBlock();
	
	/** \brief Drop all shaders and parameter blocks. */
	void DropShaders();
	
	
	
	/** \brief Light is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Set if light is visible. */
	void SetVisible( bool visible );
	
	
	
	/** \brief Set touching dirty. */
	void SetDirtyTouching();
	
	/** \brief Notifiy env maps in the world about light changes. */
	void EnvMapNotifyLightChanged();
	
	
	
	/** \brief Prepare for quick disposal of light. */
	void PrepareQuickDispose();
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/** \brief Add a component. */
	void AddComponent( deoglRComponent *component );
	
	/** \brief Remove a component. */
	void RemoveComponent( deoglRComponent *component );
	
	/** \brief Remove all components. */
	void RemoveAllComponents();
	
	/**
	 * \brief Test component against this light.
	 * \details If the component is in contact with the bounding box it is added to the
	 *          list of touching component if not already listed. If not touching the
	 *          component is removed if listed.
	 */
	void TestComponent( deoglRComponent *oglComponent );
	/*@}*/
	
	
	
	/** \name Culling */
	/*@{*/
	/** \brief Start occlusion test. */
	void StartOcclusionTest( const decDVector &cameraPosition );
	
	/** \brief The occlusion test finished with a result of invisible for the element. */
	virtual void OcclusionTestInvisible();
	
	/** \brief Occlusion query. */
	deoglOcclusionQuery &GetOcclusionQuery();
	
	/**
	 * \brief Light is hidden according to the last time the occlusion query has been run.
	 * \details Deals with various special cases preventing the caller to know or care about them.
	 */
	bool IsHiddenByOccQuery();
	/*@}*/
	
	
	
	/** \name Optimizations */
	/*@{*/
	/** \brief Light volume has been improved. */
	void LightVolumeImproved();
	
	/** \brief Replace light volume with an improved version. */
	void ReplaceLightVolume( decConvexVolumeList *list );
	
	/** \brief Replace shadow caster with an improved version. */
	void ReplaceShadowCaster( deoglShadowCaster *shadowCaster );
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * \brief Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * \brief Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove( bool marked );
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateFullExtends();
	void pUpdateExtends();
	void pUpdateCollideLists();
	void pCheckTouching();
	void pUpdateRenderables();
};

#endif
