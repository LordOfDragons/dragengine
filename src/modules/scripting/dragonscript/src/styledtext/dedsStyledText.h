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
#ifndef _DEDSSTYLEDTEXT_H_
#define _DEDSSTYLEDTEXT_H_

// includes
#include <dragengine/deObject.h>
#include "dedsStyledTextNode.h"

// predefinitions



/**
 * @brief Styled Text.
 * Provides support for styled text. The text is defined similar to HTML
 * using a box model. Instead of using HTML though a small subset thereof
 * is used in a slightly changed way as the comlexity is not needed. The
 * root of the text is one node which is usually a container node but can
 * be anything. The nodes build up as a tree structure of child nodes.
 * Styles are defined globally and applied to nodes. In contrary to HTML
 * styles can not be inlined to remove complexity. This class is used
 * only to define the text. To render a styled text use the
 * dedsStyledTextRenderer class.
 */
class dedsStyledText : public deObject{
private:
	dedsStyledTextNode::Ref pRootNode;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<dedsStyledText> Ref;


	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new styled text. */
	dedsStyledText();
	/** Cleans up the styled text. */
	virtual ~dedsStyledText();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the root node. */
	inline const dedsStyledTextNode::Ref &GetRootNode() const{ return pRootNode; }
	/** Sets the root node. */
	void SetRootNode(dedsStyledTextNode *node);
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
