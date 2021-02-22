package com.example.knowyourgovernment;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.location.Address;
import android.location.Criteria;
import android.location.Geocoder;
import android.location.Location;
import android.location.LocationManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.net.URI;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import static androidx.core.content.PermissionChecker.PERMISSION_GRANTED;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{
    private static final String TAG = "MainActivity";
    public ArrayList<Official> officialList = new ArrayList<>();
    private RecyclerView recyclerView;
    private OfficialAdapter officialAdapter;

    private static int MY_LOCATION_REQUEST_CODE_ID = 329;
    public static String location;
    private TextView locationText1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        locationText1 = findViewById(R.id.currentLocation);

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        recyclerView = findViewById(R.id.recyclerView);
        officialAdapter = new OfficialAdapter(officialList, this);
        recyclerView.setAdapter(officialAdapter);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION)
                != PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(
                    this,
                    new String[]{
                            Manifest.permission.ACCESS_FINE_LOCATION
                    },
                    MY_LOCATION_REQUEST_CODE_ID);
        } else {
            setLocation();
        }

        if (!connected()){
            locationText1.setText("No Network Connection");
            builder.setTitle("No Network Connection");
            builder.setPositiveButton("Got it", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) { }});
            AlertDialog alertDialog = builder.create();
            alertDialog.show();
        } else if (location != null){
            new OfficialDataAsyncTask(this).execute(location);
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.aboutMenu:
                Intent intent = new Intent(this, AboutActivity.class);
                startActivity(intent);
                return true;
            case R.id.searchMenu:
                if (connected()) {
                    final EditText et = new EditText(this);
                    et.setGravity(Gravity.CENTER);
                    AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    builder.setView(et);
                    builder.setTitle("Enter \"City, State\" or \"Zipcode\"");
                    builder.setPositiveButton("Enter", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            String loc = et.getText().toString();
                            new OfficialDataAsyncTask(MainActivity.this).execute(Uri.encode(loc));
                        }
                    });
                    builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) { }});
                    AlertDialog alertDialog = builder.create();
                    alertDialog.show();
                } else {
                    AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    builder.setTitle("No network connection");
                    builder.setPositiveButton("Got it", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) { }});
                    AlertDialog alertDialog = builder.create();
                    alertDialog.show();
                }
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    @Override
    public void onClick(View v) {
        int pos = recyclerView.getChildLayoutPosition(v);
        Official o = officialList.get(pos);
        Intent intent = new Intent(this, OfficialActivity.class);
        intent.putExtra("Official", o);
        startActivity(intent);
    }


    private boolean connected() {
        ConnectivityManager cm = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        if (cm == null) {
            Toast.makeText(this, "Connectivity Manager is null!", Toast.LENGTH_LONG).show();
            return false;
        }
        NetworkInfo netInfo = cm.getActiveNetworkInfo();
        return netInfo != null && netInfo.isConnected();
    }

    @Override
    public void onRequestPermissionsResult(
            int requestCode, @NonNull
            String[] permissions, @NonNull
                    int[] grantResults) {

        super.onRequestPermissionsResult(requestCode, permissions, grantResults);

        if (requestCode == MY_LOCATION_REQUEST_CODE_ID) {
            if (permissions[0].equals(Manifest.permission.ACCESS_FINE_LOCATION) &&
                    grantResults[0] == PERMISSION_GRANTED) {
                setLocation();
                return;
            }
        }
        locationText1.setText("No Location Permission");
    }

    @SuppressLint("MissingPermission")
    private void setLocation() {
        Criteria criteria = new Criteria();
        criteria.setPowerRequirement(Criteria.POWER_LOW);
        //criteria.setPowerRequirement(Criteria.POWER_HIGH);
        criteria.setAccuracy(Criteria.ACCURACY_MEDIUM);
        //criteria.setAccuracy(Criteria.ACCURACY_FINE);
        criteria.setAltitudeRequired(false);
        criteria.setBearingRequired(false);
        criteria.setSpeedRequired(false);
        LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        String bestProvider = locationManager.getBestProvider(criteria, true);
        Log.d(TAG, "setLocation: " + bestProvider);
        Location l = locationManager.getLastKnownLocation(bestProvider);
        Geocoder geocoder = new Geocoder(this, Locale.getDefault());

        if (l != null) {
            double lat = l.getLatitude();
            double lon = l.getLongitude();
            try {
                List<Address> addresses = geocoder.getFromLocation(lat, lon, 10);
                Address address = addresses.get(0);
                if (!address.getPostalCode().equals("")) {
                    location = address.getPostalCode();
                }
                else if (!address.getSubLocality().equals("")) {
                    location = address.getSubLocality();
                }
                else if (!address.getLocality().equals("")){
                    location = address.getLocality();
                } else {
                    locationText1.setText("Could Not Find Usable Location");
                }
            } catch (IOException e) {
                Log.e(TAG, "setLocation: ", e);
            }
        } else {
            locationText1.setText("No Location Data");
            Log.d(TAG, "setLocation: Location Data Is Null");
        }
    }

    public void displayLocation(String s) {
        location = s;
        locationText1.setText(s);
    }

    public void inputNotFound() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Location Not Found");
        builder.setPositiveButton("Got it", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) { }});
        AlertDialog alertDialog = builder.create();
        alertDialog.show();
    }

    public void addOfficial(Official o) {
        officialList.add(o);
        officialAdapter.notifyDataSetChanged();
    }
}
