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
 153;
 0.00000;22.56687;-0.02233;,
 0.57607;22.56687;-1.67356;,
 0.00000;22.56687;-1.92900;,
 0.99778;22.56687;-0.97567;,
 1.15214;22.56687;-0.02233;,
 0.99778;22.56687;0.93102;,
 0.57607;22.56687;1.62890;,
 0.00000;22.56687;1.88435;,
 -0.57607;22.56687;1.62890;,
 -0.99778;22.56687;0.93102;,
 -1.15214;22.56687;-0.02232;,
 -0.99778;22.56687;-0.97567;,
 -0.57607;22.56687;-1.67356;,
 0.00000;21.91272;-1.96391;,
 0.58661;21.91272;-1.70379;,
 0.62312;20.33342;-1.80841;,
 0.00000;20.33342;-2.08472;,
 1.01605;21.91272;-0.99312;,
 1.07927;20.33342;-1.05352;,
 1.17323;21.91272;-0.02233;,
 1.24623;20.33342;-0.02233;,
 1.01605;21.91272;0.94846;,
 1.07927;20.33342;1.00887;,
 0.58661;21.91272;1.65913;,
 0.62312;20.33342;1.76376;,
 0.00000;21.91272;1.91925;,
 0.00000;20.33342;2.04007;,
 -0.58661;21.91272;1.65913;,
 -0.62312;20.33342;1.76376;,
 -1.01605;21.91272;0.94847;,
 -1.07927;20.33342;1.00887;,
 -1.17323;21.91272;-0.02232;,
 -1.24623;20.33342;-0.02232;,
 -1.01605;21.91272;-0.99311;,
 -1.07927;20.33342;-1.05352;,
 -0.58661;21.91272;-1.70379;,
 -0.62312;20.33342;-1.80841;,
 0.62312;2.46583;-1.80841;,
 0.00000;2.46583;-2.08472;,
 1.07927;2.46583;-1.05352;,
 1.24623;2.46583;-0.02233;,
 1.07927;2.46583;1.00887;,
 0.62312;2.46583;1.76376;,
 0.00000;2.46583;2.04007;,
 -0.62312;2.46583;1.76376;,
 -1.07927;2.46583;1.00887;,
 -1.24623;2.46583;-0.02232;,
 -1.07927;2.46583;-1.05352;,
 -0.62312;2.46583;-1.80841;,
 0.58661;0.88654;-1.70379;,
 0.00000;0.88654;-1.96391;,
 1.01605;0.88654;-0.99312;,
 1.17323;0.88654;-0.02233;,
 1.01605;0.88654;0.94846;,
 0.58661;0.88654;1.65913;,
 0.00000;0.88654;1.91925;,
 -0.58661;0.88654;1.65913;,
 -1.01605;0.88654;0.94847;,
 -1.17323;0.88654;-0.02232;,
 -1.01605;0.88654;-0.99311;,
 -0.58661;0.88654;-1.70379;,
 0.49849;0.23239;-1.45120;,
 0.00000;0.23239;-1.67224;,
 0.86342;0.23239;-0.84728;,
 0.99698;0.23239;-0.02233;,
 0.86342;0.23239;0.80263;,
 0.49849;0.23239;1.40654;,
 0.00000;0.23239;1.62759;,
 -0.49849;0.23239;1.40654;,
 -0.86342;0.23239;0.80263;,
 -0.99698;0.23239;-0.02232;,
 -0.86342;0.23239;-0.84728;,
 -0.49849;0.23239;-1.45119;,
 0.00000;0.23239;-0.02233;,
 1.58320;22.61112;-2.84644;,
 0.00000;22.61678;-3.38957;,
 0.00000;21.96263;-3.43092;,
 1.59687;21.95696;-2.88226;,
 2.51599;22.60246;-1.56242;,
 2.53966;21.94830;-1.58309;,
 2.80326;22.59946;-0.02233;,
 2.83060;21.94530;-0.02233;,
 2.51599;22.60246;1.51777;,
 2.53966;21.94830;1.53844;,
 1.58320;22.61112;2.80179;,
 1.59687;21.95696;2.83761;,
 0.00000;22.61678;3.34492;,
 0.00000;21.96263;3.38627;,
 -1.58320;22.61112;2.80179;,
 -1.59687;21.95696;2.83761;,
 -2.51599;22.60246;1.51777;,
 -2.53966;21.94830;1.53844;,
 -2.80326;22.59946;-0.02232;,
 -2.83060;21.94530;-0.02232;,
 -2.51599;22.60246;-1.56242;,
 -2.53966;21.94830;-1.58309;,
 -1.58320;22.61112;-2.84644;,
 -1.59687;21.95696;-2.88226;,
 0.63131;37.91396;-0.67218;,
 0.63131;37.91396;1.53517;,
 0.63131;22.47982;1.36964;,
 0.63131;22.47982;-0.83771;,
 0.63131;37.91396;1.53517;,
 -0.63131;37.91396;1.53517;,
 -0.63131;22.47982;1.36964;,
 0.63131;22.47982;1.36964;,
 -0.63131;37.91396;1.53517;,
 -0.63131;37.91396;-0.67218;,
 -0.63131;22.47982;-0.83771;,
 -0.63131;22.47982;1.36964;,
 0.63131;22.47982;-0.83771;,
 0.63131;22.47982;1.36964;,
 -0.63131;22.47982;-0.83771;,
 0.00000;22.47982;-1.62310;,
 0.00000;37.91396;-1.45757;,
 0.63131;37.91396;-0.67218;,
 0.63131;22.47982;-0.83771;,
 0.00000;37.91396;-1.45757;,
 0.00000;22.47982;-1.62310;,
 -0.63131;37.91396;-0.67218;,
 0.00000;22.47982;-1.62310;,
 -0.51199;66.20508;1.80945;,
 -0.52492;66.20508;-0.17239;,
 0.51199;66.20508;1.80945;,
 0.63131;37.91396;1.53517;,
 0.52492;66.20508;-0.17239;,
 0.63131;37.91396;-0.67218;,
 0.00000;66.20508;-0.97793;,
 0.00000;37.91396;-1.45757;,
 0.52492;66.20508;-0.17239;,
 -0.38723;77.51383;2.33999;,
 -0.41524;77.51383;0.62566;,
 0.38723;77.51383;2.33999;,
 0.41524;77.51383;0.62564;,
 0.00000;77.51383;-0.03235;,
 0.41524;77.51383;0.62564;,
 -0.24841;78.80205;2.33999;,
 -0.21283;78.80205;1.39789;,
 0.24841;78.80205;2.33999;,
 0.21283;78.80205;1.39789;,
 0.00000;78.80205;0.84776;,
 0.21283;78.80205;1.39789;,
 -0.10022;79.38976;2.33999;,
 -0.09678;79.38976;2.09570;,
 0.10022;79.38976;2.33999;,
 0.09678;79.38976;2.09570;,
 0.00000;79.38976;1.74117;,
 0.09678;79.38976;2.09570;,
 0.00000;79.69646;2.34012;,
 0.00000;79.69646;2.34012;,
 0.00000;79.69646;2.34012;,
 0.00000;79.69646;2.34012;,
 0.00000;79.69646;2.34012;;
 
 140;
 3;0,1,2;,
 3;0,3,1;,
 3;0,4,3;,
 3;0,5,4;,
 3;0,6,5;,
 3;0,7,6;,
 3;0,8,7;,
 3;0,9,8;,
 3;0,10,9;,
 3;0,11,10;,
 3;0,12,11;,
 3;0,2,12;,
 4;13,14,15,16;,
 4;14,17,18,15;,
 4;17,19,20,18;,
 4;19,21,22,20;,
 4;21,23,24,22;,
 4;23,25,26,24;,
 4;25,27,28,26;,
 4;27,29,30,28;,
 4;29,31,32,30;,
 4;31,33,34,32;,
 4;33,35,36,34;,
 4;35,13,16,36;,
 4;16,15,37,38;,
 4;15,18,39,37;,
 4;18,20,40,39;,
 4;20,22,41,40;,
 4;22,24,42,41;,
 4;24,26,43,42;,
 4;26,28,44,43;,
 4;28,30,45,44;,
 4;30,32,46,45;,
 4;32,34,47,46;,
 4;34,36,48,47;,
 4;36,16,38,48;,
 4;38,37,49,50;,
 4;37,39,51,49;,
 4;39,40,52,51;,
 4;40,41,53,52;,
 4;41,42,54,53;,
 4;42,43,55,54;,
 4;43,44,56,55;,
 4;44,45,57,56;,
 4;45,46,58,57;,
 4;46,47,59,58;,
 4;47,48,60,59;,
 4;48,38,50,60;,
 4;50,49,61,62;,
 4;49,51,63,61;,
 4;51,52,64,63;,
 4;52,53,65,64;,
 4;53,54,66,65;,
 4;54,55,67,66;,
 4;55,56,68,67;,
 4;56,57,69,68;,
 4;57,58,70,69;,
 4;58,59,71,70;,
 4;59,60,72,71;,
 4;60,50,62,72;,
 3;62,61,73;,
 3;61,63,73;,
 3;63,64,73;,
 3;64,65,73;,
 3;65,66,73;,
 3;66,67,73;,
 3;67,68,73;,
 3;68,69,73;,
 3;69,70,73;,
 3;70,71,73;,
 3;71,72,73;,
 3;72,62,73;,
 4;74,75,2,1;,
 4;76,77,14,13;,
 4;75,74,77,76;,
 4;78,74,1,3;,
 4;77,79,17,14;,
 4;74,78,79,77;,
 4;80,78,3,4;,
 4;79,81,19,17;,
 4;78,80,81,79;,
 4;82,80,4,5;,
 4;81,83,21,19;,
 4;80,82,83,81;,
 4;84,82,5,6;,
 4;83,85,23,21;,
 4;82,84,85,83;,
 4;86,84,6,7;,
 4;85,87,25,23;,
 4;84,86,87,85;,
 4;88,86,7,8;,
 4;87,89,27,25;,
 4;86,88,89,87;,
 4;90,88,8,9;,
 4;89,91,29,27;,
 4;88,90,91,89;,
 4;92,90,9,10;,
 4;91,93,31,29;,
 4;90,92,93,91;,
 4;94,92,10,11;,
 4;93,95,33,31;,
 4;92,94,95,93;,
 4;96,94,11,12;,
 4;95,97,35,33;,
 4;94,96,97,95;,
 4;75,96,12,2;,
 4;97,76,13,35;,
 4;96,75,76,97;,
 4;98,99,100,101;,
 4;102,103,104,105;,
 4;106,107,108,109;,
 4;110,111,104,112;,
 4;113,114,115,116;,
 4;117,118,112,119;,
 3;120,116,112;,
 4;121,122,119,103;,
 4;123,121,103,124;,
 4;125,123,124,126;,
 4;122,127,128,119;,
 4;127,129,115,128;,
 4;130,131,122,121;,
 4;132,130,121,123;,
 4;133,132,123,125;,
 4;131,134,127,122;,
 4;134,135,129,127;,
 4;136,137,131,130;,
 4;138,136,130,132;,
 4;139,138,132,133;,
 4;137,140,134,131;,
 4;140,141,135,134;,
 4;142,143,137,136;,
 4;144,142,136,138;,
 4;145,144,138,139;,
 4;143,146,140,137;,
 4;146,147,141,140;,
 3;148,144,145;,
 3;149,142,144;,
 3;150,143,142;,
 3;151,146,143;,
 3;152,147,146;;
 
 MeshMaterialList {
  3;
  140;
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
  1,
  1,
  1,
  1,
  2,
  2,
  1,
  1,
  1,
  1,
  2,
  2,
  1,
  1,
  1,
  2,
  2,
  1,
  1,
  1,
  2,
  2,
  1,
  1,
  1,
  2,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.401600;0.062400;0.050400;1.000000;;
   61.000000;
   0.400000;0.400000;0.400000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.109600;0.009600;0.012800;1.000000;;
   77.000000;
   0.710000;0.710000;0.710000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  156;
  0.000000;1.000000;0.000000;,
  0.000000;0.999882;0.015367;,
  -0.007350;0.999904;0.011736;,
  -0.009929;0.999937;0.005189;,
  -0.010224;0.999948;0.000000;,
  -0.009929;0.999937;-0.005189;,
  -0.007350;0.999904;-0.011736;,
  -0.000000;0.999882;-0.015367;,
  0.007350;0.999904;-0.011735;,
  0.009929;0.999937;-0.005188;,
  0.010224;0.999948;-0.000000;,
  0.009929;0.999937;0.005189;,
  0.007350;0.999904;0.011736;,
  0.000000;0.076273;-0.997087;,
  0.659844;0.064843;-0.748600;,
  0.937480;0.050699;-0.344328;,
  0.998933;0.046175;-0.000000;,
  0.937480;0.050700;0.344328;,
  0.659843;0.064847;0.748600;,
  0.000000;0.076278;0.997087;,
  -0.659843;0.064846;0.748600;,
  -0.937479;0.050699;0.344329;,
  -0.998933;0.046175;0.000000;,
  -0.937480;0.050700;-0.344327;,
  -0.659844;0.064844;-0.748599;,
  0.000000;0.038173;-0.999271;,
  0.660829;0.032441;-0.749835;,
  0.938378;0.025358;-0.344679;,
  0.999733;0.023094;-0.000000;,
  0.938378;0.025359;0.344679;,
  0.660829;0.032443;0.749835;,
  0.000000;0.038176;0.999271;,
  -0.660829;0.032443;0.749835;,
  -0.938378;0.025358;0.344679;,
  -0.999733;0.023094;0.000000;,
  -0.938378;0.025358;-0.344678;,
  -0.660830;0.032442;-0.749834;,
  0.000000;-0.038174;-0.999271;,
  0.660829;-0.032441;-0.749835;,
  0.938378;-0.025358;-0.344679;,
  0.999733;-0.023094;-0.000000;,
  0.938378;-0.025359;0.344678;,
  0.660829;-0.032443;0.749835;,
  0.000000;-0.038176;0.999271;,
  -0.660829;-0.032443;0.749835;,
  -0.938378;-0.025358;0.344679;,
  -0.999733;-0.023094;0.000000;,
  -0.938378;-0.025358;-0.344678;,
  -0.660830;-0.032442;-0.749834;,
  -0.000001;-0.246471;-0.969150;,
  0.647816;-0.211896;-0.731734;,
  0.925488;-0.168389;-0.339289;,
  0.988053;-0.154113;-0.000000;,
  0.925488;-0.168389;0.339289;,
  0.647817;-0.211896;0.731733;,
  0.000000;-0.246470;0.969151;,
  -0.647816;-0.211897;0.731734;,
  -0.925487;-0.168389;0.339291;,
  -0.988053;-0.154113;0.000000;,
  -0.925488;-0.168389;-0.339288;,
  -0.647817;-0.211898;-0.731732;,
  -0.000002;-0.407227;-0.913327;,
  0.621658;-0.353622;-0.698922;,
  0.900558;-0.283585;-0.329507;,
  0.965568;-0.260149;-0.000000;,
  0.900558;-0.283583;0.329508;,
  0.621660;-0.353618;0.698922;,
  0.000000;-0.407221;0.913329;,
  -0.621659;-0.353620;0.698923;,
  -0.900557;-0.283585;0.329509;,
  -0.965568;-0.260149;0.000000;,
  -0.900558;-0.283584;-0.329507;,
  -0.621660;-0.353624;-0.698920;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.999528;0.030731;,
  -0.014699;0.999617;0.023470;,
  -0.019858;0.999749;0.010377;,
  -0.020446;0.999791;0.000000;,
  -0.019858;0.999749;-0.010377;,
  -0.014699;0.999617;-0.023470;,
  -0.000001;0.999528;-0.030731;,
  0.014698;0.999617;-0.023469;,
  0.019858;0.999749;-0.010376;,
  0.020446;0.999791;-0.000000;,
  0.019858;0.999749;0.010377;,
  0.014699;0.999617;0.023470;,
  0.000000;-0.999532;-0.030599;,
  0.014655;-0.999620;-0.023362;,
  0.019791;-0.999751;-0.010322;,
  0.020370;-0.999793;-0.000000;,
  0.019791;-0.999751;0.010322;,
  0.014655;-0.999620;0.023362;,
  -0.000000;-0.999532;0.030599;,
  -0.014655;-0.999620;0.023362;,
  -0.019791;-0.999751;0.010322;,
  -0.020370;-0.999793;0.000000;,
  -0.019791;-0.999751;-0.010322;,
  -0.014655;-0.999620;-0.023362;,
  0.000000;0.062450;-0.998048;,
  0.593899;0.056350;-0.802564;,
  0.917749;0.045920;-0.394497;,
  0.999122;0.041900;-0.000000;,
  0.917749;0.045923;0.394497;,
  0.593899;0.056351;0.802564;,
  -0.000000;0.062444;0.998048;,
  -0.593899;0.056348;0.802564;,
  -0.917749;0.045921;0.394498;,
  -0.999122;0.041900;0.000000;,
  -0.917749;0.045919;-0.394497;,
  -0.593899;0.056350;-0.802564;,
  0.947565;0.005914;-0.319508;,
  0.943238;0.003562;-0.332097;,
  0.999997;0.001979;0.001631;,
  1.000000;0.000000;0.000000;,
  0.000000;-0.010209;0.999948;,
  0.000000;-0.010724;0.999942;,
  -0.947565;0.005914;-0.319508;,
  -0.943238;0.003562;-0.332097;,
  0.794728;0.009224;-0.606896;,
  0.779398;0.006719;-0.626493;,
  0.956469;0.018640;-0.291238;,
  0.000000;-0.028283;0.999600;,
  -0.999950;0.006854;0.007346;,
  -0.956469;0.018640;-0.291237;,
  -0.825682;0.028732;-0.563403;,
  0.958312;0.137065;-0.250702;,
  0.000000;-0.023438;0.999725;,
  -0.997669;0.068230;0.000371;,
  -0.958313;0.137063;-0.250699;,
  -0.850942;0.197006;-0.486915;,
  0.934637;0.301617;-0.188362;,
  0.000000;0.000000;1.000000;,
  -0.982855;0.183506;-0.017961;,
  -0.934638;0.301616;-0.188361;,
  -0.843750;0.406339;-0.350677;,
  0.917180;0.373412;-0.139087;,
  0.000000;-0.000212;1.000000;,
  -0.961122;0.275448;-0.019294;,
  -0.917180;0.373412;-0.139087;,
  -0.849537;0.461676;-0.255231;,
  0.913217;0.387817;-0.125031;,
  -0.999997;0.001979;0.001631;,
  -1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.794728;0.009224;-0.606896;,
  -0.779398;0.006719;-0.626493;,
  0.999950;0.006854;0.007346;,
  0.825681;0.028732;-0.563405;,
  0.997669;0.068230;0.000371;,
  0.850940;0.197008;-0.486919;,
  0.982855;0.183506;-0.017961;,
  0.843748;0.406341;-0.350679;,
  0.961122;0.275448;-0.019294;,
  0.849537;0.461676;-0.255231;,
  -0.000000;-0.000424;1.000000;,
  -0.913217;0.387817;-0.125031;;
  140;
  3;0,2,1;,
  3;0,3,2;,
  3;0,4,3;,
  3;0,5,4;,
  3;0,6,5;,
  3;0,7,6;,
  3;0,8,7;,
  3;0,9,8;,
  3;0,10,9;,
  3;0,11,10;,
  3;0,12,11;,
  3;0,1,12;,
  4;13,14,26,25;,
  4;14,15,27,26;,
  4;15,16,28,27;,
  4;16,17,29,28;,
  4;17,18,30,29;,
  4;18,19,31,30;,
  4;19,20,32,31;,
  4;20,21,33,32;,
  4;21,22,34,33;,
  4;22,23,35,34;,
  4;23,24,36,35;,
  4;24,13,25,36;,
  4;25,26,38,37;,
  4;26,27,39,38;,
  4;27,28,40,39;,
  4;28,29,41,40;,
  4;29,30,42,41;,
  4;30,31,43,42;,
  4;31,32,44,43;,
  4;32,33,45,44;,
  4;33,34,46,45;,
  4;34,35,47,46;,
  4;35,36,48,47;,
  4;36,25,37,48;,
  4;37,38,50,49;,
  4;38,39,51,50;,
  4;39,40,52,51;,
  4;40,41,53,52;,
  4;41,42,54,53;,
  4;42,43,55,54;,
  4;43,44,56,55;,
  4;44,45,57,56;,
  4;45,46,58,57;,
  4;46,47,59,58;,
  4;47,48,60,59;,
  4;48,37,49,60;,
  4;49,50,62,61;,
  4;50,51,63,62;,
  4;51,52,64,63;,
  4;52,53,65,64;,
  4;53,54,66,65;,
  4;54,55,67,66;,
  4;55,56,68,67;,
  4;56,57,69,68;,
  4;57,58,70,69;,
  4;58,59,71,70;,
  4;59,60,72,71;,
  4;60,49,61,72;,
  3;73,73,73;,
  3;73,73,73;,
  3;73,73,73;,
  3;73,73,73;,
  3;73,73,73;,
  3;73,73,73;,
  3;73,73,73;,
  3;73,73,73;,
  3;73,73,73;,
  3;73,73,73;,
  3;73,73,73;,
  3;73,73,73;,
  4;75,74,1,2;,
  4;86,87,87,86;,
  4;98,99,99,98;,
  4;76,75,2,3;,
  4;87,88,88,87;,
  4;99,100,100,99;,
  4;77,76,3,4;,
  4;88,89,89,88;,
  4;100,101,101,100;,
  4;78,77,4,5;,
  4;89,90,90,89;,
  4;101,102,102,101;,
  4;79,78,5,6;,
  4;90,91,91,90;,
  4;102,103,103,102;,
  4;80,79,6,7;,
  4;91,92,92,91;,
  4;103,104,104,103;,
  4;81,80,7,8;,
  4;92,93,93,92;,
  4;104,105,105,104;,
  4;82,81,8,9;,
  4;93,94,94,93;,
  4;105,106,106,105;,
  4;83,82,9,10;,
  4;94,95,95,94;,
  4;106,107,107,106;,
  4;84,83,10,11;,
  4;95,96,96,95;,
  4;107,108,108,107;,
  4;85,84,11,12;,
  4;96,97,97,96;,
  4;108,109,109,108;,
  4;74,85,12,1;,
  4;97,86,86,97;,
  4;109,98,98,109;,
  4;110,112,113,111;,
  4;114,114,115,115;,
  4;141,116,117,142;,
  4;143,143,143,143;,
  4;119,118,110,111;,
  4;144,145,117,116;,
  3;143,143,143;,
  4;122,123,116,141;,
  4;121,121,114,114;,
  4;120,146,112,110;,
  4;123,124,144,116;,
  4;147,120,110,118;,
  4;127,128,123,122;,
  4;126,126,121,121;,
  4;125,148,146,120;,
  4;128,129,124,123;,
  4;149,125,120,147;,
  4;132,133,128,127;,
  4;131,131,126,126;,
  4;130,150,148,125;,
  4;133,134,129,128;,
  4;151,130,125,149;,
  4;137,138,133,132;,
  4;136,136,131,131;,
  4;135,152,150,130;,
  4;138,139,134,133;,
  4;153,135,130,151;,
  3;140,152,135;,
  3;154,136,136;,
  3;155,138,137;,
  3;155,139,138;,
  3;140,135,153;;
 }
 MeshTextureCoords {
  153;
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
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.500000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  0.500000;1.000000;,
  1.000000;0.500000;,
  0.500000;0.000000;,
  0.250000;0.000000;,
  0.750000;0.000000;;
 }
}
