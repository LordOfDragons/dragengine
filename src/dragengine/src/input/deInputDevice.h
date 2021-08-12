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

#ifndef _INPUTDEVICE_H_
#define _INPUTDEVICE_H_

#include "../deObject.h"
#include "../common/collection/decObjectOrderedSet.h"
#include "../common/math/decMath.h"
#include "../common/string/decString.h"
#include "../resources/image/deImageReference.h"
#include "../resources/model/deModelReference.h"
#include "../resources/skin/deSkinReference.h"
#include "../resources/rig/deRigReference.h"

class deInputDeviceButton;
class deInputDeviceAxis;
class deInputDeviceFeedback;


/**
 * \brief Input device installed on the host system.
 * 
 * Input devices provide digital or analog input. Devices can have buttons and axes.
 * 
 * The device identifier uniquely identifies the device across different platforms for
 * the same module. It can be safely used by the scripting module to save input bindings.
 * The identifier composes only of letters, digits and underscore to allow script using
 * them as prefix. The input module should add a unique prefix to the identifier to mark
 * the identifier originating fromt his module in case different input modules are involved.
 * 
 * Devices can have axes, buttons and feedbacks. Axes receive an analog value in the range
 * from -1 to 1 for absolue axes. For relative axes like the mouse only the relative change
 * is returned. Buttons return a pressed or unpressed state. Feedbacks on the other hand
 * allow manipulating a feedback state of the device if present.
 * 
 * Devices can have a pose indicating the coordinate frame and velocities for use with VR
 * tracking. These are all the edtVR* types. If a device does not support tracking the
 * identity coordinate frame with zero velocities is returned.
 * 
 * Devices can have bone poses for use by VR controllers. To keep the system simple the bone
 * configuration is indicated using an enumeration type. This ensures a consistent bone
 * configuration developers can rely upon.
 */
class deInputDevice : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deInputDevice> Ref;
	
	
	
public:
	/** \brief Device types. */
	enum eDeviceTypes{
		/** \brief Mouse. */
		edtMouse,
		
		/** \brief Keyboard. */
		edtKeyboard,
		
		/** \brief Game pad. */
		edtGamepad,
		
		/** \brief Joystick. */
		edtJoystick,
		
		/** \brief Touchpad. */
		edtTouchpad,
		
		/** \brief Touchscreen. */
		edtTouchscreen,
		
		/** \brief Racing wheel. */
		edtRacingWheel,
		
		/** \brief Generic device. */
		edtGeneric,
		
		/**
		 * \brief VR Head mounted display.
		 * \version 1.6
		 */
		edtVRHMD,
		
		/**
		 * \brief VR Controller.
		 * \version 1.6
		 */
		edtVRController,
		
		/**
		 * \brief VR Treadmill.
		 * \version 1.6
		 */
		edtVRTreadmill,
		
		/**
		 * \brief VR Tracker.
		 * \version 1.6
		 */
		edtVRTracker,
		
		/**
		 * \brief VR Base Station.
		 * \version 1.6
		 */
		edtVRBaseStation
	};
	
	/**
	 * \brief Bone configurations.
	 * \version 1.6
	 */
	enum eBoneConfigurations{
		/** \brief No bones supported. */
		ebcNone,
		
		/**
		 * \brief Articulated hand bones.
		 * 
		 * Bones are defined for fingers in this order:
		 * - Wrist: 0
		 * - Thumb: bones 1 to 4
		 * - Index finger: bones 5 to 8
		 * - Middle finger: bones 9 to 12
		 * - Ring finger: bones 13 to 16
		 * - Pinky finger: bones 17 to 20
		 * 
		 * For a total of 21 bones. Each finger has 4 bones in these order:
		 * - Segment 0: inside palm
		 * - Segment 1: first finger segment
		 * - Segment 2: second finger segment
		 * - Segment 3: third finger segment
		 * 
		 * Poses are relative to the parent bone inside chains and the device coordinate
		 * frame for chain root bones.
		 */
		ebcHand
	};
	
	/**
	 * \brief Convenience enumeration for hand bones.
	 * \version 1.6
	 */
	enum eHandBones{
		ehbWrist,
		ehbThumb0,
		ehbThumb1,
		ehbThumb2,
		ehbThumb3,
		ehbIndex0,
		ehbIndex1,
		ehbIndex2,
		ehbIndex3,
		ehbMiddle0,
		ehbMiddle1,
		ehbMiddle2,
		ehbMiddle3,
		ehbRing0,
		ehbRing1,
		ehbRing2,
		ehbRing3,
		ehbPinky0,
		ehbPinky1,
		ehbPinky2,
		ehbPinky3
	};
	
	/**
	 * \brief Convenience value count of hand bones.
	 * \version 1.6
	 */
	static const int HandBoneCount = 21;
	
	
	
