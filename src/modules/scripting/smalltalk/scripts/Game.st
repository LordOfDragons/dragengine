" Smalltalk Class
| 
| Copyright (C) 2009, Plüss Roland ( roland@rptd.ch )
| 
| This program is free software; you can redistribute it and/or 
| modify it under the terms of the GNU General Public License 
| as published by the Free Software Foundation; either 
| version 2 of the License, or (at your option) any later 
| version.
| 
| This program is distributed in the hope that it will be useful,
| but WITHOUT ANY WARRANTY; without even the implied warranty of
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
| GNU General Public License for more details.
| 
| You should have received a copy of the GNU General Public License
| along with this program; if not, write to the Free Software
| Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA."

Object subclass: DEGame [
	<comment:
		'I am the game base class. Each game has to create exactly
		one subclass of me. I provide an empty implementation for
		each method the Smalltalk Scripting Module calls during
		running the game. In addition I know how to obtain command
		line parameters provided to the game.'>
	
	DEGame class >> new [
		<category: 'instance creation'>
		^ super new init.
	]
	
	init [
		<category: 'initialization'>
		"Initializes a newly created game instance. Implement this
		method in your game class to setup class variables. This
		is not the initialization of the game itself. For this
		#initGame is send."
	]
	
	initGame [
		<category: 'game engine events'>
		"Send by the game engine to initialize the game."
	]
	
	cleanUp [
		<category: 'game engine events'>
		"Send by the game engine to shut down the game."
	]
	
	onFrameUpdate: elapsed [
		<category: 'game engine events'>
		"Send by the game engine once during each frame update.
		Receiver has to update the game state but not render
		to the screen yet."
	]
	
	onResizeRenderWindow [
		<category: 'game engine events'>
		"Send by the game engine after the size of the render
		window has changed."
	]
	
	eventKeyPress: keycode modifiers: modifiers keychar: keychar [
		<category: 'game engine events'>
		"Send by the game engine when the player pressed down
		and holds a key on the keyboard or any other character
		input device."
	]
	
	eventKeyRelease: keycode modifiers: modifiers keychar: keychar [
		<category: 'game engine events'>
		"Send by the game engine when the player releases a
		previously pressed down key on the keyboard or any
		other character input device."
	]
	
	eventKeyType: keycode modifiers: modifiers keychar: keychar [
		<category: 'game engine events'>
		"Send by the game engine when the player pressed and
		released a key on the keyboard or any other character
		input device. This message is send after #eventKeyPress
		and #eventKeyRelease for the same key has been send
		previously."
	]
	
	eventMousePress: button modifiers: modifiers [
		<category: 'game engine events'>
		"Send by the game engine when the player pressed down
		and holds a button on the mouse or any pointing device."
	]
	
	eventMouseRelease: button modifiers: modifiers [
		<category: 'game engine events'>
		"Send by the game engine when the player released a
		button on the mouse or any pointing device."
	]
	
	eventMouseMove: dx dy: dy button: button modifiers: modifiers [
		<category: 'game engine events'>
		"Send by the game engine when the player moves the
		mouse or any pointing device. The motion is given
		relatively to the current location if the device is
		in relative mode otherwise the absolute position is
		returned."
	]
	
	paintScreen [
		<category: 'game engine events'>
		"Send by the game engine at the end of each frame
		update. The receiver has to paint the screen using
		the main render window obtained through the
		DEGraphicSystem object."
	]
]
