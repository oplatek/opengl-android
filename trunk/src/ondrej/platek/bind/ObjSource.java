package ondrej.platek.bind;

import java.io.FileNotFoundException;
import java.io.InputStreamReader;

import ondrej.platek.R;

import android.app.Activity;
import android.widget.Toast;

public abstract class ObjSource implements java.io.Serializable{
	private static final long serialVersionUID = 5666893768260711975L;
	public static final String TITLE = "OBJ_SOURCE";
	
	public int ID;
	public String Title = "No title provided" ;
	public String Info = "No info available";
	public abstract InputStreamReader GetObjReader(Activity ctx) throws FileNotFoundException;
}
