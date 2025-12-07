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

#ifndef _SEVCIDRAGNODE_H_
#define _SEVCIDRAGNODE_H_

#include <deigde/gui/event/igdeMouseDragListener.h>
#include <deigde/undo/igdeUndo.h>

class seViewConstructedView;
class deCanvas;



/**
 * \brief Drag node interaction.
 */
class seVCIDragNode : public igdeMouseDragListener{
protected:
	enum eDragAction{
		edaNone,
		edaMoveNode,
		edaResizeTopLeft,
		edaResizeTopRight,
		edaResizeBottomLeft,
		edaResizeBottomRight,
		edaResizeTop,
		edaResizeBottom,
		edaResizeLeft,
		edaResizeRight,
		edaRotateTopLeft,
		edaRotateTopRight,
		edaRotateBottomLeft,
		edaRotateBottomRight,
		edaShearTop,
		edaShearBottom,
		edaShearLeft,
		edaShearRight
	};
	
	
	
private:
	seViewConstructedView &pView;
	eDragAction pDragAction;
	decTexMatrix2 pDragMatrix;
	igdeUndo::Ref pUndo;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seVCIDragNode(seViewConstructedView &view);
	
	/** \brief Clean up listener. */
	virtual ~seVCIDragNode();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Dragging begins.
	 * \returns true to start drag operation or false to abort without calling OnDragFinish.
	 */
	virtual bool OnDragBegin();
	
	/**
	 * \brief Update dragging.
	 */
	virtual void OnDragUpdate();
	
	/**
	 * \brief Dragging ends.
	 */
	virtual void OnDragFinish(bool cancelled);
	/*@}*/
	
	
	
protected:
	bool pInsideMarker(const deCanvas &canvas, const decPoint &position) const;
	void pMove();
	void pRotate();
	void pResize();
	void pShear();
};

#endif
