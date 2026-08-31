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

#ifndef _IGDEBEOSFONT_H_
#define _IGDEBEOSFONT_H_

#include "../../../resources/igdeFont.h"

#include <dragengine/common/math/decMath.h>

class igdeFont;
class deFont;
class BFont;


/**
 * BeOS native font resource.
 */
class igdeNativeBeosFont : public igdeFont::cNativeFont{
private:
	igdeFont *pOwner;
	BFont *pFont;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native font. */
	igdeNativeBeosFont();
	
	/** \brief Create native font with owner. */
	igdeNativeBeosFont(igdeFont &owner);
	
	/** \brief Clean up native font. */
	virtual ~igdeNativeBeosFont();
	
	/** \brief Create native font. */
	static igdeNativeBeosFont* CreateNativeFont(igdeFont &owner);
	
	/** \brief Destroy native font. */
	void DestroyNativeFont() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create engine font. */
	deFont::Ref CreateEngineFont() override;
	
	/** \brief Text size. */
	decPoint TextSize(const char *text) const override;
	/*@}*/
};

typedef igdeNativeBeosFont igdeNativeFont;

#endif
