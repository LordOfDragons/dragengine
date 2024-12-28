package ch.dragondreams.delauncher.ui.main

import android.view.LayoutInflater
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import ch.dragondreams.delauncher.R
import ch.dragondreams.delauncher.databinding.FragmentEngineModuleBinding
import ch.dragondreams.delauncher.launcher.EngineModule

class EngineModuleRecyclerViewAdapter(
    private val entries: MutableList<EngineModule>,
    private val listener: Listener
) : RecyclerView.Adapter<EngineModuleRecyclerViewAdapter.ViewHolder>() {
    interface Listener {
        fun onItemClicked(module: EngineModule)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        return ViewHolder(
            FragmentEngineModuleBinding.inflate(
                LayoutInflater.from(parent.context),
                parent,
                false
            )
        )
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val item = entries[position]
        holder.textName.text = item.name
        holder.textVersion.text = item.version
        holder.textType.text = EngineModule.mapTypeName[item.type]
        when (item.status) {
            EngineModule.Status.Ready -> holder.imageUsable.setImageResource(R.drawable.checkmark_green)
            EngineModule.Status.NotTested -> holder.imageUsable.setImageResource(R.drawable.cross_red)
            EngineModule.Status.Broken -> holder.imageUsable.setImageResource(R.drawable.cross_red)
        }
    }

    override fun getItemCount(): Int = entries.size

    inner class ViewHolder(binding: FragmentEngineModuleBinding) : RecyclerView.ViewHolder(binding.root) {
        val textName: TextView = binding.textName
        val textVersion: TextView = binding.textVersion
        val textType: TextView = binding.textType
        val imageUsable: ImageView = binding.imageUsable

        init {
            binding.root.setOnClickListener {
                listener.onItemClicked(entries[bindingAdapterPosition])
            }
        }

        override fun toString(): String {
            return super.toString() + " '" + textVersion.text + "'"
        }
    }
}