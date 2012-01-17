package ondrej.platek.objLoader;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.Vector;


public class TDModel {
    Vector<Float> v;
    Vector<Float> vn;
    Vector<Float> vt;
    Vector<TDModelPart> parts;
    FloatBuffer vertexBuffer;

    public TDModel(Vector<Float> v, Vector<Float> vn, Vector<Float> vt,
            Vector<TDModelPart> parts) {
        super();
        this.v = v;
        this.vn = vn;
        this.vt = vt;
        this.parts = parts;
    }

    public String toString(){
        String str=new String();
        str+="Number of parts: "+parts.size();
        str+="\nNumber of vertexes: "+v.size();
        str+="\nNumber of vns: "+vn.size();
        str+="\nNumber of vts: "+vt.size();
        str+="\n/////////////////////////\n";
        for(int i=0; i<parts.size(); i++){
            str+="Part "+i+'\n';
            str+=parts.get(i).toString();
            str+="\n/////////////////////////";
        }
        return str;
    }

    public int getVertexNumber(){
        return this.v.size();
    }

    public float[] GetVertexArr() {
        return toPrimitiveArrayF(v);
    }

    public float[] getNormalsArr() {
        return toPrimitiveArrayF(vn);
    }
    public int getNormalsNumber(){
        return this.vn.size();
    }

    public void buildVertexBuffer(){
        ByteBuffer vBuf = ByteBuffer.allocateDirect(v.size() * 4);
        vBuf.order(ByteOrder.nativeOrder());
        vertexBuffer = vBuf.asFloatBuffer();
        vertexBuffer.put(toPrimitiveArrayF(v));
        vertexBuffer.position(0);
    }

    private static float[] toPrimitiveArrayF(Vector<Float> vector){
        float[] f;
        f=new float[vector.size()];
        for (int i=0; i<vector.size(); i++){
            f[i]=vector.get(i);
        }
        return f;
    }

    public int getPartsNumber() {
        return parts.size();
    }
 
    public int[] getPartsSizes() {
        int[] sizes = new int[getPartsNumber()];
        for(int i = 0; i < getPartsNumber(); i++) {
            sizes[i] = parts.get(i).getFacesCount();
        }
        return sizes;
    }

    public short[][] getNormalsPointer() {
        short[][] n = new short[getPartsNumber()][];
        for(int i = 0; i < getPartsNumber(); ++i) {
            n[i] = parts.get(i).getNormalsPointer();
        }
        return n;
    }

    public short[][] getFaces() {
        short[][] f = new short[getPartsNumber()][];
        for(int i = 0; i < getPartsNumber(); ++i) {
            f[i] = parts.get(i).getFaces();
        }
        return f;
    }
}


