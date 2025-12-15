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

#ifndef _DECSTRING_H_
#define _DECSTRING_H_

#include <stdarg.h>
#include "../../dragengine_export.h"

class decStringList;


/**
 * \brief Mutable String.
 * 
 * Stores a 0 terminated, mutable, ASCII character string. This class is designed
 * for storing short strings without large overhead. For this only the string pointer
 * is stored. All operations required to know the length of the string have to calculate
 * it whenever needed. Therefore if you need to do text operations on a larger string use
 * the StringBuffer class instead which stores a string length and can handle 0 characters
 * inside the string.
 */
class DE_DLL_EXPORT decString{
private:
	char *pString;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new empty string. */
	decString();
	
	/** \brief Create new string. */
	decString(const char *string);
	
	/** \brief Create new string being the copy of another string. */
	decString(const decString &string);
	
	/** \brief Create new string being the concatenation of two other strings. */
	decString(const decString &string1, const decString &string2);
	
	/** \brief Create new string being the concatenation of two other strings. */
	decString(const decString &string1, const char *string2);
	
	/** \brief Clean up string. */
	~decString();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief String is empty. */
	bool IsEmpty() const;
	
	/** \brief Set string to the empty string. */
	void Empty();
	
	/** \brief Number of characters. */
	int GetLength() const;
	
	/** \brief Character at the given position. */
	int GetAt(int position) const;
	
	/** \brief Set character at the given position. */
	void SetAt(int position, int character);
	
	/** \brief Set string from another string. */
	void Set(const decString &string);
	
	/** \brief Set string from another string. */
	void Set(const char *string);
	
	/** \brief Set string to a run of characters. */
	void Set(int character, int count);
	
	/** \brief Set string to a char value. */
	void SetValue(char value);
	
	/** \brief Set string to an unsigned char value. */
	void SetValue(unsigned char value);
	
	/** \brief Set string to a short value. */
	void SetValue(short value);
	
	/** \brief Set string to an unsigned short value. */
	void SetValue(unsigned short value);
	
	/** \brief Set string to a signed integer value. */
	void SetValue(int value);
	
	/** \brief Set string to an unsigned integer value. */
	void SetValue(unsigned int value);
	
	/** \brief Set string to a float value. */
	void SetValue(float value);
	
	/** \brief Set string to a double value. */
	void SetValue(double value);
	
	/** \brief Format string using printf style formatting. */
	void Format(const char *format, ...)
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
		
	/** \brief Formats a string using printf style formatting. */
	void FormatUsing(const char *format, va_list args);
	
	/** \brief Appends a string. */
	void Append(const decString &string);
	
	/** \brief Appends a string. */
	void Append(const char *string);
	
	/** \brief Appends a character. */
	void AppendCharacter(char character);
	void AppendCharacter(unsigned char character);
	void AppendCharacter(int character);
	
	/** \brief Appends a char value. */
	void AppendValue(char value);
	
	/** \brief Appends an unsigned char value. */
	void AppendValue(unsigned char value);
	
	/** \brief Appends a short value. */
	void AppendValue(short value);
	
	/** \brief Appends an unsigned short value. */
	void AppendValue(unsigned short value);
	
	/** \brief Appends a signed integer value. */
	void AppendValue(int value);
	
	/** \brief Appends an unsigned integer value. */
	void AppendValue(unsigned int value);
	
	/** \brief Appends a signed long long value. */
	void AppendValue(long long value);
	
	/** \brief Appends an unsigned long long value. */
	void AppendValue(unsigned long long value);
	
	/** \brief Appends a float value. */
	void AppendValue(float value);
	
	/** \brief Appends a double value. */
	void AppendValue(double value);
	
	/** \brief Append formatted string using printf style formatting. */
	void AppendFormat(const char *format, ...)
		#ifdef __GNUC__
		__attribute__ ((format (printf, 2, 3)))
		#endif
		;
		
	/** \brief Appends a formatted string using printf style formatting. */
	void AppendFormatUsing(const char *format, va_list args);
	
	/**
	 * \brief Index of first occurance of a character or -1 if not found.
	 * 
	 * Negative start or end is measured from the string end.
	 */
	int Find(int character) const;
	int Find(int character, int start) const;
	int Find(int character, int start, int end) const;
	
	/**
	 * \brief Index of first occurance of any charatcer in a list of characters or -1 if not found.
	 * 
	 * Negative start or end is measured from the string end.
	 */
	int Find(const char *characters) const;
	int Find(const char *characters, int start) const;
	int Find(const char *characters, int start, int end) const;
	int Find(const decString &characters) const;
	int Find(const decString &characters, int start) const;
	int Find(const decString &characters, int start, int end) const;
	
	/**
	 * \brief Index of last occurance of a character or -1 if not found.
	 * 
	 * Negative start or end is measured from the string end.
	 */
	int FindReverse(int character) const;
	int FindReverse(int character, int start) const;
	int FindReverse(int character, int start, int end) const;
	
