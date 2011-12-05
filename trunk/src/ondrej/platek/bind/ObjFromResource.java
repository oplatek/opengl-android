package ondrej.platek.bind;

import java.io.FileNotFoundException;
import java.io.InputStreamReader;

import android.app.Activity;

public class ObjFromResource extends ObjSource {
	public int RES_ID;
	public ObjFromResource(int resID) {
		this.RES_ID = resID;
	}
	
	@Override
	public InputStreamReader GetObjReader(Activity ctx) throws FileNotFoundException {
		InputStreamReader res = new InputStreamReader(ctx.getResources().openRawResource(RES_ID));
		return res;
	}

}
