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

// include only once
#ifndef _FEFONTIMAGE_H_
#define _FEFONTIMAGE_H_

// includes
#include <dragengine/deObject.h>

#include <dragengine/common/string/decString.h>

// predefinitions
class feFont;

class deEngine;
#include <dragengine/resources/image/deImage.h>



/**
 * @brief Font Image.
 * Image in a font containing glyphs.
 */
class feFontImage : public deObject{
private:
	deEngine *pEngine;
	feFont *pParentFont;
	
	deImage::Ref pEngImage;
	
	decString pFilename;
	int pWidth;
	int pHeight;
	
	bool pChanged;
	bool pSaved;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<feFontImage> Ref;


	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new font image. */
	feFontImage(deEngine *engine);
	/** Cleans up the font image. */
	virtual ~feFontImage();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the linked engine object. */
	inline deEngine *GetEngine() const{ return pEngine; }
	/** Retrieves the engine image. */
	inline const deImage::Ref &GetEngineImage() const{ return pEngImage; }
	
	/** Retrieves the parent font or NULL if not assigned. */
	inline feFont *GetParentFont() const{ return pParentFont; }
	/** Sets the parent font or NULL it not assigned. */
	void SetParentFont(feFont *font);
	
	/** Retrieves the width of the image. */
	inline int GetWidth() const{ return pWidth; }
	/** Retrieves the height of the image. */
	inline int GetHeight() const{ return pHeight; }
	/** Sets the size of the image. */
	void SetSize(int width, int height);
	/** Retrieves the filename. */
	inline const decString &GetFilename() const{ return pFilename; }
	/** Sets the filename. */
	void SetFilename(const char *filename, bool load);
	/** Loads the image from the stored path. */
    void LoadImage();
	
	/** Determines if the image has changed. */
	inline bool GetChanged() const{ return pChanged; }
	/** Sets if the image has changed. */
	void SetChanged(bool changed);
	/** Determines if the image has been saved. */
	inline bool GetSaved() const{ return pSaved; }
	/** Sets if the image has been saved. */
	void SetSaved(bool saved);
	
	/** Notifies the parent if assigned that the image changed. */
    void NotifyImageChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif
