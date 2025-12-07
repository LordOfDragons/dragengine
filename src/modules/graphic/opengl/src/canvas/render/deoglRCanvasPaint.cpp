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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRCanvasPaint.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/deoglRenderCanvasContext.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTRenderers.h"
#include "../../renderthread/deoglRTBufferObject.h"
#include "../../vbo/deoglSharedVBO.h"
#include "../../vbo/deoglSharedVBOBlock.h"
#include "../../vbo/deoglSharedVBOList.h"
#include "../../vbo/writer/deoglVBOWriterCanvasPaint.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCanvasPaint
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvasPaint::deoglRCanvasPaint(deoglRenderThread &renderThread) :
deoglRCanvas(renderThread),
pShapeType(deCanvasPaint::estPolygon),
pLineColor(0.0f, 0.0f, 0.0f, 1.0f),
pFillColor(0.0f, 0.0f, 0.0f, 0.0f),
pThickness(1.0f),
pRoundCornerX(0.0f),
pRoundCornerY(0.0f),
pStartAngle(0.0f),
pEndAngle(0.0f),

pPoints(NULL),
pPointCount(0),

pVBOBlock(NULL),
pDirtyVBOBlock(true),
pVBOBlockPointCount(0)
{
	LEAK_CHECK_CREATE(renderThread, CanvasPaint);
}

deoglRCanvasPaint::~deoglRCanvasPaint(){
	LEAK_CHECK_FREE(GetRenderThread(), CanvasPaint);
	pCleanUp();
}



// Management
///////////////

void deoglRCanvasPaint::SetSize(const decVector2 &size){
	deoglRCanvas::SetSize(size);
	pDirtyVBOBlock = true;
}

void deoglRCanvasPaint::SetShapeType(deCanvasPaint::eShapeTypes shapeType){
	pShapeType = shapeType;
	pDirtyVBOBlock = true;
}

void deoglRCanvasPaint::SetLineColor(const decColor &color){
	pLineColor = color;
}

void deoglRCanvasPaint::SetFillColor(const decColor &color){
	pFillColor = color;
}

void deoglRCanvasPaint::SetThickness(float thickness){
	pThickness = thickness;
	pIsThick = pThickness >= 1.5f;
	pDirtyVBOBlock = true;
}

void deoglRCanvasPaint::SetRoundCornerX(float roundCorner){
	pRoundCornerX = roundCorner;
}

void deoglRCanvasPaint::SetRoundCornerY(float roundCorner){
	pRoundCornerY = roundCorner;
}

void deoglRCanvasPaint::SetStartAngle(float angle){
	pStartAngle = angle;
}

void deoglRCanvasPaint::SetEndAngle(float angle){
	pEndAngle = angle;
}



void deoglRCanvasPaint::SetPointCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count == pPointCount){
		return;
	}
	
	if(pPoints){
		delete [] pPoints;
		pPoints = NULL;
		pPointCount = 0;
	}
	
	if(count > 0){
		pPoints = new decVector2[count];
		pPointCount = count;
	}
	
	pDirtyVBOBlock = true;
}



void deoglRCanvasPaint::PrepareForRender(const deoglRenderPlanMasked *renderPlanMask){
	if(pFillColor.a < FLOAT_SAFE_EPSILON && pLineColor.a < FLOAT_SAFE_EPSILON){
		return;
	}
	
	deoglRCanvas::PrepareForRender(renderPlanMask);
	pPrepareVBOBlock();
}

void deoglRCanvasPaint::Render(const deoglRenderCanvasContext &context){
	if(pFillColor.a < FLOAT_SAFE_EPSILON && pLineColor.a < FLOAT_SAFE_EPSILON){
		return;
	}
	
	const deoglRenderCanvasContext paintContext(context, *this);
	GetRenderThread().GetRenderers().GetCanvas().DrawCanvasPaint(paintContext, *this);
}



// Private Functions
//////////////////////

void deoglRCanvasPaint::pCleanUp(){
	if(pPoints){
		delete [] pPoints;
	}
	if(pVBOBlock){
		pVBOBlock->DelayedRemove();
	}
}



