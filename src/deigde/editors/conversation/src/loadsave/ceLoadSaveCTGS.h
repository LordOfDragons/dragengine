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

#ifndef _CELOADSAVECTGS_H_
#define _CELOADSAVECTGS_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class cePlayback;
class ceLoadSaveSystem;
class ceConversation;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Load/Save conversation test game state.
 */
class ceLoadSaveCTGS : public igdeBaseXML{
private:
	ceLoadSaveSystem &pLoadSaveSystem;
	decString pName;
	decString pPattern;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create loader. */
	ceLoadSaveCTGS( ceLoadSaveSystem &loadSaveSystem, deLogger *logger, const char *loggerSource );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Load conversation test game state. */
	void LoadCTGS( ceConversation &conversation, decBaseFileReader &reader );
	
	/** \brief Save conversation test game state. */
	void SaveCTGS( const ceConversation &conversation, decBaseFileWriter &writer );
	
	
	
	/** \brief Write game state tag. */
	void WriteGameState( decXmlWriter &writer, const cePlayback &playback, const char *tagName );
	
	/** \brief Read game state tag. */
	void ReadGameState( const decXmlElementTag &root, cePlayback &playback );
	/*@}*/
	
	
	
private:
};

#endif
