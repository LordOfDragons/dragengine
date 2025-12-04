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

#ifndef _IGDEWOBJECT_H_
#define _IGDEWOBJECT_H_

#include "../../gamedefinition/class/igdeGDClass::Ref.h"
#include "../../triggersystem/igdeTriggerListener::Ref.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/utils/decCollisionFilter.h>
#include <dragengine/resources/camera/deCamera::Ref.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin::Ref.h>
#include <dragengine/resources/world/deWorld::Ref.h>

class igdeTriggerTargetList;
class igdeEnvironment;
class igdeWOSubObject;
class igdeWOSOVisitor;

class deComponent;
class deCollider;
class deColliderVolume;
class deColliderComponent;
class deBaseScriptingCollider;



/**
 * \brief Object Wrapper.
 * 
 * Provides a simple way to display a object defined by a game definition object class
 * in a world. The object can be fine tuned using string properties.
 * 
 * These are the default layer masks set for a newly constructed igdeWObject:
 * - Render Layer Mask: 1 (Bit 0 set)
 * - Render Environment Map Mask: 2 (Bit 1 set)
 * - Audio: 4 (Bit 2 set)
 */
class DE_DLL_EXPORT igdeWObject : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeWObject> Ref;
	
	/** \brief Asynchronous loading finished. */
	class DE_DLL_EXPORT cAsyncLoadFinished{
	public:
		/** \brief Create listener. */
		cAsyncLoadFinished();
		
		/** \brief Clean up listener. */
		virtual ~cAsyncLoadFinished();
		
		/** \brief Loading finished. */
		virtual void LoadFinished( igdeWObject &wrapper, bool succeeded ) = 0;
		
		/** \brief Any content visibility changed. */
		virtual void AnyContentVisibleChanged(igdeWObject &wrapper);
		
		/** \brief Extends changed. */
		virtual void ExtendsChanged(igdeWObject &wrapper);
	};
	
	
	
private:
	igdeEnvironment &pEnvironment;
	
	deWorld::Ref pWorld;
	deCamera::Ref pCamera;
	igdeGDClass::Ref pGDClass;
	
	deColliderComponent::Ref pColliderComponent;
	deColliderVolume::Ref pColliderFallback;
	decObjectSet pCollidersInteraction;
	void *pColliderUserPointer;
	
	deCollider::Ref pParentCollider;
	decString pAttachToBone;
	
	decObjectOrderedSet pSubObjects;
	igdeTriggerListener::Ref pTriggerListener;
	
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pScaling;
	decDMatrix pMatrix, pInvMatrix;
	
	decStringDictionary pProperties;
	
	int pRenderLayerMask;
	int pRenderEnvMapMask;
	int pAudioLayerMask;
	
	decCollisionFilter pCollisionFilter;
	decCollisionFilter pCollisionFilterParticles;
	decCollisionFilter pCollisionFilterForceField;
	decCollisionFilter pCollisionFilterFallback;
	decCollisionFilter pCollisionFilterInteract;
	bool pDynamicCollider;
	
	bool pVisible;
	bool pPartiallyHidden;
	
	deBaseScriptingCollider *pListenerCollider;
	
	igdeTriggerTargetList *pTriggerTable;
	decObjectList pListTriggerTarget;
	
	decVector pBoxMinExtend;
	decVector pBoxMaxExtend;
	bool pHasBoxExtends;
	bool pDirtyExtends;
	bool pDirtyFallbackColliderShape;
	
	deSkin::Ref pOutlineSkin;
	deDynamicSkin::Ref pOutlineDynamicSkin;
	decColor pOutlineColor;
	
	cAsyncLoadFinished *pAsyncLoadFinished;
	int pAsyncLoadCounter;
	
	bool pAnyContentVisible;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create wrapper. */
	igdeWObject( igdeEnvironment &environment );
	
