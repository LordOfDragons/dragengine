/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// include only once
#ifndef _DEDSSTYLEDTEXT_H_
#define _DEDSSTYLEDTEXT_H_

// includes
#include <dragengine/deObject.h>

// predefinitions
class dedsStyledTextNode;



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
	dedsStyledTextNode *pRootNode;
	
public:
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
	inline dedsStyledTextNode *GetRootNode() const{ return pRootNode; }
	/** Sets the root node. */
	void SetRootNode( dedsStyledTextNode *node );
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
