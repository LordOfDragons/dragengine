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

#ifndef _RECONFIGURATION_H_
#define _RECONFIGURATION_H_

class reWindowMain;



/**
 * \brief Editor configuration.
 */
class reConfiguration{
private:
	reWindowMain &pWindowMain;
	
	float pGridSize;
	bool pSnapToGrid;
	float pRotSnap;
	float pSensitivity;
	
	bool pPreventSaving;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create configuration. */
	reConfiguration( reWindowMain &windowMain );
	
	/** \brief Clean up configuration. */
	~reConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline reWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Grid size. */
	inline float GetGridSize() const{ return pGridSize; }
	
	/** \brief Set grid size. */
	void SetGridSize( float size );
	
	/** \brief Snap elements to grid. */
	inline bool GetSnapToGrid() const{ return pSnapToGrid; }
	
	/** \brief Set snap elements to grid. */
	void SetSnapToGrid( bool snap );
	
	/** \brief Rotation snap angle. */
	inline float GetRotSnapAngle() const{ return pRotSnap; }
	
	/** \brief Set rotation snap angle. */
	void SetRotSnapAngle( float angle );
	
	/** \brief Mouse sensitivity. */
	inline float GetSensitivity() const{ return pSensitivity; }
	
	/** \brief Set mouse sensitivity. */
	void SetSensitivity( float sensitivity );
	
	
	
	/** \brief Prevent saving. */
	inline bool GetPreventSaving() const{ return pPreventSaving; }
	
	/** \brief Set prevent saving. */
	void SetPreventSaving( bool preventSaving );
	
	/** \brief Load configuration. */
	void LoadConfiguration();
	
	/** \brief Save configuration. */
	void SaveConfiguration();
	/*@}*/
	
	
	
private:
	void pReset();
};

#endif
