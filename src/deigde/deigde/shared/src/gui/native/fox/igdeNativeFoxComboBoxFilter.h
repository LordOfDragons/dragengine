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

#ifndef _IGDENATIVEFOXCOMBOBOXFILTER_H_
#define _IGDENATIVEFOXCOMBOBOXFILTER_H_

#include "foxtoolkit.h"
#include "../../resources/igdeFont.h"

class igdeComboBoxFilter;
class igdeEnvironment;
class igdeGuiTheme;
class igdeListItem;
class igdeWidget;


/**
 * FOX Native filter combo box.
 */
class igdeNativeFoxComboBoxFilter : public FXComboBox{
	FXDECLARE(igdeNativeFoxComboBoxFilter)
	
protected:
	igdeNativeFoxComboBoxFilter();
	
public:
	enum eFoxIDs{
		ID_SELF = FXComboBox::ID_LAST,
		ID_EDIT_FILTER
	};
	
private:
	igdeComboBoxFilter *pOwner;
	igdeFont::Ref pFont;
	FXLabel *pLabelFilter;
	FXTextField *pEditFilter;
	FXColor pOrgBackColor;
	FXColor pInvalidBackColor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxComboBoxFilter(igdeComboBoxFilter &owner, FXComposite *parent, int layoutFlags,
		const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxComboBoxFilter();
	
	/** \brief Create native widget. */
	static igdeNativeFoxComboBoxFilter* CreateNativeWidget(igdeComboBoxFilter &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void BuildList();
	virtual void UpdateItem(int index);
	virtual void UpdateFilterString();
	virtual void SyncSelection(bool changing);
	virtual void OnInvalidValueChanged();
	virtual void UpdateText();
	virtual void InsertItem(int index, const igdeListItem &item);
	virtual void RemoveItem(int index);
	virtual void RemoveAllItems();
	virtual void MoveItem(int fromIndex, int toIndex);
	virtual void Focus();
	virtual void UpdateRowCount();
	virtual void UpdateEnabled();
	virtual void UpdateEditable();
	virtual void UpdateDescription();
	
	static int ComboBoxFlags(const igdeComboBoxFilter &owner);
	static igdeFont *ComboBoxFont(const igdeComboBoxFilter &owner, const igdeGuiTheme &guitheme);
	static int ComboBoxPadLeft(const igdeGuiTheme &guitheme);
	static int ComboBoxPadRight(const igdeGuiTheme &guitheme);
	static int ComboBoxPadTop(const igdeGuiTheme &guitheme);
	static int ComboBoxPadBottom(const igdeGuiTheme &guitheme);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCommand(FXObject*, FXSelector, void*);
	long onChanged(FXObject*, FXSelector, void*);
	long onEditFilterChanged(FXObject*, FXSelector, void*);
	/*@}*/
};

typedef igdeNativeFoxComboBoxFilter igdeNativeComboBoxFilter;

#endif
