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

#ifndef _MEDECAL_H_
#define _MEDECAL_H_

#include "../meColliderOwner.h"

#include <deigde/gui/igdeTimer.h>

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/decStringDictionary.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/collider/deColliderVolume.h>

class meObject;
class meWorld;
class meAttachedDecal;

class igdeWDebugDrawerShape;

class igdeEnvironment;
class deDecal;



/**
 * \brief Decal.
 */
class meDecal : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meDecal> Ref;
	
	
private:
	igdeEnvironment *pEnvironment;
	
	deSkin::Ref pEngSkin;
	
	meAttachedDecal **pAttachedDecals;
	int pAttachedDecalCount;
	igdeTimer::Ref pTimerReattachDecals;
	
	meWorld *pWorld;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWDebugDrawerShape *pDDSDecal;
	deColliderVolume::Ref pCollider;
	
	meObject *pParentObject;
	
	decString pSkinPath;
	decString pTextureName;
	decDVector pPosition;
	decVector pRotation;
	decVector pSize;
	decVector2 pTexCoordOffset;
	decVector2 pTexCoordScaling;
	float pTexCoordRotation;
	
	decStringDictionary pProperties;
	decString pActiveProperty;
	
	decColor pColorTint;
	
	deDynamicSkin::Ref pDynamicSkin;
	
	int pID;
	bool pVisible;
	
	bool pSelected;
	bool pActive;
	
	meColliderOwner pColliderOwner;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new decal. */
	meDecal(igdeEnvironment *environment);
	/** \brief Creates a new decal from the given decal. */
	meDecal(const meDecal &decal);
	/** \brief Cleans up the decal. */
	virtual ~meDecal();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment *GetEnvironment() const{ return pEnvironment; }
	/** \brief Retrieves the collider. */
	inline const deColliderVolume::Ref &GetCollider() const{ return pCollider; }
	
	/** \brief Retrieves the world or NULL. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** \brief Sets the world or NULL. */
	void SetWorld(meWorld *world);
	
	/** \brief Retrieves the parent object or NULL. */
	inline meObject *GetParentObject() const{ return pParentObject; }
	/** \brief Sets the parent object or NULL. */
	void SetParentObject(meObject *object);
	
	/** \brief Retrieves the skin file path. */
	inline const decString &GetSkinPath() const{ return pSkinPath; }
	/** \brief Sets the skin file path. */
	void SetSkinPath(const char *skinPath);
	
	/** \brief Load skin if set. */
	void LoadSkin();
	
	/** \brief Default size using loaded skin as base if possible. */
	decVector GetDefaultSize(float baseSize = 0.5f) const;
	
	/** \brief Retrieves the skin texture name. */
	inline const decString &GetTextureName() const{ return pTextureName; }
	/** \brief Sets the skin texture name. */
	void SetTextureName(const char *textureName);
	/** \brief Retrieves the engine skin or NULL if not found. */
	inline const deSkin::Ref &GetEngineSkin() const{ return pEngSkin; }
	/** \brief Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** \brief Sets the position. */
	void SetPosition(const decDVector &position);
	/** \brief Retrieves the rotation. */
	inline const decVector &GetRotation() const{ return pRotation; }
	/** \brief Sets the rotation. */
	void SetRotation(const decVector &rotation);
	/** \brief Retrieves the size. */
	inline const decVector &GetSize() const{ return pSize; }
	/** \brief Sets the size. */
	void SetSize(const decVector &size);
	/** \brief Retrieves the texture coordinates offset. */
	inline const decVector2 &GetTexCoordOffset() const{ return pTexCoordOffset; }
	/** \brief Sets the texture coordinates offset. */
	void SetTexCoordOffset(const decVector2 &offset);
	/** \brief Retrieves the texture coordinates scaling. */
	inline const decVector2 &GetTexCoordScaling() const{ return pTexCoordScaling; }
	/** \brief Sets the texture coordinates scaling. */
	void SetTexCoordScaling(const decVector2 &scaling);
	/** \brief Retrieves the texture coordinates rotation. */
	inline float GetTexCoordRotation() const{ return pTexCoordRotation; }
	/** \brief Sets the texture coordinates rotation. */
	void SetTexCoordRotation(float rotation);
	
	/** \brief Retrieves the color tint. */
	inline const decColor &GetColorTint() const{ return pColorTint; }
	/** \brief Sets the color tint. */
	void SetColorTint(const decColor &color);
	
	/** \brief Retrieves the dynamic skin or NULL if there is none. */
	inline const deDynamicSkin::Ref &GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Updates the dynamic skin. */
	void UpdateDynamicSkin();
	
	/** \brief Retrieves the ID. */
	inline int GetID() const{ return pID; }
	/** \brief Sets the ID. */
	void SetID(int id);
	/** \brief Determines if the decal is visible. */
	inline bool GetVisible() const{ return pVisible; }
	/** \brief Sets if the decal is visible. */
	void SetVisible(bool visible);
	
	/** \brief Determines if the decal is selected. */
	inline bool GetSelected() const{ return pSelected; }
	/** \brief Sets if the decal is selected. */
	void SetSelected(bool selected);
	/** \brief Determines if the decal is the active one. */
	inline bool GetActive() const{ return pActive; }
	/** \brief Sets if the decal is the active one. */
	void SetActive(bool active);
	
	/** \brief Notifies that the parent changed. */
	void NotifyParentChanged();
	
	/** \brief Show states changed. This typically changes debug drawer shape visibilites. */
	void ShowStateChanged();
	
	/** \brief Invalidate decals scheduling an update in the near future. */
	void InvalidateDecals();
	
	/** \brief Game definition changed. */
	void OnGameDefinitionChanged();
	/*@}*/
	
	
	
	/** \name Properties */
	/*@{*/
	/** \brief Properties. */
	inline const decStringDictionary &GetProperties() const{ return pProperties; }
	
	/** \brief Set property. */
	void SetProperty(const char *key, const char *value);
	
	/** \brief Set properties. */
	void SetProperties(const decStringDictionary &properties);
	
	/** \brief Remove property if present. */
	void RemoveProperty(const char *key);
	
	/** \brief Removes all properties. */
	void RemoveAllProperties();
	
	/** \brief Active property. */
	inline const decString &GetActiveProperty() const{ return pActiveProperty; }
	
	/** \brief Set active property. */
	void SetActiveProperty(const char *property);
	
	
	
	/** \brief Notify world texture active property changed. */
	void NotifyActivePropertyChanged();
	
	/** \brief Notify world texture properties changed. */
	void NotifyPropertiesChanged();
	/*@}*/
	
	
	
	/** \name For internal use only! */
	/*@{*/
	void DetachDecals();
	void AttachDecals();
	/*@}*/
	
	
	
private:
	void pInitShared();
	void pCleanUp();
	
	void pRepositionShapes();
	void pUpdateShapes();
	void pUpdateDDSColors();
	void pUpdateTransform();
	
	void pLoadSkin();
	void pUpdateSkin();
};

#endif
