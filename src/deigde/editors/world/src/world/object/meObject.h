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

#include "meObjectList.h"
#include "../meColliderOwner.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decUniqueID.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>

#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShapeList.h>

class meCamera;
class meDecal;
class meObjectLink;
class meObjectTexture;
class meWorld;

class igdeEnvironment;
class igdeWCoordSysArrows;
class igdeWDebugDrawerShape;
class igdeWObject;
class igdeGameDefinition;

class deCollider;
class deColliderVolume;
class deComponent;
class deComponentTexture;
class decShape;
class decStringList;
class deDebugDrawer;
class deDecalList;
class deEngine;


/**
 * \brief World object.
 */
class meObject : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<meObject> Ref;
	
	
	
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
		deSkin::Ref skin;
		int texture;
		deDynamicSkin::Ref dynamicSkin;
		decTexMatrix2 texCoordTransform;
		
		cWOTexture( const deComponentTexture &componentTexture );
	};
	
	igdeEnvironment *pEnvironment;
	
	meWorld *pWorld;
	
	deDebugDrawer *pDebugDrawer;
	igdeWDebugDrawerShape *pDDSObject;
	igdeWDebugDrawerShape *pDDSLightAoE;
	igdeWDebugDrawerShape *pDDSOcclusionMesh;
	igdeWDebugDrawerShape *pDDSObjectShapes;
	igdeWDebugDrawerShapeList pDDSListNavSpaces;
	igdeWCoordSysArrows *pDDSCoordSysArrows;
	
	igdeWObject *pWObject;
	decObjectList pWOTextures;
	deComponent *pEngComponentBroken;
	deColliderVolume *pColDetCollider;
	meCamera *pCamera;
	
	float pRange;
	
	decObjectList pLinks;
	decObjectList pSnapPoints;
	
	meObjectTexture **pTextures;
	int pTextureCount;
	int pTextureSize;
	meObjectTexture *pActiveTexture;
	
	meDecal **pDecals;
	int pDecalCount;
	int pDecalSize;
	
	meObject *pAttachedTo;
	meObjectList pAttachedObjectsList;
	decString pAttachedToID;
	
	decObjectList pMapIDGroup;
	decObjectList pTexMapIDGroup;
	
	decString pClassName;
	igdeGDClass *pClassDef;
	decDVector pPosition;
	decVector pRotation;
	decVector pSize;
	decVector pScaling;
	decStringDictionary pProperties;
	decString pActiveProperty;
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
	/** \brief Creates a new object. */
	meObject( igdeEnvironment *environment );
	/** \brief Cleans up the object. */
	virtual ~meObject();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the environment. */
	inline igdeEnvironment *GetEnvironment() const{ return pEnvironment; }
	/** \brief Retrieves the object wrapper. */
	inline igdeWObject *GetObjectWrapper() const{ return pWObject; }
	/** \brief Retrieves the collision detection collider. */
	inline deColliderVolume *GetColDetCollider() const{ return pColDetCollider; }
	
	/** \brief Debug drawer. */
	inline deDebugDrawer *GetDebugDrawer() const{ return pDebugDrawer; }
	
	/** \brief Dispose of the component. */
	void Dispose();
	
	/** \brief Retrieves the camera or nullptr if not existing. */
	inline meCamera *GetCamera() const{ return pCamera; }
	
	inline meWorld *GetWorld() const{ return pWorld; }
	void SetWorld( meWorld *world );
	
	inline bool GetSelected() const{ return pSelected; }
	void SetSelected( bool selected );
	inline bool GetActive() const{ return pActive; }
	void SetActive( bool active );
	inline bool GetVisible() const{ return pVisible; }
	void SetVisible( bool visible );
	/** \brief Determines if missing textures are shown. */
	inline bool GetShowMissingTextures() const{ return pShowMissingTextures; }
	/** \brief Sets if missing textures are shown. */
	void SetShowMissingTextures( bool showMissingTextures );
	
	/** \brief Retrieves the scale mode of the object class or esmFree if no class exists. */
	igdeGDClass::eScaleModes GetScaleMode() const;
	
	/**
	 * \brief Determines if this object is a ghost according to the object class.
	 * \details if the object class does not exist the object is assumed to be no ghost.
	 */
	bool IsGhost() const;
	
	/** \brief Game definition changed. */
	void OnGameDefinitionChanged();
	
	/** \brief Active camera changes. */
	void OnActiveCameraChanged();
	
	/** \name Links */
	/*@{*/
	/** \brief Retrieves the number of links. */
	int GetLinkCount() const;
	/** \brief Retrieves a link by index. */
	meObjectLink *GetLinkAt( int index ) const;
	/** \brief Retrieves the link to another object or NULL if not linked. */
	meObjectLink *GetLinkTo( meObject *target ) const;
	/** \brief Determines if a link exists to another object. */
	bool HasLinkTo( meObject *target ) const;
	/** \brief Determines if a link exists. */
	bool HasLink( meObjectLink *link ) const;
	/** \brief Retrieves the index of a link or -1 if not found. */
	int IndexOfLink( meObjectLink *link ) const;
	/** \brief Adds a link. */
	void AddLink( meObjectLink *link );
	/** \brief Removes a link. */
	void RemoveLink( meObjectLink *link );
	/** \brief Removes all links. */
	void RemoveAllLinks();
	
	/**
	 * \brief Determines if a link to another object is possible.
	 * \details A link is possible if both objects have a game definition class and if the
	 *          other object exists as a link partner (either directly or as superclass)
	 *          in the game definition of this object.
	 */
	bool CanLinkTo( meObject *object ) const;
	/*@}*/
	
	/** \brief Updates the component mostly playing the animation if any. */
	void Update( float elapsed );
	
	/** \brief Retrieves the class name. */
	inline const decString &GetClassName() const{ return pClassName; }
	/** \brief Sets the class name. */
	void SetClassName( const char *className );
	
	/** \brief Retrieves the game definition class or NULL if not found. */
	inline igdeGDClass *GetGDClass() const{ return pClassDef; }
	
	/** \brief Retrieves the position. */
	inline decDVector GetPosition() const{ return pPosition; }
	/** \brief Sets the position. */
	void SetPosition(const decDVector &pos);
	/** \brief Retrieves the rotation. */
	inline decVector GetRotation() const{ return pRotation; }
	/** \brief Sets the rotation. */
	void SetRotation(const decVector &rot);
	/** \brief Retrieves the size. */
	inline decVector GetSize() const{ return pSize; }
	/** \brief Sets the size. */
	void SetSize(const decVector &size);
	/** \brief Retrieves the scaling. */
	inline decVector GetScaling() const{ return pScaling; }
	/** \brief Sets the scaling. */
	void SetScaling( const decVector &scaling );
	
	/** Set scaling and size for undo actions. */
	void SetSizeAndScaling( const decVector &size, const decVector &scaling );
	
	/** \brief ID. */
	inline const decUniqueID &GetID() const{ return pID; }
	
	/** \brief Set ID. */
	void SetID( const decUniqueID &id );
	
	/** \brief Update debug drawer with all object shapes defined in all properties. */
	void UpdateDDSObjectShapes();
	
	
	
	/** \brief Object this object is attached to or \em NULL. */
	inline meObject *GetAttachedTo() const{ return pAttachedTo; }
	
	/** \brief Set object this object is attached to or \em NULL. */
	void SetAttachedTo( meObject *object );
	
	/** \brief List of attached objects. */
	inline meObjectList &GetAttachedObjectsList(){ return pAttachedObjectsList; }
	inline const meObjectList &GetAttachedObjectsList() const { return pAttachedObjectsList; }
	
	/** \brief Attachted to ID used temporary for loading. */
	inline const decString &GetAttachedToID() const{ return pAttachedToID; }
	
	/** \brief Set attachted to ID used temporary for loading. */
	void SetAttachedToID( const char *id );
	
	
	
	/** \brief Update navigation test path. */
	void UpdateNavPathTest();
	
	/** \brief Get object matrix. */
	decDMatrix GetObjectMatrix();
	
	/** \brief Get inverse object matrix. */
	decDMatrix GetInverseObjectMatrix();
	
	
	
	/** \brief Decrement use count on identifier group identifiers. */
	void DecrementIDGroupIDUsage();
	
	/** \brief Increment use count on identifier group identifiers. */
	void IncrementIDGroupIDUsage();
	
	/** \brief Update id group list matching game definition class. */
	void UpdateIDGroupList();
	
	
	
	/** \brief Show states changed. This typically changes debug drawer shape visibilites. */
	void ShowStateChanged();
	
	/** \brief For internal use only. */
	void WOAsyncFinished();
	void WOAnyContentVisibleChanged();
	void WOExtendsChanged();
	/*@}*/
	
	/** \name Decals */
	/*@{*/
	/** \brief Retrieves the number of decals. */
	inline int GetDecalCount() const{ return pDecalCount; }
	/** \brief Retrieves the decal at the given index. */
	meDecal *GetDecalAt( int index ) const;
	/** \brief Retrieves the index of the decal of -1 if not found. */
	int IndexOfDecal( meDecal *decal ) const;
	/** \brief Determines if the decal exists. */
	bool HasDecal( meDecal *decal ) const;
	/** \brief Adds a decal. */
	void AddDecal( meDecal *decal );
	/** \brief Inserts a decal at the given location. */
	void InsertDecalAt( meDecal *decal, int index );
	/** \brief Removes a decal. */
	void RemoveDecal( meDecal *decal );
	/** \brief Removes all decals. */
	void RemoveAllDecals();
	/** \brief Moves a decal to a new location. */
	void MoveDecalTo( meDecal *decal, int index );
	/*@}*/
	
	/** \name Collision Detection */
	/*@{*/
	/**
	 * \brief Stores all decals in contact with the given surface point into the provided list.
	 * \details The point is best taken from an earlier collision test and is in world coordinates.
	 */
	void FindDecalsAt( const decVector &point, deDecalList *list );
	/**
	 * \brief Stores all decals in contact with the given shape into the provided list.
	 */
	void FindDecalsTouching( decShape *shape, deDecalList *list );
	
	/** \brief Update trigger targets. */
	void UpdateTriggerTargets();
	/** \brief Checks links. */
	void CheckLinks();
	/*@}*/
	
	/** \name Texture */
	/*@{*/
	/** \brief Retrieves the number of textures. */
	inline int GetTextureCount() const{ return pTextureCount; }
	/** \brief Retrieves the texture at the given index. */
	meObjectTexture *GetTextureAt( int index ) const;
	/** \brief Retrieves the texture with the given name of NULL if not found. */
	meObjectTexture *GetTextureNamed( const char *name ) const;
	/** \brief Determines if a texture with the given name exists. */
	bool HasTextureNamed( const char *name ) const;
	/** \brief Retrieves the index of the texture or -1 if not found. */
	int IndexOfTexture( meObjectTexture *texture ) const;
	/** \brief Retrieves the index of the texture with the given name or -1 if not found. */
	int IndexOfTextureNamed( const char *name ) const;
	/** \brief Determines if the texture exists. */
	bool HasTexture( meObjectTexture *texture ) const;
	/** \brief Adds a texture. */
	void AddTexture( meObjectTexture *texture );
	/** \brief Removes a texture. */
	void RemoveTexture( meObjectTexture *texture );
	/** \brief Removes all textures. */
	void RemoveAllTextures();
	/** \brief Retrieves the active texture or NULL. */
	inline meObjectTexture *GetActiveTexture() const{ return pActiveTexture; }
	/** \brief Sets the active texture or NULL. */
	void SetActiveTexture( meObjectTexture *texture );
	/** \brief Updates the engine component textures. */
	void UpdateComponentTextures();
	/**
	 * \brief Checks if the component is fully invisible or otherwise broken in a way the user can not
	 *        see the component in the editor. Returns true if this is the case false otherwise.
	 */
	bool IsComponentBroken() const;
	/** \brief Retrieves the list of texture names in the object. */
	void GetTextureNameList( decStringList &list ) const;
	/** \brief Retrieves the list of texture names in the component model if existing. */
	void GetModelTextureNameList( decStringList &list ) const;
	/*@}*/
	
	
	
	/** \name Properties */
	/*@{*/
	/** \brief Properties. */
	inline const decStringDictionary &GetProperties() const{ return pProperties; }
	
	/** \brief Set property. */
	void SetProperty( const char *key, const char *value );
	
	/** \brief Set properties. */
	void SetProperties( const decStringDictionary &properties );
	
	/** \brief Remove property if present. */
	void RemoveProperty( const char *key );
	
	/** \brief Removes all properties. */
	void RemoveAllProperties();
	
	/** \brief Active property. */
	inline const decString &GetActiveProperty() const{ return pActiveProperty; }
	
	/** \brief Set active property. */
	void SetActiveProperty( const char *property );
	
	
	
	/** \brief Property is a shape property. */
	bool IsPropertyShape( const char *property ) const;
	
	/** \brief Property is a shape list property. */
	bool IsPropertyShapeList( const char *property ) const;
	
	/** \brief Property is a shape or shape list property. */
	bool IsPropertyShapeOrShapeList( const char *property ) const;
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
	
	float pGetRangeFor( float halfIntDist, float attExp, float targetIntensity ) const;
	
	void pNotifyDecalsAboutChange();
	bool pAnyGDClassHasAnyPartialVisOf( const igdeGDClass &gdclass, const igdeTagManager &tags ) const;
	
	void pUpdateIDGroupList( const igdeGDClass &gdclass, const decString &prefix );
};

#endif
