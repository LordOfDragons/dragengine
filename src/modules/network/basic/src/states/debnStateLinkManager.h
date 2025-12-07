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
#ifndef _DEBNSTATELINKMANAGER_H_
#define _DEBNSTATELINKMANAGER_H_

// predefintions
class debnStateLink;
class debnState;



/**
 * @brief State Link Manager.
 * Manages a list of established network state links.
 */
class debnStateLinkManager{
private:
	debnStateLink *pHeadLink;
	debnStateLink *pTailLink;
	int pLinkCount;
	int pNextIdentifier;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new state link manager object. */
	debnStateLinkManager();
	/** Cleans up the state link manager object. */
	~debnStateLinkManager();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of links. */
	inline int GetLinkCount() const{return pLinkCount;}
	/** Retrieves the head link or NULL. */
	inline debnStateLink *GetHeadLink() const{return pHeadLink;}
	/** Retrieves the link with the given identifier or NULL if not found. */
	debnStateLink *GetLinkWithIdentifier(int identifier) const;
	/** Retrieves the link with the given network state or NULL if not found. */
	debnStateLink *GetLinkWithNetworkState(debnState *networkState) const;
	/** Add a link. */
	void AddLink(debnStateLink *link);
	/** Removes a link. */
	void RemoveLink(debnStateLink *link);
	/** Removes all links. */
	void RemoveAllLinks();
	
	/** Retrieves a new free identifier. */
	int GetFreeIdentifier();
	/*@}*/
	
private:
};

// end of include only once
#endif
