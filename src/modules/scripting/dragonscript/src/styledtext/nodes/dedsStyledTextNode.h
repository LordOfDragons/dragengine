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

// include only once
#ifndef _DEDSSTYLEDTEXTNODE_H_
#define _DEDSSTYLEDTEXTNODE_H_

// includes
#include <dragengine/deObject.h>

// predefinitions



/**
 * @brief Styled Text Node.
 * Node in a styled text. No parent node reference is held since
 * this class only stores the definition and working classes
 * derived thereof can find the parent using visiting.
 */
class dedsStyledTextNode : public deObject{
private:
	void *pStyle;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dedsStyledTextNode> Ref;


	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new styled text node. */
	dedsStyledTextNode();

protected:
	/** Cleans up the styled text node. */
	virtual ~dedsStyledTextNode();
	/*@}*/
	
public:
	/** @name Management */
	/*@{*/
	/** Retrieves the style or NULL to use the parent node style. */
	inline void *GetStyle() const{ return pStyle; }
	/** Sets the style or NULL to use the parent node style. */
	void SetStyle(void *style);
	/*@}*/
};

// end of include only once
#endif
