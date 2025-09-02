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

#include <memory.h>

#include "deFont.h"
#include "deFontManager.h"
#include "../image/deImage.h"
#include "../skin/deSkin.h"
#include "../skin/deSkinTexture.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../common/string/decString.h"
#include "../../common/string/unicode/decUnicodeString.h"
#include "../../logger/deLogger.h"
#include "../../parallel/deParallelTask.h"
#include "../../parallel/deParallelProcessing.h"
#include "../../threading/deMutexGuard.h"
#include "../../systems/modules/graphic/deBaseGraphicFont.h"


class cFontSize : public deObject{
public:
	typedef deTObjectReference<cFontSize> Ref;
	
private:
	deFont &pFont;
	int pLineHeight;
	const deFontSize::Ref pSize;
	
public:
	cFontSize(deFont &font, int lineHeight, const deFontSize::Ref size) :
	pFont(font), pLineHeight(lineHeight), pSize(size){
	}
	
protected:
	~cFontSize() override{
		if(pSize && pSize->GetTaskLoad()){
			pSize->GetTaskLoad()->Cancel();
			pFont.GetEngine()->GetParallelProcessing().WaitForTask(pSize->GetTaskLoad());
			pSize->SetTaskLoad(nullptr);
		}
	}
	
public:
	inline int GetLineHeight() const{ return pLineHeight; }
	inline const deFontSize::Ref &GetSize() const{ return pSize; }
};


// Class deFont
/////////////////

// Constructors, destructor
/////////////////////////////

deFont::deFont( deFontManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime ) :
deFileResource( manager, vfs, filename, modificationTime ),
pGlyphs(nullptr),
pGlyphCount(0),
pGlyphGroups(nullptr),
pGlyphGroupCount(0),
pGlyphMap(nullptr),
pGlyphMapCount(0),
pFontWidth(0),
pLineHeight(1),
pBaseLine(1),
pColorFont(false),
pScalable(false),
pPeerGraphic(nullptr){
}

deFont::~deFont(){
	if(pPeerGraphic){
		delete pPeerGraphic;
	}
	
	pSizes.RemoveAll();
	
	pFreeGlyphMap();
	
	if(pGlyphs){
		delete [] pGlyphs;
		pGlyphs = nullptr;
		pGlyphCount = 0;
	}
}


// Management
///////////////

void deFont::SetLineHeight( int lineHeight ){
	DEASSERT_TRUE(lineHeight > 0)
	
	pLineHeight = lineHeight;
}

void deFont::SetIsColorFont( bool colorFont ){
	pColorFont = colorFont;
}

void deFont::SetScalable(bool scalable){
	pScalable = scalable;
}

void deFont::SetBaseLine(int baseLine){
	DEASSERT_TRUE(baseLine >= 0)
	
	pBaseLine = baseLine;
}


void deFont::UpdateGlyphs(){
	// update font width
	pFontWidth = 0;
	
	int i;
	for( i=0; i<pGlyphCount; i++ ){
		if( pGlyphs[ i ].GetWidth() > pFontWidth ){
			pFontWidth = pGlyphs[ i ].GetWidth();
		}
	}
	
	// create glyph map
	pFreeGlyphMap();
	pCreateGlyphMap();
}

bool deFont::Verify(){
	if( ! pImage ){
		GetEngine()->GetLogger()->LogErrorFormat( "Dragengine",
			"deFont::Verify(%s): Image missing (path %s)",
			GetFilename().GetString(), pImagePath.GetString() );
		return false;
	}
	
	switch( pImage->GetComponentCount() ){
	case 1: // todo, taintable font
	case 2: // we can't use this format at all ( or maybe taintable + alpha? )
		GetEngine()->GetLogger()->LogErrorFormat( "Dragengine",
			"deFont::Verify(%s): Unsupported image component found %d (path %s)",
			GetFilename().GetString(), pImage->GetComponentCount(), pImagePath.GetString() );
		return false;
		
	case 3:
	case 4:
		break;
		
	default:
		return false; // should never happen
	}
	
	return true;
}


void deFont::SetGlyphCount( int count ){
	if( count < 0 || count > 0xfffe ){
		DETHROW( deeInvalidParam );
	}
	
	pFreeGlyphMap();
	
	if( pGlyphs ){
		delete [] pGlyphs;
		pGlyphs = NULL;
		pGlyphCount = 0;
	}
	
	if( count > 0 ){
		pGlyphs = new deFontGlyph[ count ];
		pGlyphCount = count;
	}
}

