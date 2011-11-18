/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package ondrej.platek.bind;

// Wrapper for native library

public class BINDLib {
     int vertexes_size = 0;
	 float[] vertexes;
	 
     public BINDLib(){
         System.loadLibrary("gl_code");
     }
     
	 public void setVertexes(float[] vertexes2,int vertSize) {
		 vertexes_size = vertSize;
		 vertexes = vertexes2;
	 }
	 
	 public void updateVertices() {
		 loadOBJ2GL();
	 }
	 
	 @Override
	 protected void finalize() {
		 releaseCppResources();
	 }
	 
    /**
     * @param width the current view width
     * @param height the current view height
     */
     public native void init(int width, int height);
     public native void step();
     native void loadOBJ2GL();
     public native void releaseCppResources();
}
