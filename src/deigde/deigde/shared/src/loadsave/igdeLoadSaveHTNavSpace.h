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

#ifndef _IGDELOADSAVEHTNAVSPACE_H_
#define _IGDELOADSAVEHTNAVSPACE_H_

#include <dragengine/common/string/decString.h>

class igdeEnvironment;

class deHeightTerrainNavSpace;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * \brief Load and save height terrain navigation space file.
 */
class DE_DLL_EXPORT igdeLoadSaveHTNavSpace{
private:
	igdeEnvironment &pEnvironment;
	decString pLoggingSource;
	
	decString pName;
	decString pPattern;
	decString pDefaultExtension;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load save object. */
	igdeLoadSaveHTNavSpace(igdeEnvironment &environment, const char *loggingSource);
	
	/** \brief Clean up load save object. */
	virtual ~igdeLoadSaveHTNavSpace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name to display in file dialog. */
	inline const decString &GetName() const{return pName;}
	
	/** \brief Set name to display in file dialog. */
	void SetName(const char *name);
	
	/** \brief File pattern to display in file dialog. */
	inline const decString &GetPattern() const{return pPattern;}
	
	/** \brief Set file pattern to display in file dialog. */
	void SetPattern(const char *pattern);
	
	/** \brief Default file extension. */
	inline const decString &GetDefaultExtension() const{return pDefaultExtension;}
	
	/** \brief Set default file extension. */
	void SetDefaultExtension(const char *extension);
	
	
	
	/** \brief Load height terrain navigation space. */
	virtual void Load(deHeightTerrainNavSpace &navspace, decBaseFileReader &reader);
	
	/** \brief Save height terrain navigation space. */
	virtual void Save(const deHeightTerrainNavSpace &navspace, decBaseFileWriter &writer);
	/*@}*/
};

#endif
