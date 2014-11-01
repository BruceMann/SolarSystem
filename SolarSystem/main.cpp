//
//  main.cpp
//  SolarSystem
//
//  Created by TangShi on 14-4-20.
//  Copyright (c) 2014年 TangShi. All rights reserved.
//

//=============================================================
// 导入OpenGL
//=============================================================

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>


#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include "globalData.h"
#include "RGBpixmap.h"


//=============================================================
// 函数声明
//=============================================================
void Init();                                // 初始化应用程序
void Uninit();                              // 清理应用程序
void DisplayFunc();                         // glut描绘回调函数
void ReshapeFunc(int, int);                 // glut窗口重置回调函数
void KeyboardFunc(unsigned char, int, int);	// glut键盘回调函数
void SpecialFunc(int key, int x, int y);    // glut特殊按键回调函数
void MouseFunc(int, int, int, int);         // glut鼠标按下与释放回调函数
void IdleFunc();                            // glut空闲处理回调函数
void TimerFunc(int);                        // glut定时处理回调函数
void DrawText(const char* text, float x, float y);	// 在屏幕上显示文本
void LoadTexture();                         // 载入纹理
void UpdateEye();                           // 更新视图参数

GLfloat getAngle(GLfloat period);  // 获得旋转角


//=============================================================
// 全局变量
//=============================================================

const static int WindowPosX = 100;
const static int WindowPosY = 80;
int WindowWidth = 960;
int WindowHeight = 540;
const char* WindowTitle = "Solar System";
static string message;
const static GLint fps = (GLint)1000/25;  // 帧率
static bool animating = true;
static int timeToQuit = 50;

const static float FOV = 60.0f;

static GLfloat TimeCoe = 60.0f;
static GLuint TimeUnit = 0;

static string imagePath; // 纹理图片路径
GLuint	TexName[10] = {0,1,2,3,4,5,6,7,8,9};// 纹理对象
GLUquadric *quad;

GLfloat EyePos[3]  = {0, 500, 200};
GLfloat EyeCent[3] = {0, 0, 0};
GLfloat EyeUp[3]   = {0, 0, 1};
GLint   Eye = 0;

//=============================================================
// main 函数
//=============================================================
int main(int argc, char** argv)
{
    /* 确认纹理贴图路径
     * 若参数数目大于1，说明用户输入了自定义路径
     * 默认使用可执行文件所在目录作为纹理贴图图片的路径
     * 若用户输入了自定义路径则使用自定义路径下的纹理贴图图片
     */
    if (argc > 1) {
        imagePath = string(argv[1]);
    }
    else {
        imagePath = string(argv[0]);
    }
    
	// 初始化glut工具
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
	// 创建window
	glutInitWindowPosition(WindowPosX, WindowPosY);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow(WindowTitle);
	
	// 初始化应用程序
	Init() ;
	
	// 设定glut回调函数
	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);
    glutSpecialFunc(SpecialFunc);
    glutMouseFunc(MouseFunc);
    //	glutIdleFunc(IdleFunc);
    glutTimerFunc(fps, TimerFunc, 1);
    
	// 进入glut事件处理循环
	glutMainLoop();
	
    // 清理应用程序
	Uninit();
	
	return 0;
}


//=============================================================
// 函数定义
//=============================================================

// 初始化应用程序
void Init()
{
    // 设置清屏颜色
    glClearColor (0.3, 0.3, 0.3, 0.3);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    LoadTexture();
    
}

// 清理应用程序

void Uninit()
{
	// nothing
    gluDeleteQuadric(quad);
}

void genTextureImage(string imageName, string& result)
{
    result.clear();
    result.append(imagePath);
    result.append("/");
    result.append(imageName);
}

