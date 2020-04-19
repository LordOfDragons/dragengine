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

#ifndef _GDEWPSSKY_H_
#define _GDEWPSSKY_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionContextMenuReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeSky;
class gdeSkyController;
class gdeCategoryList;
class gdeWindowProperties;
class gdeWPSSkyListener;



/**
 * \brief Sky property panel.
 */
class gdeWPSSky : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSSkyListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeActionReference pActionControllerAdd;
	igdeActionReference pActionControllerRemove;
	igdeActionContextMenuReference pActionControllerMenu;
	
	igdeEditPathReference pEditPath;
	igdeTextFieldReference pEditName;
	igdeTextAreaReference pEditDescription;
	
	igdeComboBoxReference pCBController;
	igdeButtonReference pBtnControllerMenu;
	igdeTextFieldReference pEditControllerName;
	igdeTextFieldReference pEditControllerValue;
	
	igdeComboBoxFilterReference pCBCategory;
	igdeButtonReference pBtnJumpToCategory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSSky( gdeWindowProperties &windowMain );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSSky();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em NULL if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Active sky. */
	gdeSky *GetSky() const;
	
	/** \brief Active controller. */
	gdeSkyController *GetController() const;
	
	
	
	/** \brief Actions. */
	inline igdeAction *GetActionControllerAdd() const{ return pActionControllerAdd; }
	inline igdeAction *GetActionControllerRemove() const{ return pActionControllerRemove; }
	inline igdeActionContextMenu *GetActionControllerMenu() const{ return pActionControllerMenu; }
	
	
	
	/** \brief Update category list. */
	void UpdateCategoryList();
	
	/** \brief Update category list. */
	void UpdateCategoryList( const gdeCategoryList &list, const char *prefix );
	
	
	
	/** \brief Update sky. */
	void UpdateSky();
	
	/** \brief Update controller. */
	void UpdateController();
	/*@}*/
};

#endif
