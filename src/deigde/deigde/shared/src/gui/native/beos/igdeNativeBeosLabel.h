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

#ifndef _IGDENATIVEBEOSLABEL_H_
#define _IGDENATIVEBEOSLABEL_H_

#include "beostoolkit.h"
#include "../../igdeLabel.h"
#include "../../resources/igdeFont.h"

class igdeLabel;
class igdeGuiTheme;


/**
 * BeOS label.
 */
class igdeNativeBeosLabel : public BStringView, public igdeLabel::cNativeLabel{
private:
	igdeLabel *pOwner;
	igdeFont::Ref pFont;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeBeosLabel(igdeLabel &owner, BView *parent,
		const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	~igdeNativeBeosLabel() override;
	
	/** \brief Create native widget. */
	static igdeNativeBeosLabel* CreateNativeWidget(igdeLabel &owner);
	
	/** \brief Post create native widget. */
	void PostCreateNativeWidget() override;
	
	/** \brief Destroy native widget. */
	void DestroyNativeWidget() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	void Focus() override;
	void UpdateStyle() override;
	void UpdateText() override;
	void UpdateAlignment() override;
	void UpdateDescription() override;
	void UpdateIcon() override;
	/*@}*/
};

typedef igdeNativeBeosLabel igdeNativeLabel;

#endif
