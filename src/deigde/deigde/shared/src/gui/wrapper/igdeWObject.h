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

#include "../../gamedefinition/class/igdeGDClass.h"
#include "../../meta/igdeMetaContext.h"
#include "../../meta/property/igdeMetaPropertyBoolean.h"
#include "../../meta/property/igdeMetaPropertyContext.h"
#include "../../meta/property/igdeMetaPropertyString.h"
#include "../../meta/property/igdeMetaPropertyPath.h"
#include "../../meta/property/igdeMetaPropertyDVector.h"
#include "../../meta/property/igdeMetaPropertyVector.h"
#include "../../triggersystem/igdeTriggerTarget.h"
#include "../../triggersystem/igdeTriggerListener.h"

#include <dragengine/common/collection/decTSet.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/utils/decCollisionFilter.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deCollider.h>

class igdeTriggerTargetList;
class igdeEnvironment;
class igdeWOSubObject;
class igdeWOSOVisitor;

class deComponent;
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
	using Ref = deTObjectReference<igdeWObject>;
	
	
	/** \brief Asynchronous loading finished. */
	class DE_DLL_EXPORT cAsyncLoadFinished{
	public:
		/** \brief Create listener. */
		cAsyncLoadFinished();
		
		/** \brief Clean up listener. */
		virtual ~cAsyncLoadFinished();
		
		/** \brief Loading finished. */
		virtual void LoadFinished(igdeWObject &wrapper, bool succeeded) = 0;
		
		/** \brief Any content visibility changed. */
		virtual void AnyContentVisibleChanged(igdeWObject &wrapper);
		
		/** \brief Extends changed. */
		virtual void ExtendsChanged(igdeWObject &wrapper);
	};
	
	
	/** \brief Meta context. */
	class DE_DLL_EXPORT MetaContext : public igdeMetaContext{
	private:
		igdeWObject *pWrapper;
		igdeWObject::Ref pGuard;
		
	public:
		using Ref = deTObjectReference<MetaContext>;
		MetaContext(igdeWObject *wrapper);
		
		inline igdeWObject *GetWrapper() const{ return pWrapper; }
		igdeWObject &GetWrapperRef() const;
		Ref Capture() const;
		igdeEnvironment &GetEnvironment() const override;
		igdeUndoSystem *GetUndoSystem() const override;
		igdeClipboard *GetClipboard() const override;
		
	protected:
		virtual ~MetaContext() override;
	};
	
	/** \brief Meta properties */
	class DE_DLL_EXPORT MetaProperties{
	public:
		template <typename T> class TBase : public T{
		public:
			template <typename... A> TBase(A&&... args) : T(std::forward<A>(args)...) {}
			
			igdeMetaContext::Ref Capture(const igdeMetaContext::Ref &context) const override{
				return context.DynamicCast<MetaContext>()->Capture();
			}
			
			bool IsValid(const igdeMetaContext::Ref &context) const override{
				const auto c = context.DynamicCast<MetaContext>();
				return c && !c->IsDisposed();
			}
			
			inline igdeWObject &Wrapper(const igdeMetaContext::Ref &context) const{
				return context.DynamicCast<MetaContext>()->GetWrapperRef();
			}
			
		protected:
			virtual ~TBase() override{}
		};
		
		class DE_DLL_EXPORT ObjectClass : public TBase<igdeMetaPropertyStringStorage>{
		public:
			ObjectClass();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			decStringSet GetPropertyAllowedStrings(const ContextRef &context) const override;
			void AddContextMenuEntries(igdeMenuCascade &contextMenu, const ContextRef &context, igdeWidget &owner) override;
			
		protected:
			~ObjectClass() override;
		};
		
		class DE_DLL_EXPORT World : public TBase<igdeMetaPropertyPathStorage>{
		public:
			World();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~World() override;
		};
		
		class DE_DLL_EXPORT Position : public TBase<igdeMetaPropertyDVectorStorage>{
		public:
			Position();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~Position() override;
		};
		
		class DE_DLL_EXPORT Rotation : public TBase<igdeMetaPropertyVectorStorageQuaternion>{
		public:
			Rotation();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~Rotation() override;
		};
		
		class DE_DLL_EXPORT Scaling : public TBase<igdeMetaPropertyVectorStorage>{
		public:
			Scaling();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			igdeMetaPropertyVectorUndo::Ref ChangePropertyValue(const ContextRef &context,
				const decVector &newValue, const char *undoInfo = nullptr,
				const char *undoInfoLong = nullptr) override;
					
		protected:
			~Scaling() override;
		};
		
		class DE_DLL_EXPORT Visible : public TBase<igdeMetaPropertyBooleanStorage>{
		public:
			Visible();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~Visible() override;
		};
		
		class DE_DLL_EXPORT DynamicCollider : public TBase<igdeMetaPropertyBooleanStorage>{
		public:
			DynamicCollider();
			Storage &GetStorage(const igdeMetaContext::Ref &context) const override;
			
		protected:
			~DynamicCollider() override;
		};
		
	private:
		MetaProperties();
		
	public:
		deTObjectReference<ObjectClass> objectClass;
		deTObjectReference<World> world;
		deTObjectReference<Position> position;
		deTObjectReference<Rotation> rotation;
		deTObjectReference<Scaling> scaling;
		deTObjectReference<Visible> visible;
		deTObjectReference<DynamicCollider> dynamicCollider;
		
		const igdeMetaContext::PropertyList::Ref properties;
		
		static MetaProperties global;
	};
	
	
