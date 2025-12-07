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

#include "deoglSCBuildConstructed.h"
#include "deoglSkinChannel.h"
#include "../../texture/deoglImage.h"
#include "../../texture/deoglRImage.h"
#include "../../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../../texture/pixelbuffer/deoglPixelBufferMipMap.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUTF8Decoder.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>



// Constants
//////////////

#define IMG_CONVERT16 (1.0f / 65535.0f)
#define IMG_CONVERT8 (1.0f / 255.0f)



// Struct deoglSCBuildConstructed::sContext
/////////////////////////////////////////////

deoglSCBuildConstructed::sContext::sContext() :
parent(NULL),
child(NULL),
root(this),
mask(NULL),
clipTo(0, 0, 1),
transparency(1.0f),
gamma(1.0f){
}

deoglSCBuildConstructed::sContext::sContext(const deSkinPropertyNode &node, sContext *pparent) :
parent(pparent),
child(NULL),
root(pparent ? pparent->root : this),
mask(NULL)
{
	const decPoint sizeAbs(abs(node.GetSize().x), abs(node.GetSize().y));
	clamp.x = decMath::max(sizeAbs.x - 1, 0);
	clamp.y = decMath::max(sizeAbs.y - 1, 0);
	
	const decVector2 offset((float)sizeAbs.x * 0.5f, (float)sizeAbs.y * 0.5f);
	const decVector2 position((float)node.GetPosition().x, (float)node.GetPosition().y);
	
	transformScreen =
		decTexMatrix2::CreateScale(
			node.GetSize().x < 0.0f ? -1.0f : 1.0f,
			node.GetSize().y < 0.0f ? -1.0f : 1.0f)
		* decTexMatrix2::CreateTranslation( -offset )
		* decTexMatrix2::CreateShear( tanf( node.GetShear() ), 0.0f )
		* decTexMatrix2::CreateRotation( node.GetRotation() )
		* decTexMatrix2::CreateTranslation( offset + position );
	
	transformInverse = transformScreen.Invert().ToTexMatrix2();
	
	if(pparent){
		transformScreen *= pparent->transformScreen;
	}
	
	transformColor = decColorMatrix::CreateContrast(node.GetContrast())
		* decColorMatrix::CreateBrightness( node.GetBrightness() )
		* decColorMatrix::CreateScaling( node.GetColorize() );
	transparency = node.GetTransparency();
	gamma = node.GetGamma();
	
	if(pparent){
		const decVector2 p1(transformScreen.GetPosition());
		const decVector2 p2(transformScreen * decVector2((float)clamp.x, 0.0f));
		const decVector2 p3(transformScreen * decVector2(0.0f, (float)clamp.y));
		const decVector2 p4(transformScreen * decVector2(clamp));
		
		const decPoint smallest(p1.Smallest(p2).Smallest(p3).Smallest(p4).Round());
		const decPoint largest(p1.Largest(p2).Largest(p3).Largest(p4).Round());
		
		localClipFrom.Set(smallest.x, smallest.y, node.GetPosition().z);
		localClipTo.Set(largest.x, largest.y, node.GetPosition().z + node.GetSize().z);
		clipFrom = localClipFrom.Largest(pparent->clipFrom);
		clipTo = localClipTo.Smallest(pparent->clipTo);
		
		transformColor *= pparent->transformColor;
		transparency *= pparent->transparency;
		gamma *= pparent->gamma;
		
		pparent->child = this;
		
	}else{
		clipTo.x = decMath::max(sizeAbs.x - 1, 0);
		clipTo.y = decMath::max(sizeAbs.y - 1, 0);
		clipTo.z = node.GetSize().z;
		localClipTo = clipTo;
	}
	
	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
	//
	// to properly support groups each group level has to store a pixel array for the
	// size of the node. child nodes have to be applied to this pixel array. once
	// finished the entire pixel array is applied to the parent. only this way
	// transparency and other modifications are properly applied
	//
	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
}

decColor deoglSCBuildConstructed::sContext::applyGamma(const decColor &color) const{
	return decColor(powf(color.r, gamma), powf(color.g, gamma), powf(color.b, gamma));
}

deoglSCBuildConstructed::sTarget::sTarget() :
targetRed(100),
targetGreen(100),
targetBlue(100),
targetAlpha(100),

pixBufComponentCount(0),
pixBufDataFloat(NULL),
pixBufDataByte(NULL),
pixBufWidth(0),
pixBufHeight(0),
pixBufDepth(0),

tileX(false),
tileY(false){
}



// Class deoglSCBuildConstructed
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSCBuildConstructed::deoglSCBuildConstructed(deoglSkinChannel &channel) :
pChannel(channel),
pContext(NULL),
pTarget(NULL){
}

deoglSCBuildConstructed::~deoglSCBuildConstructed(){
}



// Management
///////////////

bool deoglSCBuildConstructed::BuildFromProperty(const deSkinPropertyConstructed &property,
int targetRed, int targetGreen, int targetBlue, int targetAlpha){
	sTarget target;
	if(! pInitPixelBuffer(target)){
		return false; // something went wrong
	}
	target.targetRed = targetRed;
	target.targetGreen = targetGreen;
	target.targetBlue = targetBlue;
	target.targetAlpha = targetAlpha;
	target.tileX = property.GetTileX();
	target.tileY = property.GetTileY();
	target.textureSize = property.GetContent().GetSize();
	if(target.textureSize.z > target.pixBufDepth){
		target.textureSize.z = target.pixBufDepth;
	}
	
	pTarget = &target;
	pFillBackground(property.GetColor());
	
	sContext context(property.GetContent(), NULL);
	if(context.localClipTo.z > target.pixBufDepth){
		context.localClipTo.z = target.pixBufDepth;
	}
	if(context.clipTo.z > target.pixBufDepth){
		context.clipTo.z = target.pixBufDepth;
	}
	
	pContext = &context;
	property.GetContent().Visit(*this);
	pContext = NULL;
	pTarget = NULL;
	
	return true;
}



