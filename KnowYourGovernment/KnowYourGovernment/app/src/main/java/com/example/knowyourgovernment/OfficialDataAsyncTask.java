package com.example.knowyourgovernment;

import android.annotation.SuppressLint;
import android.os.AsyncTask;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class OfficialDataAsyncTask extends AsyncTask<String, Void, String> {
    private static final String TAG = "OfficialDataAsyncTask";
    private String googleAPI = "https://www.googleapis.com/civicinfo/v2/representatives?key=";
    private String googleKey = "AIzaSyD5PKvtVR0NFWnTGD3SL7j9xY24MpOQJJI";
    private String googleQuery = "&address=";
    @SuppressLint("StaticFieldLeak")
    private MainActivity mainActivity;

    public OfficialDataAsyncTask(MainActivity mainActivity) {
        this.mainActivity = mainActivity;
    }

    @Override
    protected String doInBackground(String... strings) {
        StringBuilder sb = new StringBuilder();
        try {
            URL url = new URL(googleAPI + googleKey + googleQuery + strings[0]);

            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setRequestMethod("GET");
            InputStream is = conn.getInputStream();
            BufferedReader reader = new BufferedReader((new InputStreamReader(is)));

            String line;
            while ((line = reader.readLine()) != null) {
                sb.append(line).append('\n');
            }
        } catch (IOException e) {
            Log.e(TAG, "doInBackground: ", e);
            return "";
        }
        return sb.toString();
    }

    @Override
    protected void onPostExecute(String s) {
        if (!s.equals("")) {
            mainActivity.officialList.clear();
            try {
                JSONObject jObj = new JSONObject(s);
                JSONObject normalizedInput = jObj.getJSONObject("normalizedInput");
                JSONArray offices = jObj.getJSONArray("offices");
                JSONArray officials = jObj.getJSONArray("officials");

                mainActivity.displayLocation(String.format("%s, %s %s", normalizedInput.getString("city"),
                        normalizedInput.getString("state"), normalizedInput.getString("zip")));

                for (int i = 0; i < offices.length(); i++) {
                    JSONObject office = offices.getJSONObject(i);
                    String title = office.getString("name");
                    JSONArray members = office.getJSONArray("officialIndices");
                    for (int j = 0; j < members.length(); j++) {
                        JSONObject official = officials.getJSONObject(members.getInt(j));

                        String name = official.getString("name");
                        Log.d(TAG, "onPostExecute: Official name: " + name);

                        String party = official.getString("party");
                        if (party.equals("")) {
                            party = "Unknown";
                        }

                        //The following data fields may not exist

                        String address = "";
                        if (official.has("address")) {
                            JSONArray addressArray = official.getJSONArray("address");
                            JSONObject addressObj = addressArray.getJSONObject(0);
                            address = String.format("%s\n%s\n%s, %s %s", addressObj.getString("line1"), addressObj.getString("line2"),
                                    addressObj.getString("city"), addressObj.getString("state"), addressObj.getString("zip"));
                        }

                        String phone = "";
                        if (official.has("phones")) {
                            JSONArray phones = official.getJSONArray("phones");
                            phone = phones.getString(0);
                        }

                        String website = "";
                        if (official.has("urls")) {
                            JSONArray urls = official.getJSONArray("urls");
                            website = urls.getString(0);
                        }

                        String email = "";
                        if (official.has("emails")) {
                            JSONArray emails = official.getJSONArray("emails");
                            email = emails.getString(0);
                        }

                        String photo = "";
                        if (official.has("photoUrl")) {
                            photo = official.getString("photoUrl");
                        }

                        String googleplus = "";
                        String facebook = "";
                        String twitter = "";
                        String youtube = "";
                        if (official.has("channels")) {
                            JSONArray channels = official.getJSONArray("channels");
                            for (int k = 0; k < channels.length(); k++) {
                                JSONObject channel = channels.getJSONObject(k);
                                String type = channel.getString("type");
                                if (type.equals("GooglePlus")) {
                                    googleplus = channel.getString("id");
                                }
                                if (type.equals("Facebook")) {
                                    facebook = channel.getString("id");
                                }
                                if (type.equals("Twitter")) {
                                    twitter = channel.getString("id");
                                }
                                if (type.equals("YouTube")) {
                                    youtube = channel.getString("id");
                                }
                            }
                        }

                        Official o = new Official(title, name, address, party, phone, website, email, photo, googleplus, facebook, twitter, youtube);
                        mainActivity.addOfficial(o);
                    }
                }
            } catch (JSONException e) {
                Log.e(TAG, "onPostExecute: ", e);
                e.printStackTrace();
            }
        } else {
            mainActivity.inputNotFound();
        }
    }
}
