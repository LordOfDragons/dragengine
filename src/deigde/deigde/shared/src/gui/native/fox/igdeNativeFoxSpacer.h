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

#ifndef _IGDENATIVEFOXSPACER_H_
#define _IGDENATIVEFOXSPACER_H_

#include "foxtoolkit.h"

class igdeSpacer;


/**
 * \brief FOX toolkit Native Spacer.
 */
class igdeNativeFoxSpacer : public FXFrame{
	FXDECLARE( igdeNativeFoxSpacer )
protected:
	igdeNativeFoxSpacer();
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create container. */
	igdeNativeFoxSpacer( igdeSpacer &owner, FXComposite *parent, int childFlags );
	
	/** \brief Clean up container. */
	virtual ~igdeNativeFoxSpacer();
	
	/** \brief Create native widget. */
	static igdeNativeFoxSpacer* CreateNativeWidget( igdeSpacer &owner );
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Set size. */
	virtual void SetSize( int width, int height );
	
	virtual FXint getDefaultWidth();
	virtual FXint getDefaultHeight();
	
	/** \brief Process layout flags. */
	static int LayoutFlags( int childFlags );
	/*@}*/
	
private:
	igdeSpacer *pOwner;
	int pWidth;
	int pHeight;
};

typedef igdeNativeFoxSpacer igdeNativeSpacer;

#endif
