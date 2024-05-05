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

#ifndef _SAELOADSAVESANIMATION_H_
#define _SAELOADSAVESANIMATION_H_

#include <deigde/utils/igdeBaseXML.h>

#include <dragengine/common/string/decString.h>

class saeSAnimation;
class saePhoneme;
class saeWord;
class saeLoadSaveSystem;

class decBaseFileReader;
class decBaseFileWriter;
class decXmlWriter;
class decXmlElementTag;



/**
 * Load/Save Speech Animation.
 */
class saeLoadSaveSAnimation : public igdeBaseXML{
private:
	saeLoadSaveSystem *pLSSys;
	decString pName;
	decString pPattern;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new loader. */
	saeLoadSaveSAnimation( saeLoadSaveSystem *lssys, deLogger *logger, const char *loggerSource );
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Retrieves the pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** Loads the speech animation from file. */
	void LoadSAnimation( saeSAnimation &sanimation, decBaseFileReader &reader );
	/** Saves the speech animation to file. */
	void SaveSAnimation( const saeSAnimation &sanimation, decBaseFileWriter &writer );
	/*@}*/
	
private:
	void pWriteSAnimation( decXmlWriter &writer, const saeSAnimation &sanimation );
	void pWriteDisplay( decXmlWriter &writer, const saeSAnimation &sanimation );
	void pWritePhoneme( decXmlWriter &writer, const saePhoneme &phoneme );
	void pWriteWord( decXmlWriter &writer, const saeWord &word );
	
	void pReadSAnimation( const decXmlElementTag &root, saeSAnimation &sanimation );
	void pReadDisplay( const decXmlElementTag &root, saeSAnimation &sanimation );
	void pReadPhoneme( const decXmlElementTag &root, saeSAnimation &sanimation );
	void pReadWord( const decXmlElementTag &root, saeSAnimation &sanimation );
};

#endif
