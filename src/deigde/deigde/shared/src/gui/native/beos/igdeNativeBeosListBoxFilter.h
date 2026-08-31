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

#ifndef _IGDENATIVEBEOSLISTBOXFILTER_H_
#define _IGDENATIVEBEOSLISTBOXFILTER_H_

#include "beostoolkit.h"
#include "../../igdeListBoxFilter.h"
#include "../../resources/igdeFont.h"

class igdeListBoxFilter;
class igdeGuiTheme;


/**
 * \brief BeOS native filter list box widget.
 */
class igdeNativeBeosListBoxFilter : public BView, public igdeListBoxFilter::cNativeListBoxFilter{
private:
	igdeListBoxFilter *pOwner;
	igdeFont::Ref pFont;
	BListView *pListView;
	BScrollView *pScroller;
	BTextControl *pFilterControl;
	bool pIgnoreSelectionChange;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeBeosListBoxFilter(igdeListBoxFilter &owner, BView *parent,
		const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	~igdeNativeBeosListBoxFilter() override;
	
	/** \brief Create native widget. */
	static igdeNativeBeosListBoxFilter* CreateNativeWidget(igdeListBoxFilter &owner);
	
	/** \brief Post create native widget. */
	void PostCreateNativeWidget() override;
	
	/** \brief Destroy native widget. */
	void DestroyNativeWidget() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	void BuildList() override;
	void UpdateItem(int index) override;
	void UpdateStyles() override;
	void UpdateSelection() override;
	void Focus() override;
	decPoint GetContentPosition() const override;
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
	/*@}*/
	
	
	
	/** \name BView overrides */
	/*@{*/
	void FrameResized(float width, float height) override;
	void MessageReceived(BMessage *message) override;
	/*@}*/
	
private:
	/** \brief Rebuild list applying filter. */
	void RebuildFilteredList();
};

typedef igdeNativeBeosListBoxFilter igdeNativeListBoxFilter;

#endif
