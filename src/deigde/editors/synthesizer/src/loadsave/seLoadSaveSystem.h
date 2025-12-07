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
	seLoadSaveSystem(seWindowMain &windowMain);
	
	/** \brief Clean up load/save system. */
	~seLoadSaveSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline seWindowMain &GetWindowMain() const{return pWindowMain;}
	
	/** \brief Load/save synthesizer. */
	inline seLoadSaveSynthesizer *GetLSSynthesizer(){return pLSSynthesizer;}
	
	/** \brief Load synthesizer from file. */
	seSynthesizer *LoadSynthesizer(const char *filename, const char *basePath = "/");
	
	/** \brief Save synthesizer to file. */
	void SaveSynthesizer(seSynthesizer *synthesizer, const char *filename);
	
	/** \brief Synthesizer file pattern list. */
	inline const igdeFilePatternList &GetSynthesizerFilePatterns() const{return pFPSynthesizer;}
	/*@}*/
	
	
	
private:
	void pBuildFilePattern();
};

#endif
