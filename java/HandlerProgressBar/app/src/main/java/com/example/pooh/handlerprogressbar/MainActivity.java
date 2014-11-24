package com.example.pooh.handlerprogressbar;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ProgressBar;


public class MainActivity extends Activity {

    private Thread thread;
    private Handler handler;
    private ProgressBar progressBar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        progressBar = (ProgressBar) findViewById(R.id.progressBar);

        handler = new Handler() {

            @Override
            public void handleMessage(Message msg) {
                progressBar.setProgress(msg.arg1);
            }
        };
        thread = new Thread(new MyThread());
        thread.start();
    }

    private class MyThread implements Runnable {
        @Override
        public void run() {
            for (int i=0; i<100; i++) {
                try {
                    Message message = Message.obtain();
                    message.arg1 = i;
                    handler.sendMessage(message);
                    Thread.sleep(100);
                } catch (InterruptedException e) {

                }
            }


        }
    }
}
