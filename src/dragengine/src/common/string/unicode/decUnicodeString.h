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

#ifndef _DECUNICODESTRING_H_
#define _DECUNICODESTRING_H_

#include "../../../dragengine_export.h"

#ifdef OS_ANDROID
#include <stdarg.h>
#endif

class decUnicodeStringList;
class decString;


/**
 * \brief Unicode string supporting encoding and decoding UTF8 formatted strings.
 */
class DE_DLL_EXPORT decUnicodeString{
private:
	void *pString; // string stored in 1, 2 or 4 byte array
	int pLength; // number of characters in the string
	int pSize; // size of the string array: 1, 2 or 4
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new empty unicode string. */
	decUnicodeString();
	
	/** \brief Create new unicode string as a copy of the given unicode string. */
	decUnicodeString( const decUnicodeString &string );
	
	/** \brief Create new unicode string as a copy of two concatenated unicode strings. */
	decUnicodeString( const decUnicodeString &string1, const decUnicodeString &string2 );
	
	/** \brief Create new unicode string from the given unicode character. */
	decUnicodeString( int character );
	
	/** \brief Create new unicode string from the given utf8 encoded string. */
	static decUnicodeString NewFromUTF8( const char *string );
	
	/** \brief Frees the unicode string. */
	~decUnicodeString();
	/*@}*/
	
	
	
	/** \name Accessors and Mutators */
	/*@{*/
	/** \brief String is empty. */
	bool IsEmpty() const;
	
	/** \brief Set string to the empty string. */
	void Empty();
	
	/** \brief Count of characters in the unicode string. */
	inline int GetLength() const{ return pLength; }
	
	/** \brief Unicode character at the given location. */
	int GetAt( int position ) const;
	
	/** \brief Set unicode character at the given location. */
	void SetAt( int position, int character );
	
	/** \brief Set string from another string. */
	void Set( const decUnicodeString &string );
	
	/** \brief Set string to a run of characters. */
	void Set( int character, int count );
	
	/** \brief Set string to a char value. */
	void SetValue( char value );
	
	/** \brief Set string to an unsigned char value. */
	void SetValue( unsigned char value );
	
	/** \brief Set string to a short value. */
	void SetValue( short value );
	
	/** \brief Set string to an unsigned short value. */
	void SetValue( unsigned short value );
	
	/** \brief Set string to a signed integer value. */
	void SetValue( int value );
	
	/** \brief Set string to an unsigned integer value. */
	void SetValue( unsigned int value );
	
	/** \brief Set string to a float value. */
	void SetValue( float value );
	
	/** \brief Set string to a double value. */
	void SetValue( double value );
	
	/** \brief Set unicode string from the given utf8 encoded string. */
	void SetFromUTF8( const char *string );
	
