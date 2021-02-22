package com.example.knowyourgovernment;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.ScrollView;
import android.widget.TextView;

import com.squareup.picasso.Picasso;

public class PhotoActivity extends AppCompatActivity {
    Official o;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_photo);

        ScrollView sv = findViewById(R.id.scrollView2);
        TextView locationText2 = findViewById(R.id.currentLocation3);
        locationText2.setText(MainActivity.location);

        TextView title = findViewById(R.id.title2);
        TextView name = findViewById(R.id.name2);
        TextView party = findViewById(R.id.party2);
        ImageView photo = findViewById(R.id.photo2);
        ImageView partypin = findViewById(R.id.partyPin2);

        Intent intent = getIntent();
        if (intent.hasExtra("Official")) {
            o = (Official) intent.getSerializableExtra("Official");
        }
        title.setText(o.getTitle());
        name.setText(o.getName());
        party.setText(o.getParty());

        String temp;
        temp = o.getParty().toUpperCase();
        if (temp.contains("DEMO")) {
            partypin.setImageResource(R.drawable.dem_logo);
            sv.setBackgroundColor(Color.BLUE);
        } else if (temp.contains("REPUB")) {
            partypin.setImageResource(R.drawable.rep_logo);
            sv.setBackgroundColor(Color.RED);
        } else {
            partypin.setVisibility(View.GONE);
        }


        temp = o.getPhoto();
        if (!temp.equals("")) {
            Picasso picasso = new Picasso.Builder(this).build();
            picasso.load(Uri.parse(temp))
                    .error(R.drawable.brokenimage)
                    .placeholder(R.drawable.placeholder)
                    .into(photo);
        } else {
            photo.setClickable(false);
        }
    }

    public void partyPinClicked(View v) {
        Intent intent = new Intent(Intent.ACTION_VIEW);;
        if (o.getParty().toUpperCase().contains("DEMO")) {
            intent.setData(Uri.parse("https://democrats.org"));
            startActivity(intent);
        } else if (o.getParty().toUpperCase().contains("REPUB")) {
            intent.setData(Uri.parse("https://www.gop.com"));
            startActivity(intent);
        }
    }
}
