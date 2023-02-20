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

#ifndef _IGDEDIALOGBROWSER_H_
#define _IGDEDIALOGBROWSER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../dialog/igdeDialog.h"
#include "../igdeTreeListReference.h"
#include "../igdeIconListBoxReference.h"
#include "../igdeTextFieldReference.h"
#include "../igdeTextAreaReference.h"
#include "../event/igdeActionReference.h"

class decPath;
class igdeGDCategory;
class igdeGDAddToListVisitor;
class igdeTreeItem;
class igdeListItem;
class igdeGDPreviewManager;
class igdeGDPreviewListener;


/**
 * \brief Base class for dialogs for user to browse for an object of specific type.
 */
class DE_DLL_EXPORT igdeDialogBrowser : public igdeDialog{
public:
	enum ePreviewSize{
		epsLarge,
		epsMedium,
		epsSmall
	};
	
	enum eViewModes{
		evmList,
		evmPreview
	};
	
	
	
private:
	igdeTreeListReference pTreeCategories;
	igdeIconListBoxReference pListItems;
	igdeTextFieldReference pEditFilter;
	
	igdeTextAreaReference pEditInfos;
	
	ePreviewSize pPreviewSize;
	eViewModes pViewMode;
	
	igdeActionReference pActionPISizeSmall;
	igdeActionReference pActionPISizeMedium;
	igdeActionReference pActionPISizeLarge;
	
	igdeActionReference pActionPIViewList;
	igdeActionReference pActionPIViewPreview;
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create dialog.
	 * \note Subclass has to call UpdateCategoryList() and UpdateItemList() when ready.
	 */
	igdeDialogBrowser( igdeEnvironment &environment, const char *title, bool canResize = true );
	
	/** \brief Clean up selection dialog. */
	virtual ~igdeDialogBrowser();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update category list. */
	void UpdateCategoryList();
	
	/** \brief Update category list starting at category. */
	void UpdateCategoryListWith( igdeGDCategory *category );
	
	/** \brief Update category list starting at category and tree item. */
	void AddCategoryToList( igdeGDCategory *category, igdeTreeItem *parent );
	
	/** \brief Update item list. */
	void UpdateItemList();
	
	/** \brief Rebuild preview of selected item. */
	void RebuildPISelectedItem();
	
	/** \brief Current item changed. */
	void CurrentItemChanged();
	
	/** \brief Selected category. */
	igdeGDCategory *GetSelectedCategory() const;
	
	/** \brief Select category. */
	void SelectCategory( igdeGDCategory *category );
	
	/** \brief Selected list item. */
	igdeListItem *GetSelectedListItem() const;
	
	/** \brief Select list item with data. */
	void SelectListItemWithData( void *data );
	
	/** \brief Preview size. */
	inline ePreviewSize GetPreviewSize() const{ return pPreviewSize; }
	
	/** \brief Set preview size. */
	void SetPreviewSize( ePreviewSize size );
	
	/** \brief View mode. */
	inline eViewModes GetViewMode() const{ return pViewMode; }
	
	/** \brief Set view mode. */
	void SetViewMode( eViewModes viewMode );
	
	/** \brief Preview icon size. */
	int GetPreviewIconSize() const;
	
	/** \brief Filter text. */
	const decString &GetFilter() const;
	
	/** \brief Set filter text. */
	void SetFilter( const char *filter );

	
	
	inline igdeAction *GetActionPISizeSmall() const{ return pActionPISizeSmall; }
	inline igdeAction *GetActionPISizeMedium() const{ return pActionPISizeMedium; }
	inline igdeAction *GetActionPISizeLarge() const{ return pActionPISizeLarge; }
	
	inline igdeAction *GetActionPIViewList() const{ return pActionPIViewList; }
	inline igdeAction *GetActionPIViewPreview() const{ return pActionPIViewPreview; }
	/*@}*/
	
	
	
protected:
	virtual igdeGDCategory *GetRootCategory() const = 0;
	virtual void AddItemsToList( igdeGDAddToListVisitor &visitor ) = 0;
	virtual void RebuildItemPreview( igdeGDPreviewManager &pvmgr, igdeGDPreviewListener *listener ) = 0;
	virtual void GetSelectedItemInfo( decString &info ) = 0;
};

#endif