// Visiting
/////////////

void deoglSCBuildConstructed::VisitGroup(deSkinPropertyNodeGroup &node){
	const int count = node.GetNodeCount();
	if(count == 0){
		return;
	}
	
	if(pContext->clamp.x == 0 || pContext->clamp.y == 0){
		return;
	}
	
	sContext * const context = pContext;
	sTarget target;
	pDrawMaskIfPresent(node, target);
	
	const bool tileX = pTarget->tileX;
	const bool tileY = pTarget->tileY;
	const decPoint3 &textureSize = pTarget->textureSize;
	const bool hasTiling = ! context->parent && (tileX || tileY);
	int i;
	
	for(i=0; i<count; i++){
		deSkinPropertyNode &childNode = *node.GetNodeAt(i);
		
		sContext childContext(childNode, context);
		if(childContext.clamp.x == 0 || childContext.clamp.y == 0){
			continue; // node has zero size
		}
		if(! (childContext.clipTo > childContext.clipFrom)){
			continue; // completely clamped by parent
		}
		
		pContext = &childContext;
		
		childNode.Visit(*this);
		
		// tileing
		if(! hasTiling){
			continue;
		}
		
		if(childContext.localClipFrom >= decPoint3() && childContext.localClipTo < textureSize){
			continue; // fully inside texture. no tiling required
		}
		
		const decTexMatrix2 transformScreen(childContext.transformScreen);
		const decTexMatrix2 transformInverse(childContext.transformInverse);
		
		if(tileX && tileY){
			pTileNode(decPoint(textureSize.x, textureSize.y), transformScreen, transformInverse, childNode);
			pTileNode(decPoint(0, textureSize.y), transformScreen, transformInverse, childNode);
			pTileNode(decPoint(-textureSize.x, textureSize.y), transformScreen, transformInverse, childNode);
			
			pTileNode(decPoint(textureSize.x, 0), transformScreen, transformInverse, childNode);
			pTileNode(decPoint(-textureSize.x, 0), transformScreen, transformInverse, childNode);
			
			pTileNode(decPoint(textureSize.x, -textureSize.y), transformScreen, transformInverse, childNode);
			pTileNode(decPoint(0, -textureSize.y), transformScreen, transformInverse, childNode);
			pTileNode(decPoint(-textureSize.x, -textureSize.y), transformScreen, transformInverse, childNode);
			
		}else if(tileX){
			pTileNode(decPoint(textureSize.x, 0), transformScreen, transformInverse, childNode);
			pTileNode(decPoint(-textureSize.x, 0), transformScreen, transformInverse, childNode);
			
		}else if(tileY){
			pTileNode(decPoint(0, textureSize.y), transformScreen, transformInverse, childNode);
			pTileNode(decPoint(0, -textureSize.y), transformScreen, transformInverse, childNode);
		}
	}
}

void deoglSCBuildConstructed::VisitImage(deSkinPropertyNodeImage &node){
	const deImage * const image = node.GetImage();
	if(! image){
		return;
	}
	
	sTarget target;
	pDrawMaskIfPresent(node, target);
	
	sContext &context = *pContext;
	const decVector2 tcScale(
		((float)(image->GetWidth() - 1) / (float)context.clamp.x) * (float)node.GetRepeat().x,
		((float)(image->GetHeight() - 1) / (float)context.clamp.y) * (float)node.GetRepeat().y);
	
	pDrawImage(context, *image, tcScale, decVector2(), 0);
}

