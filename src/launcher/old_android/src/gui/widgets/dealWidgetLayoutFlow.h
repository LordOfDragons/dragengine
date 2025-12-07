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

#ifndef _DEALWIDGETLAYOUTFLOW_H_
#define _DEALWIDGETLAYOUTFLOW_H_

#include "dealWidgetLayout.h"



/**
 * \brief Flow widget layout.
 * 
 * Supports flow and box layouts along X and Y axis.
 * 
 */
class dealWidgetLayoutFlow : public dealWidgetLayout{
public:
	/** \brief Layout type. */
	enum eLayoutTypes{
		/** \brief Scale widgets relative to each other to fit into content area. */
		eltFill,
		
		/** \brief Widgets are resized to their minimum size and aligned left/top. */
		eltTop,
			
		/** \brief Widgets are resized to their minimum size and aligned left/top with last widget stretched to fill. */
		eltTopFill,
	
		/** \brief Widgets are resized to their minimum size and center aligned. */
		eltCenter,
	
		/** \brief Widgets are resized to their minimum size and center aligned and first and last widget stretched to fill. */
		eltCenterFill,
		
		/** \brief Widgets are resized to their minimum size and aligned right/bottom. */
		eltBottom,
		
		/** \brief Widgets are resized to their minimum size and aligned right/bottom and first widget stretched to fill. */
		eltBottomFill
	};
	
private:
	bool pHorizontal;
	int pGap;
	eLayoutTypes pLayoutX;
	eLayoutTypes pLayoutY;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	dealWidgetLayoutFlow(dealDisplay &display);
	
	/** \brief Create widget. */
	dealWidgetLayoutFlow(dealDisplay &display, bool horizontal, int gap,
		eLayoutTypes layoutX, eLayoutTypes layoutY);
	
	/** \brief Clean up widget. */
	virtual ~dealWidgetLayoutFlow();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Layout horizontal instead of vertical. */
	inline bool GetHorizontal() const{ return pHorizontal; }
	
	/** \brief Set if widgets are laid out horizontal instead of vertical. */
	void SetHorizontal(bool horizontal);
	
	/** \brief Gap in pixels between widgets. */
	inline int GetGap() const{ return pGap; }
	
	/** \brief Set gap in pxiels between widgets. */
	void SetGap(int gap);
	
	/** \brief Layout type in X direction. */
	inline eLayoutTypes GetLayoutX() const{ return pLayoutX; }
	
	/** \brief Set layout type in X direction. */
	void SetLayoutX(eLayoutTypes type);
	
	/** \brief Layout type in Y direction. */
	inline eLayoutTypes GetLayoutY() const{ return pLayoutY; }
	
	/** \brief Set layout type in Y direction. */
	void SetLayoutY(eLayoutTypes type);
	
	
	
	/** \brief Minimum size of widget. */
	virtual decPoint GetMinimumSize();
	
	/** \brief Layout child widgets. */
	virtual void LayoutWidgets();
	
	
	
	/** \brief Real minimum size of widget. */
	decPoint GetRealMinimumSize();
	/*@}*/
};

#endif