int deoglRCanvasPaint::pRequiredPointCount(){
	switch(pShapeType){
	case deCanvasPaint::estPoints:
		return pIsThick ? pPointCount * 6 : pPointCount;
		
	case deCanvasPaint::estLines:
		if(pPointCount % 2 != 0){
			return 0;
		}
		return pIsThick ? pPointCount * 3 : pPointCount;
		
	case deCanvasPaint::estPolygon:
		if(pPointCount < 2){
			return 0;
		}
		return pIsThick ? pPointCount + (pPointCount - 1) * 4 : pPointCount;
		
	case deCanvasPaint::estRectangle:
		if(pRoundCornerX > FLOAT_SAFE_EPSILON || pRoundCornerY > FLOAT_SAFE_EPSILON){
			const decVector2 center(GetSize().x * 0.5f * pRoundCornerX, GetSize().y * 0.5f * pRoundCornerY);
			const int points = decMath::max(4, (int)(center.Length() / 5.0f));
			return pIsThick ? points * 4 + (points * 4 + 1) * 2 : points * 8;
			
		}else{
			return pIsThick ? 14 : 8;
		}
		
	case deCanvasPaint::estEllipse:
	case deCanvasPaint::estPie:{
		const int points = decMath::max(4, (int)((GetSize().x + GetSize().y) * 2.0f / 5.0f));
		if(pIsThick){
			return points * 3 + 2 + (pShapeType == deCanvasPaint::estPie ? 3 : 0);
			
		}else{
			return points * 2 + (pShapeType == deCanvasPaint::estPie ? 2 : 0);
		}
		}
	}
	
	return 0;
}

void deoglRCanvasPaint::pPrepareVBOBlock(){
	const int pointCount = pRequiredPointCount();
	if(pointCount == 0){
		pDrawCountFill = 0;
		pDrawCountLine = 0;
		return;
	}
	
	if(pointCount > pVBOBlockPointCount){
		pDirtyVBOBlock = true;
		if(pVBOBlock){
			pVBOBlock->GetVBO()->RemoveBlock(pVBOBlock);
			pVBOBlock = NULL;
		}
	}
	
	if(!pVBOBlock){
		pVBOBlock = GetRenderThread().GetBufferObject().GetSharedVBOListForType(
			deoglRTBufferObject::esvbolCanvasPaint).AddData(pointCount);
		pVBOBlockPointCount = pointCount;
		pDirtyVBOBlock = true;
	}
	
	if(pDirtyVBOBlock){
		pWriteVBOData();
		pVBOBlock->SetValid(false);
		pDirtyVBOBlock = false;
	}
}

