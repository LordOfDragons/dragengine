package ch.dragondreams.delauncher.launcher.internal

import ch.dragondreams.delauncher.launcher.GameProfile

class GameProfile private constructor(
    val nativeProfile: Long = 0L
){
    private var nativeRefCount: Int = 0

    private external fun gameProfileRelease(game: Long)
    private external fun gameProfileGetInfo(game: Long): GameProfileInfo
    private external fun gameProfileGetStatus(game: Long): GameProfileStatus

    fun dispose() {
        if (nativeRefCount-- == 0){
            gameProfileRelease(nativeProfile)
            dropInstance(nativeProfile)
        }
    }

    fun addReference() {
        nativeRefCount++
    }

    fun updateInfo(profile: GameProfile){
        gameProfileGetInfo(nativeProfile).update(profile)
    }

    fun updateStatus(profile: GameProfile){
        gameProfileGetStatus(nativeProfile).update(profile)
    }

    companion object {
        private external fun gameProfileCreate(): Long
        private external fun gameProfileCopy(profile: Long): Long

        private val mapProfiles: MutableMap<Long, ch.dragondreams.delauncher.launcher.internal.GameProfile> = mutableMapOf()

        fun getInstance(nativeProfile: Long): ch.dragondreams.delauncher.launcher.internal.GameProfile?{
            var profile: ch.dragondreams.delauncher.launcher.internal.GameProfile? = null
            if(nativeProfile != 0L) {
                profile = mapProfiles[nativeProfile]
                if (profile == null) {
                    profile = GameProfile(nativeProfile)
                    mapProfiles[nativeProfile] = profile
                }
            }
            return profile
        }

        fun createInstance(): ch.dragondreams.delauncher.launcher.internal.GameProfile {
            return getInstance(gameProfileCreate())!!
        }

        fun copyInstance(profile: ch.dragondreams.delauncher.launcher.internal.GameProfile):
                ch.dragondreams.delauncher.launcher.internal.GameProfile {
            return getInstance(gameProfileCopy(profile.nativeProfile))!!
        }

        private fun dropInstance(nativeProfile: Long){
            mapProfiles.remove(nativeProfile)
        }
    }
}
