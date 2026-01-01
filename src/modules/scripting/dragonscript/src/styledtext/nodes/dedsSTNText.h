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

#ifndef _DEDSSTNTEXT_H_
#define _DEDSSTNTEXT_H_

#include "dedsStyledTextNode.h"

#include <dragengine/common/string/decString.h>


/**
 * Text node in a styled text object storing a single text string.
 * Line breaks are honored.
 */
class dedsSTNText : public dedsStyledTextNode{
public:
	typedef deTObjectReference<dedsSTNText> Ref;
	
	
private:
	decString pText;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create text node. */
	dedsSTNText();
	
	/** Create text node with text. */
	dedsSTNText(const char *text);
	
protected:
	/** Cleans up the text node. */
	~dedsSTNText() override;
	/*@}*/
	


public:
	/** \name Management */
	/*@{*/
	/** Text. */
	inline const decString &GetText() const{ return pText; }

	/** Set text. */
	void SetText(const char *text);
	/*@}*/
};

#endif