void deoglSCBuildConstructed::VisitShape(deSkinPropertyNodeShape &node){
	if(node.GetSize().x == 0 || node.GetSize().y == 0 || node.GetSize().z == 0){
		return;
	}
	
	sContext &context = *pContext;
	sTarget target;
	pDrawMaskIfPresent(node, target);
	
	const float thickness = node.GetThickness();
	const decColor &colorLine = node.GetLineColor();
	const decColor &colorFill = node.GetFillColor();
	
	// paint image
	const int strideLine = pTarget->pixBufWidth * pTarget->pixBufComponentCount;
	const int strideImage = strideLine * pTarget->pixBufHeight;
	const int pixBufComponentCount = pTarget->pixBufComponentCount;
	const int depth = context.clipTo.z - context.clipFrom.z;
	decVector2 position;
	float maskAlpha;
	decColor color;
	decPoint i;
	int z;
	
	switch(node.GetShapeType()){
	case deSkinPropertyNodeShape::estRectangle:{
		const decVector2 innerFrom(thickness, thickness);
		const decVector2 innerTo(
			(float)context.clamp.x - thickness,
			(float)context.clamp.y - thickness);
		
		for(i.y=context.clipFrom.y; i.y<=context.clipTo.y; i.y++){
			int offset = strideImage * context.clipFrom.z + strideLine * i.y
				+ pixBufComponentCount * context.clipFrom.x;
			
			for(i.x=context.clipFrom.x; i.x<=context.clipTo.x; i.x++){
				// transform position from root up to this context clipping at each step
				sContext *marchContext = context.root;
				
				position.x = (float)i.x;
				position.y = (float)i.y;
				maskAlpha = 1.0f;
				
				while(marchContext){
					position = marchContext->transformInverse * position;
					const decPoint rounded(position.Round());
					
					if(! (rounded >= decPoint() && rounded <= marchContext->clamp)){
						break;
					}
					
					if(marchContext->mask){
						maskAlpha *= (float)marchContext->mask->pixBufDataFloat[
							marchContext->mask->pixBufWidth * rounded.y + rounded.x];
					}
					
					marchContext = marchContext->child;
				}
				
				if(marchContext){
					offset += pixBufComponentCount;
					continue;
				}
				
				// paint shape
				if(position >= innerFrom && position <= innerTo){
					color = colorFill;
					
				}else{
					color = colorLine;
				}
				
				color.a *= maskAlpha;
				
				for(z=0; z<depth; z++){
					pWritePixel(context, offset + strideImage * z, color);
				}
				
				offset += pixBufComponentCount;
			}
		}
		}break;
		
	case deSkinPropertyNodeShape::estEllipse:{
		const decPoint size(node.GetSize().x - 1, node.GetSize().y - 1);
		const decVector2 center((float)size.x * 0.5f, (float)size.y * 0.5f);
		const decPoint absSize(size.Absolute());
		const decVector2 axisLen((float)absSize.x * 0.5f, (float)absSize.y * 0.5f);
		const float scale = absSize.y > 0 ? axisLen.x / axisLen.y : 1.0f;
		const float invScale = 1.0f / scale;
		const float unitThreshold = axisLen.y + 0.5f;
		const float thicknessThreshold = thickness + 0.5f;
		
		for(i.y=context.clipFrom.y; i.y<=context.clipTo.y; i.y++){
			int offset = strideImage * context.clipFrom.z + strideLine * i.y
				+ pixBufComponentCount * context.clipFrom.x;
			
			for(i.x=context.clipFrom.x; i.x<=context.clipTo.x; i.x++){
				// transform position from root up to this context clipping at each step
				position.x = (float)i.x;
				position.y = (float)i.y;
				maskAlpha = 1.0f;
				
				sContext *marchContext = context.root;
				while(marchContext){
					position = marchContext->transformInverse * position;
					const decPoint rounded(position.Round());
					
					if(! (rounded >= decPoint() && rounded <= marchContext->clamp)){
						break;
					}
					
					if(marchContext->mask){
						maskAlpha *= (float)marchContext->mask->pixBufDataFloat[
							marchContext->mask->pixBufWidth * rounded.y + rounded.x];
					}
					
					marchContext = marchContext->child;
				}
				
				if(marchContext){
					offset += pixBufComponentCount;
					continue;
				}
				
				// paint shape
				const decVector2 relPos(position - center);
				const decVector2 unitRelPos(relPos.x * invScale, relPos.y);
				const float unitRelPosLen = unitRelPos.Length();
				if(unitRelPosLen > unitThreshold){
					offset += pixBufComponentCount;
					continue;
				}
				
				decVector2 ellipsePos;
				if(unitRelPosLen > 0.0f){
					ellipsePos = unitRelPos * (axisLen.y / unitRelPosLen);
					ellipsePos.x *= scale;
				}
				
				if(relPos.Length() < ellipsePos.Length() - thicknessThreshold){
					color = colorFill;
					
				}else{
					color = colorLine;
				}
				
				color.a *= maskAlpha;
				
				for(z=0; z<depth; z++){
					pWritePixel(context, offset + strideImage * z, color);
				}
				
				offset += pixBufComponentCount;
			}
		}
		}break;
	}
}

void deoglSCBuildConstructed::VisitText(deSkinPropertyNodeText &node){
	deFont * const font = node.GetFont();
	if(!font){
		return;
	}
	
	const float textSize = node.GetFontSize();
	const deFontSize * const size = font->EnsureSizePrepared((int)(textSize + 0.5f));
	
	const deImage * const image = (size ? size->GetImage() : font->GetImage());
	if(!image){
		return;
	}
	
	sContext &context = *pContext;
	sTarget target;
	pDrawMaskIfPresent(node, target);
	
	const int lineHeight = size ? size->GetLineHeight() : font->GetLineHeight();
	
	decUTF8Decoder utf8Decoder;
	utf8Decoder.SetString(node.GetText());
	const int len = utf8Decoder.GetLength();
	const float fontScale = textSize / (float)lineHeight;
	
	// image parameters required for painting
	const decTexMatrix2 transformInverse(context.transformInverse);
	const decVector2 tcScale((float)lineHeight / textSize, (float)lineHeight / textSize);
	const decPoint3 clipFrom(context.clipFrom);
	const decPoint3 clipTo(context.clipTo);
	float glyphPosition = 0.0f;
	
	// context.clamp.y = (int)(textSize - 0.5f); // (fontsize - 1) + 0.5
	
	if(!font->GetIsColorFont()){
		decColor fontColor(node.GetColor());
		fontColor.r = powf(fontColor.r, node.GetGamma());
		fontColor.g = powf(fontColor.g, node.GetGamma());
		fontColor.b = powf(fontColor.b, node.GetGamma());
		context.transformColor *= decColorMatrix::CreateScaling(fontColor);
	}
	
	while(utf8Decoder.GetPosition() < len){
		const int character = utf8Decoder.DecodeNextCharacter();
		if(character < 0){
			continue;
		}
		
		const deFontGlyph &glyph = font->GetGlyph(character, size);
		if(glyph.GetWidth() == 0 || glyph.GetHeight() == 0){
			continue;
		}
		
		// adjust transformation and clamping
		const float realGlyphPosition = glyphPosition - (float)glyph.GetBearing() * fontScale;
		const float glyphWidth = (float)glyph.GetWidth() * fontScale;
		const float glyphHeight = (float)glyph.GetHeight() * fontScale;
		const float glyphY = -(float)glyph.GetBearingY() * fontScale;
		
		context.transformInverse.a13 = transformInverse.a13 - realGlyphPosition;
		context.transformInverse.a23 = transformInverse.a23 - glyphY;
		context.clamp.x = (int)(glyphWidth - 0.5f); // (glyphWidth - 1) + 0.5
		context.clamp.y = (int)(glyphY + glyphHeight - 0.5f);
		
		// calculate clip boundaries
		const float x2 = realGlyphPosition + glyphWidth - 1.0f;
		
		const decVector2 p1(context.transformScreen * decVector2(realGlyphPosition, glyphY));
		const decVector2 p2(context.transformScreen * decVector2(x2, glyphY));
		const decVector2 p3(context.transformScreen * decVector2(realGlyphPosition, (float)context.clamp.y));
		const decVector2 p4(context.transformScreen * decVector2(x2, (float)context.clamp.y));
		
		const decPoint smallest(p1.Smallest(p2).Smallest(p3).Smallest(p4).Round());
		const decPoint largest(p1.Largest(p2).Largest(p3).Largest(p4).Round());
		
		context.clipFrom = decPoint3(smallest.x, smallest.y, clipFrom.z).Largest(clipFrom);
		context.clipTo = decPoint3(largest.x, largest.y, clipTo.z).Smallest(clipTo);
		
		// draw glyph and advance to next position
		const decVector2 tcOffset((float)glyph.GetX(), (float)glyph.GetY());
		pDrawImage(context, *image, tcScale, tcOffset, glyph.GetZ());
		
		glyphPosition += (float)glyph.GetAdvance() * fontScale;
	}
}



