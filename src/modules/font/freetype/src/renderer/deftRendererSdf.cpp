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

#include "deftRendererSdf.h"
#include "../deftModule.h"
#include "../includeMsdfgen.h"
#include "../layout/deftLayout.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/font/deFontSize.h>
#include <dragengine/resources/image/deImageManager.h>


struct sFTColorBGRA{
	unsigned char blue, green, red, alpha;
};

struct sFTOutlineData{
	msdfgen::Shape &shape;
	msdfgen::Point2 start, current;
	bool contourOpen;
	
	sFTOutlineData(msdfgen::Shape &s) :
	shape(s),
	contourOpen(false){
	}
};

static msdfgen::Point2 sFTPoint(const FT_Vector &v){
	return msdfgen::Point2(v.x, v.y);
}

static bool sFTSamePoint(const msdfgen::Point2 &a, const msdfgen::Point2 &b){
	return a.x == b.x && a.y == b.y;
}

static void sFTCloseContour(sFTOutlineData &data){
	if(!data.contourOpen){
		return;
	}
	if(!sFTSamePoint(data.current, data.start)){
		data.shape.contours.back().addEdge(msdfgen::EdgeHolder(data.current, data.start));
	}
	data.contourOpen = false;
}

static int sFTMoveTo(const FT_Vector *to, void *user){
	sFTOutlineData &data = *static_cast<sFTOutlineData*>(user);
	sFTCloseContour(data);
	data.shape.addContour();
	data.start = sFTPoint(*to);
	data.current = data.start;
	data.contourOpen = true;
	return 0;
}

static int sFTLineTo(const FT_Vector *to, void *user){
	sFTOutlineData &data = *static_cast<sFTOutlineData*>(user);
	const msdfgen::Point2 p = sFTPoint(*to);
	if(!sFTSamePoint(p, data.current)){
		data.shape.contours.back().addEdge(msdfgen::EdgeHolder(data.current, p));
	}
	data.current = p;
	return 0;
}

static int sFTConicTo(const FT_Vector *control, const FT_Vector *to, void *user){
	sFTOutlineData &data = *static_cast<sFTOutlineData*>(user);
	const msdfgen::Point2 c = sFTPoint(*control);
	const msdfgen::Point2 p = sFTPoint(*to);
	if(!sFTSamePoint(p, data.current)){
		data.shape.contours.back().addEdge(msdfgen::EdgeHolder(data.current, c, p));
	}
	data.current = p;
	return 0;
}

static int sFTCubicTo(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, void *user){
	sFTOutlineData &data = *static_cast<sFTOutlineData*>(user);
	const msdfgen::Point2 c1 = sFTPoint(*control1);
	const msdfgen::Point2 c2 = sFTPoint(*control2);
	const msdfgen::Point2 p = sFTPoint(*to);
	if(!sFTSamePoint(p, data.current)){
		data.shape.contours.back().addEdge(msdfgen::EdgeHolder(data.current, c1, c2, p));
	}
	data.current = p;
	return 0;
}


// Class deftRendererSdf
///////////////////////

// Constructor, destructor
////////////////////////////

deftRendererSdf::deftRendererSdf(deftModule &module, FT_Face face) :
pModule(module),
pFace(face),
pLoadFlags(FT_LOAD_DEFAULT),
pUnitScale(1.0),
pSdfMinVal(0.0),
pSdfMaxVal(1.0),
pPixelRange(1.0){
}

deftRendererSdf::~deftRendererSdf() = default;


// Management
///////////////

void deftRendererSdf::SetLoadFlags(FT_Int32 loadFlags){
	pLoadFlags = loadFlags;
}

void deftRendererSdf::SetUnitScale(double unitScale){
	pUnitScale = unitScale;
}

void deftRendererSdf::SetSdfDistanceRange(double minVal, double maxVal){
	pSdfMinVal = minVal;
	pSdfMaxVal = maxVal;
}

void deftRendererSdf::SetPixelRange(double pixelRange){
	pPixelRange = pixelRange;
}


void deftRendererSdf::Render(const deftLayout &layout, deFontSize &fontSize){
	const auto &imageSize = layout.GetImageSize();
	const auto image = pModule.GetGameEngine()->GetImageManager()->CreateImage(
		imageSize.x, imageSize.y, imageSize.z, 4, 16);
	
	pRenderSdf(layout, image);
	
	image->NotifyImageDataChanged();
	fontSize.SetImage(image);
}


