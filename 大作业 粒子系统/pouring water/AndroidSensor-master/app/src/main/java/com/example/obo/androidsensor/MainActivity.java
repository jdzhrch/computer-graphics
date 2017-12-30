package com.example.obo.androidsensor;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Looper;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.IOException;
import java.io.PrintStream;
import java.net.Socket;
import java.net.UnknownHostException;

public class MainActivity extends AppCompatActivity {

    private String SERVER_HOST_IP = "192.168.199.133";

    /* 服务器端口 */
    private final int SERVER_HOST_PORT = 9400;
    boolean isConnect = false;
    boolean ifStarted = false;
    private Button btnConnect;
    private Button btnClose;
    private EditText edtShow;
    private EditText edtIpAddress;
    private Socket socket;
    private PrintStream output;
    private Thread thread;

    private static final String TAG = "MainActivity";
    SensorManager sensorManager;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();

        thread=new Thread(new Runnable() {

            @Override
            public void run() {
                Looper.prepare();
                initClientSocket();
                while (isConnect) {
                    try {
                        Thread.currentThread().sleep(5000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();

                    }
                    sendMessage(edtShow.getText().toString());
                }

            }
        });
        btnConnect.setOnClickListener(new Button.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                SERVER_HOST_IP = edtIpAddress.getText().toString();
                isConnect = true;
                if(ifStarted == false) {
                    thread.start();
                    ifStarted = true;
                }
            }
        });

        btnClose.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View v) {
                isConnect = false;
                closeSocket();
            }
        });

        // 陀螺仪传感器，负责水平旋转
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        sensorManager.registerListener(sensorEventListener, sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
//        sensorManager.registerListener(sensorEventListener, sensorManager.getDefaultSensor(Sensor.TYPE_ORIENTATION), SensorManager.SENSOR_DELAY_GAME);
        // 重力传感器，负责上下旋转
        sensorManager.registerListener(sensorEventListener, sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE), SensorManager.SENSOR_DELAY_UI);
    }

    SensorEventListener sensorEventListener = new SensorEventListener() {

        @Override
        public void onSensorChanged(SensorEvent sensorEvent) {
            if(sensorEvent.sensor.getType() == Sensor.TYPE_ACCELEROMETER){  //
                Log.i(TAG,"onSensorChanged");
                float X_lateral = sensorEvent.values[0];
                float Y_longitudinal = sensorEvent.values[1];
                float Z_vertical = sensorEvent.values[2];
                /*Log.i(TAG,"\n X "+X_lateral);
                Log.i(TAG,"\n Y "+Y_longitudinal);
                Log.i(TAG,"\n Z "+Z_vertical);*/
                edtShow.setText(" x:" + sensorEvent.values[0] + " y:" + sensorEvent.values[1]  + " z:" + sensorEvent.values[2]);
            } else if (sensorEvent.sensor.getType() == Sensor.TYPE_GYROSCOPE) {
                float X_lateral = sensorEvent.values[0];
                float Y_longitudinal = sensorEvent.values[1];
                float Z_vertical = sensorEvent.values[2];
                /*Log.i(TAG,"scop X "+X_lateral);
                Log.i(TAG,"scop Y "+Y_longitudinal);
                Log.i(TAG,"scop Z "+Z_vertical);
                Log.i(TAG, " ");*/
                edtShow.setText("scope x:" + sensorEvent.values[0] + "y:" + sensorEvent.values[1]  + "z:" + sensorEvent.values[2]);
            }
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int i) {
            Log.i(TAG, "onAccuracyChanged");
        }
    };

    public void initView()
    {
        btnConnect = (Button)findViewById(R.id.btnConnect);
        btnClose = (Button)findViewById(R.id.btnClose);
        edtShow = (EditText) findViewById(R.id.editText);
        edtIpAddress = (EditText) findViewById(R.id.ipAddress);

        edtShow.setEnabled(false);
    }

    public void closeSocket()
    {
        try
        {
            output.close();
            socket.close();
        }
        catch (IOException e)
        {
            handleException(e, "close exception: ");
        }
    }

    private void initClientSocket()
    {
        try
        {
      /* 连接服务器 */
            socket = new Socket(SERVER_HOST_IP, SERVER_HOST_PORT);
      /* 获取输出流 */
            output = new PrintStream(socket.getOutputStream(), true, "utf-8");
        }
        catch (UnknownHostException e)
        {
            handleException(e, "unknown host exception: " + e.toString());
        }
        catch (IOException e) {
            handleException(e, "io exception: " + e.toString());
        }
    }

    private void sendMessage(String msg)
    {
        output.print(msg);
    }

    public void toastText(String message)
    {
        Toast.makeText(this, message, Toast.LENGTH_LONG).show();
    }

    public void handleException(Exception e, String prefix)
    {
        e.printStackTrace();
        toastText(prefix + e.toString());
    }
}
