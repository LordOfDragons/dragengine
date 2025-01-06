package ch.dragondreams.delauncher.ui.main

import android.graphics.Bitmap
import android.util.Log
import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import ch.dragondreams.delauncher.R
import ch.dragondreams.delauncher.databinding.FragmentGameBinding
import ch.dragondreams.delauncher.launcher.Game
import ch.dragondreams.delauncher.launcher.GameIcon

class GamesRecyclerViewAdapter(
    private val entries: MutableList<Game>,
    private val listener: Listener
) : RecyclerView.Adapter<GamesRecyclerViewAdapter.ViewHolder>() {
    interface Listener {
        fun onItemClicked(game: Game)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        return ViewHolder(
            FragmentGameBinding.inflate(
                LayoutInflater.from(parent.context),
                parent,
                false
            )
        )
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val item = entries[position]
        holder.textTitle.text = item.title

        // this can be various things, for example the source of the game
        // or whatever. using the creator for the time being
        holder.textInfo.text = item.creator

        val bitmap: Bitmap? = largestGameIconBitmap(item)
        if(bitmap != null){
            holder.imageLogo.setImageBitmap(bitmap)
        }else {
            holder.imageLogo.setImageResource(R.drawable.image_unknown)
        }
    }

    override fun getItemCount(): Int = entries.size

    private fun largestGameIconBitmap(game: Game): Bitmap?{
        var icon: GameIcon? = null
        var bitmap: Bitmap? = null

        game.icons.forEach { i ->
            if(i.size > (icon?.size ?: 0)){
                icon = i
            }
        }

        try {
            bitmap = icon?.getImage()
        }catch (e: Exception){
            Log.e("GamesRecyclerViewAdapter",
                "largestGameIconBitmap: failed creating bitmap", e)
        }

        return bitmap
    }

    inner class ViewHolder(binding: FragmentGameBinding) : RecyclerView.ViewHolder(binding.root) {
        val textTitle: TextView = binding.textTitle
        val textInfo: TextView = binding.textInfo
        val imageLogo: ImageView = binding.imageLogo

        init {
            binding.root.setOnClickListener {
                listener.onItemClicked(entries[bindingAdapterPosition])
            }
        }

        override fun toString(): String {
            return super.toString() + " '" + textTitle.text + "'"
        }
    }
}