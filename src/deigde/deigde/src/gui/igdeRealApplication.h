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

#ifndef _IGDEIGDEREALAPPLICATION_H_
#define _IGDEIGDEREALAPPLICATION_H_

#include "../environment/igdeEnvironmentIGDE.h"

#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeWidgetReference.h>


/**
 * \brief Create DEIGDE application.
 */
class igdeRealApplication : public igdeApplication{
private:
	igdeEnvironmentIGDE pEnvironment;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create DEIGDE application. */
	igdeRealApplication();
	
	/** \brief Clean up DEIGDE application. */
	virtual ~igdeRealApplication();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironmentIGDE &GetEnvironment(){ return pEnvironment; }
	inline const igdeEnvironmentIGDE &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Main window. */
	igdeWindowMain &GetWindowMain() const;
	
	
	
protected:
	/**
	 * \brief Initialize application.
	 * \returns true to run application or false to abord.
	 */
	virtual bool Initialize( const decUnicodeStringList &arguments );
	
	/** \brief Clean up application. */
	virtual void CleanUp();
};

#endif
