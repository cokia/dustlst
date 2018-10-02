package com.traffic.shimhg02.makerfair;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.v4.app.Fragment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.traffic.shimhg02.makerfair.R;


public class DustDataFragment extends Fragment implements View.OnClickListener {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view=inflater.inflate(R.layout.fragment_dustdata,
                container,
                false);
        Button btn=(Button)view.findViewById(R.id.button_Dust);
        Button bt=(Button)view.findViewById(R.id.button_All);
        btn.setOnClickListener(this);
        bt.setOnClickListener(this);
        return view;
    }

    @Override
    public void onClick(View v) {
        switch (v.getId())
        {
            case R.id.button_All:
            {
                EditText date_EditText = (EditText)((Activity)v.getContext()).findViewById(R.id.date);
                EditText region_EditText = (EditText)((Activity)v.getContext()).findViewById(R.id.region);
                if(region_EditText.getText().toString().equals(""))
                {
                    Toast.makeText(v.getContext(), "지역을 제대로 입력해주세요", Toast.LENGTH_LONG).show();
                    return;
                }
                String date = date_EditText.getText().toString();
                try{
                    String[] dateArray = date.split("[년월일]");
                    date = "";
                    for(int i = 0; i < dateArray.length; i++)
                        date = date + dateArray[i];
                    if(date.length() != 8)
                        throw new Exception();
                }catch (Exception e){
                    Toast.makeText(v.getContext(), "날짜를 제대로 입력해주세요", Toast.LENGTH_LONG).show();
                    return;
                }
                Intent intent = new Intent(v.getContext(), AllInfo.class);
                intent.putExtra("date", date);
                intent.putExtra("region", region_EditText.getText().toString());
                startActivity(intent);
                break;
            }
            case R.id.button_Dust:
            {
                EditText region_EditText =  (EditText)((Activity)v.getContext()).findViewById(R.id.region_dust);
                if(region_EditText.getText().toString().equals(""))
                {
                    Toast.makeText(v.getContext(), "지역을 제대로 입력해주세요", Toast.LENGTH_LONG).show();
                    return;
                }
                Intent intent = new Intent(v.getContext(), Dust.class);
                intent.putExtra("region", region_EditText.getText().toString());
                startActivity(intent);
                break;
            }
        }
    }
}
