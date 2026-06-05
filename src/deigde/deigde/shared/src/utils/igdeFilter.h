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

#ifndef _IGDEFILTER_H_
#define _IGDEFILTER_H_

#include <dragengine/common/string/decString.h>


/**
 * \brief Filter using fuzzy matching.
 */
class DE_DLL_EXPORT igdeFilter{
public:
	/** \brief Matchable string. */
	class DE_DLL_EXPORT Matchable{
	private:
		friend class igdeFilter;
		decString pString, pStringLower;
		int pStringLowerLen = 0;
		
	public:
		/** \name Constructors and Destructors */
		/*@{*/
		/** \brief Create empty matchable string. */
		Matchable();
		
		/** \brief Create matchable string. */
		explicit Matchable(const char *string);
		
		/** \brief Create copy of matchable string. */
		Matchable(const Matchable &other);
		
		/** \brief Move matchable string. */
		Matchable(Matchable &&other) noexcept;
		
		/** \brief Clean up matchable string. */
		~Matchable();
		/*@}*/
		
		
		/** \name Management */
		/*@{*/
		/** \brief String. */
		inline const decString &GetString() const{ return pString; }
		/*@}*/
		
		
		/** \name Operators. */
		/*@{*/
		/** \brief Assignment operator. */
		Matchable &operator=(const Matchable &other);
		
		/** \brief Move assignment operator. */
		Matchable &operator=(Matchable &&other) noexcept;
		
		/** \brief Same string. */
		bool operator==(const Matchable &other) const;
		
		/** \brief Not empty string. */
		operator bool() const;
		
		/** \brief Empty string. */
		bool operator!() const;
		/*@}*/
	};
	
	
private:
	decString pFilter, pFilterLower;
	int pFilterLowerLen = 0;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty filter. */
	igdeFilter();
	
	/** \brief Create filter. */
	explicit igdeFilter(const char *filter);
	
	/** \brief Create copy of filter. */
	igdeFilter(const igdeFilter &other);
	
	/** \brief Move filter. */
	igdeFilter(igdeFilter &&other) noexcept;
	
	/** \brief Clean up filter. */
	~igdeFilter();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Filter. */
	inline const decString &GetFilter() const{ return pFilter; }
	
	/** \brief String matches filter. */
	bool Matches(const Matchable &string) const;
	
	/** \brief String does not match filter. */
	bool MatchesNot(const Matchable &string) const;
	/*@}*/
	
	
	/** \name Operators. */
	/*@{*/
	/** \brief Assignment operator. */
	igdeFilter &operator=(const igdeFilter &other);
	
	/** \brief Move assignment operator. */
	igdeFilter &operator=(igdeFilter &&other) noexcept;
	
	/** \brief Same filter. */
	bool operator==(const igdeFilter &other) const;
	
	/** \brief Not empty filter. */
	operator bool() const;
	
	/** \brief Empty filter. */
	bool operator!() const;
	/*@}*/
};

#endif
