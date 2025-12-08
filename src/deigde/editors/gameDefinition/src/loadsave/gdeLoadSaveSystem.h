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

#ifndef _GDELOADSAVESYSTEM_H_
#define _GDELOADSAVESYSTEM_H_

#include <deigde/gui/filedialog/igdeFilePatternList.h>

#include "gdeLoadSaveGameDefinition.h"
#include "gdeLoadSaveXmlEClass.h"

class gdeWindowMain;
class gdeGameDefinition;
class gdeObjectClass;
class igdeGameDefinition;



/**
 * \brief Load/save system.
 */
class gdeLoadSaveSystem{
private:
	gdeWindowMain &pWindowMain;
	
	gdeLoadSaveGameDefinition pLSGameDef;
	igdeFilePatternList pFPGameDef;
	
	gdeLoadSaveXmlEClass pLSXmlEClass;
	igdeFilePatternList pFPXmlEClass;
	
	decString pFoxFPLGameDef;
	decString pFoxFPLXmlEClass;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save system. */
	gdeLoadSaveSystem(gdeWindowMain &windowMain);
	
	/** \brief Clean up load/save system. */
	~gdeLoadSaveSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load save game definition. */
	inline gdeLoadSaveGameDefinition &GetLSGameDefinition(){ return pLSGameDef; }
	
	/** \brief Load game definition from file. */
	gdeGameDefinition::Ref LoadGameDefinition(const char *filename);
	
	/** \brief Save game definition to file. */
	void SaveGameDefinition(const gdeGameDefinition &gameDefinition, const char *filename);
	
	/** \brief File pattern list. */
	inline const igdeFilePatternList &GetGameDefFilePatterns() const{ return pFPGameDef; }
	
	/** \brief FOX game definition file pattern list. */
	inline const decString &GetFoxFPLGameDefinition() const{ return pFoxFPLGameDef; }
	
	
	
	/** \brief Load save xml element class. */
	inline gdeLoadSaveXmlEClass &GetLSXmlEClass(){ return pLSXmlEClass; }
	
	/** \brief Load xml element class from file. */
	gdeObjectClass *LoadXmlEClass(const char *filename);
	
	/** \brief Save xml element class to file. */
	void SaveXmlEClass(const gdeGameDefinition &gameDefinition,
		const gdeObjectClass &objectClass, const char *filename);
	
	/** \brief XML element class file pattern list. */
	inline const igdeFilePatternList &GetXmlEClassFilePatterns() const{ return pFPXmlEClass; }
	
	/** \brief FOX XML element class file pattern list. */
	inline const decString &GetFoxFPLXmlEClass() const{ return pFoxFPLXmlEClass; }
	/*@}*/
	
	
	
private:
	void pBuildFilePattern();
	void pConvertToFOX(const igdeFilePatternList &fpl, decString &foxfpl);
};

#endif
