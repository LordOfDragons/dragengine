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

class decStringList;



/**
 * @brief Mutable String.
 * Stores a 0 terminated, mutable, ASCII character string. This class is designed
 * for storing short strings without large overhead. For this only the string pointer
 * is stored. All operations required to know the length of the string have to calculate
 * it whenever needed. Therefore if you need to do text operations on a larger string use
 * the StringBuffer class instead which stores a string length and can handle 0 characters
 * inside the string.
 */
class decString{
private:
	char *pString;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new empty string. */
	decString();
	/** Creates a new string. */
	decString(const char *string);
	/** Creates a new string being the copy of another string. */
	decString(const decString &string);
	/** Creates a new string being the concatenation of two other strings. */
	decString(const decString &string1, const decString &string2);
	/** Creates a new string being the concatenation of two other strings. */
	decString(const decString &string1, const char *string2);
	/** Cleans up the string. */
	~decString();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Determines if the string is empty. */
	bool IsEmpty() const;
	/** Sets the string to the empty string. */
	void Empty();
	/** Retrieves the number of characters. */
	int GetLength() const;
	/** Retrieves the character at the given position. */
	int GetAt(int position) const;
	/** Sets the character at the given position. */
	void SetAt(int position, int character);
	
	/** Sets the string from another string. */
	void Set(const decString &string);
	/** Sets the string from another string. */
	void Set(const char *string);
	/** Sets the string to a run of characters. */
	void Set(int character, int count);
	/** Set string to a char value. */
	void SetValue(char value);
	/** Set string to an unsigned char value. */
	void SetValue(unsigned char value);
	/** Set string to a short value. */
	void SetValue(short value);
	/** Set string to an unsigned short value. */
	void SetValue(unsigned short value);
	/** Set string to a signed integer value. */
	void SetValue(int value);
	/** Set string to an unsigned integer value. */
	void SetValue(unsigned int value);
	/** Set string to a float value. */
	void SetValue(float value);
	/** Set string to a double value. */
	void SetValue(double value);
	/** Formats a string using printf style formatting. */
	void Format(const char *format, ...);
	/** Formats a string using printf style formatting. */
	void FormatUsing(const char *format, va_list args);
	
	/** Appends a string. */
	void Append(const decString &string);
	/** Appends a string. */
	void Append(const char *string);
	/** Appends a character. */
	void AppendCharacter(char character);
	void AppendCharacter(unsigned char character);
	void AppendCharacter(int character);
	/** Appends a char value. */
	void AppendValue(char value);
	/** Appends an unsigned char value. */
	void AppendValue(unsigned char value);
	/** Appends a short value. */
	void AppendValue(short value);
	/** Appends an unsigned short value. */
	void AppendValue(unsigned short value);
	/** Appends a signed integer value. */
	void AppendValue(int value);
	/** Appends an unsigned integer value. */
	void AppendValue(unsigned int value);
	/** Appends a float value. */
	void AppendValue(float value);
	/** Appends a double value. */
	void AppendValue(double value);
	/** Appends a formatted string using printf style formatting. */
	void AppendFormat(const char *format, ...);
	/** Appends a formatted string using printf style formatting. */
	void AppendFormatUsing(const char *format, va_list args);
	
