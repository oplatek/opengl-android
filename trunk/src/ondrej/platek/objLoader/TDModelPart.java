package ondrej.platek.objLoader;

import java.util.Vector;

public class TDModelPart {
	Vector<Short> faces;
	Vector<Short> vtPointer;
	Vector<Short> vnPointer;
	Material material;
	float[] normals;
	
	public TDModelPart(Vector<Short> faces, Vector<Short> vtPointer,
			Vector<Short> vnPointer, Material material, Vector<Float> vn) {
		super();
		this.faces = faces;
		this.vtPointer = vtPointer;
		this.vnPointer = vnPointer;
		this.material = material;
		normals = new float[ vn.size()];
		for(int i=0; i<vn.size(); i++){
			normals[i] = vn.get(i);
		}
	}
	public String toString(){
		String str=new String();
		if(material!=null)
			str+="Material name:"+material.getName();
		else
			str+="Material not defined!";
		str+="\nNumber of faces:"+faces.size();
		str+="\nNumber of vnPointers:"+vnPointer.size();
		str+="\nNumber of vtPointers:"+vtPointer.size();
		return str;
	}
	
	private static short[] toPrimitiveArrayS(Vector<Short> vector){
		short[] s;
		s=new short[vector.size()];
		for (int i=0; i<vector.size(); i++){
			s[i]=vector.get(i);
		}
		return s;
	}
	public short[] getFaces() {
		return toPrimitiveArrayS(vnPointer);
	}
	public int getFacesCount(){
		return faces.size();
	}
	public Material getMaterial(){
		return material;
	}
	public float[] getNormals() {
		return normals;		
	}
	
	
}
