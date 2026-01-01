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

#ifndef _MEOBJECT_H_
#define _MEOBJECT_H_

#include "meObject.h"
#include "meObjectLink.h"
#include "meObjectSnapPoint.h"
#include "texture/meObjectTexture.h"
#include "../meColliderOwner.h"
#include "../decal/meDecal.h"
#include "../idgroup/meMapIDGroup.h"

#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/debug/deDebugDrawer.h>

class meCamera;
class meWorld;

class igdeEnvironment;
class igdeWCoordSysArrows;
class igdeWDebugDrawerShape;
class igdeGameDefinition;

class deCollider;
class deComponentTexture;
class decShape;
class deEngine;


/**
 * World object.
 */
class meObject : public deObject{
public:
	typedef deTObjectReference<meObject> Ref;
	typedef decTObjectOrderedSet<meObject> List;
	
	
private:
	class cWOAsyncFinished : public igdeWObject::cAsyncLoadFinished{
		meObject &pObject;
		
	public:
		cWOAsyncFinished(meObject &object);
		void LoadFinished(igdeWObject &wrapper, bool succeeded) override;
		void AnyContentVisibleChanged(igdeWObject &wrapper) override;
		void ExtendsChanged(igdeWObject &wrapper) override;
	};
	
	class cWOTexture : public deObject{
	public:
		typedef deTObjectReference<cWOTexture> Ref;
		
		deSkin::Ref skin;
		int texture;
		deDynamicSkin::Ref dynamicSkin;
		decTexMatrix2 texCoordTransform;
		
		cWOTexture(const deComponentTexture &componentTexture);
		
	protected:
		~cWOTexture() override = default;
	};
	
	igdeEnvironment *pEnvironment;
	
	meWorld *pWorld;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWDebugDrawerShape::Ref pDDSObject;
	igdeWDebugDrawerShape::Ref pDDSLightAoE;
	igdeWDebugDrawerShape::Ref pDDSOcclusionMesh;
	igdeWDebugDrawerShape::Ref pDDSObjectShapes;
	igdeWDebugDrawerShape::List pDDSListNavSpaces;
	igdeWCoordSysArrows *pDDSCoordSysArrows;
	
	igdeWObject::Ref pWObject;
	decTObjectOrderedSet<cWOTexture> pWOTextures;
	deComponent::Ref pEngComponentBroken;
	deColliderVolume::Ref pColDetCollider;
	meCamera *pCamera;
	
	float pRange;
	
	meObjectLink::List pLinks;
	meObjectSnapPoint::List pSnapPoints;
	
	meObjectTexture::List pTextures;
	meObjectTexture::Ref pActiveTexture;
	
	meDecal::List pDecals;
	
	Ref pAttachedTo;
	List pAttachedObjects;
	decString pAttachedToID;
	
	meMapIDGroup::List pMapIDGroup, pTexMapIDGroup;
	
	decString pClassName;
	igdeGDClass *pClassDef;
	decDVector pPosition;
	decVector pRotation;
	decVector pSize;
	decVector pScaling;
	decStringDictionary pProperties;
	decStringList pAttachBehaviors;
	decString pActiveProperty;
	int pActiveAttachBehavior;
	decUniqueID pID;
	bool pSelected;
	bool pActive;
	bool pVisible;
	bool pShowMissingTextures;
	
	meColliderOwner pColliderOwner;
	
	cWOAsyncFinished pWOAsyncFinished;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new object. */
	meObject(igdeEnvironment *environment);
	
protected:
	/** Cleans up the object. */
	virtual ~meObject();
	
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the environment. */
	inline igdeEnvironment *GetEnvironment() const{ return pEnvironment; }
	/** Retrieves the object wrapper. */
	inline const igdeWObject::Ref &GetObjectWrapper() const{ return pWObject; }
	/** Retrieves the collision detection collider. */
	inline const deColliderVolume::Ref &GetColDetCollider() const{ return pColDetCollider; }
	
	/** Debug drawer. */
	inline const deDebugDrawer::Ref &GetDebugDrawer() const{ return pDebugDrawer; }
	
	/** Dispose of the component. */
	void Dispose();
	
	/** Retrieves the camera or nullptr if not existing. */
	inline meCamera *GetCamera() const{ return pCamera; }
	
	inline meWorld *GetWorld() const{ return pWorld; }
	void SetWorld(meWorld *world);
	
	inline bool GetSelected() const{ return pSelected; }
	void SetSelected(bool selected);
	inline bool GetActive() const{ return pActive; }
	void SetActive(bool active);
	inline bool GetVisible() const{ return pVisible; }
	void SetVisible(bool visible);
	/** Determines if missing textures are shown. */
	inline bool GetShowMissingTextures() const{ return pShowMissingTextures; }
	/** Sets if missing textures are shown. */
	void SetShowMissingTextures(bool showMissingTextures);
	