private:
	/** \brief Device identifier unique across reboots. */
	decString pID;
	
	/** \brief Display name. */
	decString pName;
	
	/** \brief Device type. */
	eDeviceTypes pType;
	
	/** \brief Model to represent the device in 3D user interfaces or NULL if not set. */
	deModelReference pDisplayModel;
	
	/** \brief Skin for display model or NULL if not set. */
	deSkinReference pDisplaySkin;
	
	/**
	 * \brief Image to represent the device in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	deImageReference pDisplayImage;
	
	/** \brief List of small icons of different size for use in binding displays. */
	decObjectOrderedSet pDisplayIcons;
	
	/** \brief Text to display centered across display image or icon. */
	decString pDisplayText;
	
	/** \brief Buttons. */
	deInputDeviceButton *pButtons;
	
	/** \brief Number of buttons. */
	int pButtonCount;
	
	/** \brief Axes. */
	deInputDeviceAxis *pAxes;
	
	/** \brief Number of axes. */
	int pAxisCount;
	
	/** \brief Feedbacks. */
	deInputDeviceFeedback *pFeedbacks;
	
	/** \brief Number of feedbacks. */
	int pFeedbackCount;
	
	/** \brief Bone configuration. */
	eBoneConfigurations pBoneConfiguration;
	
	/**
	 * \brief Finger tip offsets.
	 * 
	 * For use with ebcHand bone configuration. Defines the offset of the finger tip point
	 * relative to the last finger segment coordinate frame. These are typically used to
	 * figure out where finger tips touch objects in the world.
	 */
	decVector pFingerTipOffset[ 5 ];
	
	/** \brief Hand rig if ebcHand is used. */
	deRigReference pHandRig;
	
	/** \brief Model to represent the device in VR environments or NULL if not set. */
	deModelReference pVRModel;
	
	/** \brief Skin for VR model or NULL if not set. */
	deSkinReference pVRSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create input device. */
	deInputDevice();
	
