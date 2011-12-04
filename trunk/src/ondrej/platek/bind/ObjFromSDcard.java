package ondrej.platek.bind;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStreamReader;

public class ObjFromSDcard extends ObjSource {
	private String path;
	public ObjFromSDcard(String path) {
		this.path = path;
	}

	@Override
	public InputStreamReader GetObjReader() throws FileNotFoundException {
		return new InputStreamReader(new FileInputStream(this.path));
	}

}