void LoadTexture()
{
    string textureImage = string();
    for (int i=0; i<10; ++i) {
        RGBpixmap pix;
        genTextureImage(PlanetImage[i], textureImage);
//        pix.readBMPFile(PlanetImage[i], false);
        pix.readBMPFile(textureImage, false);
        pix.setTexture(TexName[i]);
    }
    quad = gluNewQuadric();
    gluQuadricNormals(quad, GL_SMOOTH);						// 使用平滑法线
    gluQuadricTexture(quad, GL_TRUE);						// 使用纹理
    gluQuadricDrawStyle(quad, GLU_FILL);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
}


// 响应窗口重置事件
void ReshapeFunc(int width, int height)
{
	WindowWidth = width;
	WindowHeight = height;
	glViewport(0, 0, WindowWidth, WindowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, (GLfloat)WindowWidth/WindowHeight, 1.0, 10000000);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(EyePos[0], EyePos[1], EyePos[2],
              EyeCent[0], EyeCent[1], EyeCent[2],
              EyeUp[0], EyeUp[1], EyeUp[2]);
}

// 定时调用
void TimerFunc(int millis)
{
    TimeUnit += 1;
	glutPostRedisplay(); // 请求重绘
    
    // 若开启动画，则循环调用
    if (animating) {
        glutTimerFunc(fps, TimerFunc, 1);
    }
    
}

// 角度计算
GLfloat getAngle(GLfloat period)
{
    // 自转角
    GLfloat ang;
    ang = TimeUnit / (period * TimeCoe);
    ang = (ang - (int)ang) * 360;
    
    return ang;
}

// 描绘函数
void DisplayFunc()
{
	// 清屏
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /* 视图变换 */
    //    UpdateEye();
    
    /* 模型变换 */
    glEnable(GL_TEXTURE_2D);
	glPushMatrix();
    {
        glPushMatrix();
        {
            // 太阳自转
            glRotatef(getAngle(RotationPeriod[Sun]), 0.0f, 0.0f, 1.0f);
            
            // 绘制太阳
            glColor3fv(PlanetColor[Sun]);
            glBindTexture(GL_TEXTURE_2D, TexName[Sun]);
            gluSphere(quad, RadiusRatio[Sun] * RE, 48, 24);
        }
        glPopMatrix();
        
        
        int n = 1500;
        GLfloat Distance;
        for (int i=1; i<9; ++i) {
            
            glPushMatrix();
            {
                // 公转轨道
                glColor3fv(PlanetColor[i]);
                glLineWidth(2.0f);
                Distance = OrbitRatio[i] * AU;
                glBegin(GL_LINE_LOOP);
                {
                    for(int i=0; i<n; ++i) {
                        glVertex2f(Distance * cos(2 * PI * i / n), Distance * sin(2 * PI * i / n));
                    }
                }
                glEnd();
                
                // 公转
                glRotatef(getAngle(RevolutionPeriod[i]), 0.0f, 0.0f, 1.0f);
                
                // 轨道半径
                glTranslatef(Distance , 0.0f, 0.0f);
                
                // 自转
                glRotatef(getAngle(RotationPeriod[i]), 0.0f, 0.0f, 1.0f);
                
                // 绘制天体
                glColor3fv(PlanetColor[i]);
                glBindTexture(GL_TEXTURE_2D, TexName[i]);
                gluSphere(quad, RadiusRatio[i] * RE, 48, 24);
            }
            glPopMatrix();
        }
        
        
        glPushMatrix();
        {
            // 地球公转
            glRotatef(getAngle(RevolutionPeriod[Earth]), 0.0f, 0.0f, 1.0f);
            
            // 地球轨道半径
            glTranslatef(OrbitRatio[Earth] * AU, 0.0f, 0.0f);
            
            // 月球公转轨道
            n = 200;
            Distance = OrbitRatio[Moon] * AU;
            glColor3fv(PlanetColor[Moon]);
            glLineWidth(1.0f);
            glBegin(GL_LINE_LOOP);
            {
                for(int i=0; i<n; ++i) {
                    glVertex2f(Distance * cos(2 * PI * i / n), Distance * sin(2 * PI * i / n));
                }
            }
            glEnd();
            
            // 月球公转
            glRotatef(getAngle(RevolutionPeriod[Moon]), 0.0f, 0.0f, 1.0f);
            
            // 月球轨道半径
            glTranslatef(Distance, 0.0f, 0.0f);
			
			//月球自转
			glRotatef(getAngle(RotationPeriod[Moon]), 0.0f, 0.0f, 1.0f);
			
            // 绘制月球
            //            glColor3fv(PlanetColor[Moon]);
            glBindTexture(GL_TEXTURE_2D, TexName[Moon]);
            gluSphere(quad, RadiusRatio[Moon] * RE, 20, 20);
        }
        glPopMatrix();
    }
	glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    /* 模型变换结束*/
    
    // 显示文本信息
    float x = 10.0f;
    float y = 20.0f;
    
    if (timeToQuit < 50) {
        timeToQuit--;
    }
    DrawText(message.c_str(), x, y);
    
    if (timeToQuit == 0) {
        exit(0);
    }
    
	// 交换显示缓冲区
	glutSwapBuffers() ;
}

