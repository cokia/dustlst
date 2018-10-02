package com.traffic.shimhg02.makerfair;

import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.TextView;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import java.util.Calendar;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

/**
 * Created by 신정민 on 2016-11-02.
 */
public class Dust extends AppCompatActivity {
    private String privateKey = "6b6743556a6a756e3739776d544a4e";
    String dataUrl;
    String date;
    String region;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.dust);
        Intent intent = getIntent();
        Calendar myCalendar = Calendar.getInstance();

        String year = String.valueOf(myCalendar.get(Calendar.YEAR));
        String month = String.valueOf(myCalendar.get(Calendar.MONTH) + 1);
        String day = String.valueOf(myCalendar.get(Calendar.DAY_OF_MONTH));
        date = year;
        if (month.length() == 2)
            date = date + month;
        else
            date = date + "0" + month;

        if (day.length() == 2)
            date = date + day;
        else
            date = date + "0" + day;

        region = intent.getStringExtra("region");

        MyTask task = new MyTask();
        task.execute(this);
    }

    public void Back(View v){
        finish();
    }

    public class MyTask extends AsyncTask<AppCompatActivity, String, Boolean> {
        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
            int id = Integer.parseInt(values[0]);
            String text = values[1];
            TextView tv = (TextView) findViewById(id);
            tv.setText(text);
        }

        @Override
        protected Boolean doInBackground(AppCompatActivity... appCompatActivities) {
            Node node;
            Element element;
            NodeList innerNode;
            try {
                dataUrl = "http://openapi.seoul.go.kr:8088/" + privateKey + "/xml/DailyAverageAirQuality/1/5/" + date + "/" + region;
                DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
                DocumentBuilder builder = null;
                Document doc = null;
                builder = factory.newDocumentBuilder();
                doc = builder.parse(dataUrl);
                doc.getDocumentElement().normalize();

                Element root = doc.getDocumentElement();
                NodeList list = doc.getElementsByTagName("DailyAverageAirQuality");
                root = (Element) list.item(0);

                NodeList result = root.getElementsByTagName("RESULT");
                node = result.item(0);
                element = (Element) node;
                innerNode = element.getElementsByTagName("CODE");
                if (!innerNode.item(0).getTextContent().equals("INFO-000")) {
                    finish();
                }

                list = root.getElementsByTagName("row");
                String currentContent;
                node = list.item(0);
                element = (Element) node;

                //미세먼지
                innerNode = element.getElementsByTagName("PM10");
                currentContent = innerNode.item(0).getTextContent();
                publishProgress(String.valueOf(R.id.dust), currentContent);
                //초미세먼지
                innerNode = element.getElementsByTagName("PM25");
                currentContent = innerNode.item(0).getTextContent();
                publishProgress(String.valueOf(R.id.microdust), currentContent);
            }catch (Exception e){
                e.printStackTrace();
                Intent intent = new Intent(appCompatActivities[0], Err.class);
                startActivity(intent);
                appCompatActivities[0].finish();
            }
            return null;
        }
    }
}
