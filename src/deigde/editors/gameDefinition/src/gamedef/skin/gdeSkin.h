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

#ifndef _GDESKIN_H_
#define _GDESKIN_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decStringSet.h>

class gdeGameDefinition;


/**
 * \brief Skin.
 */
class gdeSkin : public deObject{
private:
	gdeGameDefinition *pGameDefinition;
	
	decString pPath;
	decString pName;
	decString pDescription;
	decString pCategory;
	decStringSet pTags;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<gdeSkin> Ref;

	/** \brief Type holding list. */
	typedef decTObjectOrderedSet<gdeSkin> List;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin. */
	gdeSkin();
	
	/** \brief Create skin. */
	gdeSkin(const char *path, const char *name);
	
	/** \brief Create copy of skin. */
	gdeSkin(const gdeSkin &skin);
	
	/** \brief Clean up skin. */
protected:
	virtual ~gdeSkin();
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent game definition. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set parent game definition. */
	void SetGameDefinition(gdeGameDefinition *gamedef);
	
	
	
	/** \brief Path. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set path. */
	void SetPath(const char *path);
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *name);
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description. */
	void SetDescription(const char *description);
	
	/** \brief Cathegory. */
	inline const decString &GetCategory() const{ return pCategory; }
	
	/** \brief Set cathegory. */
	void SetCategory(const char *category);
	
	/** \brief Tags. */
	inline const decStringSet &GetTags() const{ return pTags; }
	
	/** \brief Set tags. */
	void SetTags(const decStringSet &tags);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
