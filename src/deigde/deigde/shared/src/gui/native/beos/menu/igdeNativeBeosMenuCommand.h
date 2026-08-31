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

#ifndef _IGDENATIVEBEOSMENUCOMMAND_H_
#define _IGDENATIVEBEOSMENUCOMMAND_H_

#include <interface/MenuItem.h>
#include "../../menu/igdeMenuCommand.h"

class igdeMenuCommand;


/**
 * \brief BeOS native command menu item.
 */
class igdeNativeBeosMenuCommand : public BMenuItem, public igdeMenuCommand::cNativeMenuCommand{
protected:
	igdeMenuCommand *pOwner;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native menu item. */
	igdeNativeBeosMenuCommand(igdeMenuCommand &owner, BMenu *parent);
	
	/** \brief Clean up native menu item. */
	~igdeNativeBeosMenuCommand() override;
	
	/** \brief Create native widget. */
	static igdeNativeBeosMenuCommand* CreateNativeWidget(igdeMenuCommand &owner);
	
	/** \brief Post create native widget. */
	void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name cNativeMenuCommand interface */
	/*@{*/
	void UpdateText() override;
	void UpdateDescription() override;
	void UpdateHotKey() override;
	void UpdateIcon() override;
	void UpdateEnabled() override;
	/*@}*/
	
	
	
	/** \name BMenuItem overrides */
	/*@{*/
	void Invoked() override;
	/*@}*/
};

typedef igdeNativeBeosMenuCommand igdeNativeMenuCommand;

#endif
