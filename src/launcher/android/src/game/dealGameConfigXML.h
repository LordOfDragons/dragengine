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

#ifndef _DEALGAMECONFIGXML_H_
#define _DEALGAMECONFIGXML_H_

#include "../dealBaseXML.h"

class dealGame;
class dealGameProfile;
class dealGPModule;
class decBaseFileReader;
class decBaseFileWriter;
class dealGameManager;



/**
 * \brief Load/Save Game Config XML.
 */
class dealGameConfigXML : public dealBaseXML{
private:
	dealGameManager &pGameManager;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game config xml read/save. */
	dealGameConfigXML( deLogger *logger, const char *loggingSource, dealGameManager &gameManager );
	
	/** \brief Clean up game config xml read/save. */
	virtual ~dealGameConfigXML();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Read from XML file. */
	void ReadFromFile( decBaseFileReader &reader, dealGame &game );
	
	/** \brief Write to XML file. */
	void WriteToFile( decBaseFileWriter &writer, const dealGame &game );
	/*@}*/
	
private:
	void pWriteConfig( decXmlWriter &writer, const dealGame &game );
	void pWriteProfiles( decXmlWriter &writer, const dealGame &game );
	void pWriteProfile( decXmlWriter &writer, const dealGameProfile &profile );
	void pWriteProfileSystems( decXmlWriter &writer, const dealGameProfile &profile );
	void pWriteProfileDisableModuleVersions( decXmlWriter &writer, const dealGameProfile &profile );
	void pWriteProfileModules( decXmlWriter &writer, const dealGameProfile &profile );
	void pWriteProfileModule( decXmlWriter &writer, const dealGPModule &module );
	void pWriteProfileModuleParameters( decXmlWriter &writer, const dealGPModule &module );
	void pWriteProfileWindow( decXmlWriter &writer, const dealGameProfile &profile );
	
	void pReadConfig( const decXmlElementTag &root, dealGame &game );
	void pReadProfiles( const decXmlElementTag &root, dealGame &game );
	void pReadProfile( const decXmlElementTag &root, dealGame &game );
	void pReadProfileSystems( const decXmlElementTag &root, dealGameProfile &profile );
	void pReadProfileDisableModuleVersions( const decXmlElementTag &root, dealGameProfile &profile );
	void pReadProfileModules( const decXmlElementTag &root, dealGameProfile &profile );
	void pReadProfileModule( const decXmlElementTag &root, dealGameProfile &profile );
	void pReadProfileModuleParameters( const decXmlElementTag &root, dealGPModule &module );
	void pReadProfileWindow( const decXmlElementTag &root, dealGameProfile &profile );
};

#endif