protected:
	/** \brief Clean up wrapper. */
	virtual ~igdeWObject();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief World or NULL. */
	inline deWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world or NULL. */
	void SetWorld( deWorld *world );
	
	/** \brief Camera to calculate screen related properties or NULL. */
	inline deCamera *GetCamera() const{ return pCamera; }
	
	/** \brief Set camera to calculate screen related properties or NULL. */
	void SetCamera( deCamera *camera );
	
	/** \brief Trigger table or NULL if not used. */
	inline igdeTriggerTargetList *GetTriggerTable() const{ return pTriggerTable; }
	
	/** \brief Set trigger table or NULL if not used. */
	void SetTriggerTable( igdeTriggerTargetList *triggerTable );
	
	/** \brief Game definition object class or NULL. */
	inline igdeGDClass *GetGDClass() const{ return pGDClass; }
	
	/** \brief Set game definition object class or NULL. */
	void SetGDClass( igdeGDClass *gdClass );
	
	/** \brief Set game definition object class by name if found. */
	void SetGDClassName( const char *gdClassName );
	
	/** \brief Position. */
	inline decDVector GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline decQuaternion GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Scaling. */
	inline decVector GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling( const decVector &scaling );
	
	/** \brief Matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Inverse matrix. */
	inline const decDMatrix &GetInverseMatrix() const{ return pInvMatrix; }
	
	/** \brief Determines if the object is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Sets if the object is visible. */
	void SetVisible( bool visible );
	
	/** \brief Determines if the object is partially hidden. */
	inline bool GetPartiallyHidden() const{ return pPartiallyHidden; }
	
	/** \brief Sets if the object is partially hidden. */
	void SetPartiallyHidden( bool partiallyHidden );
	
	/** \brief Render layer mask. */
	inline int GetRenderLayerMask() const{ return pRenderLayerMask; }
	
	/** \brief Set render layer mask. */
	void SetRenderLayerMask( int mask );
	
	/** \brief Render environment map group mask. */
	inline int GetRenderEnvMapMask() const{ return pRenderEnvMapMask; }
	
	/** \brief Set render environment map group mask. */
	void SetRenderEnvMapMask( int mask );
	
	/** \brief Audio layer mask. */
	inline int GetAudioLayerMask() const{ return pAudioLayerMask; }
	
	/** \brief Set audio layer mask. */
	void SetAudioLayerMask( int mask );
	
	/** \brief Box minimum extends. */
	const decVector &GetBoxMinExtend();
	
	/** \brief Box maximum extends. */
	const decVector &GetBoxMaxExtend();
	
	/** \brief Has box extends. */
	bool GetHasBoxExtends();
	
	
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter( const decCollisionFilter &collisionFilter );
	
	/** \brief Collision filter particles. */
	inline const decCollisionFilter &GetCollisionFilterParticles() const{ return pCollisionFilterParticles; }
	
	/** \brief Set collision filter particles. */
	void SetCollisionFilterParticles( const decCollisionFilter &collisionFilter );
	
	/** \brief Collision filter force fields. */
	inline const decCollisionFilter &GetCollisionFilterForceFields() const{ return pCollisionFilterForceField; }
	
	/** \brief Set collision filter force fields. */
	void SetCollisionFilterForceFields( const decCollisionFilter &collisionFilter );
	
	/** \brief Collision filter fallback. */
	inline const decCollisionFilter &GetCollisionFilterFallback() const{ return pCollisionFilterFallback; }
	
	/** \brief Set collision filter fallback. */
	void SetCollisionFilterFallback( const decCollisionFilter &collisionFilter );
	
	/** \brief Collision filter for interaction. */
	inline const decCollisionFilter &GetCollisionFilterInteract() const{ return pCollisionFilterInteract; }
	
	/** \brief Set collision filter for interaction. */
	void SetCollisionFilterInteract( const decCollisionFilter &collisionFilter );
	
	
	
	/** \brief Determines if the collider is allowed to be dynamic or always kinematic. */
	inline bool GetDynamicCollider() const{ return pDynamicCollider; }
	
	/** \brief Sets if the collider is allowed to be dynamic or always kinematic. */
	void SetDynamicCollider( bool dynamic );
	
	/** \brief Parent collider or NULL if there is none. */
	inline deCollider *GetParentCollider() const{ return pParentCollider; }
	
	/** \brief Determines if the object is attached to a parent collider. */
	bool IsAttachedCollider() const;
	
	/**
	 * \brief Attach object to a parent collider component using rig attachment mode.
	 * 
	 * If attached already the object is first detached from the previous parent collider
	 * before being attached to the new collider.
	 */
	void AttachColliderRig( deColliderComponent *parentCollider );
	
	/**
	 * \brief Attach object to a parent collider component using bone attachment mode.
	 * 
	 * If attached already the object is first detached from the previous parent collider
	 * before being attached to the new collider.
	 */
	void AttachColliderBone( deColliderComponent *parentCollider, const char *bone,
		const decVector &position, const decQuaternion &orientation );
	
	/** \brief Detach object from parent collider if attached. */
	void DetachCollider();
	
	/**
	 * \brief Collider in use.
	 * 
	 * If a component exists this is GetColliderComponent() otherwise GetColliderFallback().
	 * This can change any time if parameters of the wrapper are changed. For this reason
	 * retrieve the collider each time you need it without caching it.
	 */
	deCollider *GetCollider() const;
	
	/**
	 * \brief Component in use.
	 */
	deComponent *GetComponent() const;
	
	/**
	 * \brief Object has any content which is visible.
	 * 
	 * Returns true if any of the wrapped resources is visible. For example a component resource
	 * without valid model or with no texture containing a valid skin is invisible. If this
	 * function returns true the caller should show a placeholder for this object since otherwise
	 * the user can not see where it is. This check does not include the visibility state of the
	 * object. Hence invisible content is still considered visible if it would be visible otherwise.
	 */
	inline bool IsAnyContentVisible() const{ return pAnyContentVisible; }
	
	/** \brief Collider component. */
	inline const deColliderComponent::Ref &GetColliderComponent() const{ return pColliderComponent; }
	
	/** \brief Fallback collider volume. */
	inline const deColliderVolume::Ref &GetColliderFallback() const{ return pColliderFallback; }
	
	/** \brief Collider user pointer. */
	inline void *GetColliderUserPointer() const{ return pColliderUserPointer; }
	
	/** \brief Set collider user pointer for all colliders used. */
	void SetColliderUserPointer( void *userPointer );
	
	/** \brief Notifies the object that a collider changed. Causes an update the next time update is called. */
	void OnColliderChanged();
	
	/** \brief Updates to play animations and do other per-frame updates. */
	void Update( float elapsed );
	
	/** \brief Reset component textures. */
	void ResetComponentTextures();
	
	/** \brief Reset physics states to the animated position clearing all movement. */
	void ResetPhysics();
	
	/** \brief Visit sub objects. */
	void VisitSubObjects( igdeWOSOVisitor &visitor );
	
	
	
	/** \brief Outline skin or NULL. */
	inline deSkin *GetOutlineSkin() const{ return pOutlineSkin; }
	
	/** \brief Set outline skin or NULL. */
	void SetOutlineSkin( deSkin *skin );
	
	/** \brief Set outline skin to shared editing outline skin. */
	void SetOutlineSkinSharedEditing();
	
	/** \brief Outline color. */
	inline const decColor &GetOutlineColor() const{ return pOutlineColor; }
	
	/** \brief Set outline color. */
	void SetOutlineColor( const decColor &color );
	
	
	
	/** \brief Asynchronous load finished listener or NULL. */
	inline cAsyncLoadFinished *GetAsyncLoadFinished() const{ return pAsyncLoadFinished; }
	
	/**
	 * \brief Set asynchronous load finished listener or NULL.
	 * 
	 * Caller is responsible to keep the listener alive. Wrapper does not hold reference.
	 */
	void SetAsyncLoadFinished( cAsyncLoadFinished *listener );
	
	/** \brief Trigger listener for internal use only. */
	inline igdeTriggerListener *GetTriggerListener() const{ return pTriggerListener; }
	
	/**
	 * \brief Determine if all sub objects (direct or indirect) finished loading.
	 * 
	 * Required if world type sub objects are present. These report finished loading
	 * as soon as the world has been loaded to avoid waiting. Use this function to
	 * check if all sub objects inside this object fully finished loading.
	 */
	bool AllSubObjectsFinishedLoading() const;
	
	
	/**
	 * \brief Project game definition changed.
	 * 
	 * Call this method if you received an OnGameDefinitionChanged() on the parent module.
	 */
	void OnGameDefinitionChanged();
	/*@}*/
	
	
	
	/** \name Properties */
	/*@{*/
	/** \brief Property list. */
	inline const decStringDictionary &GetProperties() const{ return pProperties; }
	
	/** \brief Sets a property. */
	void SetProperty( const char *key, const char *value );
	
	/** \brief Set property list from another property list. */
	void SetPropertiesFrom( const decStringDictionary &list );
	
	/** \brief Removes a property if existing. */
	void RemoveProperty( const char *key );
	
	/** \brief Removes all properties. */
	void RemoveAllProperties();
	
	/** \brief Update trigger targets. */
	void UpdateTriggerTargets();
	
	/** \brief Update triggerable properties. For internal use only. */
	void UpdateTriggerableProperties();
	/*@}*/
	
	
	
	/** \name Internal use only */
	/*@{*/
	void SubObjectFinishedLoading( igdeWOSubObject &subObject, bool success );
	void SubObjectExtendsDirty();
	void SetInteractCollider( deColliderComponent *collider );
	inline deDynamicSkin *GetOutlineDynamicSkin() const{ return pOutlineDynamicSkin; }
	void AddInteractionCollider( deCollider *collider );
	void RemoveInteractionCollider( deCollider *collider );
	void UpdateAnyContentVisibile();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pUpdateVisiblity();
	
	void pCreateSubObjects();
	void pCreateSubObjects( const decString &prefix, const igdeGDClass &gdclas, int filter );
	void pDestroySubObjects();
	void pSubObjectsReattachToColliders();
	void pSubObjectsInitTriggers();
	void pSubObjectsUpdateGeometry();
	void pSubObjectsUpdateTriggers();
	void pSubObjectsUpdateVisibility();
	void pSubObjectsUpdateLayerMasks();
	void pSubObjectsUpdateCollisionFilter();
	void pSubObjectsAllFinishedLoading();
	void pSubObjectsColliderUserPointerChanged();
	void pCheckAsyncLoadFinished();
	
	void pUpdateProperties();
	void pUpdateColliderResponseType();
	void pUpdateColliderShapes();
	
	void pPrepareExtends();
	void pUpdateMatrices();
};

#endif
