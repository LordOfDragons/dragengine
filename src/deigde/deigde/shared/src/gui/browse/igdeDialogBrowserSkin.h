/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEDIALOGBROWSERSKIN_H_
#define _IGDEDIALOGBROWSERSKIN_H_

#include "igdeDialogBrowser.h"


class igdeGDSkin;


/**
 * \brief Dialog for user to browse skin.
 */
class igdeDialogBrowserSkin : public igdeDialogBrowser{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogBrowserSkin( igdeEnvironment &environment, const char *title = "Select Skin" );
	
protected:
	/** \brief Clean up selection dialog. */
	virtual ~igdeDialogBrowserSkin();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Selected skin. */
	igdeGDSkin *GetSelectedSkin() const;
	
	/** \brief Set selected skin. */
	void SetSelectedSkin( igdeGDSkin *gdSkin );
	
	
	
	/** \brief Convenience method to select skin. */
	static bool SelectSkin( igdeWidget *owner, igdeGDSkin* &skin, const char *title = "Select Skin" );
	
	/** \brief Convenience method to select skin. */
	static bool SelectSkin( igdeWidget *owner, decString &skin, const char *title = "Select Skin" );
	/*@}*/
	
	
	
protected:
	virtual igdeGDCategory *GetRootCategory() const;
	virtual void AddItemsToList( igdeGDAddToListVisitor &visitor );
	virtual void RebuildItemPreview( igdeGDPreviewManager &pvmgr, igdeGDPreviewListener *listener );
	virtual void GetSelectedItemInfo( decString &info );
};

#endif