	/**
	 * Retrieve index of first occurance of a character or -1 if not found. Negative
	 * start or end is measured from the string end.
	 */
	int Find(int character) const;
	int Find(int character, int start) const;
	int Find(int character, int start, int end) const;
	/**
	 * Retrieve index of first occurance of any charatcer in a list of characters or
	 * -1 if not found. Negative start or end is measured from the string end.
	 */
	int Find(const char *characters) const;
	int Find(const char *characters, int start) const;
	int Find(const char *characters, int start, int end) const;
	int Find(const decString &characters) const;
	int Find(const decString &characters, int start) const;
	int Find(const decString &characters, int start, int end) const;
	/**
	 * Retrieve index of last occurance of a character or -1 if not found. Negative
	 * start or end is measured from the string end.
	 */
	int FindReverse(int character) const;
	int FindReverse(int character, int start) const;
	int FindReverse(int character, int start, int end) const;
	/**
	 * Retrieve index of last occurance of any character in a list of characters or
	 * -1 if not found. Negative start or end is measured from the string end.
	 */
	int FindReverse(const char *characters) const;
	int FindReverse(const char *characters, int start) const;
	int FindReverse(const char *characters, int start, int end) const;
	int FindReverse(const decString &characters) const;
	int FindReverse(const decString &characters, int start) const;
	int FindReverse(const decString &characters, int start, int end) const;
	/**
	 * Retrieve index of first occurance of a string or -1 if not found. Negative
	 * start or end is measured from the string end.
	 */
	int FindString(const char *string) const;
	int FindString(const char *string, int start) const;
	int FindString(const char *string, int start, int end) const;
	int FindString(const decString &string) const;
	int FindString(const decString &string, int start) const;
	int FindString(const decString &string, int start, int end) const;
	/**
	 * Retrieve index of first occurance of a character from the end to the start or -1 if
	 * not found. Negative start or end is measured from the string end.
	 */
	int FindStringReverse(const char *string) const;
	int FindStringReverse(const char *string, int start) const;
	int FindStringReverse(const char *string, int start, int end) const;
	int FindStringReverse(const decString &string) const;
	int FindStringReverse(const decString &string, int start) const;
	int FindStringReverse(const decString &string, int start, int end) const;
	
	/** Retrieves the first number of characters. */
	decString GetLeft(int count) const;
	/** Retrieves the last number of characters. */
	decString GetRight(int count) const;
	/** Retrieves a sub string. Negative start or end is measured from the string end. */
	decString GetMiddle(int start) const;
	decString GetMiddle(int start, int end) const;
	
	/** Reverse string. */
	void Reverse();
	/** Retrieve reversed string. */
	decString GetReversed() const;
	
	/** Split string. */
	decStringList Split(int character) const;
	/** Split string on multiple characters. */
	decStringList Split(const char *characters) const;
	decStringList Split(const decString &characters) const;
	
	/** Replace all occurances of a character. */
	void Replace(int replaceCharacter, int withCharacter);
	/** Replace all occurances of any character in a list of characters. */
	void Replace(const char *replaceCharacters, int withCharacter);
	void Replace(const decString &replaceCharacters, int withCharacter);
	/** Replace all occurances of a string. */
	void ReplaceString(const char *replaceString, const char *withString);
	void ReplaceString(const decString &replaceString, const decString &withString);
	/** Retrieves string with all occurances of a character replaced. */
	decString GetReplaced(int replaceCharacter, int withCharacter) const;
	/** Retrieves string with all occurances of any character of a list of characters replaced. */
	decString GetReplaced(const char *replaceCharacters, int withCharacter) const;
	decString GetReplaced(const decString &replaceCharacters, int withCharacter) const;
	/** Retrieves string with all occurances of a string replaced. */
	decString GetReplacedString(const char *replaceString, const char *withString) const;
	decString GetReplacedString(const decString &replaceString, const decString &withString) const;
	
	/** Removes whitespaces from the beginning. */
	void TrimLeft();
	/** Retrieves a string with whitespaces removed from the beginning. */
	decString GetTrimmedLeft() const;
	/** Removes whitespaces from the end. */
	void TrimRight();
	/** Retrieves a string with whitespaces removed from the end. */
	decString GetTrimmedRight() const;
	/** Remove whitespaces from the beginning and end. */
	void Trim();
	/** Retrieves a string with whitespaces removed from the beginning and end. */
	decString GetTrimmed() const;
	
	/** Converts all characters to lower case. */
	void ToLower();
	/** Retrieves string with all characters converted to lower case. */
	decString GetLower() const;
	/** Converts all character to upper case. */
	void ToUpper();
	/** Retrieves string with all characters converted to upper case. */
	decString GetUpper() const;
	
	/** Retrieves integer value. */
	int ToInt() const;
	/** Retrieves the long int value. */
	long long ToLong() const;
	/** Retrieves float value. */
	float ToFloat() const;
	/** Retrieves double value. */
	double ToDouble() const;
	
