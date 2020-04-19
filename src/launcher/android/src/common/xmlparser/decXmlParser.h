/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DECXMLPARSER_H_
#define _DECXMLPARSER_H_

#define DEXP_EOF 0x100

class decXmlDocument;
class decXmlComment;
class decXmlContainer;
class decXmlElement;
class decXmlVisitor;
class decXmlAttValue;
class decBaseFileReader;
class deLogger;



/**
 * \brief XML Parser.
 *
 * The XML Paser processes an XML file provided by a file reader object. The content of
 * the file is parsed and syntax checked but not validated. The resulting XML tree is
 * then available in the document. One parser can not parse two XML files at the same time.
 *
 * A typical scenario looks like this:
 * \code decXMLParser parser;
 * decXmlDocument document;
 * if( parser.ParseXml( myFileReader, &document ) ){
 *     // success
 * }else{
 *     // failure
 * }
 * \endcode
 */
class decXmlParser{
private:
	decBaseFileReader *pFile;
	int pLine;
	int pPos;
	int pCurChar;
	char *pToken;
	int pTokenLen;
	int pTokenSize;
	int pTokenLine;
	int pTokenPos;
	char *pCleanString;
	int pCleanStringSize;
	int pFilePos;
	int pFileLen;
	
	deLogger *pLogger;
	bool pHasFatalError;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \briefCreate xml parser. */
	decXmlParser( deLogger *logger );
	
	/** Clean up xml parser. */
	virtual ~decXmlParser();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Parse XML file using the given file reader into the given document object.
	 * 
	 * Only one XML file can be processed at the same time. Calling ParseXML on the same
	 * parser while one instance is running results in the second invokation to fail.
	 * 
	 * \return true on success or false otherwise
	 */
	bool ParseXml( decBaseFileReader *file, decXmlDocument *doc );
	/*@}*/
	
	
	
	/**
	 * \name Error Handling
	 * 
	 * Those functions are provided to allow applications to capture all
	 * errors during parsing time. Simply overwrite the functions you
	 * are interested in.
	 */
	/*@{*/
	/**
	 * \brief The end of the XML file has been reached although not allowed yet.
	 * 
	 * \param line Line number where the error occured
	 * \param pos Position from the beginning of the line where the error occured.
	 */
	virtual void UnexpectedEOF( int line, int pos );
	
	/**
	 * \brief A token has been parse that is not expected at this place.
	 * 
	 * \param line Line number where the error occured
	 * \param pos Position from the beginning of the line where the error occured.
	 * \param token The unexpected token in unparsed form
	 */
	virtual void UnexpectedToken( int line, int pos, const char *token );
	/*@}*/
	
	
	
	/**
	 * \name Parsing Functions
	 * 
	 * Those functions are used only by the ParseXml and should not be called
	 * directly except you want to write an extended XML parser.
	 */
	/*@{*/
	/** \brief Prepare parsing the file by reseting all counters. */
	void PrepareParse( decBaseFileReader *file );
	
	/** \brief Parse XML file. */
	void ParseDocument( decXmlDocument *doc );
	
	/** \brief Parse XML file prolog. */
	void ParseProlog( decXmlDocument *doc );
	
	/** \brief Parse XML Declaration. */
	void ParseXMLDecl( decXmlDocument *doc );
	
	/** \brief Parse document type declaration. */
	void ParseDocTypeDecl( decXmlDocument *doc );
	
	/** \brief Parse system literal. */
	void ParseSystemLiteral( decXmlDocument *doc );
	
	/** \brief Parse public literal. */
	void ParsePublicLiteral( decXmlDocument *doc );
	
	/**
	 * \brief Parse element tag but only if the tag name matches requiredName.
	 * \return true if an element tag has been parsed
	 */
	bool ParseElementTag( decXmlContainer *container, const char *requiredName );
	
	/**
	 * \brief Parse reference if one exists.
	 * \return true if a reference has been parsed
	 */
	bool ParseReference( decXmlContainer *container );
	
