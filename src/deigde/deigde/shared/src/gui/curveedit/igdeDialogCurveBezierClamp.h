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

#ifndef _IGDEDIALOGCURVEBEZIERCLAMP_H_
#define _IGDEDIALOGCURVEBEZIERCLAMP_H_

#include "../dialog/igdeDialog.h"
#include "../igdeCheckBox.h"
#include "../composed/igdeEditVector2.h"



/**
 * \brief Dialog bezier curve clamp parameters.
 */
class DE_DLL_EXPORT igdeDialogCurveBezierClamp : public igdeDialog{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeDialogCurveBezierClamp> Ref;
	
	
private:
	igdeCheckBox::Ref pChkClamp;
	igdeEditVector2::Ref pEditMinimum;
	igdeEditVector2::Ref pEditMaximum;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogCurveBezierClamp( igdeEnvironment &environment );
	
	/** \brief Create dialog. */
	igdeDialogCurveBezierClamp( igdeEnvironment &environment, bool clamp,
		const decVector2 &minimum, const decVector2 &maximum );
	
	
	
protected:
	/** \brief Clean up dialog. */
	virtual ~igdeDialogCurveBezierClamp();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Clamp. */
	bool GetClamp() const;
	
	/** \brief Set clamp. */
	void SetClamp( bool clamp );
	
	/** \brief Minimum clamp. */
	decVector2 GetClampMin() const;
	
	/** \brief Set minimum clamp. */
	void SetClampMin( const decVector2 &minimum );
	
	/** \brief Maximum clamp. */
	decVector2 GetClampMax() const;
	
	/** \brief Set maximum clamp. */
	void SetClampMax( const decVector2 &maximum );
	/*@}*/
	
	
	
private:
	void pCreateContent( igdeEnvironment &environment );
};

#endif
