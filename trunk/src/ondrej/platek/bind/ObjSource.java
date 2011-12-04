package ondrej.platek.bind;

import java.io.FileNotFoundException;
import java.io.InputStreamReader;

import android.app.Activity;

public abstract class ObjSource implements java.io.Serializable{
	/**
	 * 
	 */
	private static final long serialVersionUID = 5666893768260711975L;
	public static final String TITLE = "OBJ_SOURCE";
	public abstract InputStreamReader GetObjReader(Activity ctx) throws FileNotFoundException;
}
