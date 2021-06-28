/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEWPSNAVSPACE_H_
#define _MEWPSNAVSPACE_H_

#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditDVectorReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class meWPSNavSpaceListener;
class meWPSelection;
class meWorld;
class meNavigationSpace;



/**
 * \brief Navigation space selection panel.
 */
class meWPSNavSpace : public igdeContainerScroll{
private:
	meWPSelection &pWPSelection;
	meWPSNavSpaceListener *pListener;
	
	meWorld *pWorld;
	
	igdeSpinTextFieldReference pSpinActive;
	igdeTextFieldReference pEditSelCount;
	igdeEditDVectorReference pEditPositon;
	igdeEditVectorReference pEditOrientation;
	igdeEditPathReference pEditPath;
	igdeListBoxReference pListUsedCostTypes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPSNavSpace( meWPSelection &wpselection );
	
protected:
	/** \brief Clean up panel. */
	virtual ~meWPSNavSpace();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Selection panel. */
	inline meWPSelection &GetWPSelection() const{ return pWPSelection; }
	
	/** \brief World. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	
	/** \brief Active navigation space. */
	meNavigationSpace *GetNavigationSpace() const;
	
	/** \brief Update selection. */
	void UpdateSelection();
	
	/** \brief Update enabled state of all controls. */
	void UpdateEnabled();
	
	/** \brief Updates the navigation space. */
	void UpdateNavSpace();
	
	/** \brief Updates the navigation space geometry. */
	void UpdateGeometry();
	
	/** \brief Updates used cost types list. */
	void UpdateUsedCostTypes();
	/*@}*/
};

#endif
