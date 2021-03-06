package ondrej.platek.bind;


import java.io.FileNotFoundException;
import java.io.InputStreamReader;

import ondrej.platek.R;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ConfigurationInfo;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;



public class BINDActivity extends Activity {
    static private String tag = "BINDActivity.java";
    static private final int CHOOSE_MENU = 0;
    BINDView glView;
    TextView info;
    TextView modelTitle;
    private boolean cameraStatic;
    ObjSource currentSource;

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
        super.onActivityResult(requestCode, resultCode, intent);
        if(resultCode == RESULT_OK) {
            Bundle extras = intent.getExtras();
            switch(requestCode) {
                case CHOOSE_MENU:
                    updateSource( (ObjSource) extras.getSerializable(ObjSource.TITLE) );
                    break;
                default:
                    // for future "intends"
                    break;
            }
        }
    }

    private void launchSelectMenu() {
        Intent i = new Intent(this, MenuActivity.class);
        startActivityForResult(i, CHOOSE_MENU);
    }

    private void updateSource(ObjSource newSource) {
        currentSource = newSource;
        glView.UpdateModel(prepareReader(currentSource));
        info.setText(currentSource.Info);
        modelTitle.setText(currentSource.Title);
    }

    private InputStreamReader prepareReader(ObjSource s) {
        InputStreamReader res;
        try{
            res = s.GetObjReader(this);
        } catch (FileNotFoundException e) {
            Toast.makeText(this, R.string.obj_not_found, Toast.LENGTH_SHORT);
            alertbox(R.string.obj_not_found);
            e.printStackTrace();
            return null;
        }
        return res;
    }
    

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        ObjFromResource defaultSource = new ObjFromResource(R.raw.clovehitch);
        defaultSource.Title = this.getString(R.string.clovehitch);
        defaultSource.Info = this.getString(R.string.clovehitch_info);
        currentSource = defaultSource;

        setContentView(R.layout.surface_view_overlay);
//        glView = (BINDView) findViewById(R.id.glview);
        FrameLayout f = (FrameLayout) findViewById(R.id.frame);
        info = (TextView) findViewById(R.id.tv_info);
        modelTitle = (TextView) findViewById(R.id.tv_title); 
        glView = new BINDView(this);
        // add glView like the first element -> the others (Title and Info) can cover it
        f.addView(glView,0);

        try {
            // Initialize GLSurface with model-Vertexes, normals,.. from default path to file.obj
            glView.Init(currentSource.GetObjReader(this));
            // currentSource is defaultSource at Creation
            updateSource(currentSource);
        } catch(Exception e){
            Toast.makeText(this, R.string.obj_not_found, Toast.LENGTH_SHORT);
            Log.e(tag,"not initialiase");
            finish();
        }
        
        // test if the SD card is working
        if(!Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())){
            alertbox(R.string.no_sd_title,R.string.no_sd_msg);
        }
        
        // Check if the system supports OpenGL ES 2.0.
        final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
        final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000;
     
        if (supportsEs2) {
            // Request an OpenGL ES 2.0 compatible context.
            // mView.setEGLContextClientVersion(2); // available from Api Level 8
        }
        else {
            // This is where you could create an OpenGL ES 1.x compatible
            // renderer if you wanted to support both ES 1 and ES 2.
            String msg = "TODO OpenGL ES 1.X is not yet supported";
            Log.e(tag, msg);

            Toast.makeText(getApplication(), msg, Toast.LENGTH_SHORT);
            return;
        }
    }

    @Override protected void onPause() {
        super.onPause();
        glView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        glView.onResume();
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        Log.i("motion", event.toString());
        debugDoGLAction();
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        if(item.isCheckable()){
            item.setChecked(true);
        }
        switch (item.getItemId()) {
        case R.id.help_about:
            launchHelp("About");
            return true;
        case R.id.help_nav:
            launchHelp("Navigation");
            return true;
        case R.id.explanations_off:            
            infoOn(false); 
            return true;
        case R.id.explanations_on:
            infoOn(true); 
            return true;
        case R.id.screensaver:
            launchScreensaverSettings();
            return true;
        case R.id.knot_select:
            launchSelectMenu();            
        default:
            return super.onOptionsItemSelected(item);
        }
    }
    
    @Override
    protected void onDestroy(){
        super.onDestroy();
    }
    
    private void launchScreensaverSettings() {
        debugDoGLAction();
        // TODO Auto-generated method stub
    }
    
    private void launchHelp(String string) {
        debugDoGLAction();
        // TODO Auto-generated method stub
        
    }

    public void infoOn(boolean v) { 
        int vis = v ? View.VISIBLE : View.GONE;
        
        info.setVisibility(vis);        
        LinearLayout wrapperInfo = (LinearLayout) findViewById(R.id.ll_info);
        wrapperInfo.setVisibility(vis);
        
        modelTitle.setVisibility(vis);
        LinearLayout wrapperTitle = (LinearLayout) findViewById(R.id.ll_title);
        wrapperTitle.setVisibility(vis);
    }

    public boolean getCameraStatic() { return this.cameraStatic; }
    //  debugging help functions START TODO delete it after usage
    boolean debug_switch = false;
    private void debugDoGLAction() {
      debug_switch = !debug_switch;
      if (debug_switch) {
        this.onPause();
      }
      else {
        this.onResume(); 
      } 
    }
    // debugging help functions END 
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        super.onCreateOptionsMenu(menu);
        getMenuInflater().inflate(R.menu.play, menu);
        return true;
    }
    

    protected void alertbox(int Rid_msg){
        alertbox(R.string.alert_def_title, Rid_msg);
    }
    protected void alertbox(int Rid_title, int Rid_msg)
    {
        String title = this.getString(Rid_title);
        String msg = this.getString(Rid_msg);
    new AlertDialog.Builder(this)
       .setMessage(msg)
       .setTitle(title)
       .setCancelable(true)
       .setNeutralButton(android.R.string.cancel,
          new DialogInterface.OnClickListener() {
          public void onClick(DialogInterface dialog, int whichButton){}
          })
       .show();
    }
}
