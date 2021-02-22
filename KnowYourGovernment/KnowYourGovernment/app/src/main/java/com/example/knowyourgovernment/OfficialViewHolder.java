package com.example.knowyourgovernment;

import android.view.View;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

public class OfficialViewHolder extends RecyclerView.ViewHolder {

    TextView officialTitle;
    TextView officialName;

    public OfficialViewHolder(@NonNull View itemView) {
        super(itemView);

        officialTitle = itemView.findViewById(R.id.officialTitle);
        officialName = itemView.findViewById(R.id.officialName);
    }
}
