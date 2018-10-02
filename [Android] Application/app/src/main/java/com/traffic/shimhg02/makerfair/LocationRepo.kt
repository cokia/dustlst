package com.traffic.shimhg02.makerfair


import com.google.gson.annotations.SerializedName

/**
 * Created by Kinetic on 2018-06-02.
 */

class LocationRepo {
    @SerializedName("lat")
    var lat = ""

    @SerializedName("lng")
    var lng = ""

    @SerializedName("placeName")
    var placeName = ""

    @SerializedName("dust1")
    var dust1 = ""

    @SerializedName("dust2")
    var dust2 = ""

    @SerializedName("humi")
    var humi = ""

    @SerializedName("temp")
    var temp = ""

    @SerializedName("time")
    var time = ""
}