// Protected Functions
////////////////////////

void deftRendererSdf::pRenderSdf(const deftLayout &layout, deImage &image){
	const auto &glyphs = layout.GetGlyphs();
	
	const int width = image.GetWidth();
	const int height = image.GetHeight();
	const int depth = image.GetDepth();
	const int strideZ = width * height;
	const int strideY = width;
	const int pixelCount = strideZ * depth;
	
	sRGBA16 * const di = image.GetDataRGBA16();
	int glyphIndex = 0;
	for(int x=0; x<pixelCount; x++){
		di[x] = {0, 0, 0, 0};
	}
	
	const double unitScale = pUnitScale;
	const msdfgen::Range range(pPixelRange / unitScale);
	const int sdfMargin = (int)(std::ceil(pPixelRange * 0.5f));
	const double sdfValDiv = 1.0 / (pSdfMaxVal - pSdfMinVal);
	const float sdfRemap1 = (float)sdfValDiv;
	const float sdfRemap2 = (float)(sdfValDiv * -pSdfMinVal);
	
	const auto toUShort = [](float v) -> unsigned short{
		return (unsigned short)(decMath::clamp(v, 0.0f, 1.0f) * 65535.0f + 0.5f);
	};
	
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	while(gindex != 0){
		if(charcode < 0xffff){
			pModule.AssertFT(FT_Load_Glyph(pFace, gindex, pLoadFlags), "FT_Load_Glyph");
			
			const deftLayout::Glyph &glyph = glyphs[glyphIndex++];
			const int cellWidth = decMath::min(glyph.size.x, width - glyph.position.x);
			const int cellHeight = decMath::min(glyph.size.y, height - glyph.position.y);
			
			do{
				if(cellWidth <= 0 || cellHeight <= 0 || pFace->glyph->format != FT_GLYPH_FORMAT_OUTLINE){
					break;
				}
				
				FT_Outline &outline = pFace->glyph->outline;
				
				msdfgen::Shape shape;
				sFTOutlineData data(shape);
				FT_Outline_Funcs funcs{};
				funcs.move_to = sFTMoveTo;
				funcs.line_to = sFTLineTo;
				funcs.conic_to = sFTConicTo;
				funcs.cubic_to = sFTCubicTo;
				pModule.AssertFT(FT_Outline_Decompose(&outline, &funcs, &data), "FT_Outline_Decompose");
				sFTCloseContour(data);
				if(shape.contours.empty()){
					break;
				}
				
				shape.normalize();
				msdfgen::edgeColoringSimple(shape, 3.0);
				
				// compute exact pixel bounds from unscaled CBox and center inside sdfMargin
				FT_BBox cbox;
				FT_Outline_Get_CBox(&outline, &cbox);
				const double left = std::floor(cbox.xMin * unitScale);
				const double top = std::ceil(cbox.yMax * unitScale);
				
				// msdfgen::Projection applies: pixel = scale * (shapeCoord + translate)
				const msdfgen::Projection projection(
					msdfgen::Vector2(unitScale, unitScale),
					msdfgen::Vector2((-left + sdfMargin) / unitScale,
						(cellHeight - sdfMargin - top) / unitScale));
				
				std::vector<float> buffer((size_t)cellWidth * cellHeight * 4);
				msdfgen::BitmapSection<float, 4> output(buffer.data(), cellWidth, cellHeight);
				msdfgen::generateMTSDF(output, shape, projection, range);
				
				for(int y=0; y<cellHeight; y++){
					const int row = cellHeight - 1 - y;
					sRGBA16 *dg = di + strideZ * glyph.position.z + strideY * (glyph.position.y + y) + glyph.position.x;
					for(int x=0; x<cellWidth; x++){
						const float * const sp = buffer.data() + ((size_t)row * cellWidth + x) * 4;
						dg[x].red = toUShort(sp[0]);
						dg[x].green = toUShort(sp[1]);
						dg[x].blue = toUShort(sp[2]);
						dg[x].alpha = toUShort(sp[3] * sdfRemap1 + sdfRemap2);
					}
				}
			}while(false);
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
}
