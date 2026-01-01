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

#ifndef _DEDSSTNPARAGRAPH_H_
#define _DEDSSTNPARAGRAPH_H_

#include "dedsStyledTextNode.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>


/**
 * @brief Styled Text Node Paragraph.
 * Paragraph node in a styled text object storing a list of child nodes.
 */
class dedsSTNParagraph : public dedsStyledTextNode{
public:
	typedef deTObjectReference<dedsSTNParagraph> Ref;
	
	
private:
	decObjectOrderedSet pNodes;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new container node. */
	dedsSTNParagraph();
	/** Cleans up the styled text node. */
	~dedsSTNParagraph() override;
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of child nodes. */
	int GetNodeCount() const;
	/** Retrieves the child node at the given index. */
	dedsStyledTextNode *GetNodeAt(int index) const;
	/** Retrieves the index of a child node or -1 if not found. */
	int IndexOfNode(dedsStyledTextNode *node) const;
	/** Determines if a child node exists in the container. */
	bool HasNode(dedsStyledTextNode *node) const;
	/** Adds a child node. */
	void AddNode(dedsStyledTextNode *node);
	/** Removes a child node. */
	void RemoveNode(dedsStyledTextNode *node);
	/** Removes all child nodes. */
	void RemoveAllNodes();
	/*@}*/
};

// end of include only once
#endif
