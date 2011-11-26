package ondrej.platek.bind;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import ondrej.platek.objLoader.TDModel;
import android.opengl.GLSurfaceView.Renderer;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;


public class NativeRenderer implements Renderer {
	private static String TAG = "NativeRendererJavaPart";
	
	private String objfile ; 
	
	private int vertexes_size = 0;
	private float[] vertexes;
	private int parts_number;
	private int[] parts_sizes;
	private float[][] normals;
	private short[][] faces;
	
	private int width;
	private int height;
	
	private boolean paused = false;
	
	private TDModel model;
	
    private native void init(float[][] normals, short[][] faces);
    private native void step();
    public native void Zoom(float dz);    
    public native void RotateAnchor(float dx,float dy);
    
	public NativeRenderer(TDModel Model) {
		model = Model;
	}
	
    public void onDrawFrame(GL10 glUnused) { 
    	if(!getPaused()) {
    		step();
    	}
    }
    
    public void onSurfaceChanged(GL10 glUnused, int Width, int Height) {
		
		// TODO is this good method to set it up? 
		// do I need to parse the obj every time -> NO
		// does the surface change every time I need to load new object -> no
		// TODO result: create different methods
		vertexes = model.GetVertexArr();
		vertexes_size = model.getVertexNumber(); 
		parts_number = model.getPartsNumber();
		normals = model.getNormals();
		faces = model.getFaces();
		parts_sizes = model.getPartsSizes();
		
		width = Width;
		height = Height;
		
		// Native part initialize width, height, vertexes and vertexes_size
		init(normals, faces);
		Log.i(TAG, "Changed screen: widht screen " + Integer.toString(width) + ", height " + Integer.toString(width));
    }
    
    public void onSurfaceCreated(GL10 glUnused, EGLConfig config) {
    	// TODO separate onSurfaceChanged and onSurfaceCreated if necessary
    }
    
	public void setPaused(boolean v) { this.paused = v; }
	public boolean getPaused() {return this.paused; }
	public boolean togglePause() { 
		setPaused(!getPaused());
		return getPaused(); 
	}
	
	public String GetObjFile(){
		return this.objfile;
	}
} // end of class Renderer