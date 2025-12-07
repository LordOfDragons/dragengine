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
#ifndef _DEBNSTATELINKLIST_H_
#define _DEBNSTATELINKLIST_H_

// predefintions
class debnStateLink;



/**
 * @brief State Link List.
 * Stores a list of state link pointers. The pointers are not held. The
 * list is organized as a fill up and empty list. State link pointers
 * can be added quickly and processed in linear order using indices.
 * Furthermore the list can be cleared. No individual removing of state
 * link pointers is possible.
 */
class debnStateLinkList{
private:
	debnStateLink **pLinks;
	int pLinkCount;
	int pLinkSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new state link list object. */
	debnStateLinkList();
	/** Cleans up the state link list object. */
	~debnStateLinkList();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of links. */
	inline int GetLinkCount() const{ return pLinkCount; }
	/** Retrieves the link at the given index. */
	debnStateLink *GetLinkAt(int index) const;
	/** Retrieves the index of the link or -1 if not found. */
	int IndexOfLink(debnStateLink *link) const;
	/** Determines if the link exists in the list. */
	bool HasLink(debnStateLink *link) const;
	/** Adds a link. */
	void AddLink(debnStateLink *link);
	/** Removes a link. */
	void RemoveLink(debnStateLink *link);
	/** Removes a link if existing in the list. */
	void RemoveLinkIfExisting(debnStateLink *link);
	/** Removes all links. */
	void RemoveAllLinks();
	/*@}*/
};

// end of include only once
#endif