private:
	igdeEnvironment &pEnvironment;
	
	MetaContext::Ref pMetaContext;
	
	deWorld::Ref pWorld;
	deCamera::Ref pCamera;
	igdeGDClass::Ref pGDClass, pWorldGDClass;
	
	deColliderComponent::Ref pColliderComponent;
	deColliderVolume::Ref pColliderFallback;
	decTObjectSet<deCollider> pCollidersInteraction;
	void *pColliderUserPointer;
	
	deCollider::Ref pParentCollider;
	decString pAttachToBone;
	
	decTObjectOrderedSet<igdeWOSubObject> pSubObjects;
	igdeTriggerListener::Ref pTriggerListener;
	
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
	bool pRequiresInteraction;
	
	bool pPartiallyHidden;
	
	deBaseScriptingCollider *pListenerCollider;
	
	igdeTriggerTargetList *pTriggerTable;
	decTObjectList<igdeTriggerTarget> pListTriggerTarget;
	
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
	/** \brief Meta property object class. */
	igdeMetaPropertyStringStorage::Storage objectClass;
	
	/** \brief Meta property world. */
	igdeMetaPropertyPathStorage::Storage world;
	
	/** \brief Meta property position. */
	igdeMetaPropertyDVectorStorage::Storage position;
	
	/** \brief Meta property rotation. */
	igdeMetaPropertyVectorStorageQuaternion::Storage rotation;
	
	/** \brief Meta property scaling. */
	igdeMetaPropertyVectorStorage::Storage scaling;
	
	/** \brief Meta property visible. */
	igdeMetaPropertyBooleanStorage::Storage visible;
	
	/** \brief Meta property dynamic collider. */
	igdeMetaPropertyBooleanStorage::Storage dynamicCollider;
	
	/** \brief Object changed event. */
	igdeTEvent<> onChanged;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create wrapper. */
	igdeWObject(igdeEnvironment &environment);
	
	igdeWObject(const igdeWObject&) = delete;
	igdeWObject(igdeWObject&&) = delete;
	igdeWObject& operator=(const igdeWObject&) = delete;
	igdeWObject& operator=(igdeWObject&&) = delete;

