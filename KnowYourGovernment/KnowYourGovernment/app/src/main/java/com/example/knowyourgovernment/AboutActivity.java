package com.example.knowyourgovernment;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.text.util.Linkify;
import android.view.View;
import android.widget.TextView;

public class AboutActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_about);

        TextView gc = findViewById(R.id.googlecivic);
        gc.setText(R.string.google_civic_information_api);
    }

    public void googleCivicClicked(View v) {
        Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://developers.google.com/civic-information"));
        startActivity(intent);
    }
}
