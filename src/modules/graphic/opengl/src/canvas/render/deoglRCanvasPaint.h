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

#ifndef _DEOGLRCANVASPAINT_H_
#define _DEOGLRCANVASPAINT_H_

#include "deoglRCanvas.h"
#include "../../vbo/deoglSharedVBOBlock.h"

#include <dragengine/resources/canvas/deCanvasPaint.h>


/**
 * Render canvas paint.
 */
class deoglRCanvasPaint : public deoglRCanvas{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRCanvasPaint> Ref;
	
	
private:
	deCanvasPaint::eShapeTypes pShapeType;
	decColor pLineColor;
	decColor pFillColor;
	float pThickness;
	bool pIsThick;
	float pRoundCornerX;
	float pRoundCornerY;
	float pStartAngle;
	float pEndAngle;
	
	decVector2 *pPoints;
	int pPointCount;
	
	deoglSharedVBOBlock::Ref pVBOBlock;
	bool pDirtyVBOBlock;
	int pVBOBlockPointCount;
	
	GLenum pDrawModeFill;
	int pDrawOffsetFill;
	int pDrawCountFill;
	
	GLenum pDrawModeLine;
	int pDrawOffsetLine;
	int pDrawCountLine;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglRCanvasPaint(deoglRenderThread &renderThread);
	
	/** Clean up peer. */
	~deoglRCanvasPaint() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set size. */
	virtual void SetSize(const decVector2 &size);
	
	/** Shape type. */
	inline deCanvasPaint::eShapeTypes GetShapeType() const{ return pShapeType; }
	
	/**
	 * Set shape type.
	 * \throws deeInvalidParam \em shapeType is not a member of deCanvasPaint::eShapeTypes.
	 */
	void SetShapeType(deCanvasPaint::eShapeTypes shapeType);
	
	/** Line color. */
	inline const decColor &GetLineColor() const{ return pLineColor; }
	
	/** Set line color. */
	void SetLineColor(const decColor &color);
	
	/** Fill color. */
	inline const decColor &GetFillColor() const{ return pFillColor; }
	
	/** Set fill color. */
	void SetFillColor(const decColor &color);
	
	/** Line thickness in units. */
	inline float GetThickness() const{ return pThickness; }
	
	/**
	 * Line thickness in units.
	 * \details \em thickness is clamped to 0 or larger.
	 */
	void SetThickness(float thickness);
	
	/** Requires thick rendering. */
	inline bool IsThick() const{ return pIsThick; }
	
	/**
	 * Round corner in X direction as percentage.
	 * 
	 * Used by estRectangle shape type. Value of 0 indicates no non-round corners while
	 * value of 1 indicates fully round corners (aka ellipse).
	 */
	inline float GetRoundCornerX() const{ return pRoundCornerX; }
	
	/**
	 * Set round corner in X direction as percentage.
	 * 
	 * Used by estRectangle shape type. Value of 0 indicates no non-round corners while
	 * value of 1 indicates fully round corners (aka ellipse).
	 */
	void SetRoundCornerX(float roundCorner);
	
	/**
	 * Round corner in Y direction as percentage.
	 * 
	 * Used by estRectangle shape type. Value of 0 indicates no non-round corners while
	 * value of 1 indicates fully round corners (aka ellipse).
	 */
	inline float GetRoundCornerY() const{ return pRoundCornerY; }
	
	/**
	 * Set round corner in Y direction as percentage.
	 * 
	 * Used by estRectangle shape type. Value of 0 indicates no non-round corners while
	 * value of 1 indicates fully round corners (aka ellipse).
	 */
	void SetRoundCornerY(float roundCorner);
	
	/**
	 * Start angle in degrees from 0 to 360.
	 * 
	 * Used by estEllipse and estPie shape type. Angle is measured clock wise.
	 */
	inline float GetStartAngle() const{ return pStartAngle; }
	
	/**
	 * Set start angle in degrees from 0 to 360.
	 * 
	 * Used by estEllipse and estPie shape type. Angle is measured clock wise.
	 */
	void SetStartAngle(float angle);
	
	/**
	 * End angle in degrees from 0 to 360.
	 * 
	 * Used by estEllipse and estPie shape type. Angle is measured clock wise.
	 */
	inline float GetEndAngle() const{ return pEndAngle; }
	
	/**
	 * Set end angle in degrees from 0 to 360.
	 * 
	 * Used by estEllipse and estPie shape type. Angle is measured clock wise.
	 */
	void SetEndAngle(float angle);
	
	
	
	/** \name Points */
	/*@{*/
	/** Number of points. */
	inline int GetPointCount() const{ return pPointCount; }
	
	/**
	 * Set number of points.
	 * \throws deeInvalidParam \em count is less than 0.
	 */
	void SetPointCount(int count);
	
	/** Points. */
	inline decVector2 *GetPoints() const{ return pPoints; }
	
	
	
	/** VBO block. */
	inline const deoglSharedVBOBlock::Ref &GetVBOBlock() const{ return pVBOBlock; }
	
	
	
	/** Fill draw mode. */
	inline GLenum GetDrawModeFill() const{ return pDrawModeFill; }
	
	/** VBO fill offset. */
	inline int GetDrawOffsetFill() const{ return pDrawOffsetFill; }
	
	/** VBO fill count. */
	inline int GetDrawCountFill() const{ return pDrawCountFill; }
	
	
	
	/** Line draw mode. */
	inline GLenum GetDrawModeLine() const{ return pDrawModeLine; }
	
	/** VBO line offset. */
	inline int GetDrawOffsetLine() const{ return pDrawOffsetLine; }
	
	/** VBO line count. */
	inline int GetDrawCountLine() const{ return pDrawCountLine; }
	/*@}*/
	
	
	
	/** Prepare for rendering. */
	void PrepareForRender(const deoglRenderPlanMasked *renderPlanMask) override;
	
	/** Render. */
	void Render(const deoglRenderCanvasContext &context) override;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	int pRequiredPointCount();
	void pPrepareVBOBlock();
	void pWriteVBOData();
	void pCalcArc(decVector2 *outPoints, const decVector2 &center, const decVector2 &size,
		float startAngle, float stopAngle, int stepCount);
};

#endif
