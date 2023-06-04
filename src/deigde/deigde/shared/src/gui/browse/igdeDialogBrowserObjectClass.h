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

#ifndef _IGDEDIALOGBROWSEROBJECTCLASS_H_
#define _IGDEDIALOGBROWSEROBJECTCLASS_H_

#include "igdeDialogBrowser.h"


class igdeGDClass;


/**
 * \brief Dialog for user to browse object class.
 */
class DE_DLL_EXPORT igdeDialogBrowserObjectClass : public igdeDialogBrowser{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogBrowserObjectClass( igdeEnvironment &environment,
		const char *title = "Select Object Class" );
	
protected:
	/** \brief Clean up selection dialog. */
	virtual ~igdeDialogBrowserObjectClass();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Selected object class. */
	igdeGDClass *GetSelectedObjectClass() const;
	
	/** \brief Set selected object class. */
	void SetSelectedObjectClass( igdeGDClass *gdClass );
	
	
	
	/** \brief Convenience method to select object class. */
	static bool SelectObjectClass( igdeWidget *owner, igdeGDClass* &objectClass,
		const char *title = "Select Object Class" );
	
	/** \brief Convenience method to select object class. */
	static bool SelectObjectClass( igdeWidget *owner, decString &objectClass,
		const char *title = "Select Object Class" );
	/*@}*/
	
	
	
protected:
	virtual igdeGDCategory *GetRootCategory() const;
	virtual void AddItemsToList( igdeGDAddToListVisitor &visitor );
	virtual void RebuildItemPreview( igdeGDPreviewManager &pvmgr, igdeGDPreviewListener *listener );
	virtual void GetSelectedItemInfo( decString &info );
};

#endif
