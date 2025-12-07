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

#include "dealGuiTheme.h"
#include "dealWidget.h"
#include "dealWidgetLayout.h"
#include "../dealDisplay.h"
#include "../dealImage.h"
#include "../dealShader.h"
#include "../../dealLauncher.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Definitions
////////////////

#define OFFSET_POINT	0
#define COUNT_POINT		1

#define OFFSET_LINE		1
#define COUNT_LINE		2

#define OFFSET_RECT		3
#define COUNT_RECT		4




// Class dealWidget
/////////////////////

// Constructors, destructors
//////////////////////////////

dealWidget::dealWidget(dealDisplay &display) :
pDisplay(display),

pParent(NULL),

pHasExplicitMinSize(false),

pBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f),

pPaddingLeft(0),
pPaddingTop(0),
pPaddingRight(0),
pPaddingBottom(0),

pVisible(true),
pEnabled(true){
}

dealWidget::dealWidget(dealDisplay &display, const decPoint &position, const decPoint &size) :
pDisplay(display),

pParent(NULL),

pPosition(position),
pSize(size),

pHasExplicitMinSize(false),

pBackgroundColor(0.0f, 0.0f, 0.0f, 0.0f),

pPaddingLeft(0),
pPaddingTop(0),
pPaddingRight(0),
pPaddingBottom(0),

pVisible(true){
}

dealWidget::~dealWidget(){
}



// Management
///////////////

void dealWidget::SetFromGuiTheme(const dealGuiTheme &guitheme){
	SetBackgroundColor(guitheme.GetWidgetBackgroundColor());
}



void dealWidget::SetParent(dealWidgetLayout *parent){
	if(parent == pParent){
		return;
	}
	
	pParent = parent;
	OnParentChanged();
}

void dealWidget::DirtyParentLayout(){
	if(pParent){
		pParent->DirtyLayout();
	}
}



void dealWidget::SetPosition(const decPoint &position){
	if(position == pPosition){
		return;
	}
	
	pPosition = position;
	
	DirtyParentLayout();
	OnPositionChanged();
}

void dealWidget::SetSize(const decPoint &size){
	if(size == pSize){
		return;
	}
	
	pSize = size;
	
	DirtyParentLayout();
	OnSizeChanged();
}



void dealWidget::SetExplicitMinimumSize(const decPoint &size){
	if(pHasExplicitMinSize && size == pExplicitMinSize){
		return;
	}
	
	pExplicitMinSize = size;
	pHasExplicitMinSize = true;
	
	DirtyParentLayout();
}

void dealWidget::ClearExplicitMinimumSize(){
	if(! pHasExplicitMinSize){
		return;
	}
	
	pHasExplicitMinSize = false;
	
	DirtyParentLayout();
}

decPoint dealWidget::GetMinimumSize(){
	if(pHasExplicitMinSize){
		return pExplicitMinSize;
	}
	
	return decPoint();
}



void dealWidget::SetBackgroundColor(const decColor &color){
	pBackgroundColor = color;
}



void dealWidget::SetPaddingLeft(int padding){
	padding = decMath::max(padding, 0);
	if(padding == pPaddingLeft){
		return;
	}
	
	pPaddingLeft = padding;
	OnPaddingChanged();
}

void dealWidget::SetPaddingTop(int padding){
	padding = decMath::max(padding, 0);
	if(padding == pPaddingTop){
		return;
	}
	
	pPaddingTop = padding;
	OnPaddingChanged();
}

void dealWidget::SetPaddingRight(int padding){
	padding = decMath::max(padding, 0);
	if(padding == pPaddingRight){
		return;
	}
	
	pPaddingRight = padding;
	OnPaddingChanged();
}

void dealWidget::SetPaddingBottom(int padding){
	padding = decMath::max(padding, 0);
	if(padding == pPaddingBottom){
		return;
	}
	
	pPaddingBottom = padding;
	OnPaddingChanged();
}

