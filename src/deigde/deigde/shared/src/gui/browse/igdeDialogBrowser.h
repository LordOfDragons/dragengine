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
#include "../event/igdeAction.h"

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
	
	igdeAction::Ref pActionPISizeSmall;
	igdeAction::Ref pActionPISizeMedium;
	igdeAction::Ref pActionPISizeLarge;
	
	igdeAction::Ref pActionPIViewList;
	igdeAction::Ref pActionPIViewPreview;
	
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
