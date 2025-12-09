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

#ifndef _DEFONTMANAGER_H_
#define _DEFONTMANAGER_H_

#include "deFont.h"
#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"

class deEngine;
class deFontSize;
class deFontBuilder;


/**
 * \brief Font resource manager.
 */
class DE_DLL_EXPORT deFontManager : public deFileResourceManager{
private:
	deFileResourceList pFonts;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create font resource manager. */
	deFontManager(deEngine *engine);
	
	/** \brief Clean up font resource manager and reports leaking resources. */
	~deFontManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of fonts. */
	int GetFontCount() const;
	
	/** \brief Root font resource for iteration purpose. */
	deFont *GetRootFont() const;
	
	/** \brief Font with filename or NULL. */
	deFont *GetFontWith(const char *filename) const;
	
	/** \brief Font with filename or NULL. */
	deFont *GetFontWith(deVirtualFileSystem *vfs, const char *filename) const;
	
	/** \brief Create new font. */
	deFont::Ref CreateFont(const char *filename, deFontBuilder &builder);
	
	/** \brief Create new font. */
	deFont::Ref CreateFont(deVirtualFileSystem *vfs, const char *filename, deFontBuilder &builder);
	
	/** \brief Load font from file relative to base path. */
	deFont::Ref LoadFont(const char *filename, const char *basePath);
	
	/** \brief Load font from file relative to base path. */
	deFont::Ref LoadFont(deVirtualFileSystem *vfs, const char *filename, const char *basePath);
	
	/** \brief Load debug font. */
	deFont::Ref LoadDebugFont();
	
	/**
	 * \brief Add loaded and prepared font.
	 * 
	 * \warning To be used only by deResourceLoader.
	 */
	void AddLoadedFont(deFont *font);
	
	/**
	 * \brief Load font size if supported and required.
	 * 
	 * If loading font size is supported font size instance is returned otherwise nullptr.
	 * The returned font size is loading in parallel if a load task is assigned. In this case
	 * to not use the font size until the load task is finished.
	 */
	deFontSize *LoadFontSize(deFont &font, int size);
	
	/** \brief Release leaking resources and report them. */
	void ReleaseLeakingResources() override;
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	/** \brief Create graphic system peers. */
	void SystemGraphicLoad() override;
	
	/** \brief Free graphic system peers. */
	void SystemGraphicUnload() override;
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * 
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource(deResource *resource) override;
	/*@}*/
	
	
	
private:
	void pLoadFontSources(deFont *font);
};

#endif
