/* 
 * Drag[en]gine Game Engine Distribution Launcher
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#pragma once

class String{
private:
	char *pString;
	
public:
	String();
	String(const String &string);
	String(const char *string);
	
	~String();
	
	int Length() const;
	int Find(char delimiter) const;
	String SubString(int from) const;
	String SubString(int from, int to) const;
	const char *Pointer() const;
	
	operator const char*() const;
	char operator[](int index) const;
	String operator+(const String &string) const;
	String operator+(char character) const;
	String operator=(const String &string);
	String operator+=(const String &string);
	String operator+=(char character);
	bool operator==(const String &string) const;
	bool operator==(const char *string) const;
	bool operator!=(const String &string) const;
	bool operator!=(const char *string) const;
};

