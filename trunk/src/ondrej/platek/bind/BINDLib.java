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
     int width;
     int height;
	 float[] vertexes;
	 
     public BINDLib(float[] Vertexes,int VertexesSize, int screenWidth, int screenHeight){
    	 vertexes = Vertexes;
    	 vertexes_size = VertexesSize;
    	 width = screenWidth;
    	 height = screenHeight;
    	 
    	 // TODO could it be loaded in static constructor? It would be better
         System.loadLibrary("gl_code");
         init();
     }
     @Override
     protected void finalize(){
    	releaseCppResources(); 
     }
	 
    /**
     * @param width the current view width
     * @param height the current view height
     */
     native void init();
     native void releaseCppResources();
     public native void step();
}