// Private Functions
//////////////////////

bool deoglSCBuildConstructed::pInitPixelBuffer(sTarget &target){
	const deoglPixelBufferMipMap * const pixelBufferMipMap = pChannel.GetPixelBufferMipMap();
	if(! pixelBufferMipMap){
		return false; // if the pixel buffer mip map is missing something went wrong
	}
	
	if(pixelBufferMipMap->GetPixelBufferCount() == 0){
		return false;
	}
	
	deoglPixelBuffer &pixelBuffer = pixelBufferMipMap->GetPixelBuffer(0);
	
	switch(pixelBuffer.GetFormat()){
	case deoglPixelBuffer::epfByte1:
		target.pixBufComponentCount = 1;
		target.pixBufDataFloat = NULL;
		target.pixBufDataByte = (GLubyte*)pixelBuffer.GetPointer();
		break;
		
	case deoglPixelBuffer::epfByte2:
		target.pixBufComponentCount = 2;
		target.pixBufDataFloat = NULL;
		target.pixBufDataByte = (GLubyte*)pixelBuffer.GetPointer();
		break;
		
	case deoglPixelBuffer::epfByte3:
		target.pixBufComponentCount = 3;
		target.pixBufDataFloat = NULL;
		target.pixBufDataByte = (GLubyte*)pixelBuffer.GetPointer();
		break;
		
	case deoglPixelBuffer::epfByte4:
		target.pixBufComponentCount = 4;
		target.pixBufDataFloat = NULL;
		target.pixBufDataByte = (GLubyte*)pixelBuffer.GetPointer();
		break;
		
	case deoglPixelBuffer::epfFloat1:
		target.pixBufComponentCount = 1;
		target.pixBufDataByte = NULL;
		target.pixBufDataFloat = (GLfloat*)pixelBuffer.GetPointer();
		break;
		
	case deoglPixelBuffer::epfFloat2:
		target.pixBufComponentCount = 2;
		target.pixBufDataByte = NULL;
		target.pixBufDataFloat = (GLfloat*)pixelBuffer.GetPointer();
		break;
		
	case deoglPixelBuffer::epfFloat3:
		target.pixBufComponentCount = 3;
		target.pixBufDataByte = NULL;
		target.pixBufDataFloat = (GLfloat*)pixelBuffer.GetPointer();
		break;
		
	case deoglPixelBuffer::epfFloat4:
		target.pixBufComponentCount = 4;
		target.pixBufDataByte = NULL;
		target.pixBufDataFloat = (GLfloat*)pixelBuffer.GetPointer();
		break;
		
	default:
		// this should never happen
		return false;
	}
	
	target.pixBufWidth = pixelBuffer.GetWidth();
	target.pixBufHeight = pixelBuffer.GetHeight();
	target.pixBufDepth = pixelBuffer.GetDepth();
	
	return true;
}

void deoglSCBuildConstructed::pDrawMaskIfPresent(const deSkinPropertyNode &node, sTarget &target){
	if(! node.GetMask()){
		return;
	}
	
	const decPoint3 absSize(node.GetSize().Absolute());
	target.maskBuffer.TakeOver(new deoglPixelBuffer(deoglPixelBuffer::epfFloat1, absSize.x, absSize.y, 1));
	target.pixBufWidth = absSize.x;
	target.pixBufHeight = absSize.y;
	target.pixBufDepth = 1;
	target.pixBufComponentCount = 1;
	target.pixBufDataFloat = (GLfloat*)target.maskBuffer->GetPointer();
	target.targetRed = 0;
	target.textureSize.Set(absSize.x, absSize.y, 1);
	memset(target.pixBufDataFloat, 0, sizeof(GLfloat) * absSize.x * absSize.y * 1);
	
	sContext maskContext;
	maskContext.clamp.x = absSize.x - 1;
	maskContext.clamp.y = absSize.y - 1;
	maskContext.clipTo.x = maskContext.clamp.x;
	maskContext.clipTo.y = maskContext.clamp.y;
	
	sContext childContext(*node.GetMask(), &maskContext);
	
	sTarget * const guardTarget = pTarget;
	sContext * const guardContext = pContext;
	
	pTarget = &target;
	pContext = &childContext;
	
	node.GetMask()->Visit(*this);
	
	pTarget = guardTarget;
	pContext = guardContext;
	
	guardContext->mask = &target;
}

