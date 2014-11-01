//
//  data.h
//  SolarSystem
//
//  Created by TangShi on 14-4-21.
//  Copyright (c) 2014年 TangShi. All rights reserved.
//

#ifndef SolarSystem_data_h
#define SolarSystem_data_h

#include <string>
using namespace std;

const static GLfloat PI = 3.1415926536f; // 圆周率
const static GLfloat AU = 149597.871f * 0.001f;   // 1天文单位(AU × 10^6 米)
const static GLfloat RE = 6.378f * 2;        // 地球半径

enum Planet
{
    Sun = 0,   // 太阳
    Mercury,   // 水星
    Venus,     // 金星
    Earth,     // 地球
    Mars,      // 火星
    Jupiter,   // 木星
    Saturn,    // 土星
    Uranus,    // 天王星
    Neptune,   // 海王星
    Moon       // 月球
};

// 天体颜色
const static GLfloat ColorSun[3] = {0.9f, 0.3f, 0.0f};
const static GLfloat ColorMercury[3] = {0.9f, 0.8, 0.7};
const static GLfloat ColorVenus[3] = {0.38f, 0.18f, 0.27f};
const static GLfloat ColorEarth[3] = {0.1f, 0.3f, 0.4f};
const static GLfloat ColorMars[3] = {0.95f, 0.54f, 0.20f};
const static GLfloat ColorJupiter[3] = {0.64f, 0.33f, 0.17f};
const static GLfloat ColorSaturn[3] = {0.66f, 0.58f, 0.54f};
const static GLfloat ColorUranus[3] = {0.51f, 0.75f, 0.91f};
const static GLfloat ColorNeptune[3] = {0.28f, 0.52f, 0.75f};
const static GLfloat ColorMoon[3] = {0.75f,0.75f,0.75f};
const static GLfloat* PlanetColor[10] = {ColorSun,ColorMercury,ColorVenus,ColorEarth, ColorMars, ColorJupiter,ColorSaturn,ColorUranus,ColorNeptune,ColorMoon};


//const static char ImageSun[]     = "Bitmaps/Sun.bmp";
//const static char ImageMercury[] = "Bitmaps/Mercury.bmp";
//const static char ImageVenus[]   = "Bitmaps/Venus.bmp";
//const static char ImageEarth[]   = "Bitmaps/Earth.bmp";
//const static char ImageMars[]    = "Bitmaps/Mars.bmp";
//const static char ImageJupiter[] = "Bitmaps/Jupiter.bmp";
//const static char ImageSaturn[]  = "Bitmaps/Saturn.bmp";
//const static char ImageUranus[]  = "Bitmaps/Uranus.bmp";
//const static char ImageNeptune[] = "Bitmaps/Neptune.bmp";
//const static char ImageMoon[]    = "Bitmaps/Moon.bmp";

//const static char *PlanetImage[] = {ImageSun,ImageMercury,ImageVenus,ImageEarth,ImageMars,ImageJupiter,ImageSaturn,ImageUranus,ImageNeptune,ImageMoon};

const string& imgsun       = "Sun.bmp";
const string& ImageSun     = "Sun.bmp";
const string& ImageMercury = "Mercury.bmp";
const string& ImageVenus   = "Venus.bmp";
const string& ImageEarth   = "Earth.bmp";
const string& ImageMars    = "Mars.bmp";
const string& ImageJupiter = "Jupiter.bmp";
const string& ImageSaturn  = "Saturn.bmp";
const string& ImageUranus  = "Uranus.bmp";
const string& ImageNeptune = "Neptune.bmp";
const string& ImageMoon    = "Moon.bmp";

const string PlanetImage[] = {ImageSun,ImageMercury,ImageVenus,ImageEarth,ImageMars,ImageJupiter,ImageSaturn,ImageUranus,ImageNeptune,ImageMoon};


// 天体半径与地球半径的比值
//const static GLfloat RadiusRatio[10] = {109.125f, 0.383f, 0.949f, 1.000f, 0.533f, 11.209f, 9.449f, 4.007f, 3.883f, 0.273f};
const static GLfloat RadiusRatio[10] = {2.547f,0.5745f,0.949f,1.0f,0.742f,1.742f,1.4890f,1.336f,1.294f,0.321f};

// 天体运行轨道半径（天文单位）
//const static GLfloat OrbitRatio[10] = {0.0f,0.387f,0.723f,1.0f,1.523f,5.202f,9.554f,19.22f,30.11f,0.00257f};
const static GLfloat OrbitRatio[10] = {0.0f,0.3694f,0.68786f,1.0f,1.35714f,2.31548f,4.24702f,6.83333f,8.03036f,0.15482f};
// 天体赤道倾角
//const static GLfloat EquatorInclination[10] = {7.25f, 0.00f, 177.40f, 23.44f, 25.19f, 3.08f, 26.70f, 97.90f, 27.80f, 5.15f};

// 天体轨道倾角
//const static GLfloat OrbitInclination[10] = {0.0f, 7.005f, 3.395f, 0.000f, 1.850f, 1.303f, 2.489f, 0.773f, 1.770f, 23.43f};

// 天体公转周期
const static GLfloat RevolutionPeriod[10] = {0.0f,2112.0f,5400.0f,8760.0f,16488.0f,103893.6f,258069.6f,735927.6f,1443823.2f,655.68f};

// 天体自转周期
const static GLfloat RotationPeriod[10] = {609.12f,1416.0f,5832.0f,24.0f,24.5f,9.83f,10.0f,24.0f,16.0f,655.68f};


#endif