protected:
	/** \brief Clean up wrapper. */
	~igdeWObject() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Meta context. */
	inline const MetaContext::Ref &GetMetaContext() const{ return pMetaContext; }
	
	/** \brief World or nullptr. */
	inline const deWorld::Ref &GetWorld() const{ return pWorld; }
	
	/** \brief Set world or nullptr. */
	void SetWorld(deWorld *world);
	
	/** \brief Camera to calculate screen related properties or nullptr. */
	inline const deCamera::Ref &GetCamera() const{ return pCamera; }
	
	/** \brief Set camera to calculate screen related properties or nullptr. */
	void SetCamera(deCamera *camera);
	
	/** \brief Trigger table or nullptr if not used. */
	inline igdeTriggerTargetList *GetTriggerTable() const{ return pTriggerTable; }
	
	/** \brief Set trigger table or nullptr if not used. */
	void SetTriggerTable(igdeTriggerTargetList *triggerTable);
	
	/** \brief Game definition object class or nullptr. */
	inline const igdeGDClass::Ref &GetGDClass() const{ return pGDClass; }
	
	/** \brief Set game definition object class or nullptr. */
	void SetGDClass(igdeGDClass *gdClass);
	
	/** \brief Set game definition object class by name if found. */
	void SetGDClassName(const char *gdClassName);
	
	/** \brief Path to world. */
	inline const decString &GetPathWorld() const{ return world; }
	
	/** \brief Set path to world. */
	void SetPathWorld(const char *path);
	
	/** \brief Position. */
	inline decDVector GetPosition() const{ return position; }
	
	/** \brief Set position. */
	void SetPosition(const decDVector &position);
	
	/** \brief Orientation. */
	inline decQuaternion GetOrientation() const{ return rotation; }
	
	/** \brief Set orientation. */
	void SetOrientation(const decQuaternion &orientation);
	
	/** \brief Scaling. */
	inline decVector GetScaling() const{ return scaling; }
	
	/** \brief Set scaling. */
	void SetScaling(const decVector &scaling);
	
	/** \brief Matrix. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Inverse matrix. */
	inline const decDMatrix &GetInverseMatrix() const{ return pInvMatrix; }
	
	/** \brief Determines if the object is visible. */
	inline bool GetVisible() const{ return visible; }
	
	/** \brief Sets if the object is visible. */
	void SetVisible(bool visible);
	
	/** \brief Determines if the object is partially hidden. */
	inline bool GetPartiallyHidden() const{ return pPartiallyHidden; }
	
	/** \brief Sets if the object is partially hidden. */
	void SetPartiallyHidden(bool partiallyHidden);
	
	/** \brief Render layer mask. */
	inline int GetRenderLayerMask() const{ return pRenderLayerMask; }
	
	/** \brief Set render layer mask. */
	void SetRenderLayerMask(int mask);
	
	/** \brief Render environment map group mask. */
	inline int GetRenderEnvMapMask() const{ return pRenderEnvMapMask; }
	
	/** \brief Set render environment map group mask. */
	void SetRenderEnvMapMask(int mask);
	
	/** \brief Audio layer mask. */
	inline int GetAudioLayerMask() const{ return pAudioLayerMask; }
	
	/** \brief Set audio layer mask. */
	void SetAudioLayerMask(int mask);
	
	/** \brief Box minimum extends. */
	const decVector &GetBoxMinExtend();
	
	/** \brief Box maximum extends. */
	const decVector &GetBoxMaxExtend();
	
	/** \brief Has box extends. */
	bool GetHasBoxExtends();
	
	
	
	/** \brief Collision filter. */
	inline const decCollisionFilter &GetCollisionFilter() const{ return pCollisionFilter; }
	
	/** \brief Set collision filter. */
	void SetCollisionFilter(const decCollisionFilter &collisionFilter);
	
	/** \brief Collision filter particles. */
	inline const decCollisionFilter &GetCollisionFilterParticles() const{ return pCollisionFilterParticles; }
	
	/** \brief Set collision filter particles. */
	void SetCollisionFilterParticles(const decCollisionFilter &collisionFilter);
	
	/** \brief Collision filter force fields. */
	inline const decCollisionFilter &GetCollisionFilterForceFields() const{ return pCollisionFilterForceField; }
	
	/** \brief Set collision filter force fields. */
	void SetCollisionFilterForceFields(const decCollisionFilter &collisionFilter);
	
	/** \brief Collision filter fallback. */
	inline const decCollisionFilter &GetCollisionFilterFallback() const{ return pCollisionFilterFallback; }
	
	/** \brief Set collision filter fallback. */
	void SetCollisionFilterFallback(const decCollisionFilter &collisionFilter);
	
	/** \brief Collision filter for interaction. */
	inline const decCollisionFilter &GetCollisionFilterInteract() const{ return pCollisionFilterInteract; }
	
	/** \brief Set collision filter for interaction. */
	void SetCollisionFilterInteract(const decCollisionFilter &collisionFilter);
	
	
	
	/** \brief Determines if the collider is allowed to be dynamic or always kinematic. */
	inline bool GetDynamicCollider() const{ return dynamicCollider; }
	
	/** \brief Sets if the collider is allowed to be dynamic or always kinematic. */
	void SetDynamicCollider(bool dynamic);
	
	/** \brief Determines if the object requires interaction. */
	inline bool GetRequiresInteraction() const{ return pRequiresInteraction; }
	
	/** \brief Sets if the object requires interaction. */
	void SetRequiresInteraction(bool requiresInteraction);
	
	/** \brief Parent collider or nullptr if there is none. */
	inline const deCollider::Ref &GetParentCollider() const{ return pParentCollider; }
	
	/** \brief Determines if the object is attached to a parent collider. */
	bool IsAttachedCollider() const;
	
	/**
	 * \brief Attach object to a parent collider component using rig attachment mode.
	 * 
	 * If attached already the object is first detached from the previous parent collider
	 * before being attached to the new collider.
	 */
	void AttachColliderRig(deColliderComponent *parentCollider);
	
	/**
	 * \brief Attach object to a parent collider component using bone attachment mode.
	 * 
	 * If attached already the object is first detached from the previous parent collider
	 * before being attached to the new collider.
	 */
	void AttachColliderBone(deColliderComponent *parentCollider, const char *bone,
		const decVector &position, const decQuaternion &orientation);
	
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
	void SetColliderUserPointer(void *userPointer);
	
	/** \brief Notifies the object that a collider changed. Causes an update the next time update is called. */
	void OnColliderChanged();
	
	/** \brief Updates to play animations and do other per-frame updates. */
	void Update(float elapsed);
	
	/** \brief Reset component textures. */
	void ResetComponentTextures();
	
	/** \brief Reset physics states to the animated position clearing all movement. */
	void ResetPhysics();
	
	/** \brief Visit sub objects. */
	void VisitSubObjects(igdeWOSOVisitor &visitor);
	
	
	
	/** \brief Outline skin or nullptr. */
	inline const deSkin::Ref &GetOutlineSkin() const{ return pOutlineSkin; }
	
	/** \brief Set outline skin or nullptr. */
	void SetOutlineSkin(deSkin *skin);
	
	/** \brief Set outline skin to shared editing outline skin. */
	void SetOutlineSkinSharedEditing();
	
	/** \brief Outline color. */
	inline const decColor &GetOutlineColor() const{ return pOutlineColor; }
	
	/** \brief Set outline color. */
	void SetOutlineColor(const decColor &color);
	
	
	
	/** \brief Asynchronous load finished listener or nullptr. */
	inline cAsyncLoadFinished *GetAsyncLoadFinished() const{ return pAsyncLoadFinished; }
	
	/**
	 * \brief Set asynchronous load finished listener or nullptr.
	 * 
	 * Caller is responsible to keep the listener alive. Wrapper does not hold reference.
	 */
	void SetAsyncLoadFinished(cAsyncLoadFinished *listener);
	
	/** \brief Trigger listener for internal use only. */
	inline const igdeTriggerListener::Ref &GetTriggerListener() const{ return pTriggerListener; }
	
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
	void SetProperty(const char *key, const char *value);
	
	/** \brief Set property list from another property list. */
	void SetPropertiesFrom(const decStringDictionary &list);
	
	/** \brief Removes a property if existing. */
	void RemoveProperty(const char *key);
	
	/** \brief Removes all properties. */
	void RemoveAllProperties();
	
	/** \brief Update trigger targets. */
	void UpdateTriggerTargets();
	
	/** \brief Update triggerable properties. For internal use only. */
	void UpdateTriggerableProperties();
	/*@}*/
	
	
	
	/** \name Internal use only */
	/*@{*/
	void SubObjectFinishedLoading(igdeWOSubObject &subObject, bool success);
	void SubObjectExtendsDirty();
	void SetInteractCollider(deColliderComponent *collider);
	inline const deDynamicSkin::Ref &GetOutlineDynamicSkin() const{ return pOutlineDynamicSkin; }
	void AddInteractionCollider(deCollider *collider);
	void RemoveInteractionCollider(deCollider *collider);
	void UpdateAnyContentVisibile();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pUpdateContent();
	void pUpdateVisiblity();
	
	igdeGDClass::Ref pCreateWorldGDClass();
	void pCreateSubObjects();
	void pCreateSubObjects(const decString &prefix, const igdeGDClass &gdclas, int filter);
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
