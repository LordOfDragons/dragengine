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

#ifndef _IGDENATIVEBEOSTABOOK_H_
#define _IGDENATIVEBEOSTABOOK_H_

#include "beostoolkit.h"
#include "../../igdeTabBook.h"

class igdeTabBook;
class igdeGuiTheme;


/**
 * \brief BeOS native tab book.
 */
class igdeNativeBeosTabBook : public BView, public igdeTabBook::cNativeTabBook{
private:
	igdeTabBook *pOwner;
	BTabView *pTabView;
	bool pIgnoreSelect;
	
public:
	igdeNativeBeosTabBook(igdeTabBook &owner, BView *parent, const igdeGuiTheme &guitheme);
	~igdeNativeBeosTabBook() override;
	
	static igdeNativeBeosTabBook* CreateNativeWidget(igdeTabBook &owner);
	void PostCreateNativeWidget() override;
	void DestroyNativeWidget() override;
	
	void Focus() override;
	void UpdateStyle() override;
	void AddHeader(const igdeTabBook::cHeader &header) override;
	void UpdateHeader(int index, const igdeTabBook::cHeader &header) override;
	void RemoveHeader(int index) override;
	void RemoveAllHeaders() override;
	void ChangePanel(int index) override;
	void *GetNativeContainer() override;
};

typedef igdeNativeBeosTabBook igdeNativeTabBook;

#endif
