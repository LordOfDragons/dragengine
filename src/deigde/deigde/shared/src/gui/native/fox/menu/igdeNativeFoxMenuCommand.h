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

#ifndef _IGDENATIVEFOXMENUCOMMAND_H_
#define _IGDENATIVEFOXMENUCOMMAND_H_

#include "../foxtoolkit.h"

class igdeMenuCommand;


/**
 * FOX native widget.
 */
class igdeNativeFoxMenuCommand : public FXMenuCommand{
	FXDECLARE(igdeNativeFoxMenuCommand)
	
protected:
	igdeNativeFoxMenuCommand();
	
public:
	enum eFoxIDs{
		ID_SELF = FXMenuCommand::ID_LAST,
	};
	
private:
	igdeMenuCommand *pOwner;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxMenuCommand(igdeMenuCommand &owner, FXComposite *parent);
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxMenuCommand();
	
	/** \brief Create native widget. */
	static igdeNativeFoxMenuCommand* CreateNativeWidget(igdeMenuCommand &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateText();
	virtual void UpdateDescription();
	virtual void UpdateHotKey();
	virtual void UpdateIcon();
	virtual void UpdateEnabled();
	
	static FXString BuildConstrText(igdeMenuCommand &owner);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCommand(FXObject*, FXSelector, void*);
	long onUpdate(FXObject*, FXSelector, void*);
	/*@}*/
};

typedef igdeNativeFoxMenuCommand igdeNativeMenuCommand;

#endif
