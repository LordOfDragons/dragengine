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

#ifndef _GDEWPSOCBILLBOARD_H_
#define _GDEWPSOCBILLBOARD_H_

#include "../../../gamedef/objectClass/billboard/gdeOCBillboard.h"

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/composed/igdeEditVector2Reference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeObjectClass;
class gdeOCBillboard;
class gdeWindowProperties;
class gdeWPSOCBillboardListener;



/**
 * \brief Object class billboard property panel.
 */
class gdeWPSOCBillboard : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSOCBillboardListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeEditPathReference pEditPathSkin;
	igdeEditVectorReference pEditAxis;
	igdeEditVector2Reference pEditSize;
	igdeEditVector2Reference pEditOffset;
	igdeCheckBoxReference pChkLocked;
	igdeCheckBoxReference pChkSpherical;
	igdeCheckBoxReference pChkRelativeSize;
	igdeCheckBoxReference pChkDoNotScale;
	igdeCheckBoxReference pChkRenderEnvMap;
	igdeCheckBoxReference pChkPartialHide;
	igdeEditVectorReference pEditPosition;
	igdeTextFieldReference pEditBoneName;
	igdeComboBoxReference pCBPropertyNames;
	igdeComboBoxFilterReference pCBPropertyNameTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCBillboard( gdeWindowProperties &windowMain );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSOCBillboard();
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
	
	/** \brief Active object class billboard or \em NULL if not set. */
	gdeOCBillboard *GetBillboard() const;
	
	/** \brief Selected property name. */
	const gdeOCBillboard::eProperties GetPropertyName() const;
	
	
	
	/** \brief Update object class property list. */
	void UpdatePropertyList();
	
	
	
	/** \brief Update billboard. */
	void UpdateBillboard();
	
	/** \brief Update billboard property name. */
	void UpdatePropertyName();
	/*@}*/
};

#endif
