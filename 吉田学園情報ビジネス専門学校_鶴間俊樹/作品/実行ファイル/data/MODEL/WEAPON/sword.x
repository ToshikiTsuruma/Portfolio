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
 205;
 -1.63576;10.15066;-0.47959;,
 -1.31374;11.21367;-0.82984;,
 -1.31361;7.81286;-0.83011;,
 -1.63576;7.62704;-0.47959;,
 -0.00014;11.21367;-1.23072;,
 -0.00000;7.81286;-1.23098;,
 1.31347;11.21367;-0.82984;,
 1.31359;7.81286;-0.83011;,
 1.63575;10.15066;-0.47959;,
 1.63575;7.62704;-0.47959;,
 -1.31361;4.07480;-0.83011;,
 -1.63576;4.26070;-0.47959;,
 -0.00000;4.07480;-1.23098;,
 1.31359;4.07480;-0.83011;,
 1.63575;4.26070;-0.47959;,
 -1.31361;0.33680;-0.83011;,
 -1.63576;0.89436;-0.47959;,
 -0.00000;0.33680;-1.23098;,
 1.31359;0.33680;-0.83011;,
 1.63575;0.89436;-0.47959;,
 1.63575;10.15066;-0.47959;,
 1.63575;10.15066;0.00000;,
 1.63575;7.62704;0.00000;,
 1.63575;7.62704;-0.47959;,
 1.63575;10.15066;0.47959;,
 1.63575;7.62704;0.47959;,
 1.63575;4.26070;0.00000;,
 1.63575;4.26070;-0.47959;,
 1.63575;4.26070;0.47959;,
 1.63575;0.89436;0.00000;,
 1.63575;0.89436;-0.47959;,
 1.63575;0.89436;0.47959;,
 1.63575;10.15066;0.47959;,
 1.31347;11.21367;0.83037;,
 1.31359;7.81286;0.83011;,
 1.63575;7.62704;0.47959;,
 -0.00014;11.21367;1.23124;,
 -0.00000;7.81286;1.23098;,
 -1.31374;11.21367;0.83037;,
 -1.31361;7.81286;0.83011;,
 -1.63576;10.15066;0.47959;,
 -1.63576;7.62704;0.47959;,
 1.31359;4.07480;0.83011;,
 1.63575;4.26070;0.47959;,
 -0.00000;4.07480;1.23098;,
 -1.31361;4.07480;0.83011;,
 -1.63576;4.26070;0.47959;,
 1.31359;0.33680;0.83011;,
 1.63575;0.89436;0.47959;,
 -0.00000;0.33680;1.23098;,
 -1.31361;0.33680;0.83011;,
 -1.63576;0.89436;0.47959;,
 -1.63576;10.15066;0.47959;,
 -1.63576;10.15066;0.00000;,
 -1.63576;7.62704;0.00000;,
 -1.63576;7.62704;0.47959;,
 -1.63576;10.15066;-0.47959;,
 -1.63576;7.62704;-0.47959;,
 -1.63576;4.26070;0.00000;,
 -1.63576;4.26070;0.47959;,
 -1.63576;4.26070;-0.47959;,
 -1.63576;0.89436;0.00000;,
 -1.63576;0.89436;0.47959;,
 -1.63576;0.89436;-0.47959;,
 -1.31374;11.21367;0.83037;,
 -1.31374;11.21367;0.00026;,
 -1.63576;10.15066;0.00000;,
 -0.00014;11.21367;0.00026;,
 1.31347;11.21367;0.83037;,
 1.31347;11.21367;0.00026;,
 1.63575;10.15066;0.00000;,
 -1.31374;11.21367;-0.82984;,
 -1.63576;10.15066;-0.47959;,
 -0.00014;11.21367;-1.23072;,
 1.31347;11.21367;-0.82984;,
 1.63575;10.15066;-0.47959;,
 -1.63576;0.89436;0.00000;,
 -1.31361;0.33680;0.00000;,
 -1.31361;0.33680;0.83011;,
 -0.00000;-0.00045;0.00000;,
 1.31359;0.33680;0.00000;,
 1.31359;0.33680;0.83011;,
 1.63575;0.89436;0.00000;,
 -1.63576;0.89436;-0.47959;,
 -1.31361;0.33680;-0.83011;,
 -0.00000;0.33680;-1.23098;,
 1.31359;0.33680;-0.83011;,
 1.63575;0.89436;-0.47959;,
 -1.32001;12.58900;-0.96890;,
 -0.00000;12.58900;-1.32001;,
 -0.00000;11.21367;-1.23075;,
 -1.31374;11.21367;-0.82984;,
 1.32000;12.58900;-0.96890;,
 1.31373;11.21367;-0.82984;,
 1.32000;12.58900;0.96890;,
 -0.00000;12.58900;1.32001;,
 -0.00000;11.21367;1.23131;,
 1.31373;11.21367;0.83097;,
 -1.32001;12.58900;0.96890;,
 -1.31654;11.21367;0.83041;,
 -1.32001;12.58900;0.96890;,
 -0.00000;12.58900;0.00000;,
 -1.32001;12.58900;0.00000;,
 1.32000;12.58900;0.96890;,
 1.32000;12.58900;0.00000;,
 -0.00000;12.58900;-1.32001;,
 -1.32001;12.58900;-0.96890;,
 1.32000;12.58900;-0.96890;,
 -1.31374;11.21367;0.00000;,
 -0.00000;11.21367;0.00000;,
 -1.31654;11.21367;0.83041;,
 1.31373;11.21367;0.00000;,
 1.31373;11.21367;0.83097;,
 -1.31374;11.21367;-0.82984;,
 -0.00000;11.21367;-1.23075;,
 1.31373;11.21367;-0.82984;,
 4.27589;12.57556;0.00000;,
 4.27589;12.57556;-0.96890;,
 1.32000;12.58900;-0.96890;,
 1.32000;12.58900;0.00000;,
 4.26963;11.20016;-0.82984;,
 4.26963;11.20016;0.00000;,
 1.31373;11.21367;0.00000;,
 1.31373;11.21367;-0.82984;,
 4.27589;12.57556;0.96890;,
 4.26963;11.20016;0.83097;,
 -4.27591;12.57703;-0.00250;,
 -4.27591;12.57852;0.96392;,
 -1.32001;12.58900;0.00000;,
 -4.27244;11.20318;0.82542;,
 -4.26964;11.20165;-0.00250;,
 -1.31374;11.21367;0.00000;,
 -4.27590;12.57556;-0.96890;,
 -1.32001;12.58900;-0.96890;,
 -4.26963;11.20016;-0.82984;,
 -1.31374;11.21367;-0.82984;,
 5.51736;12.43848;0.00000;,
 5.51736;12.43848;-0.78188;,
 5.51083;11.32861;-0.66966;,
 5.51083;11.32861;0.00000;,
 5.51736;12.43848;0.78188;,
 5.51083;11.32861;0.67056;,
 -5.51738;12.44015;-0.00271;,
 -5.51738;12.44179;0.77645;,
 -5.51377;11.33188;0.66469;,
 -5.51084;11.33021;-0.00271;,
 -5.51737;12.43848;-0.78188;,
 -5.51084;11.32861;-0.66966;,
 -1.47310;69.67473;-0.32664;,
 -1.15791;69.67473;-0.54599;,
 -1.34479;65.87072;-0.75883;,
 -1.71086;65.87072;-0.52725;,
 1.15790;69.67473;-0.54599;,
 1.34479;65.87072;-0.75883;,
 1.47309;69.67473;-0.32664;,
 1.71085;65.87072;-0.52725;,
 -1.34479;12.12614;-0.75883;,
 -1.71086;12.12614;-0.52725;,
 1.34479;12.12614;-0.75883;,
 1.71085;12.12614;-0.52725;,
 1.47309;69.67473;-0.32664;,
 2.04734;69.67473;0.00000;,
 2.35418;65.87072;0.00000;,
 1.71085;65.87072;-0.52725;,
 1.47309;69.67473;0.32664;,
 1.71085;65.87072;0.52725;,
 2.35418;12.12614;0.00000;,
 1.71085;12.12614;-0.52725;,
 1.71085;12.12614;0.52725;,
 1.47309;69.67473;0.32664;,
 1.15790;69.67473;0.54599;,
 1.34479;65.87072;0.75883;,
 1.71085;65.87072;0.52725;,
 -1.15791;69.67473;0.54599;,
 -1.34479;65.87072;0.75883;,
 -1.47310;69.67473;0.32664;,
 -1.71086;65.87072;0.52725;,
 1.34479;12.12614;0.75883;,
 1.71085;12.12614;0.52725;,
 -1.34479;12.12614;0.75883;,
 -1.71086;12.12614;0.52725;,
 -1.47310;69.67473;0.32664;,
 -2.04734;69.67473;0.00000;,
 -2.35419;65.87072;0.00000;,
 -1.71086;65.87072;0.52725;,
 -1.47310;69.67473;-0.32664;,
 -1.71086;65.87072;-0.52725;,
 -2.35419;12.12614;0.00000;,
 -1.71086;12.12614;0.52725;,
 -1.71086;12.12614;-0.52725;,
 -2.35419;12.12614;0.00000;,
 -1.34479;12.12614;0.00000;,
 -1.34479;12.12614;0.75883;,
 1.34479;12.12614;0.00000;,
 1.34479;12.12614;0.75883;,
 2.35418;12.12614;0.00000;,
 -1.71086;12.12614;-0.52725;,
 -1.34479;12.12614;-0.75883;,
 1.34479;12.12614;-0.75883;,
 1.71085;12.12614;-0.52725;,
 -0.00000;76.35670;0.00000;,
 -0.00000;76.35670;0.00000;,
 -0.00000;76.35670;0.00000;,
 -0.00000;76.35670;0.00000;,
 -0.00000;76.35670;0.00000;;
 
 128;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;3,2,10,11;,
 4;2,5,12,10;,
 4;5,7,13,12;,
 4;7,9,14,13;,
 4;11,10,15,16;,
 4;10,12,17,15;,
 4;12,13,18,17;,
 4;13,14,19,18;,
 4;20,21,22,23;,
 4;21,24,25,22;,
 4;23,22,26,27;,
 4;22,25,28,26;,
 4;27,26,29,30;,
 4;26,28,31,29;,
 4;32,33,34,35;,
 4;33,36,37,34;,
 4;36,38,39,37;,
 4;38,40,41,39;,
 4;35,34,42,43;,
 4;34,37,44,42;,
 4;37,39,45,44;,
 4;39,41,46,45;,
 4;43,42,47,48;,
 4;42,44,49,47;,
 4;44,45,50,49;,
 4;45,46,51,50;,
 4;52,53,54,55;,
 4;53,56,57,54;,
 4;55,54,58,59;,
 4;54,57,60,58;,
 4;59,58,61,62;,
 4;58,60,63,61;,
 4;52,64,65,66;,
 4;64,36,67,65;,
 4;36,68,69,67;,
 4;68,24,70,69;,
 4;66,65,71,72;,
 4;65,67,73,71;,
 4;67,69,74,73;,
 4;69,70,75,74;,
 4;76,77,78,62;,
 4;77,79,49,78;,
 4;79,80,81,49;,
 4;80,82,31,81;,
 4;83,84,77,76;,
 4;84,85,79,77;,
 4;85,86,80,79;,
 4;86,87,82,80;,
 4;88,89,90,91;,
 4;89,92,93,90;,
 4;94,95,96,97;,
 4;95,98,99,96;,
 4;100,95,101,102;,
 4;95,103,104,101;,
 4;102,101,105,106;,
 4;101,104,107,105;,
 4;108,109,96,110;,
 4;109,111,112,96;,
 4;113,114,109,108;,
 4;114,115,111,109;,
 4;116,117,118,119;,
 4;120,121,122,123;,
 4;117,120,123,118;,
 4;124,116,119,103;,
 4;125,124,103,112;,
 4;121,125,112,122;,
 4;126,127,100,128;,
 4;129,130,131,110;,
 4;127,129,110,100;,
 4;132,126,128,133;,
 4;134,132,133,135;,
 4;130,134,135,131;,
 4;136,137,117,116;,
 4;138,139,121,120;,
 4;137,138,120,117;,
 4;137,136,139,138;,
 4;140,136,116,124;,
 4;141,140,124,125;,
 4;139,141,125,121;,
 4;136,140,141,139;,
 4;142,143,127,126;,
 4;144,145,130,129;,
 4;143,144,129,127;,
 4;143,142,145,144;,
 4;146,142,126,132;,
 4;147,146,132,134;,
 4;145,147,134,130;,
 4;142,146,147,145;,
 4;148,149,150,151;,
 4;149,152,153,150;,
 4;152,154,155,153;,
 4;151,150,156,157;,
 4;150,153,158,156;,
 4;153,155,159,158;,
 4;160,161,162,163;,
 4;161,164,165,162;,
 4;163,162,166,167;,
 4;162,165,168,166;,
 4;169,170,171,172;,
 4;170,173,174,171;,
 4;173,175,176,174;,
 4;172,171,177,178;,
 4;171,174,179,177;,
 4;174,176,180,179;,
 4;181,182,183,184;,
 4;182,185,186,183;,
 4;184,183,187,188;,
 4;183,186,189,187;,
 4;190,191,192,188;,
 4;191,193,194,192;,
 4;193,195,168,194;,
 4;196,197,191,190;,
 4;197,198,193,191;,
 4;198,199,195,193;,
 3;200,149,148;,
 3;201,152,149;,
 3;202,154,152;,
 3;203,161,160;,
 3;204,164,161;,
 3;200,170,169;,
 3;201,173,170;,
 3;202,175,173;,
 3;203,182,181;,
 3;204,185,182;;
 
 MeshMaterialList {
  3;
  128;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.060000;0.144000;0.235200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.708800;0.511200;0.040800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.300000;0.300000;0.300000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  144;
  -0.964609;0.103840;-0.242378;,
  -0.767376;0.112098;-0.631323;,
  -0.000000;0.000077;-1.000000;,
  0.767325;0.112205;-0.631365;,
  -0.931731;0.000003;-0.363148;,
  -0.532755;0.000019;-0.846270;,
  0.000000;0.000039;-1.000000;,
  0.532727;0.000033;-0.846287;,
  -0.931738;0.000000;-0.363130;,
  -0.532771;0.000000;-0.846259;,
  0.000000;0.000000;-1.000000;,
  0.532769;0.000000;-0.846261;,
  -0.951480;-0.182928;-0.247432;,
  -0.532771;0.000000;-0.846259;,
  0.000000;0.000000;-1.000000;,
  0.532769;0.000000;-0.846261;,
  0.964588;0.103932;-0.242420;,
  0.989188;0.146655;0.000000;,
  0.931719;0.000011;-0.363179;,
  1.000000;0.000000;0.000000;,
  0.931737;0.000000;-0.363133;,
  0.951479;-0.182930;-0.247434;,
  0.965884;-0.258976;-0.000000;,
  0.964612;0.103909;0.242336;,
  0.767400;0.112128;0.631288;,
  -0.000000;-0.000076;1.000000;,
  -0.767450;0.112022;0.631246;,
  0.931745;-0.000003;0.363114;,
  0.532785;-0.000019;0.846250;,
  0.000000;-0.000038;1.000000;,
  -0.532813;-0.000032;0.846233;,
  0.931737;0.000000;0.363133;,
  0.532769;0.000000;0.846261;,
  0.000000;0.000000;1.000000;,
  -0.532771;0.000000;0.846259;,
  0.951479;-0.182930;0.247434;,
  0.532769;0.000000;0.846261;,
  0.000000;0.000000;1.000000;,
  -0.964632;0.103816;0.242295;,
  -0.989205;0.146541;0.000000;,
  -0.931757;-0.000011;0.363084;,
  -1.000000;0.000000;0.000000;,
  -0.931738;0.000000;0.363130;,
  -0.951480;-0.182928;0.247432;,
  -0.965884;-0.258974;0.000000;,
  -0.957052;0.289918;0.000000;,
  0.000000;1.000000;0.000000;,
  -0.572851;-0.819659;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.572849;-0.819661;-0.000000;,
  0.956985;0.290138;0.000000;,
  -0.571290;-0.817426;0.073775;,
  0.000000;-0.990816;0.135215;,
  0.571288;-0.817427;0.073775;,
  -0.571290;-0.817426;-0.073775;,
  0.000000;-0.990816;-0.135215;,
  0.571288;-0.817427;-0.073775;,
  -0.137902;-0.091232;-0.986235;,
  0.000000;-0.083287;-0.996526;,
  0.137902;-0.091232;-0.986235;,
  0.002275;0.999997;0.000000;,
  -0.002285;-0.999997;0.000000;,
  0.137815;-0.090534;0.986312;,
  0.000050;-0.082631;0.996580;,
  -0.138647;-0.090679;0.986182;,
  -0.002024;0.999998;-0.000384;,
  0.002031;-0.999998;0.000458;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.057121;0.998366;-0.001297;,
  -0.068671;-0.100712;-0.992543;,
  0.057228;0.998361;0.000000;,
  0.049254;-0.998786;-0.000000;,
  0.068723;-0.099912;0.992620;,
  -0.056687;0.998391;-0.001294;,
  -0.056904;0.998379;-0.001296;,
  -0.049756;-0.998760;0.001351;,
  -0.049551;-0.998771;0.001423;,
  -0.109783;0.993954;-0.001827;,
  -0.137473;-0.100349;-0.985409;,
  0.109747;0.993960;0.000000;,
  0.109747;0.993960;0.000000;,
  0.102935;-0.994688;-0.000000;,
  0.137571;-0.099555;0.985476;,
  -0.109419;0.993994;-0.001812;,
  -0.109601;0.993974;-0.001820;,
  -0.103177;-0.994661;0.001761;,
  -0.103023;-0.994677;0.001925;,
  -0.001898;0.999998;-0.000386;,
  0.002275;0.999997;0.000000;,
  -0.002149;0.999998;-0.000382;,
  0.001904;-0.999998;0.000468;,
  -0.002285;-0.999997;0.000000;,
  0.002159;-0.999998;0.000448;,
  0.068671;-0.100713;-0.992543;,
  -0.069765;-0.100149;0.992523;,
  -0.049347;-0.998781;0.001494;,
  0.137472;-0.100351;-0.985409;,
  0.999983;-0.005889;-0.000000;,
  0.999983;-0.005889;-0.000000;,
  -0.137879;-0.099631;0.985425;,
  -0.999987;-0.004574;-0.002184;,
  -0.999986;-0.005230;-0.001088;,
  -0.102869;-0.994693;0.002088;,
  -0.999983;-0.005886;0.000008;,
  0.000000;0.161326;-0.986901;,
  -0.543099;0.113534;-0.831957;,
  -0.291767;0.098629;-0.951391;,
  0.291769;0.098630;-0.951390;,
  -0.572228;0.035012;-0.819347;,
  -0.283295;0.034340;-0.958418;,
  0.283299;0.034340;-0.958417;,
  -0.585346;0.000000;-0.810784;,
  -0.278302;0.000000;-0.960494;,
  0.278307;0.000000;-0.960492;,
  0.543101;0.113534;-0.831957;,
  0.527924;0.107098;-0.842512;,
  0.572230;0.035012;-0.819345;,
  0.600322;0.032057;-0.799115;,
  0.585348;0.000000;-0.810782;,
  0.633878;0.000000;-0.773433;,
  0.543101;0.113534;0.831957;,
  0.291769;0.098630;0.951390;,
  -0.291767;0.098629;0.951391;,
  0.572230;0.035012;0.819345;,
  0.283299;0.034340;0.958417;,
  -0.283295;0.034340;0.958418;,
  0.585348;0.000000;0.810782;,
  0.278307;0.000000;0.960492;,
  -0.278302;0.000000;0.960494;,
  -0.543099;0.113534;0.831957;,
  -0.527926;0.107099;0.842510;,
  -0.572228;0.035012;0.819347;,
  -0.600325;0.032058;0.799113;,
  -0.585346;0.000000;0.810784;,
  -0.633881;0.000000;0.773431;,
  0.000000;-1.000000;-0.000000;,
  0.527924;0.107098;0.842512;,
  0.600322;0.032057;0.799115;,
  0.633878;0.000000;0.773433;,
  -0.527926;0.107099;-0.842510;,
  -0.600325;0.032058;-0.799113;,
  -0.633881;0.000000;-0.773431;,
  0.000000;0.161326;0.986901;;
  128;
  4;0,1,5,4;,
  4;1,2,6,5;,
  4;2,3,7,6;,
  4;3,16,18,7;,
  4;4,5,9,8;,
  4;5,6,10,9;,
  4;6,7,11,10;,
  4;7,18,20,11;,
  4;8,9,13,12;,
  4;9,10,14,13;,
  4;10,11,15,14;,
  4;11,20,21,15;,
  4;16,17,19,18;,
  4;17,23,27,19;,
  4;18,19,19,20;,
  4;19,27,31,19;,
  4;20,19,22,21;,
  4;19,31,35,22;,
  4;23,24,28,27;,
  4;24,25,29,28;,
  4;25,26,30,29;,
  4;26,38,40,30;,
  4;27,28,32,31;,
  4;28,29,33,32;,
  4;29,30,34,33;,
  4;30,40,42,34;,
  4;31,32,36,35;,
  4;32,33,37,36;,
  4;33,34,34,37;,
  4;34,42,43,34;,
  4;38,39,41,40;,
  4;39,0,4,41;,
  4;40,41,41,42;,
  4;41,4,8,41;,
  4;42,41,44,43;,
  4;41,8,12,44;,
  4;38,26,45,39;,
  4;46,46,46,46;,
  4;46,46,46,46;,
  4;24,23,17,50;,
  4;39,45,1,0;,
  4;46,46,46,46;,
  4;46,46,46,46;,
  4;50,17,16,3;,
  4;44,47,51,43;,
  4;47,48,52,51;,
  4;48,49,53,52;,
  4;49,22,35,53;,
  4;12,54,47,44;,
  4;54,55,48,47;,
  4;55,56,49,48;,
  4;56,21,22,49;,
  4;57,58,58,57;,
  4;58,59,59,58;,
  4;62,63,63,62;,
  4;63,64,64,63;,
  4;88,67,67,65;,
  4;67,89,60,67;,
  4;65,67,67,90;,
  4;67,60,89,67;,
  4;66,68,68,91;,
  4;68,61,92,68;,
  4;93,68,68,66;,
  4;68,61,61,68;,
  4;71,71,89,60;,
  4;72,72,61,61;,
  4;94,94,59,59;,
  4;71,71,60,89;,
  4;73,73,62,62;,
  4;72,72,92,61;,
  4;75,74,88,65;,
  4;76,77,66,91;,
  4;95,95,64,64;,
  4;69,75,65,90;,
  4;70,70,57,57;,
  4;77,96,93,66;,
  4;81,80,71,71;,
  4;82,82,72,72;,
  4;97,97,94,94;,
  4;98,99,99,98;,
  4;80,81,71,71;,
  4;83,83,73,73;,
  4;82,82,72,72;,
  4;99,99,99,99;,
  4;85,84,74,75;,
  4;86,87,77,76;,
  4;100,100,95,95;,
  4;101,102,102,101;,
  4;78,85,75,69;,
  4;79,79,70,70;,
  4;87,103,96,77;,
  4;102,104,104,102;,
  4;106,107,110,109;,
  4;107,108,111,110;,
  4;108,115,117,111;,
  4;109,110,113,112;,
  4;110,111,114,113;,
  4;111,117,119,114;,
  4;115,116,118,117;,
  4;137,121,124,138;,
  4;117,118,120,119;,
  4;138,124,127,139;,
  4;121,122,125,124;,
  4;122,123,126,125;,
  4;123,130,132,126;,
  4;124,125,128,127;,
  4;125,126,129,128;,
  4;126,132,134,129;,
  4;130,131,133,132;,
  4;140,106,109,141;,
  4;132,133,135,134;,
  4;141,109,112,142;,
  4;136,136,136,136;,
  4;136,136,136,136;,
  4;136,136,136,136;,
  4;136,136,136,136;,
  4;136,136,136,136;,
  4;136,136,136,136;,
  3;105,107,106;,
  3;105,108,107;,
  3;105,115,108;,
  3;105,116,115;,
  3;143,121,137;,
  3;143,122,121;,
  3;143,123,122;,
  3;143,130,123;,
  3;143,131,130;,
  3;105,106,140;;
 }
 MeshTextureCoords {
  205;
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.333330;,
  0.000000;0.333330;,
  0.500000;0.000000;,
  0.500000;0.333330;,
  0.750000;0.000000;,
  0.750000;0.333330;,
  1.000000;0.000000;,
  1.000000;0.333330;,
  0.250000;0.666670;,
  0.000000;0.666670;,
  0.500000;0.666670;,
  0.750000;0.666670;,
  1.000000;0.666670;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.333330;,
  0.000000;0.333330;,
  1.000000;0.000000;,
  1.000000;0.333330;,
  0.500000;0.666670;,
  0.000000;0.666670;,
  1.000000;0.666670;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.333330;,
  0.000000;0.333330;,
  0.500000;0.000000;,
  0.500000;0.333330;,
  0.750000;0.000000;,
  0.750000;0.333330;,
  1.000000;0.000000;,
  1.000000;0.333330;,
  0.250000;0.666670;,
  0.000000;0.666670;,
  0.500000;0.666670;,
  0.750000;0.666670;,
  1.000000;0.666670;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;0.333330;,
  0.000000;0.333330;,
  1.000000;0.000000;,
  1.000000;0.333330;,
  0.500000;0.666670;,
  0.000000;0.666670;,
  1.000000;0.666670;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.000000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.250000;1.000000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  0.750000;1.000000;,
  1.000000;0.500000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.000000;1.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  1.000000;0.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.500000;0.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.500000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.333330;,
  0.333330;0.333330;,
  0.333330;0.666670;,
  0.000000;0.666670;,
  0.666670;0.333330;,
  0.666670;0.666670;,
  1.000000;0.333330;,
  1.000000;0.666670;,
  0.333330;1.000000;,
  0.000000;1.000000;,
  0.666670;1.000000;,
  1.000000;1.000000;,
  0.000000;0.333330;,
  0.500000;0.333330;,
  0.500000;0.666670;,
  0.000000;0.666670;,
  1.000000;0.333330;,
  1.000000;0.666670;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.333330;,
  0.333330;0.333330;,
  0.333330;0.666670;,
  0.000000;0.666670;,
  0.666670;0.333330;,
  0.666670;0.666670;,
  1.000000;0.333330;,
  1.000000;0.666670;,
  0.333330;1.000000;,
  0.000000;1.000000;,
  0.666670;1.000000;,
  1.000000;1.000000;,
  0.000000;0.333330;,
  0.500000;0.333330;,
  0.500000;0.666670;,
  0.000000;0.666670;,
  1.000000;0.333330;,
  1.000000;0.666670;,
  0.500000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.500000;,
  0.333330;0.500000;,
  0.333330;1.000000;,
  0.666670;0.500000;,
  0.666670;1.000000;,
  1.000000;0.500000;,
  0.000000;0.000000;,
  0.333330;0.000000;,
  0.666670;0.000000;,
  1.000000;0.000000;,
  0.166670;0.000000;,
  0.500000;0.000000;,
  0.833330;0.000000;,
  0.250000;0.000000;,
  0.750000;0.000000;;
 }
}
