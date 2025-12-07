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

#ifndef _IGDENATIVEFOXPROGRESSBAR_H_
#define _IGDENATIVEFOXPROGRESSBAR_H_

#include "foxtoolkit.h"


class igdeProgressBar;
class igdeGuiTheme;
class igdeFont;


/**
 * \brief FOX toolkit Native Text Area.
 */
class igdeNativeFoxProgressBar : public FXProgressBar{
	FXDECLARE(igdeNativeFoxProgressBar)
protected:
	   igdeNativeFoxProgressBar();
	
public:
	enum eFoxIDs{
		ID_SELF = FXProgressBar::ID_LAST,
		ID_RESIZER,
		ID_LAST
	};
	
private:
	igdeProgressBar *pOwner;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text widget. */
	igdeNativeFoxProgressBar(igdeProgressBar &owner, FXComposite *parent,
		const igdeUIFoxHelper::sChildLayoutFlags &layoutFlags, const igdeGuiTheme &guitheme);
	
	/** \brief Clean up text widget. */
	virtual ~igdeNativeFoxProgressBar();
	
	/** \brief Create native widget. */
	static igdeNativeFoxProgressBar* CreateNativeWidget(igdeProgressBar &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateRange();
	virtual void UpdateValue();
	virtual void UpdateEnabled();
	virtual void UpdateDescription();
	
	static int ProgressBarFlags(const igdeProgressBar &owner);
	static igdeFont *ProgressBarFont(const igdeProgressBar &owner, const igdeGuiTheme &guitheme);
	static int ProgressBarPadLeft(const igdeGuiTheme &guitheme);
	static int ProgressBarPadRight(const igdeGuiTheme &guitheme);
	static int ProgressBarPadTop(const igdeGuiTheme &guitheme);
	static int ProgressBarPadBottom(const igdeGuiTheme &guitheme);
	/*@}*/
};

typedef igdeNativeFoxProgressBar igdeNativeProgressBar;

#endif
