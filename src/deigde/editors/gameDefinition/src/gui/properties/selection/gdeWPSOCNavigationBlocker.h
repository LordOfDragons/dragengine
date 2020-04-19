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

#ifndef _GDEWPSOCNAVIGATIONBLOCKER_H_
#define _GDEWPSOCNAVIGATIONBLOCKER_H_

#include "../../../gamedef/objectClass/navblocker/gdeOCNavigationBlocker.h"

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeObjectClass;
class gdeOCNavigationBlocker;
class gdeWindowProperties;
class gdeWPSOCNavigationBlockerListener;



/**
 * \brief Object class navigation blocker property panel.
 */
class gdeWPSOCNavigationBlocker : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSOCNavigationBlockerListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeEditVectorReference pEditScaling;
	igdeTextFieldReference pEditBoneName;
	igdeCheckBoxReference pChkEnabled;
	igdeTextFieldReference pEditLayer;
	igdeComboBoxReference pCBType;
	igdeTextFieldReference pEditBlockingPriority;
	igdeTextFieldReference pEditShape;
	
	igdeComboBoxReference pCBPropertyNames;
	igdeComboBoxFilterReference pCBPropertyNameTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCNavigationBlocker( gdeWindowProperties &windowMain );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSOCNavigationBlocker();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em NULL if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Active object class or \em NULL if absent. */
	gdeObjectClass *GetObjectClass() const;
	
	/** \brief Active object class navigation blocker or \em NULL if not set. */
	gdeOCNavigationBlocker *GetNavigationBlocker() const;
	
	/** \brief Selected property name. */
	const gdeOCNavigationBlocker::eProperties GetPropertyName() const;
	
	
	
	/** \brief Update object class property list. */
	void UpdatePropertyList();
	
	
	
	/** \brief Update navigation blocker. */
	void UpdateNavigationBlocker();
	
	/** \brief Update navigation blocker property name. */
	void UpdatePropertyName();
	/*@}*/
};

#endif