void deoglSCBuildConstructed::pFillBackground(const decColor &color){
	const int pixelCount = pTarget->pixBufWidth * pTarget->pixBufHeight * pTarget->pixBufDepth;
	const int pixBufComponentCount = pTarget->pixBufComponentCount;
	const int targetRed = pTarget->targetRed;
	const int targetGreen = pTarget->targetGreen;
	const int targetBlue = pTarget->targetBlue;
	const int targetAlpha = pTarget->targetAlpha;
	int i;
	
	if(pTarget->pixBufDataByte){
		GLubyte *ptrDest = pTarget->pixBufDataByte;
		
		const GLubyte r = (GLubyte)decMath::clamp((int)(color.r * 255.0), 0, 255);
		const GLubyte g = (GLubyte)decMath::clamp((int)(color.g * 255.0), 0, 255);
		const GLubyte b = (GLubyte)decMath::clamp((int)(color.b * 255.0), 0, 255);
		const GLubyte a = (GLubyte)decMath::clamp((int)(color.a * 255.0), 0, 255);
		
		for(i=0; i<pixelCount; i++){
			if(targetRed < pixBufComponentCount){
				ptrDest[targetRed] = r;
			}
			if(targetGreen < pixBufComponentCount){
				ptrDest[targetGreen] = g;
			}
			if(targetBlue < pixBufComponentCount){
				ptrDest[targetBlue] = b;
			}
			if(targetAlpha < pixBufComponentCount){
				ptrDest[targetAlpha] = a;
			}
			ptrDest += pixBufComponentCount;
		}
		
	}else if(pTarget->pixBufDataFloat){
		GLfloat *ptrDest = pTarget->pixBufDataFloat;
		
		const GLfloat r = (GLfloat)color.r;
		const GLfloat g = (GLfloat)color.g;
		const GLfloat b = (GLfloat)color.b;
		const GLfloat a = (GLfloat)color.a;
		
		for(i=0; i<pixelCount; i++){
			if(targetRed < pixBufComponentCount){
				ptrDest[targetRed] = r;
			}
			if(targetGreen < pixBufComponentCount){
				ptrDest[targetGreen] = g;
			}
			if(targetBlue < pixBufComponentCount){
				ptrDest[targetBlue] = b;
			}
			if(targetAlpha < pixBufComponentCount){
				ptrDest[targetAlpha] = a;
			}
			ptrDest += pixBufComponentCount;
		}
	}
}

void deoglSCBuildConstructed::pTileNode(const decPoint &offset, const decTexMatrix2 &transformScreen,
const decTexMatrix2 &transformInverse, deSkinPropertyNode &childNode){
	pContext->clipFrom = pContext->localClipFrom;
	pContext->clipFrom.x += offset.x;
	pContext->clipFrom.y += offset.y;
	pContext->clipTo = pContext->localClipTo;
	pContext->clipTo.x += offset.x;
	pContext->clipTo.y += offset.y;
	
	if(! (pContext->clipTo > decPoint3() && pContext->clipFrom < pTarget->textureSize)){
		return;
	}
	
	pContext->transformScreen.a13 = transformScreen.a13 + (float)offset.x;
	pContext->transformScreen.a23 = transformScreen.a23 + (float)offset.y;
	
	pContext->transformInverse = decTexMatrix2::CreateTranslation(decVector2(-offset)) * transformInverse;
	
	pContext->clipFrom = pContext->clipFrom.Largest(pContext->parent->clipFrom);
	pContext->clipTo = pContext->clipTo.Smallest(pContext->parent->clipTo);
	
	childNode.Visit(*this);
}