	/**
	 * \brief Index of last occurance of any character in a list of characters or -1 if not found.
	 * 
	 * Negative start or end is measured from the string end.
	 */
	int FindReverse(const char *characters) const;
	int FindReverse(const char *characters, int start) const;
	int FindReverse(const char *characters, int start, int end) const;
	int FindReverse(const decString &characters) const;
	int FindReverse(const decString &characters, int start) const;
	int FindReverse(const decString &characters, int start, int end) const;
	
	/**
	 * \brief Index of first occurance of a string or -1 if not found.
	 * 
	 * Negative start or end is measured from the string end.
	 */
	int FindString(const char *string) const;
	int FindString(const char *string, int start) const;
	int FindString(const char *string, int start, int end) const;
	int FindString(const decString &string) const;
	int FindString(const decString &string, int start) const;
	int FindString(const decString &string, int start, int end) const;
	
	/**
	 * \brief Index of first occurance of a character from the end to the start or -1 if not found.
	 * 
	 * Negative start or end is measured from the string end.
	 */
	int FindStringReverse(const char *string) const;
	int FindStringReverse(const char *string, int start) const;
	int FindStringReverse(const char *string, int start, int end) const;
	int FindStringReverse(const decString &string) const;
	int FindStringReverse(const decString &string, int start) const;
	int FindStringReverse(const decString &string, int start, int end) const;
	
	/** \brief First number of characters. */
	decString GetLeft(int count) const;
	
	/** \brief Last number of characters. */
	decString GetRight(int count) const;
	
	/** \brief Retrieves a sub string. Negative start or end is measured from the string end. */
	decString GetMiddle(int start) const;
	decString GetMiddle(int start, int end) const;
	
	/** \brief Reverse string. */
	void Reverse();
	
	/** \brief Retrieve reversed string. */
	decString GetReversed() const;
	
	/** \brief Split string. */
	decStringList Split(int character) const;
	
	/** \brief Split string on multiple characters. */
	decStringList Split(const char *characters) const;
	decStringList Split(const decString &characters) const;
	
	/** \brief Replace all occurances of a character. */
	void Replace(int replaceCharacter, int withCharacter);
	
	/** \brief Replace all occurances of any character in a list of characters. */
	void Replace(const char *replaceCharacters, int withCharacter);
	void Replace(const decString &replaceCharacters, int withCharacter);
	
	/** \brief Replace all occurances of a string. */
	void ReplaceString(const char *replaceString, const char *withString);
	void ReplaceString(const decString &replaceString, const decString &withString);
	
	/** \brief Retrieves string with all occurances of a character replaced. */
	decString GetReplaced(int replaceCharacter, int withCharacter) const;
	
	/** \brief Retrieves string with all occurances of any character of a list of characters replaced. */
	decString GetReplaced(const char *replaceCharacters, int withCharacter) const;
	decString GetReplaced(const decString &replaceCharacters, int withCharacter) const;
	
	/** \brief Retrieves string with all occurances of a string replaced. */
	decString GetReplacedString(const char *replaceString, const char *withString) const;
	decString GetReplacedString(const decString &replaceString, const decString &withString) const;
	
	/** \brief Removes whitespaces from the beginning. */
	void TrimLeft();
	
	/** \brief Retrieves a string with whitespaces removed from the beginning. */
	decString GetTrimmedLeft() const;
	
	/** \brief Removes whitespaces from the end. */
	void TrimRight();
	
	/** \brief Retrieves a string with whitespaces removed from the end. */
	decString GetTrimmedRight() const;
	
	/** \brief Remove whitespaces from the beginning and end. */
	void Trim();
	
	/** \brief Retrieves a string with whitespaces removed from the beginning and end. */
	decString GetTrimmed() const;
	
	/** \brief Converts all characters to lower case. */
	void ToLower();
	
	/** \brief Retrieves string with all characters converted to lower case. */
	decString GetLower() const;
	
	/** \brief Converts all character to upper case. */
	void ToUpper();
	
	/** \brief Retrieves string with all characters converted to upper case. */
	decString GetUpper() const;
	
	/** \brief Retrieves integer value. */
	int ToInt() const;
	
	/** \brief Long int value. */
	long long ToLong() const;
	
	/** \brief Retrieves float value. */
	float ToFloat() const;
	
	/** \brief Retrieves double value. */
	double ToDouble() const;
	
	/** \brief Integer value throwing exception if not valid. */
	int ToIntValid() const;
	
	/** \brief Long int value throwing exception if not valid. */
	long long ToLongValid() const;
	
	/** \brief Float value throwing exception if not valid. */
	float ToFloatValid() const;
	
	/** \brief Double value throwing exception if not valid. */
	double ToDoubleValid() const;
	
	/** \brief Const pointer to string. */
	const char *GetString() const;
	
	/** \brief Mutable pointer to string. */
	char *GetMutableString() const;
	
	/** \brief String equals another string case sensitive. */
	bool Equals(const decString &string) const;
	
	/** \brief String equals another string case sensitive. */
	bool Equals(const char *string) const;
	
