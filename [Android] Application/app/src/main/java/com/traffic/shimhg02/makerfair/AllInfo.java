package com.traffic.shimhg02.makerfair;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

/**
 * Created by 신정민 on 2016-11-02.
 */
public class AllInfo extends AppCompatActivity {
    private String privateKey = "6b6743556a6a756e3739776d544a4e";
    String dataUrl;
    String date;
    String region;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.allinfo);
        Intent intent = getIntent();
        date = intent.getStringExtra("date");
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
            try{
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
                //이산화질소
                innerNode = element.getElementsByTagName("NO2");
                currentContent = innerNode.item(0).getTextContent();
                publishProgress(String.valueOf(R.id.NO2), currentContent);
                //오존 농도
                innerNode = element.getElementsByTagName("O3");
                currentContent = innerNode.item(0).getTextContent();
                publishProgress(String.valueOf(R.id.O3), currentContent);
                //일산화탄소
                innerNode = element.getElementsByTagName("CO");
                currentContent = innerNode.item(0).getTextContent();
                publishProgress(String.valueOf(R.id.CO), currentContent);
                //아황산가스
                innerNode = element.getElementsByTagName("SO2");
                currentContent = innerNode.item(0).getTextContent();
                publishProgress(String.valueOf(R.id.SO2), currentContent);
                //미세먼지
                innerNode = element.getElementsByTagName("PM10");
                currentContent = innerNode.item(0).getTextContent();
                publishProgress(String.valueOf(R.id.dust_allinfo), currentContent);
                //초미세먼지
                innerNode = element.getElementsByTagName("PM25");
                currentContent = innerNode.item(0).getTextContent();
                publishProgress(String.valueOf(R.id.microdust_allinfo), currentContent);

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