void deoglSCBuildConstructed::pDrawImage(const sContext &context, const deImage &image,
const decVector2 &tcScale, const decVector2 &tcOffset, int layer){
	// obtain appropriate data pointer for image
	const sGrayscale8 *imgGrayscale8 = nullptr;
	const sGrayscale16 *imgGrayscale16 = nullptr;
	const sGrayscale32 *imgGrayscale32 = nullptr;
	const sGrayscaleAlpha8 *imgGrayscaleAlpha8 = nullptr;
	const sGrayscaleAlpha16 *imgGrayscaleAlpha16 = nullptr;
	const sGrayscaleAlpha32 *imgGrayscaleAlpha32 = nullptr;
	const sRGB8 *imgRGB8 = nullptr;
	const sRGB16 *imgRGB16 = nullptr;
	const sRGB32 *imgRGB32 = nullptr;
	const sRGBA8 *imgRGBA8 = nullptr;
	const sRGBA16 *imgRGBA16 = nullptr;
	const sRGBA32 *imgRGBA32 = nullptr;
	const deoglPixelBuffer::sByte1 *imgPbByte1 = nullptr;
	const deoglPixelBuffer::sByte2 *imgPbByte2 = nullptr;
	const deoglPixelBuffer::sByte3 *imgPbByte3 = nullptr;
	const deoglPixelBuffer::sByte4 *imgPbByte4 = nullptr;
	const deoglPixelBuffer::sFloat1 *imgPbFloat1 = nullptr;
	const deoglPixelBuffer::sFloat2 *imgPbFloat2 = nullptr;
	const deoglPixelBuffer::sFloat3 *imgPbFloat3 = nullptr;
	const deoglPixelBuffer::sFloat4 *imgPbFloat4 = nullptr;
	bool hasAlpha = false;
	
	deoglPixelBuffer * const pixelBuffer = ((deoglImage*)image.GetPeerGraphic())->GetPixelBuffer();
	
	if(pixelBuffer){
		// image skin pixel buffer is used for memory optimization
		
		switch(pixelBuffer->GetFormat()){
		case deoglPixelBuffer::epfByte1:
			imgPbByte1 = pixelBuffer->GetPointerByte1();
			break;
			
		case deoglPixelBuffer::epfByte2:
			imgPbByte2 = pixelBuffer->GetPointerByte2();
			hasAlpha = true;
			break;
			
		case deoglPixelBuffer::epfByte3:
			imgPbByte3 = pixelBuffer->GetPointerByte3();
			break;
			
		case deoglPixelBuffer::epfByte4:
			imgPbByte4 = pixelBuffer->GetPointerByte4();
			hasAlpha = true;
			break;
			
		case deoglPixelBuffer::epfFloat1:
			imgPbFloat1 = pixelBuffer->GetPointerFloat1();
			break;
			
		case deoglPixelBuffer::epfFloat2:
			imgPbFloat2 = pixelBuffer->GetPointerFloat2();
			hasAlpha = true;
			break;
			
		case deoglPixelBuffer::epfFloat3:
			imgPbFloat3 = pixelBuffer->GetPointerFloat3();
			break;
			
		case deoglPixelBuffer::epfFloat4:
			imgPbFloat4 = pixelBuffer->GetPointerFloat4();
			hasAlpha = true;
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		
	}else if(image.GetData()){
		// direct memory access is used for regular mode and as fallback
		
		if(image.GetBitCount() == 8){
			if(image.GetComponentCount() == 1){
				imgGrayscale8 = image.GetDataGrayscale8();
				
			}else if(image.GetComponentCount() == 2){
				imgGrayscaleAlpha8 = image.GetDataGrayscaleAlpha8();
				hasAlpha = true;
				
			}else if(image.GetComponentCount() == 3){
				imgRGB8 = image.GetDataRGB8();
				
			}else{
				imgRGBA8 = image.GetDataRGBA8();
				hasAlpha = true;
			}
			
		}else if(image.GetBitCount() == 16){
			if(image.GetComponentCount() == 1){
				imgGrayscale16 = image.GetDataGrayscale16();
				
			}else if(image.GetComponentCount() == 2){
				imgGrayscaleAlpha16 = image.GetDataGrayscaleAlpha16();
				hasAlpha = true;
				
			}else if(image.GetComponentCount() == 3){
				imgRGB16 = image.GetDataRGB16();
				
			}else{
				imgRGBA16 = image.GetDataRGBA16();
				hasAlpha = true;
			}
			
		}else{
			if(image.GetComponentCount() == 1){
				imgGrayscale32 = image.GetDataGrayscale32();
				
			}else if(image.GetComponentCount() == 2){
				imgGrayscaleAlpha32 = image.GetDataGrayscaleAlpha32();
				hasAlpha = true;
				
			}else if(image.GetComponentCount() == 3){
				imgRGB32 = image.GetDataRGB32();
				
			}else{
				imgRGBA32 = image.GetDataRGBA32();
				hasAlpha = true;
			}
		}
		
	}else{
		DETHROW(deeInvalidParam);
	}
	
	// image parameters required for painting
	const int imgWidth = image.GetWidth();
	const int imgHeight = image.GetHeight();
	const int imgSize = imgWidth * imgHeight;
	
	// paint image
	const int strideLine = pTarget->pixBufWidth * pTarget->pixBufComponentCount;
	const int strideImage = strideLine * pTarget->pixBufHeight;
	const int pixBufComponentCount = pTarget->pixBufComponentCount;
	const int depth = context.clipTo.z - context.clipFrom.z;
	float boxFilterBlends[4];
	decPoint texCoord[4];
	float boxFilterDummy;
	decVector2 position;
	decPoint center;
	float maskAlpha;
	decPoint i;
	int j, z;
	
	for(i.y=context.clipFrom.y; i.y<=context.clipTo.y; i.y++){
		int offset = strideImage * context.clipFrom.z + strideLine * i.y
			+ pixBufComponentCount * context.clipFrom.x;
		
		for(i.x=context.clipFrom.x; i.x<=context.clipTo.x; i.x++){
			// transform position from root up to this context clipping at each step
			sContext *marchContext = context.root;
			
			position.x = (float)i.x;
			position.y = (float)i.y;
			maskAlpha = 1.0f;
			
			while(marchContext){
				position = marchContext->transformInverse * position;
				center = position.Round();
				
				if(! (center >= decPoint() && center <= marchContext->clamp)){
					break;
				}
				
				if(marchContext->mask){
					maskAlpha *= (float)marchContext->mask->pixBufDataFloat[
						marchContext->mask->pixBufWidth * center.y + center.x];
				}
				
				marchContext = marchContext->child;
			}
			
			if(marchContext){
				offset += pixBufComponentCount;
				continue;
			}
			
			// calculate box filter parameters
			const float imgRealX = (float)center.x * tcScale.x + tcOffset.x;
			const float imgRealY = (float)center.y * tcScale.y + tcOffset.y;
			
			const float boxFilterBlendX = modff(imgRealX, &boxFilterDummy);
			const float boxFilterBlendY = modff(imgRealY, &boxFilterDummy);
			const int boxFilterX = (int)(imgRealX + 0.0001f);
			const int boxFilterY = (int)(imgRealY + 0.0001f);
			
			texCoord[0].Set(boxFilterX % imgWidth, boxFilterY % imgHeight);
			texCoord[1].Set((boxFilterX + 1) % imgWidth, texCoord[0].y);
			texCoord[2].Set(texCoord[0].x, (boxFilterY + 1) % imgHeight);
			texCoord[3].Set(texCoord[1].x, texCoord[2].y);
			
			boxFilterBlends[0] = (1.0f - boxFilterBlendX) * (1.0f - boxFilterBlendY);
			boxFilterBlends[1] = boxFilterBlendX * (1.0f - boxFilterBlendY);
			boxFilterBlends[2] = (1.0f - boxFilterBlendX) * boxFilterBlendY;
			boxFilterBlends[3] = boxFilterBlendX * boxFilterBlendY;
			
			// calculate final color using box filter
			decColor color(0.0f, 0.0f, 0.0f, hasAlpha ? 0.0f : 1.0f);
			
			for(j=0; j<4; j++){
				const int imgOffset = imgSize * layer + imgWidth * texCoord[j].y + texCoord[j].x;
				
				// memory optimization
				if(imgPbByte1){
					const float value = ((float)imgPbByte1[imgOffset].r * IMG_CONVERT8) * boxFilterBlends[j];
					color.r += value;
					color.g += value;
					color.b += value;
					
				}else if(imgPbFloat1){
					const float value = imgPbFloat1[imgOffset].r * boxFilterBlends[j];
					color.r += value;
					color.g += value;
					color.b += value;
					
				}else if(imgPbByte2){
					const float value = ((float)imgPbByte2[imgOffset].r * IMG_CONVERT8) * boxFilterBlends[j];
					color.r += value;
					color.g += value;
					color.b += value;
					color.a += ((float)imgPbByte2[imgOffset].g * IMG_CONVERT8) * boxFilterBlends[j];
					
				}else if(imgPbFloat2){
					const float value = imgPbFloat2[imgOffset].r * boxFilterBlends[j];
					color.r += value;
					color.g += value;
					color.b += value;
					color.a += imgPbFloat2[imgOffset].g * boxFilterBlends[j];
					
				}else if(imgPbByte3){
					color.r += ((float)imgPbByte3[imgOffset].r * IMG_CONVERT8) * boxFilterBlends[j];
					color.g += ((float)imgPbByte3[imgOffset].g * IMG_CONVERT8) * boxFilterBlends[j];
					color.b += ((float)imgPbByte3[imgOffset].b * IMG_CONVERT8) * boxFilterBlends[j];
					
				}else if(imgPbFloat3){
					color.r += imgPbFloat3[imgOffset].r * boxFilterBlends[j];
					color.g += imgPbFloat3[imgOffset].g * boxFilterBlends[j];
					color.b += imgPbFloat3[imgOffset].b * boxFilterBlends[j];
					
				}else if(imgPbByte4){
					color.r += ((float)imgPbByte4[imgOffset].r * IMG_CONVERT8) * boxFilterBlends[j];
					color.g += ((float)imgPbByte4[imgOffset].g * IMG_CONVERT8) * boxFilterBlends[j];
					color.b += ((float)imgPbByte4[imgOffset].b * IMG_CONVERT8) * boxFilterBlends[j];
					color.a += ((float)imgPbByte4[imgOffset].a * IMG_CONVERT8) * boxFilterBlends[j];
					
				}else if(imgPbFloat4){
					color.r += imgPbFloat4[imgOffset].r * boxFilterBlends[j];
					color.g += imgPbFloat4[imgOffset].g * boxFilterBlends[j];
					color.b += imgPbFloat4[imgOffset].b * boxFilterBlends[j];
					color.a += imgPbFloat4[imgOffset].a * boxFilterBlends[j];
					
				// regular and fallback
				}else if(imgGrayscale8){
					const float value = ((float)imgGrayscale8[imgOffset].value * IMG_CONVERT8) * boxFilterBlends[j];
					color.r += value;
					color.g += value;
					color.b += value;
					
				}else if(imgGrayscale16){
					const float value = ((float)imgGrayscale16[imgOffset].value * IMG_CONVERT16) * boxFilterBlends[j];
					color.r += value;
					color.g += value;
					color.b += value;
					
				}else if(imgGrayscale32){
					const float value = imgGrayscale32[imgOffset].value * boxFilterBlends[j];
					color.r += value;
					color.g += value;
					color.b += value;
					
				}else if(imgGrayscaleAlpha8){
					const float value = ((float)imgGrayscaleAlpha8[imgOffset].value * IMG_CONVERT8) * boxFilterBlends[j];
					color.r += value;
					color.g += value;
					color.b += value;
					color.a += ((float)imgGrayscaleAlpha8[imgOffset].alpha * IMG_CONVERT8) * boxFilterBlends[j];
					
				}else if(imgGrayscaleAlpha16){
					const float value = ((float)imgGrayscaleAlpha16[imgOffset].value * IMG_CONVERT16) * boxFilterBlends[j];
					color.r += value;
					color.g += value;
					color.b += value;
					color.a += ((float)imgGrayscaleAlpha16[imgOffset].alpha * IMG_CONVERT16) * boxFilterBlends[j];
					
				}else if(imgGrayscaleAlpha32){
					const float value = imgGrayscaleAlpha32[imgOffset].value * boxFilterBlends[j];
					color.r += value;
					color.g += value;
					color.b += value;
					color.a += imgGrayscaleAlpha32[imgOffset].alpha * boxFilterBlends[j];
					
				}else if(imgRGB8){
					color.r += ((float)imgRGB8[imgOffset].red * IMG_CONVERT8) * boxFilterBlends[j];
					color.g += ((float)imgRGB8[imgOffset].green * IMG_CONVERT8) * boxFilterBlends[j];
					color.b += ((float)imgRGB8[imgOffset].blue * IMG_CONVERT8) * boxFilterBlends[j];
					
				}else if(imgRGB16){
					color.r += ((float)imgRGB16[imgOffset].red * IMG_CONVERT16) * boxFilterBlends[j];
					color.g += ((float)imgRGB16[imgOffset].green * IMG_CONVERT16) * boxFilterBlends[j];
					color.b += ((float)imgRGB16[imgOffset].blue * IMG_CONVERT16) * boxFilterBlends[j];
					
				}else if(imgRGB32){
					color.r += imgRGB32[imgOffset].red * boxFilterBlends[j];
					color.g += imgRGB32[imgOffset].green * boxFilterBlends[j];
					color.b += imgRGB32[imgOffset].blue * boxFilterBlends[j];
					
				}else if(imgRGBA8){
					color.r += ((float)imgRGBA8[imgOffset].red * IMG_CONVERT8) * boxFilterBlends[j];
					color.g += ((float)imgRGBA8[imgOffset].green * IMG_CONVERT8) * boxFilterBlends[j];
					color.b += ((float)imgRGBA8[imgOffset].blue * IMG_CONVERT8) * boxFilterBlends[j];
					color.a += ((float)imgRGBA8[imgOffset].alpha * IMG_CONVERT8) * boxFilterBlends[j];
					
				}else if(imgRGBA16){
					color.r += ((float)imgRGBA16[imgOffset].red * IMG_CONVERT16) * boxFilterBlends[j];
					color.g += ((float)imgRGBA16[imgOffset].green * IMG_CONVERT16) * boxFilterBlends[j];
					color.b += ((float)imgRGBA16[imgOffset].blue * IMG_CONVERT16) * boxFilterBlends[j];
					color.a += ((float)imgRGBA16[imgOffset].alpha * IMG_CONVERT16) * boxFilterBlends[j];
					
				}else if(imgRGBA32){
					color.r += imgRGBA32[imgOffset].red * boxFilterBlends[j];
					color.g += imgRGBA32[imgOffset].green * boxFilterBlends[j];
					color.b += imgRGBA32[imgOffset].blue * boxFilterBlends[j];
					color.a += imgRGBA32[imgOffset].alpha * boxFilterBlends[j];
				}
			}
			
			// write pixel
			color.a *= maskAlpha;
			
			for(z=0; z<depth; z++){
				pWritePixel(context, offset + strideImage * z, color);
			}
			
			offset += pixBufComponentCount;
		}
	}
}

void deoglSCBuildConstructed::pWritePixel(const sContext &context,
int offset, const decColor &color){
	if(context.transparency < 0.001f || color.a < 0.001f){
		return;
	}
	
	const decColor c(context.applyGamma(context.transformColor * color));
	
	if(context.transparency > 0.999f && color.a > 0.999f){
		pWritePixelSet(offset, c);
		
	}else{
		pWritePixelBlend(offset, c, color.a * context.transparency);
	}
}

void deoglSCBuildConstructed::pWritePixelSet(int offset, const decColor &color){
	const sTarget &target = *pTarget;
	
	if(target.pixBufDataByte){
		GLubyte * const pixel = target.pixBufDataByte + offset;
		
		if(target.targetRed < target.pixBufComponentCount){
			pixel[target.targetRed] = (GLubyte)decMath::clamp((int)(color.r * 255.0f), 0, 255);
		}
		if(target.targetGreen < target.pixBufComponentCount){
			pixel[target.targetGreen] = (GLubyte)decMath::clamp((int)(color.g * 255.0f), 0, 255);
		}
		if(target.targetBlue < target.pixBufComponentCount){
			pixel[target.targetBlue] = (GLubyte)decMath::clamp((int)(color.b * 255.0f), 0, 255);
		}
		if(target.targetAlpha < target.pixBufComponentCount){
			pixel[target.targetAlpha] = (GLubyte)255;
		}
		
	}else{
		GLfloat * const pixel = target.pixBufDataFloat + offset;
		
		if(target.targetRed < target.pixBufComponentCount){
			pixel[target.targetRed] = color.r;
		}
		if(target.targetGreen < target.pixBufComponentCount){
			pixel[target.targetGreen] = color.g;
		}
		if(target.targetBlue < target.pixBufComponentCount){
			pixel[target.targetBlue] = color.b;
		}
		if(target.targetAlpha < target.pixBufComponentCount){
			pixel[target.targetAlpha] = (GLfloat)1.0;
		}
	}
}

void deoglSCBuildConstructed::pWritePixelBlend(int offset, const decColor &color, float alpha){
	const sTarget &target = *pTarget;
	
	if(target.pixBufDataByte){
		GLubyte * const pixel = target.pixBufDataByte + offset;
		
		if(target.targetRed < target.pixBufComponentCount){
			pixel[target.targetRed] = (GLubyte)decMath::clamp((int)decMath::mix(
				(float)pixel[target.targetRed], color.r * 255.0f, alpha), 0, 255);
		}
		if(target.targetGreen < target.pixBufComponentCount){
			pixel[target.targetGreen] = (GLubyte)decMath::clamp((int)decMath::mix(
				(float)pixel[target.targetGreen], color.g * 255.0f, alpha), 0, 255);
		}
		if(target.targetBlue < target.pixBufComponentCount){
			pixel[target.targetBlue] = (GLubyte)decMath::clamp((int)decMath::mix(
				(float)pixel[target.targetBlue], color.b * 255.0f, alpha), 0, 255);
		}
		if(target.targetAlpha < target.pixBufComponentCount){
			pixel[target.targetAlpha] = (GLubyte)decMath::min((int)(
				(float)pixel[target.targetAlpha] + alpha), 255);
		}
		
	}else{
		GLfloat * const pixel = target.pixBufDataFloat + offset;
		
		if(target.targetRed < target.pixBufComponentCount){
			pixel[target.targetRed] = (GLfloat)decMath::mix(
				(float)pixel[target.targetRed], color.r, alpha);
		}
		if(target.targetGreen < target.pixBufComponentCount){
			pixel[target.targetGreen] = (GLfloat)decMath::mix(
				(float)pixel[target.targetGreen], color.g, alpha);
		}
		if(target.targetBlue < target.pixBufComponentCount){
			pixel[target.targetBlue] = (GLfloat)decMath::mix(
				(float)pixel[target.targetBlue], color.b, alpha);
		}
		if(target.targetAlpha < target.pixBufComponentCount){
			pixel[target.targetAlpha] = (GLfloat)decMath::min(
				(float)pixel[target.targetAlpha] + alpha, 1.0f);
		}
	}
}
