package com.example.knowyourgovernment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.ArrayList;

public class OfficialAdapter extends RecyclerView.Adapter<OfficialViewHolder> {

    private ArrayList<Official> officialList;
    private MainActivity mainActivity;

    public OfficialAdapter(ArrayList<Official> officialList, MainActivity mainActivity) {
        this.officialList = officialList;
        this.mainActivity = mainActivity;
    }

    @NonNull
    @Override
    public OfficialViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(parent.getContext()).inflate(R.layout.official_row, parent, false);
        itemView.setOnClickListener(mainActivity);
        return new OfficialViewHolder(itemView);
    }

    @Override
    public void onBindViewHolder(@NonNull OfficialViewHolder holder, int position) {
        Official selectedOfficial = officialList.get(position);
        holder.officialTitle.setText(selectedOfficial.getTitle());
        holder.officialName.setText(selectedOfficial.getName() + " (" + selectedOfficial.getParty() + ")");
    }

    @Override
    public int getItemCount() {
        return officialList.size();
    }
}
