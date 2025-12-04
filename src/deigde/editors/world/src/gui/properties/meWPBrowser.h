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
#include <deigde/gui/event/igdeAction::Ref.h>
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
	
	
	igdeAction::Ref pActionSetClass;
	igdeAction::Ref pActionSetSkin;
	igdeAction::Ref pActionSetDecal;
	igdeAction::Ref pActionSetSky;
	
	igdeAction::Ref pActionPISizeSmall;
	igdeAction::Ref pActionPISizeMedium;
	igdeAction::Ref pActionPISizeLarge;
	
	igdeAction::Ref pActionPIViewList;
	igdeAction::Ref pActionPIViewPreview;
	
	igdeAction::Ref pActionPIRebuild;
	
	
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
