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

#include "deglWindowLoggerTableItem.h"



// Class deglWindowLoggerTableItem
////////////////////////////////////

FXIMPLEMENT(deglWindowLoggerTableItem, FXTableItem, nullptr, 0)

// Constructor, destructor
////////////////////////////

deglWindowLoggerTableItem::deglWindowLoggerTableItem(){}

deglWindowLoggerTableItem::deglWindowLoggerTableItem(const FXString &text, FXIcon *ic, void *ptr) :
FXTableItem(text, ic, ptr),
pTextColor(0),
pFont(nullptr){
}

deglWindowLoggerTableItem::~deglWindowLoggerTableItem(){
}



// Management
///////////////

void deglWindowLoggerTableItem::SetTextColor(FXColor color){
	pTextColor = color;
}

void deglWindowLoggerTableItem::SetFont(FXFont *font){
	pFont = font;
}



FXint deglWindowLoggerTableItem::getWidth(const FXTable *table) const{
	FXFont *font = table->getFont();
	FXint beg, end, tw, iw, s, w, t;
	FXint ml = table->getMarginLeft();
	FXint mr = table->getMarginRight();
	FXString lbl = getText();
	FXIcon *icn = getIcon();
	
	if(pFont){
		font = pFont;
	}
	
	tw = iw = beg = s = 0;
	if(icn){
		iw = icn->getWidth();
	}
	
	do{
		end = beg;
		while(end < lbl.length() && lbl[end] != '\n'){
			end++;
		}
		if((t = font->getTextWidth(&lbl[beg], end - beg)) > tw){
			tw = t;
		}
		beg = end + 1;
	}while(end < lbl.length());
	
	if(iw && tw){
		s = 4;
	}
	
	if(state & (BEFORE | AFTER)){
		w = iw + tw + s;
		
	}else{
		w = FXMAX(iw, tw);
	}
	
	return ml + mr + w;
}

FXint deglWindowLoggerTableItem::getHeight(const FXTable *table) const{
	FXFont *font = table->getFont();
	FXint beg, end, th, ih, h;
	FXint mt = table->getMarginTop();
	FXint mb = table->getMarginBottom();
	FXString lbl = getText();
	FXIcon *icn = getIcon();
	th = ih = beg = 0;
	
	if(icn){
		ih = icn->getHeight();
	}
	
	do{
		end = beg;
		while(end < lbl.length() && lbl[end] != '\n'){
			end++;
		}
		th += font->getFontHeight();
		beg = end + 1;
	}while(end < lbl.length());
	
	if(state & (ABOVE | BELOW)){
		h = ih + th;
		
	}else{
		h = FXMAX(ih, th);
	}
	
	return h + mt + mb;
}

void deglWindowLoggerTableItem::drawBackground(const FXTable *table, FXDC &dc, FXint x, FXint y, FXint w, FXint h) const{
	FXint hg = table->isHorzGridShown() ? 1 : 0;
	FXint vg = table->isVertGridShown() ? 1 : 0;
	
	if(isSelected()){
		if(pTextColor == 0){
			dc.setForeground(table->getSelBackColor());
			
		}else{
			dc.setForeground(table->getSelBackColor()); //pTextColor);
		}
		
	}else{
		dc.setForeground(table->getBackColor());
	}
	
	dc.fillRectangle(x + vg, y + hg, w - vg, h - hg);
}