void dealWidget::SetPadding(int horizontal, int vertical){
	horizontal = decMath::max(horizontal, 0);
	vertical = decMath::max(vertical, 0);
	
	if(pPaddingLeft == horizontal && pPaddingRight == horizontal
	&&  pPaddingTop == vertical && pPaddingBottom == vertical){
		return;
	}
	
	pPaddingLeft = horizontal;
	pPaddingRight = horizontal;
	pPaddingTop = vertical;
	pPaddingBottom = vertical;
	OnPaddingChanged();
}

void dealWidget::SetPadding(int padding){
	padding = decMath::max(padding, 0);
	
	if(pPaddingLeft == padding && pPaddingRight == padding
	&&  pPaddingTop == padding && pPaddingBottom == padding){
		return;
	}
	
	pPaddingLeft = padding;
	pPaddingRight = padding;
	pPaddingTop = padding;
	pPaddingBottom = padding;
	OnPaddingChanged();
}

void dealWidget::SetPaddingLeft(int left, int top, int right, int bottom){
	left = decMath::max(left, 0);
	top = decMath::max(top, 0);
	right = decMath::max(right, 0);
	bottom = decMath::max(bottom, 0);
	
	if(pPaddingLeft == left && pPaddingRight == right
	&&  pPaddingTop == top && pPaddingBottom == bottom){
		return;
	}
	
	pPaddingLeft = left;
	pPaddingRight = right;
	pPaddingTop = top;
	pPaddingBottom = bottom;
	OnPaddingChanged();
}

decPoint dealWidget::GetContentArea() const{
	return decPoint().Largest(pSize - decPoint(pPaddingLeft + pPaddingRight, pPaddingTop + pPaddingBottom));
}



void dealWidget::SetVisible(bool visible){
	if(visible == pVisible){
		return;
	}
	
	pVisible = visible;
	
	DirtyParentLayout();
	OnVisibilityChanged();
}

void dealWidget::SetEnabled(bool enabled){
	if(enabled == pEnabled){
		return;
	}
	
	pEnabled = enabled;
	
	DirtyParentLayout();
	OnEnabledChanged();
}

bool dealWidget::GetFullEnabled() const{
	if(! pEnabled){
		return false;
	}
	
	if(pParent){
		return pParent->GetFullEnabled();
	}
	
	return true;
}



decPoint dealWidget::GetDialogPosition() const{
	if(pParent){
		return pParent->GetDialogPosition() + pPosition;
		
	}else{
		return pPosition;
	}
}

decPoint dealWidget::GetScreenPosition() const{
	if(pParent){
		return pParent->GetScreenPosition() + pPosition;
		
	}else{
		return pPosition;
	}
}

bool dealWidget::IsPointInside(const decPoint &point) const{
	return point >= decPoint() && point < pSize;
}

dealWidget *dealWidget::WidgetAtPosition(const decPoint &point) const{
	if(IsPointInside(point)){
		return (dealWidget*)this;
		
	}else{
		return NULL;
	}
}



void dealWidget::CaptureInput(){
	pDisplay.SetCaptureWidget(this);
}

void dealWidget::ReleaseCapture(){
	if(pDisplay.GetCaptureWidget() == this){
		pDisplay.SetCaptureWidget(NULL);
	}
}



void dealWidget::Render(const sRenderContext &context){
	RenderBackground(context);
	RenderContent(context);
}

void dealWidget::RenderBackground(const sRenderContext &context){
	if(pBackgroundColor.a < COLOR_THRESHOLD){
		return;
	}
	
	dealShader &shader = *pDisplay.GetShapeShader();
	shader.Activate();
	
	ShaderSetTransform(context, shader, 0, 0, pSize.x - 1, pSize.y - 1);
	ShaderSetColorMatrix(shader, pBackgroundColor);
	ShaderSetGamma(shader, 1.0f);
	ShaderSetClipRect(context, shader);
	
	DrawRectangle();
}