	/** Retrieves the pointer to the text. */
	const char *GetString() const;
	
	/** Determines if the string equals another string case sensitive. */
	bool Equals(const decString &string) const;
	/** Determines if the string equals another string case sensitive. */
	bool Equals(const char *string) const;
	/** Determines if the string equals another string case insensitive. */
	bool EqualsInsensitive(const decString &string) const;
	/** Determines if the string equals another string case insensitive. */
	bool EqualsInsensitive(const char *string) const;
	/** Compares the string to another string case sensitive returns -1(less), 0(equal) or 1(greater). */
	int Compare(const decString &string) const;
	/** Compares the string to another string case sensitive returns -1(less), 0(equal) or 1(greater). */
	int Compare(const char *string) const;
	/** Compares the string to another string case insensitive returns -1(less), 0(equal) or 1(greater). */
	int CompareInsensitive(const decString &string) const;
	/** Compares the string to another string case insensitive returns -1(less), 0(equal) or 1(greater). */
	int CompareInsensitive(const char *string) const;
	
	/** Determines if this string matches a given pattern. */
	bool MatchesPattern(const decString &pattern) const;
	/** Determines if this string matches a given pattern. */
	bool MatchesPattern(const char *pattern) const;
	/** Determines if a string matches a given pattern. */
	static bool StringMatchesPattern(const char *string, const char *pattern);
	
	/** \brief Calculate string hash. */
	unsigned int Hash() const;
	/** \brief Calculate string hash. */
	static unsigned int Hash(const char *string);
	
	/** Retrieves a character. */
	char operator[](int position) const;
	/** Sets a character. */
	char &operator[](int position);
	/** Determines if the string is empty. */
	bool operator!() const;
	/** Determines if the string equals another string case sensitive. */
	bool operator==(const decString &string) const;
	/** Determines if the string equals another string case sensitive. */
	bool operator==(const char *string) const;
	/** Determines if the string equals another string case sensitive. */
	bool operator!=(const decString &string) const;
	/** Determines if the string equals another string case sensitive. */
	bool operator!=(const char *string) const;
	/** Determines if the string is lexographically less than another string case sensitive. */
	bool operator<(const decString &string) const;
	/** Determines if the string is lexographically less than another string case sensitive. */
	bool operator<(const char *string) const;
	/** Determines if the string is lexographically less than or equal to another string case sensitive. */
	bool operator<=(const decString &string) const;
	/** Determines if the string is lexographically less than or equal to another string case sensitive. */
	bool operator<=(const char *string) const;
	/** Determines if the string is lexographically greater than another string case sensitive. */
	bool operator>(const decString &string) const;
	/** Determines if the string is lexographically greater than another string case sensitive. */
	bool operator>(const char *string) const;
	/** Determines if the string is lexographically greater than or equal to another string case sensitive. */
	bool operator>=(const decString &string) const;
	/** Determines if the string is lexographically greater than or equal to another string case sensitive. */
	bool operator>=(const char *string) const;
	
	/** Returns a new string being the concatenation of this string and another string. */
	decString operator+(const decString &string) const;
	/** Returns a new string being the concatenation of this string and another string. */
	decString operator+(const char *string) const;
	
	/** Sets the string to another string. */
	decString &operator=(const decString &string);
	/** Sets the string to another string. */
	decString &operator=(const char *string);
	/** Appends a string to this string. */
	decString &operator+=(const decString &string);
	/** Appends a string to this string. */
	decString &operator+=(const char *string);
	
	/** Retrieves the character pointer. */
	operator const char*() const;
	/*@}*/
	
private:
	int pCompare(const char *string) const;
	int pCompareInsensitive(const char *string) const;
	
	/**
	 * File matching function borrowed from the fox toolkit ( www.fox-toolkit.org ).
	 * Original file is fxfilematch.cpp .
	 */
	static bool domatch(const char *pattern,const char *string, int flags);
	static bool fxfilematch(const char *pattern,const char *string, int flags);
};

#endif
