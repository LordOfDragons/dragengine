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

#ifndef _GDEWPSCATEGORY_H_
#define _GDEWPSCATEGORY_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

#include "../../../undosys/category/gdeUCategoryBase.h"

class gdeWindowProperties;
class gdeWPSCategoryListener;
class gdeCategory;
class gdeGameDefinition;



/**
 * \brief Category property panel.
 */
class gdeWPSCategory : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSCategoryListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeTextFieldReference pEditName;
	igdeTextAreaReference pEditDescription;
	igdeWidgetReference pAutoCategorizePattern;
	igdeCheckBoxReference pChkHidden;
	
	igdeListBoxReference pListElements;
	igdeButtonReference pBtnJumpToElement;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSCategory( gdeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSCategory();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em NULL if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Category or \em NULL if not set. */
	gdeCategory *GetCategory() const;
	
	/** \brief Category type if category is set. */
	gdeUCategoryBase::eCategoryType GetCategoryType() const;
	
	
	
	/** \brief Update category. */
	void UpdateCategory();
	
	/** \brief Update list of elements using this category. */
	void UpdateListElements();
	/*@}*/
};

#endif
 