void UpdateEye()
{
    //TODO: 视图变换有待更正
    
    GLfloat ang;
    if (Eye == 0) {
        ang = getAngle(RotationPeriod[Sun]);
        GLfloat r = RadiusRatio[Sun] * RE;
        GLfloat x = r * cos(ang);
        GLfloat y = r * sin(ang);
        EyePos[0] = x;
        EyePos[1] = y;
        EyePos[2] = 0;
        EyeCent[0] = x + cos(ang);
        EyeCent[1] = y + sin(ang);
        EyeCent[2] = 0;
    }
    
    else if (Eye > 0 && Eye < 9) {
        ang = getAngle(RevolutionPeriod[Eye]); // 公转角
        GLfloat R = OrbitRatio[Eye] * AU;      // 轨道半径
		GLfloat x = R * cos(ang);
		GLfloat y = R * sin(ang);
        EyePos[0] = x;
        EyePos[1] = y;
        EyeCent[0] = 0;
        EyeCent[1] = 0;
        
        if (Eye == 3) {
            EyeCent[0] = x;
            EyeCent[1] = y;
            R = OrbitRatio[Moon] * AU;
            ang = getAngle(RevolutionPeriod[Moon]);
            EyeCent[0] += R * cos(ang);
            EyeCent[1] += R * sin(ang);
        }
        
        ang = getAngle(RotationPeriod[Eye]); // 自转角
		GLfloat r = RadiusRatio[Eye] * RE;   // 星球半径
        x = r * cos(ang);
		y = r * sin(ang);
        EyePos[0] += x;
        EyePos[1] += y;
        
        
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(EyePos[0], EyePos[1], EyePos[2],
              EyeCent[0], EyeCent[1], EyeCent[2],
              EyeUp[0], EyeUp[1], EyeUp[2]);
}

// 响应按键事件
void KeyboardFunc(unsigned char key, int x, int y)
{
    // 切换视角
    //    if (key >= '0' && key <= '9') {
    //        Eye = key - '0';
    //        std::cout << Eye << std::endl;
    //    }
    
    
	switch (key)
	{
        case 'n':
        case 'N':
            TimeUnit = TimeUnit / TimeCoe;
            TimeCoe = 60.0f;
            TimeUnit = TimeUnit * TimeCoe;
            message = "One min per frame!";
            break;
		case 'H':
        case 'h':
            TimeUnit = TimeUnit / TimeCoe;
            TimeCoe = 1.0f;
            TimeUnit = TimeUnit * TimeCoe;
            message = "One hour per frame!";
            break;
        case 'D':
        case 'd':
            TimeUnit = TimeUnit / TimeCoe;
            TimeCoe = 0.041666667f;
            TimeUnit = TimeUnit * TimeCoe;
            message = "One day per frame!";
            break;
        case 'M':
        case 'm':
            TimeUnit = TimeUnit / TimeCoe;
            TimeCoe = 0.001388889f;
            TimeUnit = TimeUnit * TimeCoe;
            message = "One month per frame!";
            break;
        case 'Y':
        case 'y':
            TimeUnit = TimeUnit / TimeCoe;
            TimeCoe = 0.000114155f;
            TimeUnit = TimeUnit * TimeCoe;
            message = "One year per frame!";
            break;
        case 'q':
        case 'Q':
            message = "Good Bye!";
            timeToQuit--;
		default:
			break;
	}
}

void SpecialFunc(int key, int x, int y)
{
    
    switch (key) {
        case GLUT_KEY_LEFT:
            EyePos[2] -= 10;
            break;
        case GLUT_KEY_RIGHT:
            EyePos[2] += 10;
            break;
        case GLUT_KEY_UP:
            EyePos[1] -= 10;
            break;
        case GLUT_KEY_DOWN:
            EyePos[1] += 10;
            break;
        default:
            break;
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(EyePos[0], EyePos[1], EyePos[2],
              EyeCent[0], EyeCent[1], EyeCent[2],
              EyeUp[0], EyeUp[1], EyeUp[2]);
}


void MouseFunc(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        animating = false;
        message = "stop animation!";
    }
    if (button == GLUT_RIGHT_BUTTON) {
        animating = true;
        message = "start animation!";
        glutTimerFunc(fps, TimerFunc, 1);
    }
}

// 在屏幕上显示文本，x 和 y 为屏幕坐标
void DrawText(const char* text, float x, float y)
{
	// 检查OpenGL状态
	bool isDepthOpen = false;
	bool isStencilOpen = false;
	bool isLightOpen = false;
	bool isFogOpen = false;
    
	if(glIsEnabled(GL_DEPTH_TEST))
	{
		isDepthOpen = true;
		glDisable(GL_DEPTH_TEST);
	}
	if(glIsEnabled(GL_STENCIL_TEST))
	{
		isStencilOpen = true;
		glDisable(GL_STENCIL_TEST);
	}
	if(glIsEnabled(GL_LIGHTING))
	{
		isLightOpen = true;
		glDisable(GL_LIGHTING);
	}
	if(glIsEnabled(GL_FOG))
	{
		isFogOpen = true;
		glDisable(GL_FOG);
	}
    
    
    //    void *font = GLUT_BITMAP_8_BY_13;
	// 设置字体颜色
	glColor3f(1.0, 0.0, 0.0);
    
	/*
	 * 设置正投影
	 */
	glMatrixMode(GL_PROJECTION);
	// 保存当前投影矩阵
	glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0, WindowWidth, 0, WindowHeight );
    // 反转Y轴（朝下为正方向）(与窗口坐标一致)
    glScalef(1, -1, 1);
    // 将原点移动到屏幕左上方(与窗口坐标一致)
    glTranslatef(0, -WindowHeight, 0);
    glMatrixMode(GL_MODELVIEW);
    
    // 保存当前模型视图矩阵
    glPushMatrix();
    glLoadIdentity();
    
    // 设置文字位置
    glRasterPos2f( x, y );
    
    // 依次描绘所有字符(使用显示列表)
    for(const char* c = text; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    // 恢复之前保存的模型视图矩阵
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    
	// 恢复之前保存的投影矩阵
	glPopMatrix();
	// 返回模型视图矩阵状态
	glMatrixMode(GL_MODELVIEW);
    
	// 恢复OpenGL状态
	if(isDepthOpen)
	{
		glEnable(GL_DEPTH_TEST);
	}
	if(isStencilOpen)
	{
		glEnable(GL_STENCIL_TEST);
	}
	if(isLightOpen)
	{
		glEnable(GL_LIGHTING);
	}
	if(isFogOpen)
	{
		glEnable(GL_FOG);
	}
}



