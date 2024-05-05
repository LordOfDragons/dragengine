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

#ifndef _DEALENGINEMODULEXML_H_
#define _DEALENGINEMODULEXML_H_

#include "dealEngineModule.h"
#include "../../dealBaseXML.h"
#include "../../common/string/decString.h"

class decBaseFileReader;
class decBaseFileWriter;



/**
 * @brief Load Engine Module XML.
 */
class dealEngineModuleXML : public dealBaseXML{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new engine module xml read. */
	dealEngineModuleXML( deLogger *logger, const char *loggerSource );
	/** Cleans up the engine module xml read. */
	virtual ~dealEngineModuleXML();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, dealEngineModule &module );
	/*@}*/
	
private:
	void pReadModule( const decXmlElementTag &root, dealEngineModule &module );
	void pReadModuleLibrary( const decXmlElementTag &root, dealEngineModule &module );
	dealEngineModule::eModuleTypes pModuleTypeFromString( const char *typeString ) const;
};

#endif // _DEGLGAME_H_
