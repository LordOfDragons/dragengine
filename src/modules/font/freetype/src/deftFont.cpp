/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deftFont.h"
#include "deftModule.h"

#include <vector>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontGlyph.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>

#include "includeMsdfgen.h"


/** Free type color in BGRS format. */
struct sFTColorBGRA{
	unsigned char blue, green, red, alpha;
};

/** Outline decomposition state used to build msdfgen shapes from FreeType outlines. */
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


// Class deftFont
///////////////////

// Constructor, destructor
////////////////////////////

deftFont::deftFont(deftModule &module, decBaseFileReader &reader) :
pModule(module),
pReader(reader),
pFace(nullptr),
pStreamRec{},
pOpenArgs{},
pMaxImageWidth(2048),
pGlyphSpacing(1),
pLineHeightLimit(128),
pHintingSafetyMargin(1),
pRealFontSize(0),
pBaseLine(0),
pRealLineHeight(0),
pComponentCount(2),
pLoadFlags(FT_LOAD_DEFAULT),
pCreateSdfFont(true)
{
	pStreamRec.size = reader.GetLength();
	pStreamRec.pos = reader.GetPosition();
	pStreamRec.descriptor.pointer = this;
	pStreamRec.read = fStreamRead;
	pStreamRec.close = fStreamClose;
	
	pOpenArgs.flags = FT_OPEN_STREAM;
	pOpenArgs.stream = &pStreamRec;
	
	module.AssertFT(FT_Open_Face(module.GetFTLibrary(), &pOpenArgs, 0, &pFace), "FT_Open_Face");
}

deftFont::~deftFont(){
	if(pFace){
		FT_Done_Face(pFace);
	}
}


// Management
///////////////

void deftFont::SetCreateSdfFont(bool createSdf){
	pCreateSdfFont = createSdf;
}


void deftFont::LoadFont(deFont &font){
	pModule.AssertFT(FT_Set_Pixel_Sizes(pFace, 0, 16), "FT_Set_Pixel_Sizes");
	
	font.SetImagePath("");
	font.SetImage(pModule.GetGameEngine()->GetImageManager()->LoadDefault());
	
	font.SetLineHeight(16);
	font.SetBaseLine(16 * 3 / 4);
	font.SetIsColorFont(false);//FT_HAS_COLOR(pFace));
	font.SetScalable(FT_IS_SCALABLE(pFace));
	
	if(font.GetScalable() && pCreateSdfFont){
		font.SetIsSdf(true);
		font.GetFixedSizes().Add(pBestSdfFixedSize(font));
		
	}else{
		if(FT_HAS_FIXED_SIZES(pFace)){
			FT_Int i;
			for(i=0; i<pFace->num_fixed_sizes; i++){
				font.GetFixedSizes().Add((int)pFace->available_sizes[i].height);
			}
		}
	}
	
	pInitGlyphs(font);
}

