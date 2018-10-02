package com.traffic.shimhg02.makerfair;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

/**
 * Created by 신정민 on 2016-11-07.
 */
public class Err extends AppCompatActivity {
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.err_dialog);
    }

    public void Back(View v){
        finish();
    }
}
