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

#ifndef _DEGLSHAREDICON_H_
#define _DEGLSHAREDICON_H_

#include "foxtoolkit.h"

#include <dragengine/deObject.h>


/**
 * FOX icons are not reference counted. This makes sharing them problematic.
 */
class deglSharedIcon : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<deglSharedIcon> Ref;
	
	
	
private:
	FXIcon *pIcon;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared icon. */
	deglSharedIcon( FXIcon *icon );
	
protected:
	/** Clean up shared icon. */
	virtual ~deglSharedIcon();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Icon. */
	inline FXIcon *GetIcon() const{ return pIcon; }
	/*@}*/
};

#endif
