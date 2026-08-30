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

#ifndef _IGDENATIVEBEOSBUTTON_H_
#define _IGDENATIVEBEOSBUTTON_H_

#include "beostoolkit.h"
#include "../../igdeButton.h"
#include "../../resources/igdeFont.h"

class igdeButton;
class igdeGuiTheme;


/**
 * BeOS Native button.
 */
class igdeNativeBeosButton : public BButton, public igdeButton::cNativeButton{
private:
	igdeButton *pOwner;
	igdeFont::Ref pFont;

public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeBeosButton(igdeButton &owner, BView *parent,
		const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	~igdeNativeBeosButton() override;
	
	/** \brief Create native widget. */
	static igdeNativeBeosButton* CreateNativeWidget(igdeButton &owner);
	
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
	void UpdateDescription() override;
	void UpdateIcon() override;
	void UpdateEnabled() override;
	
	void AttachedToWindow() override;
	void MessageReceived(BMessage *msg) override;
	bool IsFocusable() const override;
	/*@}*/
};

typedef igdeNativeBeosButton igdeNativeButton;

#endif