void dealWidget::RenderContent(const sRenderContext &context){
}



void dealWidget::OnPositionChanged(){
}

void dealWidget::OnSizeChanged(){
}

void dealWidget::OnParentChanged(){
}

void dealWidget::OnPaddingChanged(){
}

void dealWidget::OnVisibilityChanged(){
}

void dealWidget::OnEnabledChanged(){
}



void dealWidget::OnKeyPress(int keycode){
	if(pParent){
		pParent->OnKeyPress(keycode);
	}
}

void dealWidget::OnKeyRelease(int keycode){
	if(pParent){
		pParent->OnKeyRelease(keycode);
	}
}

void dealWidget::OnMousePress(int buttons, const decPoint &position){
	if(pParent){
		pParent->OnMousePress(buttons, pPosition + position);
	}
}

void dealWidget::OnMouseRelease(int buttons, const decPoint &position){
	if(pParent){
		pParent->OnMouseRelease(buttons, pPosition + position);
	}
}

void dealWidget::OnMouseMove(int buttons, const decPoint &position){
	if(pParent){
		pParent->OnMouseMove(buttons, pPosition + position);
	}
}



void dealWidget::ShaderSetTransform(const sRenderContext &context,
dealShader &shader, int x1, int y1, int x2, int y2) const{
	const float fx1 = (float)x1;
	const float fy1 = (float)y1;
	const float fx2 = (float)x2;
	const float fy2 = (float)y2;
	decTexMatrix matrix(decTexMatrix::CreateScale(fx2 - fx1, fy2 - fy1) *
		decTexMatrix::CreateTranslation(fx1, fy1) * context.transform);
	shader.SetParameterTexMatrix3x3(dealDisplay::espsTransform, matrix);
}

void dealWidget::ShaderSetTransform(const sRenderContext &context,
dealShader &shader, const decPoint &position, const decPoint &size) const{
	ShaderSetTransform(context, shader, position.x, position.y, position.x + size.x, position.y + size.y);
}

void dealWidget::ShaderSetTCTransform(dealShader &shader){
	shader.SetParameterTexMatrix3x3(dealDisplay::espsTCTransform, decTexMatrix());
}

void dealWidget::ShaderSetTCTransform(dealShader &shader, const decTexMatrix &matrix){
	shader.SetParameterTexMatrix3x3(dealDisplay::espsTCTransform, matrix);
}

void dealWidget::ShaderSetTCTransform(dealShader &shader, const decTexMatrix2 &matrix){
	shader.SetParameterTexMatrix3x3(dealDisplay::espsTCTransform, matrix);
}

void dealWidget::ShaderSetTCTransform(dealShader &shader, float u1, float v1, float u2, float v2){
	ShaderSetTCTransform(shader, decVector2(u1, v1), decVector2(u2 - u1, v2 - v1));
}

void dealWidget::ShaderSetTCTransform(dealShader &shader, const decVector2 &position, const decVector2 &size){
	ShaderSetTCTransform(shader, decTexMatrix2::CreateST(size, position));
}

void dealWidget::ShaderSetColorMatrix(dealShader &shader, const decColor &color) const{
	ShaderSetColorMatrix(shader, decColorMatrix::CreateScaling(color));
}

void dealWidget::ShaderSetColorMatrixDisabled(dealShader &shader, const decColor &color) const{
	ShaderSetColorMatrix(shader, decColorMatrix::CreateScaling(color) *
		decColorMatrix::CreateContrast(0.5f) * decColorMatrix::CreateBrightness(0.15f));
}

void dealWidget::ShaderSetColorMatrix(dealShader &shader, const decColorMatrix &matrix) const{
	shader.SetParameterColorMatrix5x4(dealDisplay::espsColorTransform, dealDisplay::espsColorTransform2, matrix);
}

void dealWidget::ShaderSetGamma(dealShader &shader, float gamma) const{
	shader.SetParameterFloat(dealDisplay::espsGamma, gamma, gamma, gamma, 1.0f);
}

