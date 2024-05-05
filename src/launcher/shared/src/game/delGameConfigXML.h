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

#ifndef _DELGAMECONFIGXML_H_
#define _DELGAMECONFIGXML_H_

#include "../utils/delSharedConfigXML.h"

class delGame;
class decBaseFileReader;
class decBaseFileWriter;
class delLauncher;


/**
 * \brief Load/Save Game Config XML.
 */
class DE_DLL_EXPORT delGameConfigXML : public delSharedConfigXML{
private:
	delLauncher &pLauncher;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game config xml read/save. */
	delGameConfigXML( delLauncher &launcher );
	
	/** \brief Clean up game config xml read/save. */
	virtual ~delGameConfigXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, delGame &game );
	
	/** \brief Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const delGame &game );
	/*@}*/
	
	
	
private:
	void pWriteConfig( decXmlWriter &writer, const delGame &game );
	
	void pReadConfig( const decXmlElementTag &root, delGame &game );
};

#endif
