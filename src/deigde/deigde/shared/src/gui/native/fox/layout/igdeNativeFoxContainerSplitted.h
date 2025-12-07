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

#ifndef _IGDENATIVEFOXCONTAINERSPLITTED_H_
#define _IGDENATIVEFOXCONTAINERSPLITTED_H_

#include "../foxtoolkit.h"

class igdeContainerSplitted;


/**
 * FOX Native button.
 */
class igdeNativeFoxContainerSplitted : public FXSplitter{
	FXDECLARE(igdeNativeFoxContainerSplitted)
	
protected:
	igdeNativeFoxContainerSplitted();
	
public:
	enum eFoxIDs{
		ID_SELF = FXButton::ID_LAST,
	};
	
private:
	igdeContainerSplitted *pOwner;
	bool pNoUpdate;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeFoxContainerSplitted(igdeContainerSplitted &owner, FXComposite *parent, int layoutFlags);
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeFoxContainerSplitted();
	
	/** \brief Create native widget. */
	static igdeNativeFoxContainerSplitted* CreateNativeWidget(igdeContainerSplitted &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	virtual void UpdateSplitValue();
	
	static int SplitterFlags(const igdeContainerSplitted &owner);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onResize(FXObject*, FXSelector, void*);
	long onCommand(FXObject*, FXSelector, void*);
	long onChildLayoutFlags(FXObject*, FXSelector, void*);
	/*@}*/
};

typedef igdeNativeFoxContainerSplitted igdeNativeContainerSplitted;

#endif
