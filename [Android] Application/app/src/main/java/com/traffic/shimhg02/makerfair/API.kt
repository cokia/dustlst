package com.traffic.shimhg02.makerfair


import retrofit2.Call
import retrofit2.http.*
import okhttp3.ResponseBody
import retrofit2.http.FormUrlEncoded
import retrofit2.http.POST
import retrofit2.http.GET



/**
 * Created by Kinetic on 2018-06-02.
 */

interface API {
    @GET("/place")
    fun getPlace(): Call<ArrayList<LocationRepo>>

    @POST ("/api/v1/login")
    fun getuser(): Call<ArrayList<ThinPlugRepo>>

}