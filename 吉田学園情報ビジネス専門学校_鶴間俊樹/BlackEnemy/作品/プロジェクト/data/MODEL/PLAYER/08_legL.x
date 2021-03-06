xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 50;
 0.00000;0.00000;3.42200;,
 -1.30959;0.00000;3.16151;,
 0.00000;-22.06395;-0.00013;,
 -2.41981;0.00000;2.41968;,
 0.00000;-22.06395;-0.00013;,
 -3.16164;0.00000;1.30946;,
 0.00000;-22.06395;-0.00013;,
 -3.42213;0.00000;-0.00013;,
 0.00000;-22.06395;-0.00013;,
 -3.16164;0.00000;-1.30972;,
 0.00000;-22.06395;-0.00013;,
 -2.41981;0.00000;-2.41994;,
 0.00000;-22.06395;-0.00013;,
 -1.30959;0.00000;-3.16177;,
 0.00000;-22.06395;-0.00013;,
 -0.00000;0.00000;-3.42226;,
 0.00000;-22.06395;-0.00013;,
 1.30959;0.00000;-3.16177;,
 0.00000;-22.06395;-0.00013;,
 2.41981;0.00000;-2.41995;,
 0.00000;-22.06395;-0.00013;,
 3.16164;0.00000;-1.30972;,
 0.00000;-22.06395;-0.00013;,
 3.42213;0.00000;-0.00013;,
 0.00000;-22.06395;-0.00013;,
 3.16164;0.00000;1.30946;,
 0.00000;-22.06395;-0.00013;,
 2.41981;0.00000;2.41968;,
 0.00000;-22.06395;-0.00013;,
 1.30959;0.00000;3.16151;,
 0.00000;-22.06395;-0.00013;,
 0.00000;0.00000;3.42200;,
 0.00000;-22.06395;-0.00013;,
 0.00000;0.00000;-0.00013;,
 -1.30959;0.00000;3.16151;,
 0.00000;0.00000;3.42200;,
 -2.41981;0.00000;2.41968;,
 -3.16164;0.00000;1.30946;,
 -3.42213;0.00000;-0.00013;,
 -3.16164;0.00000;-1.30972;,
 -2.41981;0.00000;-2.41994;,
 -1.30959;0.00000;-3.16177;,
 -0.00000;0.00000;-3.42226;,
 1.30959;0.00000;-3.16177;,
 2.41981;0.00000;-2.41995;,
 3.16164;0.00000;-1.30972;,
 3.42213;0.00000;-0.00013;,
 3.16164;0.00000;1.30946;,
 2.41981;0.00000;2.41968;,
 1.30959;0.00000;3.16151;;
 
 32;
 3;0,1,2;,
 3;1,3,4;,
 3;3,5,6;,
 3;5,7,8;,
 3;7,9,10;,
 3;9,11,12;,
 3;11,13,14;,
 3;13,15,16;,
 3;15,17,18;,
 3;17,19,20;,
 3;19,21,22;,
 3;21,23,24;,
 3;23,25,26;,
 3;25,27,28;,
 3;27,29,30;,
 3;29,31,32;,
 3;33,34,35;,
 3;33,36,34;,
 3;33,37,36;,
 3;33,38,37;,
 3;33,39,38;,
 3;33,40,39;,
 3;33,41,40;,
 3;33,42,41;,
 3;33,43,42;,
 3;33,44,43;,
 3;33,45,44;,
 3;33,46,45;,
 3;33,47,46;,
 3;33,48,47;,
 3;33,49,48;,
 3;33,35,49;;
 
 MeshMaterialList {
  1;
  32;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  21;
  -0.000001;-0.153268;0.988185;,
  -0.378161;-0.153268;0.912964;,
  -0.698752;-0.153268;0.698752;,
  -0.912964;-0.153268;0.378161;,
  -0.988185;-0.153268;0.000000;,
  -0.912964;-0.153268;-0.378162;,
  -0.698752;-0.153268;-0.698752;,
  -0.378162;-0.153268;-0.912964;,
  -0.000000;-0.153268;-0.988185;,
  0.378162;-0.153268;-0.912964;,
  0.698752;-0.153268;-0.698752;,
  0.912964;-0.153268;-0.378161;,
  0.988185;-0.153268;-0.000000;,
  0.912964;-0.153268;0.378161;,
  0.698752;-0.153268;0.698752;,
  0.378161;-0.153268;0.912964;,
  -0.818537;-0.175687;0.546929;,
  0.000000;1.000000;0.000000;,
  -0.192056;-0.175687;-0.965530;,
  0.965530;-0.175687;0.192056;,
  0.192870;-0.150390;0.969631;;
  32;
  3;0,1,16;,
  3;1,2,16;,
  3;2,3,16;,
  3;3,4,16;,
  3;4,5,16;,
  3;5,6,18;,
  3;6,7,18;,
  3;7,8,18;,
  3;8,9,18;,
  3;9,10,18;,
  3;10,11,19;,
  3;11,12,19;,
  3;12,13,19;,
  3;13,14,19;,
  3;14,15,19;,
  3;15,0,20;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;;
 }
 MeshTextureCoords {
  50;
  0.000000;1.000000;,
  0.062500;1.000000;,
  0.031250;0.000000;,
  0.125000;1.000000;,
  0.062500;0.000000;,
  0.187500;1.000000;,
  0.093750;0.000000;,
  0.250000;1.000000;,
  0.125000;0.000000;,
  0.312500;1.000000;,
  0.156250;0.000000;,
  0.375000;1.000000;,
  0.187500;0.000000;,
  0.437500;1.000000;,
  0.218750;0.000000;,
  0.500000;1.000000;,
  0.250000;0.000000;,
  0.562500;1.000000;,
  0.281250;0.000000;,
  0.625000;1.000000;,
  0.312500;0.000000;,
  0.687500;1.000000;,
  0.343750;0.000000;,
  0.750000;1.000000;,
  0.375000;0.000000;,
  0.812500;1.000000;,
  0.406250;0.000000;,
  0.875000;1.000000;,
  0.437500;0.000000;,
  0.937500;1.000000;,
  0.468750;0.000000;,
  1.000000;1.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
