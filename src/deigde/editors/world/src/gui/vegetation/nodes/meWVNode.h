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

#ifndef _MEWVNODE_H_
#define _MEWVNODE_H_

#include <deigde/gui/nodeview/igdeNVNode.h>
#include <deigde/undo/igdeUndo.h>

class meWVNodeSlot;
class meHTVRule;
class meWindowVegetation;



/**
 * \brief Vegetation Editing Window Node.
 */
class meWVNode : public igdeNVNode{
private:
	meWindowVegetation &pWindowVegetation;
	meHTVRule *pRule;
	
	igdeUndo::Ref pUndoMove;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create node. */
	meWVNode(meWindowVegetation &windowVegetation, meHTVRule *rule);
	
protected:
	/** \brief Clean up object. */
	virtual ~meWVNode();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Vegetation editor window. */
	inline meWindowVegetation &GetWindowVegetation() const{return pWindowVegetation;}
	
	/** \brief Rule. */
	inline meHTVRule *GetRule() const{return pRule;}
	
	/** \brief Update node. */
	virtual void Update();
	
	/** \brief Update class lists. */
	virtual void UpdateClassLists();
	
	/** \brief Can delete node. */
	virtual bool CanDelete() const;
	
	/** \brief Can duplicate node. */
	virtual bool CanDuplicate() const;
	
	/** \brief Set node position. */
	void UpdatePositionFromRule();
	/*@}*/
};

#endif
