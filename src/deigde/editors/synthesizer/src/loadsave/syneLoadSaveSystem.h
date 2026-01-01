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

#ifndef _SYNELOADSAVESYSTEM_H_
#define _SYNELOADSAVESYSTEM_H_

#include "../synthesizer/syneSynthesizer.h"

#include <deigde/gui/filedialog/igdeFilePattern.h>

class syneLoadSaveSynthesizer;
class syneWindowMain;
class igdeGameDefinition;



/**
 * \brief Load/Save System.
 */
class syneLoadSaveSystem{
private:
	syneWindowMain &pWindowMain;
	
	syneLoadSaveSynthesizer *pLSSynthesizer;
	igdeFilePattern::List pFPSynthesizer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create load/save system. */
	syneLoadSaveSystem(syneWindowMain &windowMain);
	
	/** \brief Clean up load/save system. */
	~syneLoadSaveSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline syneWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Load/save synthesizer. */
	inline syneLoadSaveSynthesizer *GetLSSynthesizer(){ return pLSSynthesizer; }
	
	/** \brief Load synthesizer from file. */
	syneSynthesizer::Ref LoadSynthesizer(const char *filename, const char *basePath = "/");
	
	/** \brief Save synthesizer to file. */
	void SaveSynthesizer(syneSynthesizer *synthesizer, const char *filename);
	
	/** \brief Synthesizer file pattern list. */
	inline const igdeFilePattern::List &GetSynthesizerFilePatterns() const{ return pFPSynthesizer; }
	/*@}*/
	
	
	
private:
	void pBuildFilePattern();
};

#endif