	/** \brief String equals another string case insensitive. */
	bool EqualsInsensitive(const decString &string) const;
	
	/** \brief String equals another string case insensitive. */
	bool EqualsInsensitive(const char *string) const;
	
	/** \brief Compares the string to another string case sensitive returns -1(less), 0(equal) or 1(greater). */
	int Compare(const decString &string) const;
	
	/** \brief Compares the string to another string case sensitive returns -1(less), 0(equal) or 1(greater). */
	int Compare(const char *string) const;
	
	/** \brief Compares the string to another string case insensitive returns -1(less), 0(equal) or 1(greater). */
	int CompareInsensitive(const decString &string) const;
	
	/** \brief Compares the string to another string case insensitive returns -1(less), 0(equal) or 1(greater). */
	int CompareInsensitive(const char *string) const;
	
	/** \brief Begins with string. */
	bool BeginsWith(const decString &string) const;
	
	/** \brief Begins with string. */
	bool BeginsWith(const char *string) const;
	
	/** \brief Begins with string. */
	bool BeginsWithInsensitive(const decString &string) const;
	
	/** \brief Begins with string. */
	bool BeginsWithInsensitive(const char *string) const;
	
	/** \brief Ends with string. */
	bool EndsWith(const decString &string) const;
	
	/** \brief Ends with string. */
	bool EndsWith(const char *string) const;
	
	/** \brief Ends with string. */
	bool EndsWithInsensitive(const decString &string) const;
	
	/** \brief Ends with string. */
	bool EndsWithInsensitive(const char *string) const;
	
	/** \brief Determines if this string matches a given pattern. */
	bool MatchesPattern(const decString &pattern) const;
	
	/** \brief Determines if this string matches a given pattern. */
	bool MatchesPattern(const char *pattern) const;
	
	/** \brief Determines if a string matches a given pattern. */
	static bool StringMatchesPattern(const char *string, const char *pattern);
	
	/** \brief Calculate string hash. */
	unsigned int Hash() const;
	
	/** \brief Calculate string hash. */
	static unsigned int Hash(const char *string);
	
	/** \brief Retrieves a character. */
	char operator[](int position) const;
	
	/** \brief Sets a character. */
	char &operator[](int position);
	
	/** \brief String is empty. */
	bool operator!() const;
	
	/** \brief String equals another string case sensitive. */
	bool operator==(const decString &string) const;
	
	/** \brief String equals another string case sensitive. */
	bool operator==(const char *string) const;
	
	/** \brief String equals another string case sensitive. */
	bool operator!=(const decString &string) const;
	
	/** \brief String equals another string case sensitive. */
	bool operator!=(const char *string) const;
	
	/** \brief String is lexographically less than another string case sensitive. */
	bool operator<(const decString &string) const;
	
	/** \brief String is lexographically less than another string case sensitive. */
	bool operator<(const char *string) const;
	
	/** \brief String is lexographically less than or equal to another string case sensitive. */
	bool operator<=(const decString &string) const;
	
	/** \brief String is lexographically less than or equal to another string case sensitive. */
	bool operator<=(const char *string) const;
	
	/** \brief String is lexographically greater than another string case sensitive. */
	bool operator>(const decString &string) const;
	
	/** \brief String is lexographically greater than another string case sensitive. */
	bool operator>(const char *string) const;
	
	/** \brief String is lexographically greater than or equal to another string case sensitive. */
	bool operator>=(const decString &string) const;
	
	/** \brief String is lexographically greater than or equal to another string case sensitive. */
	bool operator>=(const char *string) const;
	
	/** \brief Returns a new string being the concatenation of this string and another string. */
	decString operator+(const decString &string) const;
	
	/** \brief Returns a new string being the concatenation of this string and another string. */
	decString operator+(const char *string) const;
	
	/** \brief Set string to another string. */
	decString &operator=(const decString &string);
	
	/** \brief Set string to another string. */
	decString &operator=(const char *string);
	
	/** \brief Appends a string to this string. */
	decString &operator+=(const decString &string);
	
	/** \brief Appends a string to this string. */
	decString &operator+=(const char *string);
	
	/** \brief Character pointer. */
	operator const char*() const;
	/*@}*/
	
	
	
private:
	int pCompare(const char *string) const;
	int pCompareInsensitive(const char *string) const;
	bool pBeginsWith(const char *string) const;
	bool pBeginsWithInsensitive(const char *string) const;
	bool pEndsWith(const char *string) const;
	bool pEndsWithInsensitive(const char *string) const;
	
	/**
	 * File matching function borrowed from the fox toolkit ( www.fox-toolkit.org ).
	 * Original file is fxfilematch.cpp .
	 */
	static bool domatch(const char *pattern,const char *string, int flags);
	static bool fxfilematch(const char *pattern,const char *string, int flags);
};


/** \brief Global hash functions used for example with decTDictionary. */

inline unsigned int DEHash(const decString &key){
	return key.Hash();
}

inline unsigned int DEHash(const char *key){
	return decString::Hash(key);
}

#endif
