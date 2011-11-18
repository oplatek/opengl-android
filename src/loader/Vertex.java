package loader;

	// Compiled from Vertex.java (version 1.6 : 50.0, super bit)
    //loader.Vertex
public class Vertex {
	  
	  // Field descriptor #6 F
	  private float x;
	  
	  // Field descriptor #6 F
	  private float y;
	  
	  // Field descriptor #6 F
	  private float z;
	  
	  // Method descriptor #10 (FFF)V
	  // Stack: 2, Locals: 4
	  public Vertex(float x, float y, float z) {
		  this.x = x;
		  this.y = y;
		  this.z = z;
	  }
	  public Vertex() { } 
	  
	  public float getX() {
		  return this.x;
	  }
	  
	  public void setX(float x) {
		  this.x = x;
	  }
	  
	  public float getY() {
		  return this.y; 
	  }
	  
	  public void setY(float y) {
		  this.y = y;
	  }
	  
	  public float getZ() {
		  return this.z;
	  }
	  
	  public void setZ(float z) {
		  this.z = z;
	  }
	  
	  public double norm() {
		 return Math.sqrt( Math.pow(this.x,2) + Math.pow(this.y,2) + Math.pow(this.z,2) );
	  }
}

