/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeFilter.h"


// Class igdeFilter::MatchableString
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeFilter::Matchable::Matchable() :
pStringLowerLen(0){
}

igdeFilter::Matchable::Matchable(const char *string) :
pString(string),
pStringLower(pString.GetLower()),
pStringLowerLen(pStringLower.GetLength()){
}

igdeFilter::Matchable::Matchable(const Matchable &other) :
pString(other.pString),
pStringLower(other.pStringLower),
pStringLowerLen(other.pStringLowerLen){
}

igdeFilter::Matchable::Matchable(Matchable &&other) noexcept :
pString(std::move(other.pString)),
pStringLower(std::move(other.pStringLower)),
pStringLowerLen(other.pStringLowerLen){
}

igdeFilter::Matchable::~Matchable() = default;


// Operators
//////////////

igdeFilter::Matchable &igdeFilter::Matchable::operator=(const Matchable &other){
	if(this != &other){
		pString = other.pString;
		pStringLower = other.pStringLower;
		pStringLowerLen = other.pStringLowerLen;
	}
	return *this;
}

igdeFilter::Matchable &igdeFilter::Matchable::operator=(Matchable &&other) noexcept{
	if(this != &other){
		pString = std::move(other.pString);
		pStringLower = std::move(other.pStringLower);
		pStringLowerLen = other.pStringLowerLen;
	}
	return *this;
}

bool igdeFilter::Matchable::operator==(const Matchable &other) const{
	return pString == other.pString;
}

igdeFilter::Matchable::operator bool() const{
	return pStringLowerLen > 0;
}

bool igdeFilter::Matchable::operator!() const{
	return pStringLowerLen == 0;
}


// Class igdeFilter
/////////////////////
	
// Constructor, destructor
////////////////////////////

igdeFilter::igdeFilter() :
pFilterLowerLen(0){
}

igdeFilter::igdeFilter(const char *filter) :
pFilter(filter),
pFilterLower(pFilter.GetLower()),
pFilterLowerLen(pFilterLower.GetLength()){
}

igdeFilter::igdeFilter(const igdeFilter &other) :
pFilter(other.pFilter),
pFilterLower(other.pFilterLower),
pFilterLowerLen(other.pFilterLowerLen){
}

igdeFilter::igdeFilter(igdeFilter &&other) noexcept :
pFilter(std::move(other.pFilter)),
pFilterLower(std::move(other.pFilterLower)),
pFilterLowerLen(other.pFilterLowerLen){
}

igdeFilter::~igdeFilter() = default;


// Management
///////////////

bool igdeFilter::Matches(const Matchable &string) const{
	int fi = 0;
	
	for(int ti=0; ti<string.pStringLowerLen && fi<pFilterLowerLen; ti++){
		if(string.pStringLower[ti] == pFilterLower[fi]){
			fi++;
		}
	}
	
	return fi == pFilterLowerLen;
}

bool igdeFilter::MatchesNot(const Matchable &string) const{
	return !Matches(string);
}


// Operators
//////////////

igdeFilter &igdeFilter::operator=(const igdeFilter &other){
	if(this != &other){
		pFilter = other.pFilter;
		pFilterLower = other.pFilterLower;
		pFilterLowerLen = other.pFilterLowerLen;
	}
	return *this;
}

igdeFilter &igdeFilter::operator=(igdeFilter &&other) noexcept{
	if(this != &other){
		pFilter = std::move(other.pFilter);
		pFilterLower = std::move(other.pFilterLower);
		pFilterLowerLen = other.pFilterLowerLen;
	}
	return *this;
}

bool igdeFilter::operator==(const igdeFilter &other) const{
	return pFilter == other.pFilter;
}

igdeFilter::operator bool() const{
	return pFilterLowerLen > 0;
}

bool igdeFilter::operator!() const{
	return pFilterLowerLen == 0;
}
