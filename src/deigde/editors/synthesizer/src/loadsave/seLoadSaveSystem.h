/* 
 * Drag[en]gine IGDE Synthesizer Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _SELOADSAVESYSTEM_H_
#define _SELOADSAVESYSTEM_H_

#include <deigde/gui/filedialog/igdeFilePatternList.h>

class seLoadSaveSynthesizer;
class seWindowMain;
class seSynthesizer;
class igdeGameDefinition;
class igdeFilePatternList;



/**
 * \brief Load/Save System.
 */
class seLoadSaveSystem{
private:
	seWindowMain &pWindowMain;
	
	seLoadSaveSynthesizer *pLSSynthesizer;
	igdeFilePatternList pFPSynthesizer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save system. */
	seLoadSaveSystem( seWindowMain &windowMain );
	
	/** \brief Clean up load/save system. */
	~seLoadSaveSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline seWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Load/save synthesizer. */
	inline seLoadSaveSynthesizer *GetLSSynthesizer(){ return pLSSynthesizer; }
	
	/** \brief Load synthesizer from file. */
	seSynthesizer *LoadSynthesizer( const char *filename, const char *basePath = "/" );
	
	/** \brief Save synthesizer to file. */
	void SaveSynthesizer( seSynthesizer *synthesizer, const char *filename );
	
	/** \brief Synthesizer file pattern list. */
	inline const igdeFilePatternList &GetSynthesizerFilePatterns() const{ return pFPSynthesizer; }
	/*@}*/
	
	
	
private:
	void pBuildFilePattern();
};

#endif
