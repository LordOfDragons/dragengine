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

#ifndef _PEELOADSAVESYSTEM_H_
#define _PEELOADSAVESYSTEM_H_

#include <deigde/gui/filedialog/igdeFilePatternList.h>

class peeLoadSaveEmitter;
class peeWindowMain;
class peeEmitter;
class igdeGameDefinition;



/**
 * \brief Load/Save System.
 * Provides loading and saving support.
 */
class peeLoadSaveSystem{
private:
	peeWindowMain &pWindowMain;
	
	peeLoadSaveEmitter *pLSEmitter;
	igdeFilePatternList pFPEmitter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new load/save system. */
	peeLoadSaveSystem( peeWindowMain &windowMain );
	/** \brief Clean up the load/save system. */
	~peeLoadSaveSystem();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline peeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** Retrieves the load save emitter. */
	inline peeLoadSaveEmitter &GetLSEmitter(){ return *pLSEmitter; }
	/** Loads the emitter from file if possible. */
	peeEmitter *LoadEmitter( const char *filename );
	/** Saves the emitter to file if possible. */
	void SaveEmitter( peeEmitter *emitter, const char *filename );
	/** Retrieves the file pattern list. */
	inline const igdeFilePatternList *GetEmitterFilePatterns() const{ return &pFPEmitter; }
	/*@}*/
	
private:
	void pBuildFilePattern();
};

#endif