	/** \brief Formats a string using printf style formatting. */
	void Format( const char *format, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
		
	/** \brief Formats a string using printf style formatting. */
	void FormatUsing( const char *format, va_list args );
	
	/** \brief Appends a string. */
	void Append( const decUnicodeString &string );
	
	/** \brief Appends a character. */
	void AppendCharacter( char character );
	void AppendCharacter( unsigned char character );
	void AppendCharacter( int character );
	
	/** \brief Appends a char value. */
	void AppendValue( char value );
	
	/** \brief Appends an unsigned char value. */
	void AppendValue( unsigned char value );
	
	/** \brief Appends a short value. */
	void AppendValue( short value );
	
	/** \brief Appends an unsigned short value. */
	void AppendValue( unsigned short value );
	
	/** \brief Appends a signed integer value. */
	void AppendValue( int value );
	
	/** \brief Appends an unsigned integer value. */
	void AppendValue( unsigned int value );
	
	/** \brief Appends a float value. */
	void AppendValue( float value );
	
	/** \brief Appends a double value. */
	void AppendValue( double value );
	
	/** \brief Append unicode string from the given utf8 encoded string. */
	void AppendFromUTF8( const char *string );
	
	/** \brief Appends a formatted string using printf style formatting. */
	void AppendFormat( const char *format, ... )
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
		
	/** \brief Appends a formatted string using printf style formatting. */
	void AppendFormatUsing( const char *format, va_list args );
	
	/**
	 * \brief Retrieve index of first occurance of a character or -1 if not found.
	 * 
	 * Negative start or end is measured from the string end.
	 */
	int Find( int character ) const;
	int Find( int character, int start ) const;
	int Find( int character, int start, int end ) const;
	
	/**
	 * \brief Retrieve index of first occurance of any charatcer in a list of characters or -1 if not found.
	 * 
	 * Negative start or end is measured from the string end.
	 */
	int Find( const decUnicodeString &characters ) const;
	int Find( const decUnicodeString &characters, int start ) const;
	int Find( const decUnicodeString &characters, int start, int end ) const;
	
	/**
	 * \brief Retrieve index of last occurance of a character or -1 if not found.
	 * 
	 * Negative start or end is measured from the string end.
	 */
	int FindReverse( int character ) const;
	int FindReverse( int character, int start ) const;
	int FindReverse( int character, int start, int end ) const;
	
	/**
	 * \brief Retrieve index of last occurance of any character in a list of characters or -1 if not found.
	 * 
	 * Negative start or end is measured from the string end.
	 */
	int FindReverse( const decUnicodeString &characters ) const;
	int FindReverse( const decUnicodeString &characters, int start ) const;
	int FindReverse( const decUnicodeString &characters, int start, int end ) const;
	
	/**
	 * \brief Retrieve index of first occurance of a string or -1 if not found.
	 * 
	 * Negative start or end is measured from the string end.
	 */
	int FindString( const decUnicodeString &string ) const;
	int FindString( const decUnicodeString &string, int start ) const;
	int FindString( const decUnicodeString &string, int start, int end ) const;
	
	/**
	 * \brief Retrieve index of first occurance of a character from the end to the start or -1 if not found.
	 * 
	 * Negative start or end is measured from the string end.
	 */
	int FindStringReverse( const decUnicodeString &string ) const;
	int FindStringReverse( const decUnicodeString &string, int start ) const;
	int FindStringReverse( const decUnicodeString &string, int start, int end ) const;
	
	/** \brief First number of characters. */
	decUnicodeString GetLeft( int count ) const;
	
	/** \brief Last number of characters. */
	decUnicodeString GetRight( int count ) const;
	
	/** \brief Retrieves a sub string. Negative start or end is measured from the string end. */
	decUnicodeString GetMiddle( int start ) const;
	decUnicodeString GetMiddle( int start, int end ) const;
	
	/** \brief Reverse string. */
	void Reverse();
	
	/** \brief Retrieve reversed string. */
	decUnicodeString GetReversed() const;
	
	/** \brief Split string. */
	decUnicodeStringList Split( int character ) const;
	
	/** \brief Split string on multiple characters. */
	decUnicodeStringList Split( const decUnicodeString &characters ) const;
	
	/** \brief Replace all occurances of a character. */
	void Replace( int replaceCharacter, int withCharacter );
	
	/** \brief Replace all occurances of any character in a list of characters. */
	void Replace( const decUnicodeString &replaceCharacters, int withCharacter);
	
	/** \brief Replace all occurances of a string. */
	void ReplaceString( const decUnicodeString &replaceString, const decUnicodeString &withString );
	
	/** \brief Retrieves string with all occurances of a character replaced. */
	decUnicodeString GetReplaced( int replaceCharacter, int withCharacter ) const;
	
	/** \brief Retrieves string with all occurances of any character of a list of characters replaced. */
	decUnicodeString GetReplaced(const decUnicodeString &replaceCharacters, int withCharacter) const;
	
	/** \brief Retrieves string with all occurances of a string replaced. */
	decUnicodeString GetReplacedString( const decUnicodeString &replaceString, const decUnicodeString &withString ) const;
	
	/** \brief Removes whitespaces from the beginning. */
	void TrimLeft();
	
	/** \brief Retrieves a string with whitespaces removed from the beginning. */
	decUnicodeString GetTrimmedLeft() const;
	
	/** \brief Removes whitespaces from the end. */
	void TrimRight();
	
	/** \brief Retrieves a string with whitespaces removed from the end. */
	decUnicodeString GetTrimmedRight() const;
	
	/** \brief Remove whitespaces from the beginning and end. */
	void Trim();
	
	/** \brief Retrieves a string with whitespaces removed from the beginning and end. */
	decUnicodeString GetTrimmed() const;
	
	/** \brief Converts all characters to lower case. */
	void ToLower();
	
	/** \brief Retrieves string with all characters converted to lower case. */
	decUnicodeString GetLower() const;
	
	/** \brief Converts all character to upper case. */
	void ToUpper();
	
	/** \brief Retrieves string with all characters converted to upper case. */
	decUnicodeString GetUpper() const;
	
	/** \brief Retrieves integer value. */
	int ToInt() const;
	
	/** \brief Long int value. */
	long long ToLong() const;
	
	/** \brief Retrieves float value. */
	float ToFloat() const;
	
	/** \brief Retrieves double value. */
	double ToDouble() const;
	
	/** \brief UTF8 encoded string. */
	decString ToUTF8() const;
	
	/** \brief String equals another string case sensitive. */
	bool Equals( const decUnicodeString &string ) const;
	
	/** \brief String equals another string case insensitive. */
	bool EqualsInsensitive( const decUnicodeString &string ) const;
	
	/** \brief Compares the string to another string case sensitive returns -1(less), 0(equal) or 1(greater). */
	int Compare( const decUnicodeString &string ) const;
	
	/** \brief Compares the string to another string case insensitive returns -1(less), 0(equal) or 1(greater). */
	int CompareInsensitive( const decUnicodeString &string ) const;
	
	/** \brief Calculate string hash. */
	unsigned int Hash() const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Retrieves a character. */
	int operator[]( int position ) const;
	
	/** \brief String is empty. */
	bool operator!() const;
	
	/** \brief String equals another string case sensitive. */
	bool operator==( const decUnicodeString &string ) const;
	
	/** \brief String equals another string case sensitive. */
	bool operator!=( const decUnicodeString &string ) const;
	
	/** \brief String is lexographically less than another string case sensitive. */
	bool operator<( const decUnicodeString &string ) const;
	
	/** \brief String is lexographically less than or equal to another string case sensitive. */
	bool operator<=( const decUnicodeString &string ) const;
	
	/** \brief String is lexographically greater than another string case sensitive. */
	bool operator>( const decUnicodeString &string ) const;
	
	/** \brief String is lexographically greater than or equal to another string case sensitive. */
	bool operator>=( const decUnicodeString &string ) const;
	
	/** \brief Returns a new string being the concatenation of this string and another string. */
	decUnicodeString operator+( const decUnicodeString &string ) const;
	
	/** \brief Set string to another string. */
	decUnicodeString &operator=( const decUnicodeString &string );
	
	/** \brief Appends a string to this string. */
	decUnicodeString &operator+=( const decUnicodeString &string );
	/*@}*/
	
	
	
private:
	int pGetAt( int position ) const;
	void pSetAt( int position, int character );
	void pFreeString();
	void pCreateString( int size, int length );
	void pGrowString( int size, int length );
	bool pIsSpace( int character ) const;
	int pCharSize( int character ) const;
};

#endif
