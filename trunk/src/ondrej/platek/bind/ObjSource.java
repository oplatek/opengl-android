package ondrej.platek.bind;

import java.io.FileNotFoundException;
import java.io.InputStreamReader;

public interface ObjSource {
	public InputStreamReader GetObjReader() throws FileNotFoundException;
}
