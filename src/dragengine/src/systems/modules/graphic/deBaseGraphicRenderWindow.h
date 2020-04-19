/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEBASEGRAPHICRENDERWINDOW_H_
#define _DEBASEGRAPHICRENDERWINDOW_H_


/**
 * \brief Graphic module render window peer.
 */
class deBaseGraphicRenderWindow{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseGraphicRenderWindow();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicRenderWindow();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Host window changed. */
	virtual void HostWindowChanged();
	
	/** \brief Size of render window changed. */
	virtual void SizeChanged();
	
	/** \brief Title of render window changed. */
	virtual void TitleChanged();
	
	/** \brief Full screen changed. */
	virtual void FullScreenChanged();
	
	/** \brief Notification of script on resize changed. */
	virtual void ScriptNotifyChanged();
	
	/** \brief Painting enabled changed. */
	virtual void PaintEnabledChanged();
	
	/** \brief Icon changed. */
	virtual void IconChanged();
	/*@}*/
};

#endif
