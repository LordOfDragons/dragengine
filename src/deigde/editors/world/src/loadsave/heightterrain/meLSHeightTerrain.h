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

#ifndef _MELSHEIGHTTERRAIN_H_
#define _MELSHEIGHTTERRAIN_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/string/decString.h>

class meHeightTerrain;
class decXmlElementTag;
class decBaseFileReader;
class decBaseFileWriter;
class deLogger;



/**
 * \brief Load Height Terrain Interface.
 */
class meLSHeightTerrain : public igdeBaseXML{
private:
	decString pName;
	decString pPattern;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new loader. */
	meLSHeightTerrain(deLogger *logger, const char *loggerSource);
	/** Cleans up the loader. */
	virtual ~meLSHeightTerrain();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const char *GetName() const{ return pName.GetString(); }
	/** Sets the name. */
	void SetName(const char *name);
	/** Retrieves the pattern. */
	inline const char *GetPattern() const{ return pPattern.GetString(); }
	/** Sets the pattern. */
	void SetPattern(const char *pattern);
	
	/** Loads a height terrain. */
	virtual void LoadFromFile(meHeightTerrain &heightTerrain, decBaseFileReader &file) = 0;
	/** Saves the height terrain. */
	virtual void SaveToFile(meHeightTerrain &heightTerrain, decBaseFileWriter &file) = 0;
	/*@}*/
};

// end of include only once
#endif