void deglWindowLoggerTableItem::drawContent(const FXTable *table, FXDC &dc, FXint x, FXint y, FXint w, FXint h) const{
	FXint tx,ty,tw,th,ix,iy,iw,ih,s,beg,end,t,xx,yy;
	FXbool hg=table->isHorzGridShown();
	FXbool vg=table->isVertGridShown();
	FXint ml=table->getMarginLeft()+vg;
	FXint mt=table->getMarginTop()+hg;
	FXint mr=table->getMarginRight();
	FXint mb=table->getMarginBottom();
	FXFont *font = table->getFont(); //dc.getFont();
	FXString lbl=getText();
	FXIcon  *icn=getIcon();
	
	if(pFont){
		font = pFont;
	}
	
	dc.setFont(font);
	
	// Text width and height
	beg=tw=th=0;
	do{
		end=beg;
		while(end<lbl.length() && lbl[end]!='\n') end++;
		if((t=font->getTextWidth(&lbl[beg],end-beg))>tw) tw=t;
		th+=font->getFontHeight();
		beg=end+1;
	}while(end<lbl.length());
	
	// Icon size
	iw=ih=0;
	if(icn){
		iw=icn->getWidth();
		ih=icn->getHeight();
	}
	
	// Icon-text spacing
	s=0;
	if(iw && tw) s=4;
	
	// Fix x coordinate
	if(state&LEFT){
		if(state&BEFORE){
			ix=x+ml; tx=ix+iw+s;
		}else if(state&AFTER){
			tx=x+ml; ix=tx+tw+s;
		}else{
			ix=x+ml; tx=x+ml;
		}
	}else if(state&RIGHT){
		if(state&BEFORE){
			tx=x+w-mr-tw; ix=tx-iw-s;
		}else if(state&AFTER){
			ix=x+w-mr-iw; tx=ix-tw-s;
		}else{
			ix=x+w-mr-iw; tx=x+w-mr-tw;
		}
	}else{
		if(state&BEFORE){
			ix=x+(ml+w-mr)/2-(tw+iw+s)/2; tx=ix+iw+s;
		}else if(state&AFTER){
			tx=x+(ml+w-mr)/2-(tw+iw+s)/2; ix=tx+tw+s;
		}else{
			ix=x+(ml+w-mr)/2-iw/2; tx=x+(ml+w-mr)/2-tw/2;
		}
	}
	
	// Fix y coordinate
	if(state&TOP){
		if(state&ABOVE){
			iy=y+mt; ty=iy+ih;
		}else if(state&BELOW){
			ty=y+mt; iy=ty+th;
		}else{
			iy=y+mt; ty=y+mt;
		}
	}else if(state&BOTTOM){
		if(state&ABOVE){
			ty=y+h-mb-th; iy=ty-ih;
		}else if(state&BELOW){
			iy=y+h-mb-ih; ty=iy-th;
		}else{
			iy=y+h-mb-ih; ty=y+h-mb-th;
		}
	}else{
		if(state&ABOVE){
			iy=y+(mt+h-mb)/2-(th+ih)/2; ty=iy+ih;
		}else if(state&BELOW){
			ty=y+(mt+h-mb)/2-(th+ih)/2; iy=ty+th;
		}else{
			iy=y+(mt+h-mb)/2-ih/2; ty=y+(mt+h-mb)/2-th/2;
		}
	}

	// Paint icon
	if(icn){
		dc.drawIcon(icn,ix,iy);
	}

	// Text color
	if(state & SELECTED){
		if(pTextColor == 0){
			dc.setForeground(table->getSelTextColor());
			
		}else{
			dc.setForeground(table->getSelTextColor()); //table->getSelBackColor());
		}
		
	}else{
		if(pTextColor == 0){
			dc.setForeground(table->getTextColor());
			
		}else{
			dc.setForeground(pTextColor);
		}
	}
	
	// Draw text
	yy=ty+font->getFontAscent();
	beg=0;
	do{
		end=beg;
		while(end<lbl.length() && lbl[end]!='\n') end++;
		if(state&LEFT){
			xx=tx;
		}else if(state&RIGHT){
			xx=tx+tw-font->getTextWidth(&lbl[beg],end-beg);
		}else{
			xx=tx+(tw-font->getTextWidth(&lbl[beg],end-beg))/2;
		}
		dc.drawText(xx,yy,&lbl[beg],end-beg);
		yy+=font->getFontHeight();
		beg=end+1;
	}while(end<lbl.length());
}
