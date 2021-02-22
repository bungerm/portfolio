package com.example.knowyourgovernment;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.text.util.Linkify;
import android.view.View;
import android.widget.ImageView;
import android.widget.ScrollView;
import android.widget.TextView;
import com.squareup.picasso.Picasso;

public class OfficialActivity extends AppCompatActivity {
    private Official o;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_official);

        ScrollView sv = findViewById(R.id.scrollView2);
        TextView locationText2 = findViewById(R.id.currentLocation2);
        locationText2.setText(MainActivity.location);

        TextView title = findViewById(R.id.title);
        TextView name = findViewById(R.id.name);
        TextView party = findViewById(R.id.party);
        TextView address = findViewById(R.id.address);
        TextView address2 = findViewById(R.id.address2);
        TextView phone = findViewById(R.id.phone);
        TextView phone2 = findViewById(R.id.phone2);
        TextView website = findViewById(R.id.website);
        TextView website2 = findViewById(R.id.website2);
        TextView email = findViewById(R.id.email);
        TextView email2 = findViewById(R.id.email2);
        ImageView photo = findViewById(R.id.photo);
        ImageView partypin = findViewById(R.id.partyPin);
        ImageView googleplus = findViewById(R.id.googleplus);
        ImageView facebook = findViewById(R.id.facebook);
        ImageView twitter = findViewById(R.id.twitter);
        ImageView youtube = findViewById(R.id.youtube);

        Intent intent = getIntent();
        if (intent.hasExtra("Official")) {
            o = (Official) intent.getSerializableExtra("Official");
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

            temp = o.getAddress();
            if (!temp.equals("")) {
                address2.setText(temp);
                Linkify.addLinks(address2, Linkify.ALL);
            } else {
                address.setVisibility(View.GONE);
                address2.setVisibility(View.GONE);
            }

            temp = o.getPhone();
            if (!temp.equals("")) {
                phone2.setText(temp);
                Linkify.addLinks(phone2, Linkify.ALL);
            } else {
                phone.setVisibility(View.GONE);
                phone2.setVisibility(View.GONE);
            }

            temp = o.getWebsite();
            if (!temp.equals("")) {
                website2.setText(temp);
                Linkify.addLinks(website2, Linkify.ALL);
            } else {
                website.setVisibility(View.GONE);
                website2.setVisibility(View.GONE);
            }

            temp = o.getEmail();
            if (!temp.equals("")) {
                email2.setText(temp);
                Linkify.addLinks(email2, Linkify.ALL);
            } else {
                email.setVisibility(View.GONE);
                email2.setVisibility(View.GONE);
            }

            temp = o.getYoutube();
            if (temp.equals("")) {
                youtube.setVisibility(View.GONE);
            }

            temp = o.getFacebook();
            if (temp.equals("")) {
                facebook.setVisibility(View.GONE);
            }

            temp = o.getTwitter();
            if (temp.equals("")) {
                twitter.setVisibility(View.GONE);
            }

            temp = o.getGoogleplus();
            if (temp.equals("")) {
                googleplus.setVisibility(View.GONE);
            }
        }
    }

    public void twitterClicked(View v) {
        Intent intent = null;
        String twitterHandle = o.getTwitter();
        try {
            getPackageManager().getPackageInfo("com.twitter.android", 0);
            intent = new Intent(Intent.ACTION_VIEW, Uri.parse("twitter://user?screen_name=" + twitterHandle));
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        } catch (Exception e) {
            intent = new Intent(Intent.ACTION_VIEW, Uri.parse("https://twitter.com/" + twitterHandle));
        }
        startActivity(intent);
    }

    public void facebookClicked(View v) {
        String facebookHandle = o.getFacebook();
        String FACEBOOK_URL = "https://www.facebook.com/" + facebookHandle;
        String urlToUse;

        PackageManager packageManager = getPackageManager();
        try {
            int versionCode = packageManager.getPackageInfo("com.facebook.katana", 0).versionCode;
            if (versionCode >= 3002850) {
                urlToUse = "fb://facewebmodal/f?href=" + FACEBOOK_URL;
            } else {
                urlToUse = "fb://page/" + facebookHandle;
            }
        } catch (PackageManager.NameNotFoundException e) {
            urlToUse = FACEBOOK_URL;
        }
        Intent facebookIntent = new Intent(Intent.ACTION_VIEW);
        facebookIntent.setData(Uri.parse(urlToUse));
        startActivity(facebookIntent);
    }

    public void googlePlusClicked(View v) {
        String googleplusHandle = o.getGoogleplus();
        Intent intent = null;
        try {
            intent = new Intent(Intent.ACTION_VIEW);
            intent.setClassName("com.google.android.apps.plus", "com.google.android.apps.plus.phone.UrlGatewayActivity");
            intent.putExtra("customAppUri", googleplusHandle);
            startActivity(intent);
        } catch (ActivityNotFoundException e) {
            startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://plus.google.com/" + googleplusHandle)));
        }
    }

    public void youTubeClicked(View v) {
        String youtubeHandle = o.getYoutube();
        Intent intent = null;
        try {
            intent = new Intent(Intent.ACTION_VIEW);
            intent.setPackage("com.google.android.youtube");
            intent.setData(Uri.parse("https://www.youtube.com/" + youtubeHandle));
            startActivity(intent);
        } catch (ActivityNotFoundException e) {
            startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://www.youtube.com/" + youtubeHandle)));
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

    public void onPhotoClicked(View v) {
        Intent intent = new Intent(this, PhotoActivity.class);
        intent.putExtra("Official", o);
        startActivity(intent);
    }
}
