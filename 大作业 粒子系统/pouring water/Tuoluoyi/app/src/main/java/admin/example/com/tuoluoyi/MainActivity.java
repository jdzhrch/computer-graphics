package admin.example.com.tuoluoyi;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;

import android.os.Looper;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.EditText;
import android.widget.Button;
import android.widget.Toast;
import android.view.View;

import java.io.IOException;
import java.io.PrintStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Timer;
import java.util.TimerTask;

import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;

import static java.lang.Thread.sleep;

import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends AppCompatActivity implements Runnable
{
    private LocationManager locationManager;


    private final String SERVER_HOST_IP = "192.168.199.133";

    /* 服务器端口 */
    private final int SERVER_HOST_PORT = 9400;

    private Button btnConnect;
    private Button btnSend;
    private EditText show;
    private Socket socket;
    private PrintStream output;
    private Thread thread;

    public void toastText(String message)
    {
        Toast.makeText(this, message, Toast.LENGTH_LONG).show();
    }

    public void handleException(Exception e, String prefix)
    {
        e.printStackTrace();
        toastText(prefix + e.toString());
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initView();

        // 获取系统LocationManager服务
        locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        // 从GPS获取最近的定位信息


        Location location = locationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
        updateView(location);

        // 将location里的位置信息显示在EditText中

        /* 设置每2秒获取一次GPS的定位信息 */
            locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 5000, 1, new LocationListener() {

                @Override
                public void onLocationChanged(Location location) {
                    // 当GPS定位信息发生改变时，更新位置
                    updateView(location);
                }

                @Override
                public void onProviderDisabled(String provider) {
                    updateView(null);
                }

                @Override
                public void onProviderEnabled(String provider) {
                    // 当GPS LocationProvider可用时，更新位置
                    updateView(locationManager.getLastKnownLocation(provider));

                }

                @Override
                public void onStatusChanged(String provider, int status,
                                            Bundle extras) {
                }
            });
        thread=new Thread(this);
        thread.start();
        btnConnect.setOnClickListener(new Button.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                //initClientSocket();
            }
        });

        btnSend.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendMessage(show.getText().toString());
            }
        });

    }
    private void updateView(Location location) {
        if (location != null) {
            StringBuffer sb = new StringBuffer();
            sb.append("position: \nlat:");
            sb.append(location.getLongitude());
            sb.append("\nlng:");
            sb.append(location.getLatitude());
            sb.append("\nhight:");
            sb.append(location.getAltitude());
            sb.append("\nspeed:");
            sb.append(location.getSpeed());
            sb.append("\ndirection:");
            sb.append(location.getBearing());
            sb.append("\njingdu:");
            sb.append(location.getAccuracy());
            sb.append("\n");
            show.setText(sb.toString());
        } else {
            // 如果传入的Location对象为空则随便显示点东西
            show.setText("juygfugf");
        }
    }

    public void initView()
    {
        btnConnect = (Button)findViewById(R.id.btnConnect);
        btnSend = (Button)findViewById(R.id.btnSend);
        show = (EditText) findViewById(R.id.editText);

        btnSend.setEnabled(false);
        show.setEnabled(false);
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
            Log.v("socket0","init client socket");
      /* 连接服务器 */
            socket = new Socket(SERVER_HOST_IP, SERVER_HOST_PORT);

            Log.v("socket1","init client socket");
      /* 获取输出流 */
            output = new PrintStream(socket.getOutputStream(), true, "utf-8");

            Log.v("socket2","init client socket");
            btnConnect.setEnabled(false);
            btnSend.setEnabled(true);
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

    @Override
    public void run() {
        Looper.prepare();
        initClientSocket();
        Log.v("sendmessage",show.getText().toString());
        while (true)
        {
            try {
                Thread.currentThread().sleep(5000);
            } catch (InterruptedException e) {
                e.printStackTrace();

            }
            Log.v("sendmessage2",show.getText().toString());
            sendMessage(show.getText().toString());
        }

    }
}