void deoglRCanvasPaint::pWriteVBOData(){
	deoglVBOWriterCanvasPaint writer(GetRenderThread());
	writer.Reset(pVBOBlock);
	
	const float thickness = pThickness > 0.5f ? decMath::max(pThickness, 1.0f) : 0.0f;
	const decVector2 pointCenter(0.5f, 0.5f);
	const decVector2 &size = GetSize();
	int i;
	
	pDrawOffsetFill = 0;
	pDrawCountFill = 0;
	pDrawOffsetLine = 0;
	pDrawCountLine = 0;
	
	switch(pShapeType){
	case deCanvasPaint::estPoints:
		if(pIsThick){
			const float ht = thickness * 0.5f;
			const decVector2 shift[6] = {
				decVector2(-ht, -ht), decVector2(ht, -ht), decVector2(ht, ht),
				decVector2(-ht, -ht), decVector2(ht, ht), decVector2(-ht, ht)};
			int j;
			
			pDrawModeLine = GL_TRIANGLES;
			pDrawCountLine = pPointCount * 6;
			
			for(i=0; i<pPointCount; i++){
				const decVector2 point(pPoints[i] + pointCenter);
				for(j=0; j<6; j++){
					writer.WritePoint(point + shift[j]);
				}
			}
			
		}else{
			pDrawModeLine = GL_POINTS;
			pDrawCountLine = pPointCount;
			
			for(i=0; i<pPointCount; i++){
				writer.WritePoint(pPoints[i] + pointCenter);
			}
		}
		break;
		
	case deCanvasPaint::estLines:
		if(pPointCount % 2 != 0){
			break;
		}
		
		if(pIsThick){
			const float ht = thickness * 0.5f;
			
			pDrawModeLine = GL_TRIANGLES;
			pDrawCountLine = pPointCount * 3;
			
			for(i=0; i<pPointCount; i+=2){
				const decVector2 a(pPoints[i] + pointCenter);
				const decVector2 b(pPoints[i + 1] + pointCenter);
				decVector2 shift;
				
				const decVector2 direction(b - a);
				const float directionLen = direction.Length();
				if(directionLen > 0.5f){
					const decVector2 normDir(direction / directionLen);
					shift.x = -normDir.y * ht;
					shift.y = normDir.x * ht;
				}
				
				writer.WritePoint(a + shift);
				writer.WritePoint(b + shift);
				writer.WritePoint(b - shift);
				
				writer.WritePoint(a + shift);
				writer.WritePoint(b - shift);
				writer.WritePoint(a - shift);
			}
			
		}else{
			pDrawModeLine = GL_LINES;
			pDrawCountLine = pPointCount;
			
			for(i=0; i<pPointCount; i++){
				writer.WritePoint(pPoints[i] + pointCenter);
			}
		}
		break;
		
	case deCanvasPaint::estPolygon:
		if(pPointCount < 2){
			break;
		}
		
		if(pIsThick){
			const float ht = thickness * 0.5f;
			decVector2 * const points = new decVector2[pPointCount + (pPointCount - 1) * 2];
			decVector2 * const inner = points;
			decVector2 * const outer = points + pPointCount;
			
			for(i=0; i<pPointCount - 1; i++){
				const decVector2 a(pPoints[i] + pointCenter);
				const decVector2 b(pPoints[i + 1] + pointCenter);
				decVector2 shift;
				
				const decVector2 direction(b - a);
				const float directionLen = direction.Length();
				if(directionLen > 0.5f){
					const decVector2 normDir(direction / directionLen);
					shift.x = -normDir.y * ht;
					shift.y = normDir.x * ht;
				}
				
				inner[i] += a - shift;
				inner[i + 1] += b - shift;
				
				outer[i * 2] = a + shift;
				outer[i * 2 + 1] = b + shift;
			}
			
			for(i=1; i<pPointCount - 1; i++){
				inner[i] *= 0.5f;
			}
			
			if(pPoints[0].IsEqualTo(pPoints[pPointCount - 1], 0.5f)){ // closed
				inner[0] += inner[pPointCount - 1];
				inner[0] *= 0.5f;
				inner[pPointCount - 1] = inner[0];
			}
			
			pDrawModeFill = GL_TRIANGLE_FAN;
			pDrawCountFill = pPointCount;
			for(i=0; i<pPointCount; i++){
				writer.WritePoint(inner[i]);
			}
			
			pDrawModeLine = GL_TRIANGLE_STRIP;
			pDrawOffsetLine = pPointCount;
			pDrawCountLine = (pPointCount - 1) * 4;
			for(i=0; i<pPointCount - 1; i++){
				writer.WritePoint(inner[i]);
				writer.WritePoint(outer[i * 2]);
				writer.WritePoint(inner[i + 1]);
				writer.WritePoint(outer[i * 2 + 1]);
			}
			
			delete [] points;
			
		}else{
			pDrawModeFill = GL_TRIANGLE_FAN;
			pDrawCountFill = pPointCount;
			
			pDrawModeLine = GL_LINE_STRIP;
			pDrawCountLine = pPointCount;
			
			for(i=0; i<pPointCount; i++){
				writer.WritePoint(pPoints[i] + pointCenter);
			}
		}
		break;
		
	case deCanvasPaint::estRectangle:{
		int cornerPointCount = 1;
		decVector2 cornerCenter;
		if(pRoundCornerX > FLOAT_SAFE_EPSILON || pRoundCornerY > FLOAT_SAFE_EPSILON){
			cornerCenter.x = size.x * 0.5f * (pRoundCornerX > FLOAT_SAFE_EPSILON ? pRoundCornerX : 0.0f);
			cornerCenter.y = size.y * 0.5f * (pRoundCornerY > FLOAT_SAFE_EPSILON ? pRoundCornerY : 0.0f);
			cornerPointCount = decMath::max(4, (int)(cornerCenter.Length() / 5.0f));
		}
		
		// fill
		const decVector2 cornerSizeInner(decVector2().Largest(cornerCenter - decVector2(thickness, thickness)));
		decVector2 * const points = new decVector2[cornerPointCount * 4 + (cornerPointCount * 4 + 1) * 2];
		decVector2 * const inner = points;
		decVector2 * const outer = points + cornerPointCount * 4;
		
		if(cornerPointCount > 1){
			pCalcArc(inner, cornerCenter, cornerSizeInner,
				DEG2RAD * 270.0f, DEG2RAD * 180.0f, cornerPointCount);
			pCalcArc(inner + cornerPointCount,
				decVector2(size.x - cornerCenter.x, cornerCenter.y), cornerSizeInner,
				DEG2RAD * 180.0f, DEG2RAD * 90.0f, cornerPointCount);
			pCalcArc(inner + cornerPointCount * 2, size - cornerCenter, cornerSizeInner,
				DEG2RAD * 90.0f, 0.0f, cornerPointCount);
			pCalcArc(inner + cornerPointCount * 3,
				decVector2(cornerCenter.x, size.y - cornerCenter.y), cornerSizeInner,
				0.0f, DEG2RAD * -90.0f, cornerPointCount);
			
		}else{
			inner[0].Set(thickness, thickness);
			inner[1].Set(size.x - thickness, thickness);
			inner[2].Set(size.x - thickness, size.y - thickness);
			inner[3].Set(thickness, size.y - thickness);
		}
		
		for(i=0; i<cornerPointCount * 4; i++){
			writer.WritePoint(inner[i]);
		}
		
		pDrawModeFill = GL_TRIANGLE_FAN;
		pDrawCountFill = cornerPointCount * 4;
		
		// outline
		pDrawOffsetLine = pDrawCountFill;
		
		if(thickness > 0.5f){
			const float geomTypeReduce = pIsThick ? 0.0f : 1.0f;
			
			if(cornerPointCount > 1){
				pCalcArc(outer, cornerCenter, cornerCenter,
					DEG2RAD * 270.0f, DEG2RAD * 180.0f, cornerPointCount);
				pCalcArc(outer + cornerPointCount,
					decVector2(size.x - geomTypeReduce - cornerCenter.x, cornerCenter.y), cornerCenter,
					DEG2RAD * 180.0f, DEG2RAD * 90.0f, cornerPointCount);
				pCalcArc(outer + cornerPointCount * 2,
					size - decVector2(geomTypeReduce, geomTypeReduce) - cornerCenter, cornerCenter,
					DEG2RAD * 90.0f, 0.0f, cornerPointCount);
				pCalcArc(outer + cornerPointCount * 3,
					decVector2(cornerCenter.x, size.y - geomTypeReduce - cornerCenter.y), cornerCenter,
					0.0f, DEG2RAD * -90.0f, cornerPointCount);
				
			}else{
				outer[0].Set(0.0f, 0.0f);
				outer[1].Set(size.x - geomTypeReduce, 0.0f);
				outer[2].Set(size.x - geomTypeReduce, size.y - geomTypeReduce);
				outer[3].Set(0.0f, size.y - geomTypeReduce);
			}
			
			if(pIsThick){
				const int count = cornerPointCount * 4;
				for(i=0; i<=count; i++){
					writer.WritePoint(inner[i % count]);
					writer.WritePoint(outer[i % count]);
				}
				
				pDrawModeLine = GL_TRIANGLE_STRIP;
				pDrawCountLine = (count + 1) * 2;
				
			}else{
				pDrawModeLine = GL_LINE_LOOP;
				pDrawCountLine = cornerPointCount * 4;
				
				for(i=0; i<cornerPointCount * 4; i++){
					writer.WritePoint(outer[i] + pointCenter);
				}
			}
		}
		
		delete [] points;
		}break;
		
	case deCanvasPaint::estEllipse:
	case deCanvasPaint::estPie:{
		const int pointCount = decMath::max(4, (int)((GetSize().x + GetSize().y) * 2.0f / 5.0f));
		const bool isPie = pShapeType == deCanvasPaint::estPie;
		const decVector2 ellipseSize(size * 0.5f);
		const decVector2 center(ellipseSize);
		float startAngle = PI - pStartAngle;
		float endAngle = PI - pEndAngle;
		
		// fill
		const decVector2 ellipseSizeInner(decVector2().Largest(ellipseSize - decVector2(thickness, thickness)));
		decVector2 * const points = new decVector2[pointCount * 3 + 2 + (isPie ? 2 : 0)];
		decVector2 * const inner = points;
		decVector2 * const outer = points + (pointCount + (isPie ? 1 : 0));
		
		pCalcArc(inner, center, ellipseSizeInner + decVector2(1.0f, 1.0f), startAngle, endAngle, pointCount);
		
		if(isPie){
			writer.WritePoint(center);
		}
		for(i=0; i<pointCount; i++){
			writer.WritePoint(inner[i]);
		}
		
		pDrawModeFill = GL_TRIANGLE_FAN;
		pDrawCountFill = pointCount + (isPie ? 1 : 0);
		
		// outline
		pDrawOffsetLine = pDrawCountFill;
		
		if(thickness > 0.5f){
			if(pIsThick){
// 				if( isPie ){
					const float lenStart = (inner[0] - center).Length() + thickness;
					const float lenEnd = (inner[pointCount - 1] - center).Length() + thickness;
					if(lenStart > 0.5f){
						startAngle += atanf(thickness * 0.5f / lenStart);
					}
					if(lenEnd > 0.5f){
						endAngle -= atanf(thickness * 0.5f / lenEnd);
					}
// 				}
				
				pCalcArc(outer, center, ellipseSize, startAngle, endAngle, pointCount);
				
				if(isPie){
					const decVector2 &a = outer[pointCount - 1];
					const decVector2 &b = outer[0];
					decVector2 shift;
					
					const decVector2 direction(b - a);
					const float directionLen = direction.Length();
					if(directionLen > 0.5f){
						const decVector2 normDir(direction / directionLen);
						shift.x = -normDir.y * thickness;
						shift.y = normDir.x * thickness;
					}
					
					const decVector2 pieOuter(center - shift);
					
					writer.WritePoint(center);
					writer.WritePoint(pieOuter);
					
					for(i=0; i<pointCount; i++){
						writer.WritePoint(inner[i]);
						writer.WritePoint(outer[i]);
					}
					
					writer.WritePoint(center);
					writer.WritePoint(pieOuter);
					
					pDrawCountLine = (pointCount + 2) * 2;
					
				}else{
					for(i=0; i<=pointCount; i++){
						writer.WritePoint(inner[i % pointCount]);
						writer.WritePoint(outer[i % pointCount]);
					}
					pDrawCountLine = (pointCount + 1) * 2;
				}
				
				pDrawModeLine = GL_TRIANGLE_STRIP;
				
			}else{
				pCalcArc(outer, center, ellipseSize, startAngle, endAngle, pointCount);
				
				if(isPie){
					writer.WritePoint(center);
				}
				for(i=0; i<pointCount; i++){
					writer.WritePoint(outer[i]);
				}
				
				pDrawModeLine = GL_LINE_LOOP;
				pDrawCountLine = pointCount + (isPie ? 1 : 0);
			}
		}
		
		delete [] points;
		}break;
	}
}

void deoglRCanvasPaint::pCalcArc(decVector2 *outPoints, const decVector2 &center,
const decVector2 &size, float startAngle, float stopAngle, int stepCount){
	const float stepAngle = (stopAngle - startAngle) / (stepCount - 1);
	int i;
	
	for(i=0; i<stepCount; i++){
		const float arcAngle = startAngle + stepAngle * i;
		outPoints[i].x = center.x + size.x * sinf(arcAngle);
		outPoints[i].y = center.y + size.y * cosf(arcAngle);
	}
}
