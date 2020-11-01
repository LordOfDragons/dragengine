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

#ifndef _MEWPBROWSER_H_
#define _MEWPBROWSER_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeIconListBoxReference.h>
#include <deigde/gui/igdeTextAreaReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeTreeListReference.h>
#include <deigde/gui/igdeSwitcherReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/layout/igdeContainerBox.h>

class meWorld;
class meWPBrowserListener;
class meWindowProperties;

class igdeGDCategory;
class igdeGDClass;
class igdeGDSkin;
class igdeGDSky;
class igdeListItem;
class igdeTreeItem;



/**
 * \brief Browser panel.
 */
class meWPBrowser : public igdeContainerBox{
public:
	enum ePreviewSize{
		epsLarge,
		epsMedium,
		epsSmall
	};
	
	enum ePreviewItemType{
		epitObjectClass,
		epitSkin,
		epitSky
	};
	
	enum eSelectionMode{
		esmCategory,
		esmFilter
	};
	
	enum eViewModes{
		evmList,
		evmPreview
	};
	
	
	
private:
	meWindowProperties &pWindowProperties;
	meWPBrowserListener *pListener;
	
	meWorld *pWorld;
	
	
	igdeActionReference pActionSetClass;
	igdeActionReference pActionSetSkin;
	igdeActionReference pActionSetDecal;
	igdeActionReference pActionSetSky;
	
	igdeActionReference pActionPISizeSmall;
	igdeActionReference pActionPISizeMedium;
	igdeActionReference pActionPISizeLarge;
	
	igdeActionReference pActionPIViewList;
	igdeActionReference pActionPIViewPreview;
	
	igdeActionReference pActionPIRebuild;
	
	
	igdeComboBoxReference pCBTypes;
	igdeButtonReference pBtnSelByCat;
	igdeButtonReference pBtnSelByFilter;
	
	igdeSwitcherReference pSwitcherSelBy;
	igdeTreeListReference pTreeCategories;
	igdeIconListBoxReference pListItems;
	igdeTextFieldReference pEditFilter;
	
	igdeTextAreaReference pEditInfos;
	
	ePreviewSize pPreviewSize;
	eSelectionMode pSelectionMode;
	eViewModes pViewMode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPBrowser( meWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~meWPBrowser();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline meWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief World. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	
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
	
	/** \brief Game project changed. */
	void OnGameProjectChanged();
	
	/** \brief Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	/** \brief Preview item type. */
	ePreviewItemType GetPreviewItemType() const;
	
	/** \brief Set preview item type. */
	void SetPreviewItemType( ePreviewItemType type );
	
	/** \brief Selected category. */
	igdeGDCategory *GetSelectedCategory() const;
	
	/** \brief Select category. */
	void SelectCategory( igdeGDCategory *category );
	
	/** \brief Selection mode. */
	inline eSelectionMode GetSelectionMode() const{ return pSelectionMode; }
	
	/** \brief Set selection mode. */
	void SetSelectionMode( eSelectionMode mode );
	
	/** \brief Selected list item. */
	igdeListItem *GetSelectedListItem() const;
	
	/** \brief Selected object class. */
	igdeGDClass *GetSelectedObjectClass() const;
	
	/** \brief Selected skin. */
	igdeGDSkin *GetSelectedSkin() const;
	
	/** \brief Selected sky. */
	igdeGDSky *GetSelectedSky() const;
	
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
	
	
	
	/** \brief Select object class. */
	void SelectObjectClass( igdeGDClass *gdclass );
	
	/** \brief Select skin. */
	void SelectSkin( igdeGDSkin *gdskin );
	
	/** \brief Select sky. */
	void SelectSky( igdeGDSky *gdsky );
	
	
	inline igdeAction *GetActionSetClass() const{ return pActionSetClass; }
	inline igdeAction *GetActionSetSkin() const{ return pActionSetSkin; }
	inline igdeAction *GetActionSetDecal() const{ return pActionSetDecal; }
	inline igdeAction *GetActionSetSky() const{ return pActionSetSky; }
	
	inline igdeAction *GetActionPISizeSmall() const{ return pActionPISizeSmall; }
	inline igdeAction *GetActionPISizeMedium() const{ return pActionPISizeMedium; }
	inline igdeAction *GetActionPISizeLarge() const{ return pActionPISizeLarge; }
	
	inline igdeAction *GetActionPIViewList() const{ return pActionPIViewList; }
	inline igdeAction *GetActionPIViewPreview() const{ return pActionPIViewPreview; }
	
	inline igdeAction *GetActionPIRebuild() const{ return pActionPIRebuild; }
	/*@}*/
	
	
	
private:
	void pSetSkin();
	void pSetDecal();
};

#endif