deFontSize::Ref deftFont::LoadFontSize(deFont &font, int lineHeight){
	const char * const filename = font.GetFilename();
	const bool isSdf = font.GetIsSdf();
	deFontSize::Ref size;
	
	if(isSdf){
		lineHeight = pBestSdfFixedSize(font);
		
	}else{
		if(lineHeight > pLineHeightLimit){
			pModule.LogInfoFormat("LoadFontSize[%s,%d]: Request font size too large. Using %d instead",
				filename, lineHeight, pLineHeightLimit);
			size = font.GetSizeWith(pLineHeightLimit);
			if(size){
				return size;
			}
			
			lineHeight = pLineHeightLimit;
		}
	}
	
	size = deFontSize::Ref::New(lineHeight, font.GetGlyphCount());
	
	const auto measurements = pFindBestSize(font, size);
	
	pRealFontSize = measurements.lineHeight;
	pRealLineHeight = (measurements.bbox.y2 - measurements.bbox.y1) + measurements.lineHeightMargin;
	
	pBearingOffset.x = 0;
	pBearingOffset.y = (lineHeight - pRealLineHeight) / 2;
	pBaseLine = measurements.bbox.y2 + pBearingOffset.y + measurements.baseLineMargin;
	
	if(isSdf){
		// FT_LOAD_NO_SCALE: ensure coordinates are in exact font design units (not 26.6 fixed-point)
		// FT_LOAD_NO_BITMAP: do not load embedded bitmaps. just in case the font has them
		// FT_LOAD_NO_HINTING: disable hinting to get raw font outlines
		pLoadFlags = FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING;
		pComponentCount = 4;
		
	}else if(FT_HAS_COLOR(pFace)){
		pLoadFlags = FT_LOAD_DEFAULT | FT_LOAD_COLOR;
		pComponentCount = 4;
		
	}else{
		pLoadFlags = FT_LOAD_COLOR;
		pComponentCount = 2;
	}
	
	if(isSdf){
		float pixelRange = decMath::linearStep((float)pRealLineHeight, 24.0f, 48.0f, 2.5f, 4.0f);
		pixelRange += 8.0f;
		size->SetSdfPixelRange(pixelRange);
		// size->SetSdfPixelRange(4.0f);
	}
	
	if(isSdf){
		pModule.LogInfoFormat(
			"LoadFontSize[%s,%d,sdf]: fs=%d bb=(%d,%d,%d,%d) h=%d gh=%d o=(%d,%d) bl=%d clr=%d m=(%g,%g) spr=%g",
			filename, lineHeight, pRealLineHeight, measurements.bbox.x1,
			measurements.bbox.y1, measurements.bbox.x2, measurements.bbox.y2,
			(int)(FT_MulFix(pFace->height, pFace->size->metrics.y_scale) / 64),
			pRealLineHeight, pBearingOffset.x, pBearingOffset.y, pBaseLine, FT_HAS_COLOR(pFace),
			measurements.lineHeightMargin, measurements.baseLineMargin, size->GetSdfPixelRange());
		
	}else{
		pModule.LogInfoFormat(
			"LoadFontSize[%s,%d]: fs=%d bb=(%d,%d,%d,%d) h=%d gh=%d o=(%d,%d) bl=%d clr=%d",
			filename, lineHeight, pRealLineHeight, measurements.bbox.x1,
			measurements.bbox.y1, measurements.bbox.x2, measurements.bbox.y2,
			(int)(FT_MulFix(pFace->height, pFace->size->metrics.y_scale) / 64),
			pRealLineHeight, pBearingOffset.x, pBearingOffset.y, pBaseLine, FT_HAS_COLOR(pFace));
	}
	
	if(isSdf){
		pInitGlyphsSdf(size, measurements.sdfUnitScale);
		
	}else{
		pInitGlyphs(size);
	}
	
	decPoint3 imageSize(pCalcLayout(size, isSdf));
	DEASSERT_FALSE(imageSize == decPoint3())
	pModule.LogInfoFormat("LoadFontSize[%s,%d]: Layout calculated, %d glyphs, image size (%d,%d,%d)",
		filename, lineHeight, size->GetGlyphs().GetCount(), imageSize.x, imageSize.y, imageSize.z);
	
	// pDebugPrintGlyphs(font, size);
	
	decTimer timer;
	auto image = pModule.GetGameEngine()->GetImageManager()->CreateImage(
		imageSize.x, imageSize.y, imageSize.z, pComponentCount, 8);
	
	if(isSdf){
		pRenderGlyphsSdf(size, image, measurements);
		
	}else{
		pRenderGlyphs(size, image);
	}
	
	image->NotifyImageDataChanged();
	size->SetImage(image);
	
	pModule.LogInfoFormat("LoadFontSize[%s,%d]: Image rendered in %dms",
		filename, lineHeight, (int)(timer.GetElapsedTime() * 1e3f));
	
	// DEBUG
	/*
	if(pComponentCount == 4){
		if(imageSize.z > 1){
			pModule.GetGameEngine()->GetImageManager()->SaveImage(image, "/capture/freetype_image.png3d");
		}else{
			pModule.GetGameEngine()->GetImageManager()->SaveImage(image, "/capture/freetype_image.png");
		}
	}
	*/
	// DEBUG
	
	// if done on manually created images data is lost and can not be retained for graphic module
	// image->ReleaseImageData();
	
	return size;
}


// Private functions
//////////////////////

