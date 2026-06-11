/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDENATIVEFOXLISTBOXFILTER_H_
#define _IGDENATIVEFOXLISTBOXFILTER_H_

#include "foxtoolkit.h"
#include "igdeNativeFoxResizer.h"
#include "../../igdeListBoxFilter.h"
#include "../../resources/igdeFont.h"

class igdeListBoxFilter;
class igdeEnvironment;
class igdeGuiTheme;
class igdeListItem;
class igdeWidget;


/**
 * FOX Native filter list box.
 */
class igdeNativeFoxListBoxFilter : public FXVerticalFrame, public igdeListBoxFilter::cNativeListBoxFilter{
	FXDECLARE(igdeNativeFoxListBoxFilter)
	
protected:
	igdeNativeFoxListBoxFilter();
	
public:
	enum eFoxIDs{
		ID_LISTBOX = FXVerticalFrame::ID_LAST,
		ID_EDIT_FILTER,
		ID_RESIZER,
		ID_LAST
	};
	
private:
	igdeListBoxFilter *pOwner;
	igdeFont::Ref pFont;
	FXList *pListBox;
	FXLabel *pLabelFilter;
	FXTextField *pEditFilter;
	igdeNativeFoxResizer *pResizer;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxListBoxFilter(igdeListBoxFilter &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	~igdeNativeFoxListBoxFilter() override;
	
	/** \brief Create native widget. */
	static igdeNativeFoxListBoxFilter* CreateNativeWidget(igdeListBoxFilter &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List box. */
	inline FXList *GetListBox() const{ return pListBox; }
	
	/** \brief Resizer or \em nullptr. */
	inline igdeNativeFoxResizer *GetResizser() const{ return pResizer; }
	
	
	
	void BuildList() override;
	void UpdateItem(int index) override;
	void UpdateStyles() override;
	void UpdateSelection() override;
	void Focus() override;
	virtual decPoint GetContentPosition() const;
	void SetContentPosition(const decPoint &position) override;
	void MakeItemVisible(int index) override;
	void InsertItem(int index) override;
	void RemoveItem(int index) override;
	void RemoveAllItems() override;
	void MoveItem(int fromIndex, int toIndex) override;
	void UpdateEnabled() override;
	void UpdateRowCount() override;
	void UpdateDescription() override;
	void UpdateFilterString() override;
	
	static int ListBoxFlags(const igdeListBoxFilter &owner);
	static igdeFont *ListBoxFont(const igdeListBoxFilter &owner, const igdeGuiTheme &guitheme);
	static int ListBoxPadLeft(const igdeGuiTheme &guitheme);
	static int ListBoxPadRight(const igdeGuiTheme &guitheme);
	static int ListBoxPadTop(const igdeGuiTheme &guitheme);
	static int ListBoxPadBottom(const igdeGuiTheme &guitheme);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onListCommand(FXObject*, FXSelector, void*);
	long onListChanged(FXObject*, FXSelector, void*);
	long onListSelected(FXObject*, FXSelector, void*);
	long onListDeselected(FXObject*, FXSelector, void*);
	long onListRightMouseDown(FXObject*, FXSelector, void*);
	long onListRightMouseUp(FXObject*, FXSelector, void*);
	long onListDoubleClicked(FXObject*, FXSelector, void*);
	
	long onEditFilterChanged(FXObject*, FXSelector, void*);
	
	long onResizerDrag(FXObject*, FXSelector, void*);
	/*@}*/
};

using igdeNativeListBoxFilter = igdeNativeFoxListBoxFilter;

#endif
