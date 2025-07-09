/**
 * Canvas panel.
 */
export class PanelCanvas {
	/**
	 * Create canvas panel.
	 * @param {String} panelId Panel identifier
	 */
	constructor (panelId) {
		this._panelId = panelId
		this._panel = document.getElementById(panelId)
		this._panelParent = this._panel.parentElement
	}
	
	/**
	 * Panel ID.
	 */
	panelId() {
		return this._panelId
	}
	
	/**
	 * Resize canvas to fit parent size.
	 */
	resizeCanvas() {
		this._panel.width = this._panelParent.innerWidth;
		this._panel.height = this._panelParent.innerHeight;
	}
	
	/**
	 * Fullscreen mode enabled.
	 */
	isFullScreen() {
		return document.fullscreenElement === this._panelParent
	}
	
	/**
	 * Enable full screen mode.
	 */
	enableFullScreen() {
		if (this._panelParent.requestFullscreen) {
			this._panelParent.requestFullscreen()
		}
	}
	
	/**
	 * Disable full screen mode.
	 */
	disableFullScreen() {
		if (document.exitFullscreen) {
			document.exitFullscreen()
		}
	}
	
	/**
	 * Toggle full screen mode.
	 */
	toggleFullScreen() {
		if (this.isFullScreen()) {
			this.disableFullScreen()
		} else {
			this.enableFullScreen()
		}
	}
}