protected:
	/** \brief Clean up input device. */
	virtual ~deInputDevice();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Device identifier.
	 * 
	 * Unique identifier for the device identifying it across removing and attaching
	 * the device to the host system. The prefix is a normalized string that contains
	 * only letters, numbers and underscores. It is suitable to be combined with
	 * button or axis identifiers to store them in config files as key bindings.
	 */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID( const char *id );
	
	/**
	 * \brief Display name.
	 * 
	 * Dispaly name is human readable like for example 'USB Gamepad #1'. The
	 * name is unique but not guaranteed to stay the same across restarting
	 * the input module or game engine.
	 */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set Display name. */
	void SetName( const char *name );
	
	/** \brief Device type. */
	inline eDeviceTypes GetType() const{ return pType; }
	
	/** \brief Device type. */
	void SetType( eDeviceTypes type );
	
	/** \brief Model to represent the device in 3D user interfaces or NULL if not set. */
	inline deModel *GetDisplayModel() const{ return pDisplayModel; }
	
	/** \brief Set model to represent the device in 3D user interfaces or NULL if not set. */
	void SetDisplayModel( deModel *model );
	
	/** \brief Skin for display model or NULL if not set. */
	inline deSkin *GetDisplaySkin() const{ return pDisplaySkin; }
	
	/** \brief Set skin for display model or NULL if not set. */
	void SetDisplaySkin( deSkin *skin );
	
	/**
	 * \brief Image to represent the device in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	inline deImage *GetDisplayImage() const{ return pDisplayImage; }
	
	/**
	 * \brief Set image to represent the device in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	void SetDisplayImage( deImage *image );
	
	/** \brief Count of icons representing the device in bindings. */
	int GetDisplayIconCount() const;
	
	/**
	 * \brief Icon at index representing the device in bindings.
	 * 
	 * Icon is of square size and typically has a size of 16, 24, 32 or 64.
	 */
	deImage *GetDisplayIconAt( int index ) const;
	
	/**
	 * \brief Add icon representing the device in bindings.
	 * 
	 * Icon is of square size and typically has a size of 16, 24, 32 or 64.
	 */
	void AddDisplayIcon( deImage *image );
	
	/** \brief Text to display centered across display image or icon. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** \brief Set text to display centered across display image or icon. */
	void SetDisplayText( const char *text );
	
	/**
	 * \brief Bone configuration.
	 * \version 1.6
	 */
	inline eBoneConfigurations GetBoneConfiguration() const{ return pBoneConfiguration; }
	
	/**
	 * \brief Set bone configuration.
	 * \version 1.6
	 */
	void SetBoneConfiguration( eBoneConfigurations configuration );
	
	/**
	 * \brief Finger tip offsets.
	 * \version 1.6
	 * 
	 * For use with ebcHand bone configuration. Defines the offset of the finger tip point
	 * relative to the last finger segment coordinate frame. These are typically used to
	 * figure out where finger tips touch objects in the world.
	 */
	const decVector &GetFingerTipOffset( int index ) const;
	
	/**
	 * \brief Finger tip offsets.
	 * \version 1.6
	 * 
	 * For use with ebcHand bone configuration. Defines the offset of the finger tip point
	 * relative to the last finger segment coordinate frame. These are typically used to
	 * figure out where finger tips touch objects in the world.
	 */
	void SetFingerTipOffset( int index, const decVector &offset );
	
	/**
	 * \brief Hand rig if ebcHand is used.
	 * \version 1.6
	 */
	inline deRig *GetHandRig() const{ return pHandRig; }
	
	/**
	 * \brief Set hand rig if ebcHand is used.
	 * \version 1.6
	 */
	void SetHandRig( deRig *rig );
	
	/**
	 * \brief Model to represent the device in VR environments or NULL if not set.
	 * \version 1.6
	 */
	inline deModel *GetVRModel() const{ return pVRModel; }
	
	/**
	 * \brief Set model to represent the device in VR environments or NULL if not set.
	 * \version 1.6
	 */
	void SetVRModel( deModel *model );
	
	/**
	 * \brief Skin for VR model or NULL if not set.
	 * \version 1.6
	 */
	inline deSkin *GetVRSkin() const{ return pVRSkin; }
	
	/**
	 * \brief Skin for VR model or NULL if not set.
	 * \version 1.6
	 */
	void SetVRSkin( deSkin *skin );
	/*@}*/
	
	
	
	/** \name Buttons */
	/*@{*/
	/** \brief Number of buttons. */
	inline int GetButtonCount() const{ return pButtonCount; }
	
	/**
	 * \brief Set number of buttons.
	 * 
	 * Resets all buttons to default values.
	 */
	void SetButtonCount( int count );
	
	/** \brief Button at index. */
	deInputDeviceButton &GetButtonAt( int index ) const;
	
	/** \brief Index of button with identifier or -1 if not found. */
	int IndexOfButtonWithID( const char *id ) const;
	/*@}*/
	
	
	
	/** \name Axes */
	/*@{*/
	/** \brief Number of axes. */
	inline int GetAxisCount() const{ return pAxisCount; }
	
	/**
	 * \brief Set number of axes.
	 * 
	 * Resets all axes to default values.
	 */
	void SetAxisCount( int count );
	
	/** \brief Axis at index. */
	deInputDeviceAxis &GetAxisAt( int index ) const;
	
	/** \brief Index of axis with identifier or -1 if not found. */
	int IndexOfAxisWithID( const char *id ) const;
	/*@}*/
	
	
	
	/** \name Feedbacks */
	/*@{*/
	/** \brief Number of feedbacks. */
	inline int GetFeedbackCount() const{ return pFeedbackCount; }
	
	/**
	 * \brief Set number of feedbacks.
	 * 
	 * Resets all feedbacks to default values.
	 */
	void SetFeedbackCount( int count );
	
	/** \brief Feedback at index. */
	deInputDeviceFeedback &GetFeedbackAt( int index ) const;
	
	/** \brief Index of feedback with identifier or -1 if not found. */
	int IndexOfFeedbackWithID( const char *id ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
