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

#ifndef _IGDENATIVEBEOSLISTBOX_H_
#define _IGDENATIVEBEOSLISTBOX_H_

#include "beostoolkit.h"
#include "../../igdeListBox.h"
#include "../../resources/igdeFont.h"

class igdeListBox;
class igdeGuiTheme;


/**
 * \brief Native listbox implementation for BeOS.
 */
class igdeNativeBeosListBox : public BView, public igdeListBox::cNativeListBox{
private:
	igdeListBox *pOwner;
	igdeFont::Ref pFont;
	BListView *pListView;
	BScrollView *pScroller;
	bool pIgnoreSelectionChange;
	
public:
	igdeNativeBeosListBox(igdeListBox &owner, BView *parent, const igdeGuiTheme &guitheme);
	~igdeNativeBeosListBox() override;
	
	static igdeNativeBeosListBox* CreateNativeWidget(igdeListBox &owner);
	void PostCreateNativeWidget() override;
	void DestroyNativeWidget() override;
	
	void BuildList() override;
	void UpdateItem(int index) override;
	void UpdateStyles() override;
	void UpdateSelection() override;
	void Focus() override;
	decPoint GetContentPosition() const override;
	void SetContentPosition(const decPoint &position) override;
	void MakeItemVisible(int index) override;
	void UpdateStyle() override;
	void InsertItem(int index) override;
	void RemoveItem(int index) override;
	void RemoveAllItems() override;
	void MoveItem(int fromIndex, int toIndex) override;
	void UpdateEnabled() override;
	void UpdateRowCount() override;
	void UpdateDescription() override;
};

typedef igdeNativeBeosListBox igdeNativeListBox;

#endif
