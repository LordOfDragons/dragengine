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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "feFont.h"
#include "feFontBuilder.h"
#include "feFontNotifier.h"
#include "glyph/feFontGlyph.h"
#include "image/feFontImage.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Class feFont
/////////////////

// Constructor, destructor
////////////////////////////

feFont::feFont(igdeEnvironment *environment) :
igdeEditableEntity(environment)
{
	pEngFont = NULL;
	
	SetFilePath("new.defont");
	pLineHeight = 10;
	pBaseLine = 7;
	pColorFont = false;
	pFontImage = NULL;
	pElementMode = eemBone;
	pWorkMode = ewmSelect;
	
	pGlyphSelection = NULL;
	
	pDirtyFont = true;
	
	try{
		pGlyphSelection = new feFontGlyphSelection(this);
		
		pFontImage.TakeOver(new feFontImage(GetEngine()));
		pFontImage->SetParentFont(this);
		
		SetChanged(false);
		SetSaved(false);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

feFont::~feFont(){
	pCleanUp();
}



// Management
///////////////

void feFont::SetLineHeight(int lineHeight){
	DEASSERT_TRUE(lineHeight > 0)
	
	if(pLineHeight != lineHeight){
		pLineHeight = lineHeight;
		NotifyFontChanged();
	}
}

void feFont::SetColorFont(bool colorFont){
	if(pColorFont != colorFont){
		pColorFont = colorFont;
		NotifyFontChanged();
	}
}

void feFont::SetBaseLine(int baseLine){
	DEASSERT_TRUE(baseLine >= 0)
	
	if(pBaseLine != baseLine){
		pBaseLine = baseLine;
		NotifyFontChanged();
	}
}

void feFont::SetElementMode(int mode){
	if(mode < eemBone || mode > eemPush) DETHROW(deeInvalidParam);
	
	if(pElementMode != mode){
		pElementMode = mode;
		NotifyModeChanged();
	}
}

void feFont::SetWorkMode(int mode){
	if(mode < ewmSelect || mode > ewm3DCursor) DETHROW(deeInvalidParam);
	
	if(pWorkMode != mode){
		pWorkMode = mode;
		NotifyModeChanged();
	}
}



void feFont::Dispose(){
	GetUndoSystem()->RemoveAll();
	pGlyphSelection->Reset();
	RemoveAllGlyphs();
}

void feFont::Reset(){
	GetUndoSystem()->RemoveAll();
	pGlyphSelection->Reset();
}

void feFont::Update(float elapsed){
	Rebuild();
}

void feFont::Invalidate(){
	pDirtyFont = true;
}

void feFont::Rebuild(){
	if(pDirtyFont){
		// free the old font
			pEngFont = NULL;
		}
		
		// build a new font only if it would be valid
		if(pFontImage->GetEngineImage()){
			// build new font. for this we use an empty name which creates an unnamed
			// font. named fonts can only exist once but unnamed fonts can exist multiple
			// times. in contrary to named fonts the unnamed fonts can not be retrieved
			// using loading but this is okay for what we use them here.
			feFontBuilder builder(this);
			
			pEngFont = GetEngine()->GetFontManager()->CreateFont("", builder);
		}
		
		// no more dirty
		pDirtyFont = false;
	}
}



// Glyphs
///////////


int feFont::GetGlyphCount() const{
	return pGlyphs.GetGlyphCount();
}

feFontGlyph *feFont::GetGlyphAt(int index) const{
	return pGlyphs.GetGlyphAt(index);
}

feFontGlyph *feFont::GetGlyphWithCode(int code) const{
	return pGlyphs.GetGlyphWithCode(code);
}

int feFont::IndexOfGlyph(feFontGlyph *glyph) const{
	return pGlyphs.IndexOfGlyph(glyph);
}

int feFont::IndexOfGlyphWithCode(int code) const{
	return pGlyphs.IndexOfGlyphWithCode(code);
}

bool feFont::HasGlyph(feFontGlyph *glyph) const{
	return pGlyphs.HasGlyph(glyph);
}

bool feFont::HasGlyphWithCode(int code) const{
	return pGlyphs.HasGlyphWithCode(code);
}

void feFont::AddGlyph(feFontGlyph *glyph){
	pGlyphs.AddGlyph(glyph);
	glyph->SetParentFont(this);
	NotifyGlyphStructureChanged();
}

void feFont::RemoveGlyph(feFontGlyph *glyph){
	glyph->SetParentFont(NULL);
	pGlyphs.RemoveGlyph(glyph);
	NotifyGlyphStructureChanged();
}

void feFont::RemoveGlyphWithCode (int code) {
	feFontGlyph *glyph = pGlyphs.GetGlyphWithCode(code);
	if(!glyph) DETHROW(deeInvalidParam);
	glyph->SetParentFont(NULL);
	pGlyphs.RemoveGlyph(glyph);
	NotifyGlyphStructureChanged();
}

void feFont::RemoveAllGlyphs(){
	int g, count = pGlyphs.GetGlyphCount();
	for(g=0; g<count; g++){
		pGlyphs.GetGlyphAt(g)->SetParentFont(NULL);
	}
	pGlyphs.RemoveAllGlyphs();
	NotifyGlyphStructureChanged();
}



// Notifiers
//////////////

void feFont::AddNotifier(feFontNotifier *notifier){
	pListeners.Add(notifier);
}

void feFont::RemoveNotifier(feFontNotifier *notifier){
	pListeners.Remove(notifier);
}



void feFont::NotifyModeChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((feFontNotifier*)pListeners.GetAt(i))->ModeChanged(this);
	}
}

void feFont::NotifyStateChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((feFontNotifier*)pListeners.GetAt(i))->StateChanged(this);
	}
	
	Invalidate();
}

void feFont::NotifyUndoChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((feFontNotifier*)pListeners.GetAt(i))->UndoChanged(this);
	}
}



void feFont::NotifyFontChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((feFontNotifier*)pListeners.GetAt(i))->FontChanged(this);
	}
	
	SetChanged(true);
}



void feFont::NotifyImageChanged(feFontImage *image){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((feFontNotifier*)pListeners.GetAt(i))->ImageChanged(this, image);
	}
	
	SetChanged(true);
}



void feFont::NotifyGlyphStructureChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((feFontNotifier*)pListeners.GetAt(i))->GlyphStructureChanged(this);
	}
	
	SetChanged(true);
}

void feFont::NotifyGlyphChanged(feFontGlyph *glyph){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((feFontNotifier*)pListeners.GetAt(i))->GlyphChanged(this, glyph);
	}
	
	SetChanged(true);
}

void feFont::NotifyGlyphSelectionChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((feFontNotifier*)pListeners.GetAt(i))->GlyphSelectionChanged(this);
	}
}

void feFont::NotifyActiveGlyphChanged(){
	const int count = pListeners.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		((feFontNotifier*)pListeners.GetAt(i))->ActiveGlyphChanged(this);
	}
}



// Private Functions
//////////////////////

void feFont::pCleanUp(){
	pListeners.RemoveAll();
	
	if(pFontImage){
		pFontImage->SetParentFont(NULL);
	}
	
	if(pGlyphSelection) delete pGlyphSelection;
	RemoveAllGlyphs();
	
	if(pEngFont) pEngFont->FreeReference();
}
