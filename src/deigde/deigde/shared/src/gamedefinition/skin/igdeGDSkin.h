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

#ifndef _IGDEGDSKIN_H_
#define _IGDEGDSKIN_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/image/deImage.h>



/**
 * \brief Game Skin.
 * 
 * Contains information about a game skin file. The path is the unique
 * identifier of a game skin. The name is an optional name used to display
 * the skin in listings. The description is an optional too giving some
 * more in depth description of the skin if required. Every skin has a
 * cathegory and optional tags. The cathegory is used to classify a skin
 * in a tree like list whereas tags are used to filter a flat list of
 * skins the user is interested in. The cathegory is a unix path like
 * string in absolute form.
 */
class DE_DLL_EXPORT igdeGDSkin : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeGDSkin> Ref;
	
	
private:
	decString pPath;
	decString pName;
	decString pDescription;
	decString pCategory;
	deImage::Ref pPreviewImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game definition skin. */
	igdeGDSkin(const char *path, const char *name);
	
	/** \brief Create game definition skin as a copy of a another game definition skin. */
	igdeGDSkin(const igdeGDSkin &skin);
	
protected:
	/** \breif Clean up game definition skin. */
	virtual ~igdeGDSkin();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription(const char *description);
	
	/** \brief Category. */
	inline const decString &GetCategory() const{ return pCategory; }
	
	/** \brief Set category. */
	void SetCategory(const char *category);
	
	/** \brief Preview image or nullptr if not created. */
	inline const deImage::Ref &GetPreviewImage() const{ return pPreviewImage; }
	
	/** \brief Set preview image or nullptr if not created. */
	void SetPreviewImage(deImage *image);
	/*@}*/
};

#endif
