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

#ifndef _IGDENATIVENULLVIEWCURVEBEZIER_H_
#define _IGDENATIVENULLVIEWCURVEBEZIER_H_

#include "../../curveedit/igdeViewCurveBezier.h"

class igdeViewCurveBezier;


/**
 * Null view curve bezier.
 */
class igdeNativeNullViewCurveBezier : public igdeViewCurveBezier::cNativeViewCurveBezier{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeNullViewCurveBezier();
	
	/** \brief Clean up native widget. */
	virtual ~igdeNativeNullViewCurveBezier();
	
	/** \brief Create native widget. */
	static igdeNativeNullViewCurveBezier* CreateNativeWidget(igdeViewCurveBezier &owner);
	
	/** \brief Post create native widget. */
	virtual void PostCreateNativeWidget();
	
	/** \brief Destroy native widget. */
	virtual void DestroyNativeWidget();
	
	void UpdateEnabled() override;
	void UpdateDefaultSize() override;
	void UpdateCurve() override;
	void UpdateSelectedPoint() override;
	void UpdateClamp() override;
	void ResetView() override;
	void FitViewToCurve() override;
	/*@}*/
};

typedef igdeNativeNullViewCurveBezier igdeNativeViewCurveBezier;

#endif