int deftFont::pCountUsableGlyphs() const{
	int glyphCount = 0;
	
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	
	while(gindex != 0){
		if(charcode < 0xffff){
			glyphCount++;
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
	
	return glyphCount;
}

void deftFont::pInitGlyphs(deFont &font){
	font.SetGlyphCount(pCountUsableGlyphs());
	
	int glyphIndex = 0;
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	while(gindex != 0){
		if(charcode < 0xffff){
			deFontGlyph &glyph = font.GetGlyphAt(glyphIndex++);
			glyph.SetUnicode((int)charcode);
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
}

deftFont::sMeasurements deftFont::pFindBestSize(const deFont &font, const deFontSize &size){
	sMeasurements measurements;
	
	if(font.GetIsSdf()){
		const double targetLineHeight = (double)size.GetLineHeight();
		measurements.sdfUnitScale = targetLineHeight / (double)pFace->height;
		
		measurements.bbox.x1 = (int)(std::floor(pFace->bbox.xMin * measurements.sdfUnitScale));
		measurements.bbox.y1 = (int)(std::floor(pFace->bbox.yMin * measurements.sdfUnitScale));
		measurements.bbox.x2 = (int)(std::ceil(pFace->bbox.xMax * measurements.sdfUnitScale));
		measurements.bbox.y2 = (int)(std::ceil(pFace->bbox.yMax * measurements.sdfUnitScale));
		
		measurements.lineHeight = (int)(std::round(pFace->units_per_EM * measurements.sdfUnitScale));
		
		const double margin = (int)std::ceil(size.GetSdfPixelRange() * 0.5f);
		measurements.baseLineMargin = margin;
		measurements.lineHeightMargin = margin * 2.0;
		
	}else{
		// const char * const filename = font.GetFilename();
		const int reqLineHeight = size.GetLineHeight();
		int prevHeight = 0;
		
		measurements.lineHeight = reqLineHeight;
		
		measurements.sdfUnitScale = 1.0;
		
		// enlarge glyphs by a small margin to get some wiggle space if hinting enlarges font
		measurements.lineHeightMargin = pHintingSafetyMargin;
		
		while(true){
			pModule.AssertFT(FT_Set_Pixel_Sizes(pFace, 0, measurements.lineHeight), "FT_Set_Pixel_Sizes");
			
			measurements.bbox.x1 = FT_MulFix(pFace->bbox.xMin, pFace->size->metrics.y_scale) / 64;
			measurements.bbox.y1 = FT_MulFix(pFace->bbox.yMin, pFace->size->metrics.y_scale) / 64;
			measurements.bbox.x2 = FT_MulFix(pFace->bbox.xMax, pFace->size->metrics.y_scale) / 64;
			measurements.bbox.y2 = FT_MulFix(pFace->bbox.yMax, pFace->size->metrics.y_scale) / 64;
			
			const int height = (int)(FT_MulFix(pFace->height, pFace->size->metrics.y_scale) / 64);
			
			/*
			pModule.LogInfoFormat(
				"LoadFontSize[%s,%d]: lh=%d bbox=(%d,%d,%d,%d) height=%d gheight=%d",
				filename, reqLineHeight, lineHeight, pBBox.x1, pBBox.y1, pBBox.x2, pBBox.y2,
				height, pBBox.y2 - pBBox.y1);
			*/
			
			if(height == reqLineHeight){
				return measurements;
			}
			
			if(height > reqLineHeight){
				if(prevHeight != 0 && prevHeight < height){
					return measurements;
				}
				
				prevHeight = height;
				measurements.lineHeight--;
				
			}else{
				prevHeight = height;
				measurements.lineHeight++;
			}
		}
	}
	
	return measurements;
}

void deftFont::pInitGlyphs(deFontSize &size){
	int glyphIndex = 0;
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	
	while(gindex != 0){
		if(charcode < 0xffff){
			pModule.AssertFT(FT_Load_Glyph(pFace, gindex, pLoadFlags), "FT_Load_Glyph");
			
			deFontGlyph &glyph = size.GetGlyphAt(glyphIndex++);
			glyph.SetUnicode((int)charcode);
			glyph.SetWidth(pFace->glyph->metrics.width / 64);
			glyph.SetHeight(decMath::min(pFace->glyph->metrics.height / 64, pRealLineHeight));
			glyph.SetAdvance(pFace->glyph->metrics.horiAdvance / 64);
			
			// bearing in freetype is positive to the right and top.
			// bearing in dragengine is positive to the left and bottom
			glyph.SetBearing(-(pFace->glyph->metrics.horiBearingX / 64 + pBearingOffset.x));
			glyph.SetBearingY(pFace->glyph->metrics.horiBearingY / 64 - pBaseLine);
			
			// enlarge glyph by a small margin to get some wiggle space if hinting enlarges font
			glyph.SetWidth(glyph.GetWidth() + pHintingSafetyMargin);
			glyph.SetHeight(glyph.GetHeight() + pHintingSafetyMargin);
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
}

void deftFont::pInitGlyphsSdf(deFontSize &size, double unitScale){
	const int sdfMargin = (int)std::ceil(size.GetSdfPixelRange() * 0.5f);
	int glyphIndex = 0;
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	
	while(gindex != 0){
		if(charcode < 0xffff){
			pModule.AssertFT(FT_Load_Glyph(pFace, gindex, pLoadFlags), "FT_Load_Glyph");
			
			const FT_GlyphSlot slot = pFace->glyph;
			deFontGlyph &glyph = size.GetGlyphAt(glyphIndex++);
			glyph.SetUnicode((int)charcode);
			
			int width = 0, height = 0, bearingX = 0, bearingY = 0;
			if(slot->format == FT_GLYPH_FORMAT_OUTLINE && slot->outline.n_points > 0){
				FT_BBox cbox;
				FT_Outline_Get_CBox(&slot->outline, &cbox);
				
				const int left = (int)std::floor(cbox.xMin * unitScale);
				const int right = (int)std::ceil(cbox.xMax * unitScale);
				const int bottom = (int)std::floor(cbox.yMin * unitScale);
				const int top = (int)std::ceil(cbox.yMax * unitScale);
				
				width = right - left + 2 * sdfMargin;
				height = top - bottom + 2 * sdfMargin;
				
				// bearing in dragengine is positive to the left and bottom
				bearingX = sdfMargin - left;
				bearingY = top + sdfMargin - pBaseLine;
			}
			
			glyph.SetWidth(width);
			glyph.SetHeight(height);
			glyph.SetBearing(bearingX);
			glyph.SetBearingY(bearingY);
			glyph.SetAdvance((int)std::round(slot->metrics.horiAdvance * unitScale));
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
}

decPoint3 deftFont::pCalcLayout(deFontSize &size, bool sdf){
	if(pRealLineHeight > pMaxImageWidth){
		return decPoint3();
	}
	
	const int glyphCount = size.GetGlyphs().GetCount();
	decPoint3 coords, imageSize;
	int i;
	
	for(i=0; i<glyphCount; i++){
		deFontGlyph &glyph = size.GetGlyphAt(i);
		const int glyphWidth = glyph.GetWidth();
		
		if(coords.x + glyphWidth > pMaxImageWidth){
			imageSize.x = decMath::max(imageSize.x, coords.x - pGlyphSpacing);
			coords.x = 0;
			coords.y += pRealLineHeight + pGlyphSpacing;
			
			if(coords.y + pRealLineHeight > pMaxImageWidth){
				if(sdf){
					coords.y -= pRealLineHeight + pGlyphSpacing;
					continue;
					
				}else{
					imageSize.y = decMath::max(imageSize.y, coords.y - pGlyphSpacing);
					coords.y = 0;
					coords.z++;
				}
			}
		}
		
		glyph.SetX(coords.x);
		glyph.SetY(coords.y + (pRealLineHeight - glyph.GetHeight()) / 2);
		glyph.SetZ(coords.z);
		
		coords.x += glyphWidth + pGlyphSpacing;
	}
	
	imageSize.x = decMath::max(imageSize.x, coords.x - pGlyphSpacing);
	imageSize.y = decMath::max(imageSize.y, coords.y + pRealLineHeight);
	imageSize.z = coords.z + 1;
	
	return imageSize;
}

void deftFont::pDebugPrintGlyphs(const deFont &font, const deFontSize &size){
	const char * const filename = font.GetFilename();
	const int lineHeight = size.GetLineHeight();
	
	size.GetGlyphs().Visit([&](const deFontGlyph &g){
		pModule.LogInfoFormat(
			"LoadFontSize[%s,%d].Glyph: u=0x%x s=(%d,%d) b=(%d,%d) a=%d tc=(%d,%d,%d)",
			filename, lineHeight, g.GetUnicode(), g.GetWidth(), g.GetHeight(), g.GetBearing(),
			g.GetBearingY(), g.GetAdvance(), g.GetX(), g.GetY(), g.GetZ());
	});
}

void deftFont::pRenderGlyphsSdf(const deFontSize &size, deImage &image, const sMeasurements &measurements){
	const int width = image.GetWidth();
	const int height = image.GetHeight();
	const int depth = image.GetDepth();
	const int strideZ = width * height;
	const int strideY = width;
	const int pixelCount = strideZ * depth;
	
	sRGBA8 * const di = image.GetDataRGBA8();
	int glyphIndex = 0;
	for(int x=0; x<pixelCount; x++){
		di[x] = {0, 0, 0, 0};
	}
	
	const double unitScale = measurements.sdfUnitScale;
	const msdfgen::Range range(size.GetSdfPixelRange() / unitScale);
	const int sdfMargin = (int)(std::ceil(size.GetSdfPixelRange() * 0.5f));
	
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	while(gindex != 0){
		if(charcode < 0xffff){
			pModule.AssertFT(FT_Load_Glyph(pFace, gindex, pLoadFlags), "FT_Load_Glyph");
			
			const deFontGlyph &glyph = size.GetGlyphs()[glyphIndex++];
			const int cellWidth = decMath::min(glyph.GetWidth(), width - glyph.GetX());
			const int cellHeight = decMath::min(glyph.GetHeight(), height - glyph.GetY());
			
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
				
				std::vector<float> buffer((size_t)cellWidth * cellHeight * 3);
				msdfgen::BitmapSection<float, 3> output(buffer.data(), cellWidth, cellHeight);
				msdfgen::generateMSDF(output, shape, projection, range);
				
				const auto toByte = [](float v) -> unsigned char{
					return (unsigned char)(decMath::clamp(v, 0.0f, 1.0f) * 255.0f + 0.5f);
				};
				
				for(int y=0; y<cellHeight; y++){
					const int row = cellHeight - 1 - y;
					sRGBA8 *dg = di + strideZ * glyph.GetZ() + strideY * (glyph.GetY() + y) + glyph.GetX();
					for(int x=0; x<cellWidth; x++){
						const float * const sp = buffer.data() + ((size_t)row * cellWidth + x) * 3;
						const float median = decMath::max(decMath::min(sp[0], sp[1]),
							decMath::min(decMath::max(sp[0], sp[1]), sp[2]));
						dg[x] = {toByte(sp[0]), toByte(sp[1]), toByte(sp[2]), toByte(median)};
					}
				}
			}while(false);
		}
		charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
	}
}

int deftFont::pBestSdfFixedSize(const deFont &font){
	const int glyphCount = font.GetGlyphCount();
	if(glyphCount == 0){
		return 48.0f;
	}
	
	// target pixel budget: 2048x2048 = 4,194,304
	// multiply by 0.75 to account for rectangle packing gaps and SDF padding
	const int targetImageSize = 2048;
	const float usableArea = (float)(targetImageSize * targetImageSize) * 0.75f;
	
	float lineHeight = std::sqrt(usableArea / (float)glyphCount);
	
	// clamp line height to the range of 24 (minimum for dense Kanji) and 48 (no real improvement)
	lineHeight = decMath::clamp(lineHeight, 24.0f, 48.0f);
	
	// test with a blown up font at 240px line height:
	// - pixel range 2-4 does not make much difference in general
	// - lineHeight=48, pixelRange=4:
	//   works good but 'r' and 'p' have slight artifacts in the corners
	// - lineHeight=64, pixelRange=4:
	//   better than 48/4 but 'r' and 'p' are still slightly imperfect
	// - lineHeight=64, pixelRange=6:
	//   pretty much the same as 64/4
	// - lineHeight=80, pixelRange=4:
	//   'r' is now good but 'p' still has a slight imperfection
	// - lineHeight=96, pixelRange=4:
	//   nearly identical to 80/4
	//
	// facit:
	// for blown up font 48 is good enough but 64 can give slightly better results but obviously
	// takes a bit longer to generate and the image is somewhat larger.
	//
	// test with a font at common size of 15px line height:
	// - lineHeight=48, pixelRange=4:
	//   letter like 'r' and 'p' are not showing problems on that scale. a certain blurring
	//   happens due to the smaller amount of pixels covered.
	// - lineHeight=96, pixelRange=4:
	//   similar to 48/4 but slightly less blurring
	//
	// facit:
	// for common font size 48 is good enough but higher works too.
	//
	// test with a small font at 8px line height:
	// - lineHeight=48, pixelRange=4:
	//   difficult to read as the thickness tends to fade out.
	// - lineHeight=96, pixelRange=4:
	//   this actually makes the situation worse due to the stronger sharpening
	//
	// facit:
	// with such small fonts reading text is hard anyway due to the limit amount of pixels
	// available to reach glyph. 48 is a good value here but slghtly higher can be acceptable too.
	
	// lineHeight = 48.0f;
	return std::floor(lineHeight * 0.5f) * 2.0f;
}

void deftFont::pRenderGlyphs(const deFontSize &size, deImage &image){
	const int pixelCount = image.GetWidth() * image.GetHeight() * image.GetDepth();
	const int strideZ = image.GetWidth() * image.GetHeight();
	const int strideY = image.GetWidth();
	
	int x, y, glyphIndex = 0;
	FT_UInt gindex;
	FT_ULong charcode = FT_Get_First_Char(pFace, &gindex);
	
	if(image.GetComponentCount() == 2){
		sGrayscaleAlpha8 * const di = image.GetDataGrayscaleAlpha8();
		for(x=0; x<pixelCount; x++){
			di[x] = {255, 0};
		}
		
		while(gindex != 0){
			if(charcode < 0xffff){
				pModule.AssertFT(FT_Load_Glyph(pFace, gindex, pLoadFlags), "FT_Load_Glyph");
				pModule.AssertFT(FT_Render_Glyph(pFace->glyph, FT_RENDER_MODE_NORMAL), "FT_Render_Glyph");
				
				const deFontGlyph &glyph = size.GetGlyphs()[glyphIndex++];
				
				if(pFace->glyph->bitmap.buffer){
					const int rows = pFace->glyph->bitmap.rows;
					
					// hinting can cause the glyph to be slightly larger than the boundary box.
					// since we can not enlarge the image anymore at this point in time we have
					// to clip the glyph. using the bearing the glyph lines up at the top. hence
					// the best we can do is clip the glyph at the bottom. this could be improved
					// by adding a clip area at the bottom of the line as safeguard
					const int safeRows = decMath::min(rows, glyph.GetHeight());
					const int padTop = rows - safeRows;
					
					const int pitch = pFace->glyph->bitmap.pitch;
					
					const unsigned char *bdBase = pFace->glyph->bitmap.buffer;
					if(pitch < 0){
						bdBase += (rows - 1 - padTop) * -pitch;
						
					}else{
						bdBase += padTop * pitch;
					}
					
					// some fonts cause the bearing to be included in the bitmap width although it
					// is not included in the metrics. this is incorrect and causes problems.
					// as a work around calculate the padding on the left side and clip the bitmap
					const unsigned char *bd = bdBase;
					const int realWidth = pFace->glyph->bitmap.width;
					int padLeft = realWidth - 1;
					
					for(y=0; y<safeRows; y++){
						for(x=0; x<realWidth; x++){
							if(bd[x] > 0){
								padLeft = decMath::min(padLeft, x);
								break;
							}
						}
						bd += pitch;
					}
					
					const int width = decMath::min(realWidth - padLeft, glyph.GetWidth());
					
					/*
					pModule.LogInfoFormat("Render: g=0x%x s=(%d,%d) p=%d pm=%d pad=%d", (int)charcode,
						pFace->glyph->bitmap.width, pFace->glyph->bitmap.rows,
						pFace->glyph->bitmap.pitch, pFace->glyph->bitmap.pixel_mode, padLeft);
					*/
					
					sGrayscaleAlpha8 *dg = di + strideZ * glyph.GetZ() + strideY * glyph.GetY() + glyph.GetX();
					bd = bdBase + padLeft;
					
					for(y=0; y<safeRows; y++){
						for(x=0; x<width; x++){
							dg[x].alpha = bd[x];
						}
						dg += strideY;
						bd += pitch;
					}
				}
			}
			charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
		}
		
	}else if(image.GetComponentCount() == 4){
		sRGBA8 * const di = image.GetDataRGBA8();
		for(x=0; x<pixelCount; x++){
			di[x] = {255, 255, 255, 0};
		}
		
		while(gindex != 0){
			if(charcode < 0xffff){
				pModule.AssertFT(FT_Load_Glyph(pFace, gindex, pLoadFlags), "FT_Load_Glyph");
				pModule.AssertFT(FT_Render_Glyph(pFace->glyph, FT_RENDER_MODE_NORMAL), "FT_Render_Glyph");
				
				const deFontGlyph &glyph = size.GetGlyphs()[glyphIndex++];
				
				if(pFace->glyph->bitmap.buffer){
					const int rows = pFace->glyph->bitmap.rows;
					
					// hinting can cause the glyph to be slightly larger than the boundary box.
					// since we can not enlarge the image anymore at this point in time we have
					// to clip the glyph. using the bearing the glyph lines up at the top. hence
					// the best we can do is clip the glyph at the bottom. this could be improved
					// by adding a clip area at the bottom of the line as safeguard
					const int safeRows = decMath::min(rows, glyph.GetHeight());
					const int padTop = rows - safeRows;
					
					const int pitch = pFace->glyph->bitmap.pitch;
					
					const unsigned char *bdBase = pFace->glyph->bitmap.buffer;
					if(pitch < 0){
						bdBase += (rows - 1 - padTop) * -pitch;
						
					}else{
						bdBase += padTop * pitch;
					}
					
					// some fonts cause the bearing to be included in the bitmap width although it
					// is not included in the metrics. this is incorrect and causes problems.
					// as a work around calculate the padding on the left side and clip the bitmap
					const unsigned char *bd = bdBase;
					const int realWidth = pFace->glyph->bitmap.width;
					int padLeft = realWidth - 1;
					
					for(y=0; y<safeRows; y++){
						const sFTColorBGRA * const bg = (sFTColorBGRA*)bd;
						for(x=0; x<realWidth; x++){
							if(bg[x].alpha > 0){
								padLeft = decMath::min(padLeft, x);
								break;
							}
						}
						bd += pitch;
					}
					
					const int width = decMath::min(realWidth - padLeft, glyph.GetWidth());
					
					/*
					pModule.LogInfoFormat("Render: g=0x%x s=(%d,%d) p=%d pm=%d pad=(%d,%d)", (int)charcode,
						pFace->glyph->bitmap.width, pFace->glyph->bitmap.rows,
						pFace->glyph->bitmap.pitch, pFace->glyph->bitmap.pixel_mode, padLeft, padTop);
					*/
					
					sRGBA8 *dg = di + strideZ * glyph.GetZ() + strideY * glyph.GetY() + glyph.GetX();
					bd = bdBase + sizeof(sFTColorBGRA) * padLeft;
					
					for(y=0; y<safeRows; y++){
						const sFTColorBGRA * const bg = (sFTColorBGRA*)bd;
						for(x=0; x<width; x++){
							dg[x].red = bg[x].red;
							dg[x].green = bg[x].green;
							dg[x].blue = bg[x].blue;
							dg[x].alpha = bg[x].alpha;
						}
						dg += strideY;
						bd += pitch;
					}
				}
			}
			charcode = FT_Get_Next_Char(pFace, charcode, &gindex);
		}
	}
}


unsigned long deftFont::fStreamRead(FT_Stream stream, unsigned long offset,
unsigned char *buffer, unsigned long count){
	deftFont &font = *(deftFont*)stream->descriptor.pointer;
	
	if(count == 0){
		try{
			font.pReader.SetPosition((int)offset);
			return 0;
			
		}catch(const deException &e){
			font.pModule.LogException(e);
			return 1;
		}
		
	}else{
		font.pReader.Read(buffer, (int)count);
		return count;
	}
}

void deftFont::fStreamClose(FT_Stream){
}
