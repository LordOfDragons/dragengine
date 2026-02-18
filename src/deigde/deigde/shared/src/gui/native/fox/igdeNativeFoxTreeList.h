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

#ifndef _IGDENATIVEFOXTREELIST_H_
#define _IGDENATIVEFOXTREELIST_H_

#include "foxtoolkit.h"
#include "../../igdeTreeList.h"
#include "../../resources/igdeFont.h"


class igdeTreeList;
class igdeTreeItem;
class igdeGuiTheme;
class igdeNativeFoxResizer;


/**
 * \brief FOX toolkit Native tree list.
 */
class igdeNativeFoxTreeList : public FXVerticalFrame, public igdeTreeList::cNativeTreeList{
	FXDECLARE(igdeNativeFoxTreeList)
protected:
	   igdeNativeFoxTreeList();
	
public:
	enum eFoxIDs{
		ID_TREELIST = FXVerticalFrame::ID_LAST,
		ID_RESIZER,
		ID_LAST
	};
	
private:
	igdeTreeList *pOwner;
	igdeFont::Ref pFont;
	FXTreeList *pTreeList;
	igdeNativeFoxResizer *pResizer;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxTreeList(igdeTreeList &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	~igdeNativeFoxTreeList() override;
	
	/** \brief Create native widget. */
	static igdeNativeFoxTreeList* CreateNativeWidget(igdeTreeList &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List box. */
	inline FXTreeList *GetTreeList() const{ return pTreeList; }
	
	/** \brief Resizer or \em nullptr. */
	inline igdeNativeFoxResizer *GetResizser() const{ return pResizer; }
	
	
	
	void BuildTree() override;
	void UpdateItem(igdeTreeItem *item) override;
	void MakeItemVisible(igdeTreeItem *item) override;
	void CreateAndInsertItem(igdeTreeItem *item) override;
	void CreateAndAppendItem(igdeTreeItem *item) override;
	void RemoveItem(igdeTreeItem *item) override;
	void RemoveAllItems(igdeTreeItem *parent) override;
	void ItemMoved(igdeTreeItem *item) override;
	void SelectItem(igdeTreeItem *item) override;
	void ItemsSortedIn(igdeTreeItem *item) override;
	void Focus() override;
	void UpdateEnabled() override;
	void UpdateRows() override;
	void UpdateDescription() override;
	
	static int TreeListFlags(const igdeTreeList &owner);
	static igdeFont *TreeListFont(const igdeTreeList &owner, const igdeGuiTheme &guitheme);
	static int TreeListPadLeft(const igdeGuiTheme &guitheme);
	static int TreeListPadRight(const igdeGuiTheme &guitheme);
	static int TreeListPadTop(const igdeGuiTheme &guitheme);
	static int TreeListPadBottom(const igdeGuiTheme &guitheme);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onExpanded(FXObject*, FXSelector, void*);
	long onCollapsed(FXObject*, FXSelector, void*);
	
	long onListCommand(FXObject*, FXSelector, void*);
	long onListChanged(FXObject*, FXSelector, void*);
	long onListRightMouseDown(FXObject*, FXSelector, void*);
	long onListRightMouseUp(FXObject*, FXSelector, void*);
	long onListDoubleClicked(FXObject*, FXSelector, void*);
	
	long onResizerDrag(FXObject*, FXSelector, void*);
	/*@}*/
	
	
	
private:
	void pDropItemsNativeWidget(igdeTreeItem *parent);
};

using igdeNativeTreeList = igdeNativeFoxTreeList;

#endif