	/**
	 * \brief Parse cd section if one exists.
	 * \return true if a cd section has been parsed
	 */
	bool ParseCDSect( decXmlContainer *container );
	
	/** \brief Parse attribute. */
	void ParseAttribute( decXmlContainer *container );
	
	/** \brief Parse attribute value. */
	void ParseAttValue( decXmlAttValue *value );
	
	/**
	 * \brief Check if next token matches a certain name.
	 * 
	 * If found the token is consumed.
	 * 
	 * \return true if the expected token has been found
	 */
	bool ParseToken( const char *expected );
	
	/** \brief Parse value assignement. */
	void ParseEquals();
	
	/** \brief Parse white spaces and returns the number of white spaces found. */
	int ParseSpaces();
	
	/** \brief Parse enconding name. */
	void ParseEncName( decXmlDocument *doc );
	
	/** \brief Parses any number of consequtive comments, pi or white spaces. */
	void ParseMisc( decXmlContainer *container );
	
	/**
	 * \brief Parse comment if present.
	 * \return true if a comment has been parsed
	 */
	bool ParseComment( decXmlContainer *container );
	
	/**
	 * \brief Parse process instruction if present.
	 * \return true if a process instruction has been parsed
	 */
	bool ParsePI( decXmlContainer *container );
	
	/**
	 * \brief Parse name token.
	 * 
	 * The parsing starts offset character ahead of the current position. If a valid
	 * name token has been parse it is removed only if autoRemove is true.
	 * 
	 * \return offset to the last character in the token measured from the current position.
	 */
	int ParseName( int offset, bool autoRemove );
	
	/** \brief token starting offset characters ahead of current position matches name. */
	bool TestToken( int offset, const char *expected );
	/*@}*/
	
	
	
	/** \name Testing */
	/*@{*/
	bool IsLatinLetter( int aChar );
	bool IsLatinDigit( int aChar );
	bool IsHex( int aChar );
	bool IsLetter( int aChar );
	bool IsDigit( int aChar );
	bool IsExtender( int aChar );
	bool IsBaseChar( int aChar );
	bool IsCombiningChar( int aChar );
	bool IsIdeographic( int aChar );
	bool IsChar( int aChar );
	bool IsPubidChar( int aChar, bool restricted );
	bool IsSpace( int aChar );
	/*@}*/
	
	
	
	/** \name Token Management */
	/*@{*/
	/**
	 * \brief Character at the given index ahead from the current position.
	 * 
	 * If the token buffer does not hold this character yet all characters up to
	 * this position are read into the token buffer.
	 */
	int GetTokenAt( int index );
	
	/** \brief Line number of the current token. */
	inline int GetTokenLineNumber() const{ return pTokenLine; }
	
	/** \brief Position number of the current token. */
	inline int GetTokenPositionNumber() const{ return pTokenPos; }
	
	/** \brief Clear current token buffer. */
	void ClearToken();
	
	/** \brief Remove given number of characters from the beginning of the token buffer. */
	void RemoveFromToken( int length );
	
	/** \brief Add character to the token buffer. */
	void AddCharToToken( int aChar );
	
	/** \brief Current position is at the end of the xml file. */
	bool IsEOF();
	
	/** \brief Raise fatal error calling error handler then throw exception. */
	void RaiseFatalError();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Clean string buffer. */
	inline const char *GetCleanString() const{ return (const char *)pCleanString ; }
	
	/** \brief Copy number of characters from token buffer to clean string buffer 0-terminated. */
	void SetCleanString( int length );
	/*@}*/
	
	
	
private:
	void pGetNextChar();
	void pGetNextCharAndAdd();
	void pGrowToken();
	void pAddCharacterData( decXmlContainer *container, const char *text, int line, int pos );
	void pAddCharacterData( decXmlContainer *container, char character, int line, int pos );
};

#endif
