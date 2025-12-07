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

#ifndef _SEPROPERTYNODESELECTION_H_
#define _SEPROPERTYNODESELECTION_H_

#include "sePropertyNodeList.h"

class seProperty;



/**
 * \brief Texture property selected nodes.
 */
class sePropertyNodeSelection{
private:
	seProperty &pProperty;
	sePropertyNodeList pSelection;
	sePropertyNode *pActive;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create selection. */
	sePropertyNodeSelection(seProperty &property);
	
	/** \brief Clean up selection. */
	~sePropertyNodeSelection();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List of selected nodes. */
	inline const sePropertyNodeList &GetSelected() const{ return pSelection; }
	
	/** \brief Add node to selection if absent. */
	void Add(sePropertyNode *node);
	
	/** \brief Remove node from selection if present. */
	void Remove(sePropertyNode *node);
	
	/** \brief Remove all nodes from selection. */
	void RemoveAll();
	
	/** \brief Set list of selected nodes. */
	void SetSelected(const sePropertyNodeList &list);
	
	
	
	/** \brief Active node or \em NULL if absent. */
	inline sePropertyNode *GetActive() const{ return pActive; }
	
	/** \brief Active node is present. */
	bool HasActive() const;
	
	/** \brief Set active node or \em NULL if absent. */
	void SetActive(sePropertyNode *navspaces);
	
	
	
	/** \brief Notify active node changed. */
	void NotifyActiveNodeChanged() const;
	
	/** \brief Notify node selection changed. */
	void NotifyNodeSelectionChanged() const;
};

#endif