bool deFont::HasGlyph( int unicode ) const{
	if( unicode < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	const int group = unicode >> 8;
	if( group >= pGlyphMapCount ){
		return false;
	}
	
	const int groupIndex = pGlyphMap[ group ] - 1;
	if( groupIndex == -1 ){
		return false;
	}
	
	const int entry = unicode & 0xff;
	const int glyphIndex = pGlyphGroups[ groupIndex * 256 + entry ] - 1;
	if( glyphIndex == -1 ){
		return false;
	}
	
	return true;
}

deFontGlyph &deFont::GetGlyphAt( int index ){
	if( index < 0 || index >= pGlyphCount ){
		DETHROW( deeInvalidParam );
	}
	return pGlyphs[ index ];
}

const deFontGlyph &deFont::GetGlyphAt( int index ) const{
	if( index < 0 || index >= pGlyphCount ){
		DETHROW( deeInvalidParam );
	}
	return pGlyphs[ index ];
}

const deFontGlyph &deFont::GetGlyph(int unicode) const{
	const int index = GetGlyphIndex(unicode);
	return index != -1 ? pGlyphs[index] : pUndefinedGlyph;
}

const deFontGlyph &deFont::GetGlyph(int unicode, const deFontSize *size) const{
	if(size){
		const int index = GetGlyphIndex(unicode);
		return index != -1 ? size->GetGlyphAt(index) : size->GetUndefinedGlyph();
		
	}else{
		return GetGlyph(unicode);
	}
}

int deFont::GetGlyphIndex(int unicode) const{
	DEASSERT_TRUE(unicode >= 0)
	
	/*
	int i;
	for(i=0; i<pGlyphCount; i++){
		if(pGlyphs[i].GetUnicode() == unicode){
			return pGlyphs[i];
		}
	}
	return pUndefinedGlyph;
	*/
	
	const int group = unicode >> 8;
	if(group >= pGlyphMapCount){
		return -1;
	}
	
	const int groupIndex = pGlyphMap[group] - 1;
	if(groupIndex == -1){
		return -1;
	}
	
	const int entry = unicode & 0xff;
	return pGlyphGroups[groupIndex * 256 + entry] - 1;
}


void deFont::SetImagePath( const char *path ){
	pImagePath = path;
}

void deFont::SetImage(deImage *image){
	pImage = image;
}


int deFont::BestFixedSizeFor(int lineHeight) const{
	const int count = pFixedSizes.GetCount();
	if(count == 0){
		return 0;
	}
	
	int i, bestLineHeight = 0, secondBestLineHeight = 0;
	
	for(i=0; i<count; i++){
		const int size = pFixedSizes.GetAt(i);
		if(size >= lineHeight){
			if(bestLineHeight == 0 || size < bestLineHeight){
				bestLineHeight = size;
			}
			
		}else{
			secondBestLineHeight = decMath::max(secondBestLineHeight, size);
		}
	}
	
	return bestLineHeight != 0 ? bestLineHeight : secondBestLineHeight;
}


int deFont::GetSizeCount(){
	const deMutexGuard guard(pMutex);
	return pSizes.GetCount();
}

deFontSize &deFont::GetSizeAt(int index){
	const deMutexGuard guard(pMutex);
	return ((cFontSize*)pSizes.GetAt(index))->GetSize();
}

deFontSize *deFont::GetSizeWith(int lineHeight){
	const deMutexGuard guard(pMutex);
	return pGetSizeWith(lineHeight);
}

deFontSize *deFont::AddSize(int lineHeight, const deFontSize::Ref &size){
	DEASSERT_TRUE(lineHeight > 0)
	DEASSERT_NOTNULL(size)
	
	const deMutexGuard guard(pMutex);
	deFontSize * const found = pGetSizeWith(lineHeight);
	if(found){
		return found;
	}
	
	pSizes.Add(cFontSize::Ref::New(new cFontSize(*this, lineHeight, size)));
	
	if(pPeerGraphic){
		pPeerGraphic->FontSizeAdded(lineHeight, size);
	}
	
	return size;
}


deFontSize *deFont::PrepareSize(int lineHeight){
	int bestLineHeight = BestFixedSizeFor(lineHeight);
	if(bestLineHeight > 0){
		lineHeight = bestLineHeight;
	}
	
	return GetEngine()->GetFontManager()->LoadFontSize(*this, lineHeight);
}

deFontSize *deFont::EnsureSizePrepared(int lineHeight){
	deFontSize * const size = PrepareSize(lineHeight);
	if(size && size->GetTaskLoad()){
		GetEngine()->GetParallelProcessing().WaitForTask(size->GetTaskLoad());
		size->SetTaskLoad(nullptr);
	}
	return size;
}


decPoint deFont::TextSize(const decUnicodeString &text, const deFontSize *size) const{
	const int lineHeight = size ? size->GetLineHeight() : pLineHeight;
	const int count = text.GetLength();
	decPoint textSize(0, lineHeight);
	int i, lineWidth = 0;
	
	for(i=0; i<count; i++){
		const int character = text.GetAt(i);
		
		if(character == '\n'){
			textSize.y += lineHeight;
			lineWidth = 0;
			
		}else{
			const deFontGlyph &glyph = GetGlyph(character, size);
			const int extendEnd = glyph.GetWidth() - glyph.GetAdvance() - glyph.GetBearing();
			lineWidth += glyph.GetAdvance();
			if(lineWidth + extendEnd > textSize.x){
				textSize.x = lineWidth + extendEnd;
			}
		}
	}
	
	return textSize;
}

decPoint deFont::TextSize(const char *text, const deFontSize *size) const{
	return TextSize(decUnicodeString::NewFromUTF8(text), size);
}


// System Peers
/////////////////

void deFont::SetPeerGraphic( deBaseGraphicFont *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}



// Private Function
/////////////////////

void deFont::pFreeGlyphMap(){
	if( pGlyphMap ){
		delete [] pGlyphMap;
		pGlyphMap = NULL;
		pGlyphMapCount = 0;
	}
	
	if( pGlyphGroups ){
		delete [] pGlyphGroups;
		pGlyphGroups = NULL;
		pGlyphGroupCount = 0;
	}
}

void deFont::pCreateGlyphMap(){
	int i;
	
	for( i=0; i<pGlyphCount; i++ ){
		const int unicode = pGlyphs[ i ].GetUnicode();
		if( unicode < 0 ){
			DETHROW( deeInvalidParam );
		}
		
		const int group = unicode >> 8;
		const int entry = unicode & 0xff;
		
		if( group >= pGlyphMapCount ){
			const int newCount = group + 1;
			unsigned short * const map = new unsigned short[ newCount ];
			if( pGlyphMap ){
				memcpy( map, pGlyphMap, sizeof( unsigned short ) * pGlyphMapCount );
				delete [] pGlyphMap;
			}
			memset( map + pGlyphMapCount, 0, sizeof( unsigned short )
				* ( newCount - pGlyphMapCount ) );
			pGlyphMap = map;
			pGlyphMapCount = newCount;
		}
		
		if( pGlyphMap[ group ] == 0 ){
			const unsigned short mapIndex = ( unsigned short )( 1 + pGlyphGroupCount );
			
			const int newCount = pGlyphGroupCount + 1;
			unsigned short * const groups = new unsigned short[ newCount * 256 ];
			if( pGlyphGroups ){
				memcpy( groups, pGlyphGroups, sizeof( unsigned short )
					* pGlyphGroupCount * 256 );
				delete [] pGlyphGroups;
			}
			memset( groups + pGlyphGroupCount * 256, 0, sizeof( unsigned short )
				* ( newCount - pGlyphGroupCount ) * 256 );
			pGlyphGroups = groups;
			pGlyphGroupCount = newCount;
			
			pGlyphMap[ group ] = mapIndex;
		}
		
		pGlyphGroups[ ( pGlyphMap[ group ] - 1 ) * 256 + entry ] = ( unsigned short )( i + 1 );
	}
}

deFontSize *deFont::pGetSizeWith(int lineHeight){
	const int count = pSizes.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		const cFontSize &fs = *(cFontSize*)pSizes.GetAt(i);
		if(fs.GetLineHeight() == lineHeight){
			return fs.GetSize();
		}
	}
	
	return nullptr;
}
