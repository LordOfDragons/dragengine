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

#ifndef _IGDENATIVEBEOSCOMBOBOXFILTER_H_
#define _IGDENATIVEBEOSCOMBOBOXFILTER_H_

#include "beostoolkit.h"
#include "../../igdeComboBoxFilter.h"
#include "../../resources/igdeFont.h"

class igdeComboBoxFilter;
class igdeGuiTheme;


/**
 * \brief BeOS native filter combo box widget.
 */
class igdeNativeBeosComboBoxFilter : public BView, public igdeComboBoxFilter::cNativeComboBoxFilter{
private:
	igdeComboBoxFilter *pOwner;
	igdeFont::Ref pFont;
	BTextControl *pTextField;
	BMenuField *pMenuField;
	BPopUpMenu *pMenu;
	bool pIgnoreSelectionChange;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeBeosComboBoxFilter(igdeComboBoxFilter &owner, BView *parent,
		const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	~igdeNativeBeosComboBoxFilter() override;
	
	/** \brief Create native widget. */
	static igdeNativeBeosComboBoxFilter* CreateNativeWidget(igdeComboBoxFilter &owner);
	
	/** \brief Post create native widget. */
	void PostCreateNativeWidget() override;
	
	/** \brief Destroy native widget. */
	void DestroyNativeWidget() override;
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
	void UpdateFilterString() override;
	/*@}*/
	
	
	
	/** \name BView overrides */
	/*@{*/
	void MessageReceived(BMessage *message) override;
	/*@}*/
	
private:
	/** \brief Rebuild menu applying filter. */
	void RebuildFilteredMenu();
};

typedef igdeNativeBeosComboBoxFilter igdeNativeComboBoxFilter;

#endif