void dealWidget::ShaderSetClipRect(const sRenderContext &context, dealShader &shader) const{
	const float clipLeft = (float)context.viewFrom.x;
	const float clipTop = context.clipBaseScreenY - (float)context.viewTo.y;
	const float clipRight = (float)context.viewTo.x;
	const float clipBottom = context.clipBaseScreenY - (float)context.viewFrom.y;
	shader.SetParameterFloat(dealDisplay::espsClipRect, clipLeft, clipTop, clipRight, clipBottom);
}

void dealWidget::ShaderSetClipRect(const sRenderContext &context,
dealShader &shader, int x1, int y1, int x2, int y2) const{
	ShaderSetClipRect(context, shader, decPoint(x1, y1), decPoint(x2 - x1, y2 - y1));
}

void dealWidget::ShaderSetClipRect(const sRenderContext &context,
dealShader &shader, const decPoint &position, const decPoint &size) const{
	const decPoint clipFrom = (context.viewFrom + position).Largest(context.viewFrom);
	const decPoint clipTo = (context.viewFrom + position + size).Smallest(context.viewTo);
	
	shader.SetParameterFloat(dealDisplay::espsClipRect, (float)clipFrom.x,
		(float)clipFrom.y, (float)clipTo.x, (float)clipTo.y);
}

void dealWidget::ShaderSetTCClamp(dealShader &shader) const{
	shader.SetParameterFloat(dealDisplay::espsTCClamp, 0.0f, 0.0f, 1.0f, 1.0f);
}

void dealWidget::ShaderSetTCClamp(dealShader &shader, float u1, float v1, float u2, float v2) const{
	shader.SetParameterFloat(dealDisplay::espsTCClamp, u1, v1, u2, v2);
}

void dealWidget::ShaderSetTCClamp(dealShader &shader, const decVector2 &position, const decVector2 &size) const{
	ShaderSetTCClamp(shader, position.x, position.y, position.x + size.x, position.y + size.y);
}

void dealWidget::BindTexture(int stage, const dealImage &image, bool linearFiltering, bool repeatWrap){
	OGL_CHECK(pDisplay.GetLauncher(), glActiveTexture(GL_TEXTURE0 + stage));
	OGL_CHECK(pDisplay.GetLauncher(), glBindTexture(GL_TEXTURE_2D, image.GetTexture()));
	
	if(linearFiltering){
		OGL_CHECK(pDisplay.GetLauncher(), glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		OGL_CHECK(pDisplay.GetLauncher(), glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		
	}else{
		OGL_CHECK(pDisplay.GetLauncher(), glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		OGL_CHECK(pDisplay.GetLauncher(), glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	}
	
	if(repeatWrap){
		OGL_CHECK(pDisplay.GetLauncher(), glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		OGL_CHECK(pDisplay.GetLauncher(), glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		
	}else{
		OGL_CHECK(pDisplay.GetLauncher(), glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		OGL_CHECK(pDisplay.GetLauncher(), glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	}
}

void dealWidget::UnbindTexture(int stage){
	OGL_CHECK(pDisplay.GetLauncher(), glActiveTexture(GL_TEXTURE0 + stage));
	OGL_CHECK(pDisplay.GetLauncher(), glBindTexture(GL_TEXTURE_2D, 0));
}

void dealWidget::DrawPoint(){
	OGL_CHECK(pDisplay.GetLauncher(), glDrawArrays(GL_POINTS, OFFSET_POINT, COUNT_POINT));
}

void dealWidget::DrawLine(){
	OGL_CHECK(pDisplay.GetLauncher(), glDrawArrays(GL_LINES, OFFSET_LINE, COUNT_LINE));
}

void dealWidget::DrawRectangle(){
	OGL_CHECK(pDisplay.GetLauncher(), glDrawArrays(GL_TRIANGLE_FAN, OFFSET_RECT, COUNT_RECT));
}
