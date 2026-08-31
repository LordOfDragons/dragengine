/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDENATIVEBEOSVIEWCURVEBEZIER_H_
#define _IGDENATIVEBEOSVIEWCURVEBEZIER_H_

#include <interface/View.h>
#include "../../../curveedit/igdeViewCurveBezier.h"
#include "../../../resources/igdeFont.h"

#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/curve/decCurveBezierPoint.h>

class igdeGuiTheme;


/**
 * \brief BeOS native bezier curve editor view.
 */
class igdeNativeBeosViewCurveBezier : public BView, public igdeViewCurveBezier::cNativeViewCurveBezier{
public:
	enum eSelectionModes{
		esmPoint,
		esmHandle1,
		esmHandle2
	};
	
	enum eDragModes{
		edmNone,
		edmDragCurvePoint,
		edmDragCurveHandle1,
		edmDragCurveHandle2,
		edmDragView
	};
	
private:
	igdeViewCurveBezier *pOwner;
	igdeFont::Ref pFont;
	
	decVector2 pGridCenter;
	decVector2 pGridZoom;
	decPoint pWindowCenter;
	decVector2 pGridScale;
	
	int pDotSize;
	int pHitThreshold;
	
	eDragModes pDragMode;
	decPoint pDragOrg;
	decCurveBezierPoint pDragOrgPoint;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native widget. */
	igdeNativeBeosViewCurveBezier(igdeViewCurveBezier &owner, BView *parent,
		const igdeGuiTheme &guitheme);
	
	/** \brief Clean up native widget. */
	~igdeNativeBeosViewCurveBezier() override;
	
	/** \brief Create native widget. */
	static igdeNativeBeosViewCurveBezier* CreateNativeWidget(igdeViewCurveBezier &owner);
	
	/** \brief Post create native widget. */
	void PostCreateNativeWidget() override;
	
	/** \brief Destroy native widget. */
	void DestroyNativeWidget() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	void UpdateEnabled() override;
	void UpdateDefaultSize() override;
	void UpdateCurve() override;
	void UpdateSelectedPoint() override;
	void UpdateClamp() override;
	void ResetView() override;
	void FitViewToCurve() override;
	
	
	
	int IndexOfPointAt(int x, int y, eSelectionModes mode) const;
	
	void UpdateParameters();
	
	int CurveXToWindow(float curveCoord) const;
	int CurveYToWindow(float curveCoord) const;
	float WindowXToCurve(int windowCoord) const;
	float WindowYToCurve(int windowCoord) const;
	
	void DrawGrid();
	void DrawCurve();
	void DrawCurveHandles();
	void InternalDrawCurve(const decVector2 &p1, const decVector2 &p2,
		const decVector2 &p3, const decVector2 &p4);
	/*@}*/
	
	
	
	/** \name BView overrides */
	/*@{*/
	void Draw(BRect updateRect) override;
	void MouseDown(BPoint where) override;
	void MouseMoved(BPoint where, uint32 code, const BMessage *message) override;
	void MouseUp(BPoint where) override;
	void FrameResized(float width, float height) override;
	/*@}*/
};

typedef igdeNativeBeosViewCurveBezier igdeNativeViewCurveBezier;

#endif