	/** Retrieves the scale mode of the object class or esmFree if no class exists. */
	igdeGDClass::eScaleModes GetScaleMode() const;
	
	/**
	 * Determines if this object is a ghost according to the object class.
	 * \details if the object class does not exist the object is assumed to be no ghost.
	 */
	bool IsGhost() const;
	
	/** Game definition changed. */
	void OnGameDefinitionChanged();
	
	/** Active camera changes. */
	void OnActiveCameraChanged();
	
	/** \name Links */
	/*@{*/
	/** Links. */
	const meObjectLink::List &GetLinks() const{ return pLinks; }
	
	/** Retrieves the link to another object or nullptr if not linked. */
	meObjectLink *GetLinkTo(meObject *target) const;
	
	/** Determines if a link exists to another object. */
	bool HasLinkTo(meObject *target) const;
	
	/** Adds a link. */
	void AddLink(meObjectLink *link);
	
	/** Removes a link. */
	void RemoveLink(meObjectLink *link);
	
	/** Removes all links. */
	void RemoveAllLinks();
	
	/**
	 * Determines if a link to another object is possible.
	 * \details A link is possible if both objects have a game definition class and if the
	 *          other object exists as a link partner (either directly or as superclass)
	 *          in the game definition of this object.
	 */
	bool CanLinkTo(meObject *object) const;
	/*@}*/
	
	/** Updates the component mostly playing the animation if any. */
	void Update(float elapsed);
	
	/** Retrieves the class name. */
	inline const decString &GetClassName() const{ return pClassName; }
	/** Sets the class name. */
	void SetClassName(const char *className);
	
	/** Retrieves the game definition class or nullptr if not found. */
	inline igdeGDClass *GetGDClass() const{ return pClassDef; }
	
	/** Retrieves the position. */
	inline decDVector GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition(const decDVector &pos);
	/** Retrieves the rotation. */
	inline decVector GetRotation() const{ return pRotation; }
	/** Sets the rotation. */
	void SetRotation(const decVector &rot);
	/** Retrieves the size. */
	inline decVector GetSize() const{ return pSize; }
	/** Sets the size. */
	void SetSize(const decVector &size);
	/** Retrieves the scaling. */
	inline decVector GetScaling() const{ return pScaling; }
	/** Sets the scaling. */
	void SetScaling(const decVector &scaling);
	
	/** Set scaling and size for undo actions. */
	void SetSizeAndScaling(const decVector &size, const decVector &scaling);
	
	/** ID. */
	inline const decUniqueID &GetID() const{ return pID; }
	
	/** Set ID. */
	void SetID(const decUniqueID &id);
	
	/** Update debug drawer with all object shapes defined in all properties. */
	void UpdateDDSObjectShapes();
	
	
	
	/** Object this object is attached to or \em nullptr. */
	inline const Ref &GetAttachedTo() const{ return pAttachedTo; }
	
	/** Set object this object is attached to or \em nullptr. */
	void SetAttachedTo(meObject *object);
	
	/** List of attached objects. */
	inline List &GetAttachedObjects(){ return pAttachedObjects; }
	inline const List &GetAttachedObjects() const { return pAttachedObjects; }
	
	/** List of all directly and indirectly attached objects. */
	meObject::List GetAllAttachedObjects() const;
	
	/** Attachted to ID used temporary for loading. */
	inline const decString &GetAttachedToID() const{ return pAttachedToID; }
	
	/** Set attachted to ID used temporary for loading. */
	void SetAttachedToID(const char *id);
	
	
	
	/** Update navigation test path. */
	void UpdateNavPathTest();
	
	/** Get object matrix. */
	decDMatrix GetObjectMatrix();
	
	/** Get inverse object matrix. */
	decDMatrix GetInverseObjectMatrix();
	
	
	
	/** Decrement use count on identifier group identifiers. */
	void DecrementIDGroupIDUsage();
	
	/** Increment use count on identifier group identifiers. */
	void IncrementIDGroupIDUsage();
	
	/** Update id group list matching game definition class. */
	void UpdateIDGroupList();
	
	
	
	/** Show states changed. This typically changes debug drawer shape visibilites. */
	void ShowStateChanged();
	
	/** For internal use only. */
	void WOAsyncFinished();
	void WOAnyContentVisibleChanged();
	void WOExtendsChanged();
	/*@}*/
	
	/** \name Decals */
	/*@{*/
	/** Decals. */
	inline const meDecal::List &GetDecals() const{ return pDecals; }
	
	/** Adds a decal. */
	void AddDecal(meDecal *decal);
	
	/** Inserts a decal at the given location. */
	void InsertDecalAt(meDecal *decal, int index);
	
	/** Removes a decal. */
	void RemoveDecal(meDecal *decal);
	
	/** Removes all decals. */
	void RemoveAllDecals();
	
	/** Moves a decal to a new location. */
	void MoveDecalTo(meDecal *decal, int index);
	/*@}*/
	
