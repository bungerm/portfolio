package com.example.knowyourgovernment;

import java.io.Serializable;

public class Official implements Serializable {
    private String title;
    private String name;
    private String address;
    private String party;
    private String phone;
    private String website;
    private String email;
    private String photo;
    private String googleplus;
    private String facebook;
    private String twitter;
    private String youtube;

    public Official(String title, String name, String address, String party, String phone, String website,
                    String email, String photo, String googleplus, String facebook, String twitter, String youtube) {
        this.title = title;
        this.name = name;
        this.address = address;
        this.party = party;
        this.phone = phone;
        this.website = website;
        this.email = email;
        this.photo = photo;
        this.googleplus = googleplus;
        this.facebook = facebook;
        this.twitter = twitter;
        this.youtube = youtube;
    }

    public String getTitle() {
        return title;
    }

    public String getName() {
        return name;
    }

    public String getAddress() {
        return address;
    }

    public String getParty() {
        return party;
    }

    public String getPhone() {
        return phone;
    }

    public String getWebsite() {
        return website;
    }

    public String getEmail() {
        return email;
    }

    public String getPhoto() {
        return photo;
    }

    public String getGoogleplus() {
        return googleplus;
    }

    public String getFacebook() {
        return facebook;
    }

    public String getTwitter() {
        return twitter;
    }

    public String getYoutube() {
        return youtube;
    }

    @Override
    public String toString() {
        return "Official{" +
                "title='" + title + '\'' +
                ", name='" + name + '\'' +
                ", address='" + address + '\'' +
                ", party='" + party + '\'' +
                ", phone='" + phone + '\'' +
                ", website='" + website + '\'' +
                ", email='" + email + '\'' +
                ", photo='" + photo + '\'' +
                ", googleplus='" + googleplus + '\'' +
                ", facebook='" + facebook + '\'' +
                ", twitter='" + twitter + '\'' +
                ", youtube='" + youtube + '\'' +
                '}';
    }
}
