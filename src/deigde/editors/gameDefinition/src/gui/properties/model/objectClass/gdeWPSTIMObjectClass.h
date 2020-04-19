/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEWPSTIMOBJECTCLASS_H_
#define _GDEWPSTIMOBJECTCLASS_H_

#include "../gdeWPSTreeItemModel.h"

class gdeObjectClass;
class gdeOCBillboard;
class gdeOCCamera;
class gdeOCComponent;
class gdeOCEnvMapProbe;
class gdeOCLight;
class gdeOCNavigationBlocker;
class gdeOCNavigationSpace;
class gdeOCParticleEmitter;
class gdeOCForceField;
class gdeOCSnapPoint;
class gdeOCSpeaker;
class gdeWPSTIMOCBillboard;
class gdeWPSTIMOCCamera;
class gdeWPSTIMOCComponent;
class gdeWPSTIMOCEnvMapProbe;
class gdeWPSTIMOCLight;
class gdeWPSTIMOCNavBlocker;
class gdeWPSTIMOCNavSpace;
class gdeWPSTIMOCParticleEmitter;
class gdeWPSTIMOCForceField;
class gdeWPSTIMOCSnapPoint;
class gdeWPSTIMOCSpeaker;


/**
 * \brief Object class.
 */
class gdeWPSTIMObjectClass : public gdeWPSTreeItemModel{
private:
	gdeObjectClass *pObjectClass;
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMObjectClass( gdeWPSTreeModel &tree, gdeObjectClass *objectClass );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~gdeWPSTIMObjectClass();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Object class. */
	inline gdeObjectClass *GetObjectClass() const{ return pObjectClass; }
	
	
	
	/** \brief Model with billboard or \em NULL. */
	gdeWPSTIMOCBillboard *GetChildWith( gdeOCBillboard *billboard ) const;
	
	/** \brief Model with camera or \em NULL. */
	gdeWPSTIMOCCamera *GetChildWith( gdeOCCamera *camera ) const;
	
	/** \brief Model with component or \em NULL. */
	gdeWPSTIMOCComponent *GetChildWith( gdeOCComponent *component ) const;
	
	/** \brief Model with envmap probe or \em NULL. */
	gdeWPSTIMOCEnvMapProbe *GetChildWith( gdeOCEnvMapProbe *envMapProbe ) const;
	
	/** \brief Model with light or \em NULL. */
	gdeWPSTIMOCLight *GetChildWith( gdeOCLight *light ) const;
	
	/** \brief Model with navblocker or \em NULL. */
	gdeWPSTIMOCNavBlocker *GetChildWith( gdeOCNavigationBlocker *navblocker ) const;
	
	/** \brief Model with navspace or \em NULL. */
	gdeWPSTIMOCNavSpace *GetChildWith( gdeOCNavigationSpace *navspace ) const;
	
	/** \brief Model with particle emitter or \em NULL. */
	gdeWPSTIMOCParticleEmitter *GetChildWith( gdeOCParticleEmitter *particleEmitter ) const;
	
	/** \brief Model with force field or \em NULL. */
	gdeWPSTIMOCForceField *GetChildWith( gdeOCForceField *forceField ) const;
	
	/** \brief Model with snap point or \em NULL. */
	gdeWPSTIMOCSnapPoint *GetChildWith( gdeOCSnapPoint *snapPoint ) const;
	
	/** \brief Model with speaker or \em NULL. */
	gdeWPSTIMOCSpeaker *GetChildWith( gdeOCSpeaker *speaker ) const;
	
	
	
	/** \brief Name changed. */
	void NameChanged();
	
	/** \brief Sub objects changed. */
	void SubObjectsChanged();
	
	/** \brief Verify if object class is valid. */
	bool IsValid() const;
	
	/** \brief Validate component and update state accordingly. */
	void Validate();
	
	/** \brief Validate due to change in object class names or existance. */
	void ValidateObjectClassName();
	
	/** \brief Validate due to change in category names or existance. */
	void ValidateCategoryName();
	
	
	
	/** \brief Added to tree. */
	virtual void OnAddedToTree();
	
	/** \brief Compare this item with another for sorting. */
	virtual int Compare( const gdeWPSTreeItemModel &item ) const;
	
	/** \brief User selected item. */
	virtual void OnSelected();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** \brief Select object mest matching name. */
	virtual void SelectBestMatching( const char *string );
	/*@}*/
	
	
	
private:
	void pAddSubObjects();
};

#endif

