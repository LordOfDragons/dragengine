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

#ifndef _MEHELPERS_H_
#define _MEHELPERS_H_

#include <dragengine/common/math/decMath.h>

#include <deigde/gamedefinition/property/igdeGDProperty.h>


class meObject;
class igdeGDClass;
class igdeGDCComponent;
class igdeGDCamera;
class igdeGDCLight;
class igdeGDCCTexture;
class igdeGDPropertyList;
class igdeGDCNavigationSpace;
class igdeGDCNavigationBlocker;

class deObjectReference;
class decString;
class decStringList;


namespace meHelpers{
	/** \brief Find first game definition component. */
	igdeGDCComponent *FindFirstComponent( const igdeGDClass *gdclass );
	
	/** \brief Find first game definition component. */
	igdeGDCComponent *FindFirstComponent( const igdeGDClass &gdclass );
	
	/** \brief Find first game definition component. */
	bool FindFirstComponent( const igdeGDClass &gdclass, igdeGDCComponent* &component );
	
	/** \brief Find first game definition component. */
	bool FindFirstComponent( const igdeGDClass &gdclass, decString &prefix, igdeGDCComponent* &component );
	
	
	
	/** \brief Find first game definition light. */
	igdeGDCLight *FindFirstLight( const igdeGDClass *gdclass );
	
	/** \brief Find first game definition light. */
	igdeGDCLight *FindFirstLight( const igdeGDClass &gdclass );
	
	/** \brief Find first game definition light. */
	bool FindFirstLight( const igdeGDClass &gdclass, igdeGDCLight* &light );
	
	/** \brief Find first game definition light. */
	bool FindFirstLight( const igdeGDClass &gdclass, decString &prefix, igdeGDCLight* &light );
	
	
	
	/** \brief Find first game definition navigationSpace. */
	igdeGDCNavigationSpace *FindFirstNavigationSpace( const igdeGDClass *gdclass );
	
	/** \brief Find first game definition navigationSpace. */
	igdeGDCNavigationSpace *FindFirstNavigationSpace( const igdeGDClass &gdclass );
	
	/** \brief Find first game definition navigationSpace. */
	bool FindFirstNavigationSpace( const igdeGDClass &gdclass, igdeGDCNavigationSpace* &navigationSpace );
	
	/** \brief Find first game definition navigationSpace. */
	bool FindFirstNavigationSpace( const igdeGDClass &gdclass, decString &prefix, igdeGDCNavigationSpace* &navigationSpace );
	
	
	
	/** \brief Find first game definition navigationBlocker. */
	igdeGDCNavigationBlocker *FindFirstNavigationBlocker( const igdeGDClass *gdclass );
	
	/** \brief Find first game definition navigationBlocker. */
	igdeGDCNavigationBlocker *FindFirstNavigationBlocker( const igdeGDClass &gdclass );
	
	/** \brief Find first game definition navigationBlocker. */
	bool FindFirstNavigationBlocker( const igdeGDClass &gdclass, igdeGDCNavigationBlocker* &navigationBlocker );
	
	/** \brief Find first game definition navigationBlocker. */
	bool FindFirstNavigationBlocker( const igdeGDClass &gdclass, decString &prefix, igdeGDCNavigationBlocker* &navigationBlocker );
	
	
	
	/** \brief Find first game definition camera. */
	igdeGDCamera *FindFirstCamera( const igdeGDClass *gdclass );
	
	/** \brief Find first game definition camera. */
	igdeGDCamera *FindFirstCamera( const igdeGDClass &gdclass );
	
	/** \brief Find first game definition camera. */
	bool FindFirstCamera( const igdeGDClass &gdclass, igdeGDCamera* &camera );
	
	/** \brief Find first game definition camera. */
	bool FindFirstCamera( const igdeGDClass &gdclass, decString &prefix, igdeGDCamera* &camera );
	
	
	
	/** \brief Get names of all path type properties matching pattern type. */
	void GetPatternTypePropertyNames( const igdeGDClass &gdclass,
		igdeGDProperty::ePathPatternTypes patternType, decStringList &names );
	
	
	
	/** \brief Create texture. */
	void CreateTexture( deObjectReference &texture, meObject *object, const char *textureName );
	
	/** \brief Create texture. */
	void CreateTexture( deObjectReference &texture, meObject *object, const char *textureName,
		const igdeGDCCTexture * gdctexture );
};

#endif