	/** \name Collision Detection */
	/*@{*/
	/**
	 * Stores all decals in contact with the given surface point into the provided list.
	 * \details The point is best taken from an earlier collision test and is in world coordinates.
	 */
	void FindDecalsAt(const decVector &point, deDecalList *list);
	/**
	 * Stores all decals in contact with the given shape into the provided list.
	 */
	void FindDecalsTouching(decShape *shape, deDecalList *list);
	
	/** Update trigger targets. */
	void UpdateTriggerTargets();
	/** Checks links. */
	void CheckLinks();
	/*@}*/
	
	/** \name Texture */
	/*@{*/
	/** Textures. */
	inline const meObjectTexture::List &GetTextures() const{ return pTextures; }
	
	/** Retrieves the texture with the given name of nullptr if not found. */
	meObjectTexture *GetTextureNamed(const char *name) const;
	
	/** Determines if a texture with the given name exists. */
	bool HasTextureNamed(const char *name) const;
	
	/** Retrieves the index of the texture with the given name or -1 if not found. */
	int IndexOfTextureNamed(const char *name) const;
	
	/** Adds a texture. */
	void AddTexture(meObjectTexture *texture);
	
	/** Removes a texture. */
	void RemoveTexture(meObjectTexture *texture);
	
	/** Removes all textures. */
	void RemoveAllTextures();
	
	/** Retrieves the active texture or nullptr. */
	inline const meObjectTexture::Ref &GetActiveTexture() const{ return pActiveTexture; }
	
	/** Sets the active texture or nullptr. */
	void SetActiveTexture(meObjectTexture *texture);
	
	/** Updates the engine component textures. */
	void UpdateComponentTextures();
	
	/**
	 * Checks if the component is fully invisible or otherwise broken in a way the user can not
	 *        see the component in the editor. Returns true if this is the case false otherwise.
	 */
	bool IsComponentBroken() const;
	
	/** Retrieves the list of texture names in the object. */
	void GetTextureNameList(decStringList &list) const;
	
	/** Retrieves the list of texture names in the component model if existing. */
	void GetModelTextureNameList(decStringList &list) const;
	/*@}*/
	
	
	
	/** \name Properties */
	/*@{*/
	/** Properties. */
	inline const decStringDictionary &GetProperties() const{ return pProperties; }
	
	/** Set property. */
	void SetProperty(const char *key, const char *value);
	
	/** Set properties. */
	void SetProperties(const decStringDictionary &properties);
	
	/** Remove property if present. */
	void RemoveProperty(const char *key);
	
	/** Removes all properties. */
	void RemoveAllProperties();
	
	/** Active property. */
	inline const decString &GetActiveProperty() const{ return pActiveProperty; }
	
	/** Set active property. */
	void SetActiveProperty(const char *property);
	
	
	
	/** Property is a shape property. */
	bool IsPropertyShape(const char *property) const;
	
	/** Property is a shape list property. */
	bool IsPropertyShapeList(const char *property) const;
	
	/** Property is a shape or shape list property. */
	bool IsPropertyShapeOrShapeList(const char *property) const;
	/*@}*/
	
	
	
	/** \name Attach behaviors */
	/*@{*/
	/** Attach behaviors. */
	inline const decStringList &GetAttachBehaviors() const{ return pAttachBehaviors; }
	
	/** Set attach behaviors. */
	void SetAttachBehaviors(const decStringList &list);
	
	/** Active attach behavior. */
	inline int GetActiveAttachBehavior() const{ return pActiveAttachBehavior; }
	
	/** Set active attach behavior. */
	void SetActiveAttachBehavior(int attachBehavior);
	/*@}*/
	
private:
	void pCleanUp();
	//void pUpdateClassDef();
	void pUpdateShapes();
	void pUpdateShapeLight();
	void pUpdateShapeOcclusionMesh();
	void pUpdateDDSColors();
	void pUpdateDDSNavSpaces();
	void pUpdateDDSCoordSysArrowsLength();
	void pRepositionDDSNavSpaces();
	void pUpdateOutline();
	
	void pUpdateComponent();
	void pUpdateBrokenComponent();
	void pUpdateProperties();
	
	void pUpdateCamera();
	void pCheckCameraProps();
	void pRepositionCamera();
	
	void pRepositionLinks();
	
	void pRemoveAllSnapPoints();
	void pCreateSnapPoints();
	
	void pSizeFromScaling();
	void pScalingFromSize();
	
	float pGetRangeFor(float halfIntDist, float attExp, float targetIntensity) const;
	
	void pNotifyDecalsAboutChange();
	bool pAnyGDClassHasAnyPartialVisOf(const igdeGDClass &gdclass, const igdeTagManager &tags) const;
	
	void pUpdateIDGroupList(const igdeGDClass &gdclass, const decString &prefix);
	bool pShowStateIsVisible();
	
	void pAddAttachedObject(meObject::List &list);
};

#endif
