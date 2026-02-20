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

#ifndef _IGDENATIVEFOXCOMBOBOX_H_
#define _IGDENATIVEFOXCOMBOBOX_H_

#include "foxtoolkit.h"
#include "../../igdeComboBox.h"
#include "../../resources/igdeFont.h"

class igdeComboBox;
class igdeEnvironment;
class igdeGuiTheme;
class igdeListItem;
class igdeWidget;


/**
 * FOX Native comboBox.
 */
class igdeNativeFoxComboBox : public FXComboBox, public igdeComboBox::cNativeComboBox{
	FXDECLARE(igdeNativeFoxComboBox)
	
protected:
	igdeNativeFoxComboBox();
	
public:
	enum eFoxIDs{
		ID_SELF = FXComboBox::ID_LAST,
	};
	
private:
	igdeComboBox *pOwner;
	igdeFont::Ref pFont;
	FXColor pOrgBackColor;
	FXColor pInvalidBackColor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxComboBox(igdeComboBox &owner, FXComposite *parent, int layoutFlags,
		const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	~igdeNativeFoxComboBox() override;
	
	/** \brief Create native widget. */
	static igdeNativeFoxComboBox* CreateNativeWidget(igdeComboBox &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	void BuildList() override;
	void UpdateItem(int index) override;
	void SyncSelection(bool changing) override;
	void OnInvalidValueChanged() override;
	void UpdateText() override;
	void InsertItem(int index, const igdeListItem &item) override;
	void RemoveItem(int index) override;
	void RemoveAllItems() override;
	void MoveItem(int fromIndex, int toIndex) override;
	void Focus() override;
	void UpdateRowCount() override;
	void UpdateEnabled() override;
	void UpdateEditable() override;
	void UpdateDescription() override;
	
	static int ComboBoxFlags(const igdeComboBox &owner);
	static igdeFont *ComboBoxFont(const igdeComboBox &owner, const igdeGuiTheme &guitheme);
	static int ComboBoxPadLeft(const igdeGuiTheme &guitheme);
	static int ComboBoxPadRight(const igdeGuiTheme &guitheme);
	static int ComboBoxPadTop(const igdeGuiTheme &guitheme);
	static int ComboBoxPadBottom(const igdeGuiTheme &guitheme);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCommand(FXObject*, FXSelector, void*);
	long onChanged(FXObject*, FXSelector, void*);
	/*@}*/
};

using igdeNativeComboBox = igdeNativeFoxComboBox;

#endif
