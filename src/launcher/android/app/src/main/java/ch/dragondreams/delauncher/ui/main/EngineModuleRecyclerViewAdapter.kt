package ch.dragondreams.delauncher.ui.main

import android.view.LayoutInflater
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import ch.dragondreams.delauncher.databinding.FragmentEngineModuleBinding
import ch.dragondreams.delauncher.launcher.EngineModule

class EngineModuleRecyclerViewAdapter(
    private val entries: MutableList<EngineModule>
) : RecyclerView.Adapter<EngineModuleRecyclerViewAdapter.ViewHolder>() {
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
    }

    override fun getItemCount(): Int = entries.size

    inner class ViewHolder(binding: FragmentEngineModuleBinding) : RecyclerView.ViewHolder(binding.root) {
        val textName: TextView = binding.name
        val textVersion: TextView = binding.version
        val textType: TextView = binding.type

        override fun toString(): String {
            return super.toString() + " '" + textVersion.text + "'"
        }
    }
}