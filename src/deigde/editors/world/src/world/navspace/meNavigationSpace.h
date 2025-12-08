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

#ifndef _MENAVIGATIONSPACE_H_
#define _MENAVIGATIONSPACE_H_

#include "../meColliderOwner.h"

#include <deigde/gui/wrapper/igdeWObject.h>

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decIntList.h>

class deRig;
class deDebugDrawer;
class deEngine;
class igdeEnvironment;
class meWorld;
class igdeWDebugDrawerShape;
class deNavigationSpace;
class deColliderComponent;
class deComponent;



/**
 * \brief Navigation Space.
 */
class meNavigationSpace : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meNavigationSpace> Ref;
	
	
private:
	igdeEnvironment *pEnvironment;
	
	meWorld *pWorld;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWDebugDrawerShape *pDDSNavSpace;
	deNavigationSpace::Ref pEngNavSpace;
	deColliderComponent::Ref pEngCollider;
	deComponent::Ref pEngColComponent;
	deRig::Ref pEngRig;
	igdeWObject::Ref pObjectPlaceholder;
	
	decDVector pPosition;
	decVector pOrientation;
	decString pFilename;
	
	decIntList pUsedCostTypes;
	
	bool pSelected;
	bool pActive;
	
	meColliderOwner pColliderOwner;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new navigation space. */
	meNavigationSpace(igdeEnvironment *environment);
	/** \brief Cleans up the navigation space. */
	virtual ~meNavigationSpace();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the game engine. */
	deEngine *GetEngine() const;
	/** \brief Retrieves the engine collider. */
	inline deColliderComponent *GetEngineCollider() const{ return pEngCollider; }
	/** \brief Retrieves the engine navigation space. */
	inline deNavigationSpace *GetEngineNavSpace() const{ return pEngNavSpace; }
	
	/** \brief Retrieves the world or NULL. */
	inline meWorld *GetWorld() const{ return pWorld; }
	/** \brief Sets the world or NULL. */
	void SetWorld(meWorld *world);
	
	/** \brief Retrieves the position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	/** \brief Sets the position. */
	void SetPosition(const decDVector &position);
	/** \brief Retrieves the orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	/** \brief Sets the orientation. */
	void SetOrientation(const decVector &orientation);
	/** \brief Retrieves the filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	/** \brief Sets the filename. */
	void SetFilename(const char *filename);
	
	/** \brief Retrieves the list of cost types used in the loaded navigation space. */
	inline decIntList &GetUsedCostTypes(){ return pUsedCostTypes; }
	inline const decIntList &GetUsedCostTypes() const{ return pUsedCostTypes; }
	
	/** \brief Determines if the navigation space is selected. */
	inline bool GetSelected() const{ return pSelected; }
	/** \brief Sets if the navigation space is selected. */
	void SetSelected(bool selected);
	/** \brief Determines if the navigation space is active. */
	inline bool GetActive() const{ return pActive; }
	/** \brief Sets if the navigation space is active. */
	void SetActive(bool active);
	
	/** \brief Load Navigation Space from the file with the stored filename. */
	void LoadFromFile();
	/** \brief Save Navigation Space to the file with the stored filename. */
	void SaveToFile();
	
	/** \brief Notifies all that the navigation space changed. */
	void NotifyChanged();
	/** \brief Notifies all that the geometry changed. */
	void NotifyGeometryChanged();
	
	/** \brief Show states changed. This typically changes debug drawer shape visibilites. */
	void ShowStateChanged();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateShapes();
	void pUpdateDDSSpace();
	void pUpdateDDSColors();
	
	void pUpdateUsedCostTypes();
};

#endif
