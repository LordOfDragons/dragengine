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

#ifndef _CERULEOFTHIRDS_H_
#define _CERULEOFTHIRDS_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/canvas/deCanvasView.h>

class igdeEnvironment;


/**
 * \brief Rule of thirds aid canvas.
 */
class ceCanvasRuleOfThirdsAid{
private:
	igdeEnvironment &pEnvironment;
	deCanvasView::Ref pCanvasView;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas. */
	ceCanvasRuleOfThirdsAid(igdeEnvironment &environment);
	
	/** \brief Clean up canvas. */
	~ceCanvasRuleOfThirdsAid();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Canvas to add to render window. */
	inline deCanvasView *GetCanvasView() const{return pCanvasView;}
	
	
	
	/** \brief Update canvas size due to render window changing size. */
	void Resize();
	
	/** \brief Set visible. */
	void SetVisible(bool visible);
	/*@}*/
	
	
	
private:
	void pCreateCanvas();
	
	void pCreateAid();
	void pAddLine(int x1, int y1, int x2, int y2);
	void pAddFocus(int x, int y);
	
	void pAddRect(int x1, int y1, int x2, int y2, const decColor &color, float transparency);
};

#endif
