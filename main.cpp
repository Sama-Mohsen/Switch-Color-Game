#include <chrono>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <irrKlang.h>
#include <iostream>
#include <unistd.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;
using namespace irrklang;

#define WINDOW_WIDTH (1920*0.75)
#define WINDOW_HEIGHT (1080*0.75)
#define PI 3.14159265358979323846
#define SQUARE_SIDE_LENGTH 115
#define CIRCLE_RADIUS_IN_SQUARE 20
#define SQUARE_NUM_CIRCLES_PER_SIDE 6
#define SQUARE_CIRCLES_SPEED 1.5
#define TOTAL_CIRCLES (SQUARE_NUM_CIRCLES_PER_SIDE * 4-4)
#define BALL_RADIUS 15
#define THRESHOLD 15
#define MAIN_SCREEN 0
#define CLASSIC_MODE 1
#define GAME_OVER_SCREEN 2
#define GAME_CONTROLS 3
#define Cyan 0
#define Purple 1
#define Pink 2
#define Yellow 3
#define SQUARE_CD_THRESHOLD 15


void InitGraphics(int argc, char *argv[]);
void SetTransformations();
void Timer(int value=0);
void OnDisplay();
void KeyPress_ClassicMode(unsigned char key, int x, int y);
void keyUp_ClassicMode(unsigned char key, int x, int y);
void mouseClick_ClassicMode(int button, int state, int x, int y);
void loadTexture(const char* filename);

void initSound(const char *soundFile);
void cleanupSound();

float mapToOrthoX_ClassicMode(float x);
float mapToOrthoY_ClassicMode(float y);

bool detectCollision_ClassicMode(double x1, double y1, double x2, double y2double,double threshold=0);
void drawCircle_ClassicMode(float centerX, float centerY, float radius);
void drawBall_ClassicMode();
void drawArc_ClassicMode(float centerX, float centerY, float radius, float startAngle, float endAngle);
//draw circles square
void Square_Side_Cyan_ClassicMode();
void Square_Side_Purple_ClassicMode();
void Square_Side_Pink_ClassicMode();
void Square_Side_Yellow_ClassicMode();
// biggest circle
void Yellow_partCircle_ClassicMode();
void Cyan_partCircle_ClassicMode();
void pink_partCircle_ClassicMode();
void Purple_partCircle_ClassicMode();
void draw_outer_Circle_ClassicMode();
void draw_inner_circle_ClassicMode ();
//middle circle
void Yellow_partCircle2_ClassicMode();
void Cyan_partCircle2_ClassicMode();
void pink_partCircle2_ClassicMode();
void Purple_partCircle2_ClassicMode();
void outer_second_circle_ClassicMode();
void draw_inner_circle2_ClassicMode ();
//smallest circle
void Yellow_partCircle3_ClassicMode();
void Cyan_partCircle3_ClassicMode();
void pink_partCircle3_ClassicMode();
void Purple_partCircle3_ClassicMode();
void outer_third_circle_ClassicMode();
void draw_inner_circle3_ClassicMode ();

//change color circle
void Yellow_partCircleSmall_ClassicMode();
void Cyan_partCircleSmall_ClassicMode();
void pink_partCircleSmall_ClassicMode();
void Purple_partCircleSmall_ClassicMode();
void draw_small_circle_ClassicMode();

//2 Circles
//C1
void Yellow_partCircle_2Circles_ClassicMode();
void Cyan_partCircle_2Circles_ClassicMode();
void pink_partCircle_2Circles_ClassicMode();
void Purple_partCircle_2Circles_ClassicMode();
void drawOuterCircle_2Circles_ClassicMode();
void draw_inner_circle_2Circles_ClassicMode();
//C12
void Yellow_partCircle_side_2Circles_ClassicMode();
void Cyan_partCircle_side_2Circles_ClassicMode();
void pink_partCircle_side_2Circles_ClassicMode();
void Purple_partCircle_side_2Circles_ClassicMode();
void drawOuterCircleSide_2Circles_ClassicMode();
void drawInnerCircleSide_2Circles_ClassicMode ();
//Circles with square path
struct MovingCircle;
void initCircles();
void updateCircles();
void drawMovingCircles();

void print(int x, int y, string word);
string int_to_str(long long score, bool scoreWord=true);

void star();

void drawPauseResumeIcon();
void drawCircle(double x0, double y0, double rX, double rY, double R1,double G1, double B1, double alfa1=1);
void mainMenu();
void gameControls();
void gameOverScreen();
void classicMode();

ISoundEngine* soundEffectsEngine = nullptr, *gameThemeMusicEngine = nullptr, *starSoundEngine=nullptr, *colorSW = nullptr;
ISound* activeSound = nullptr;
const int NUM_CIRCLES_PER_SIDE_ClassicMode = 6;
const float SIDE_LENGTH_ClassicMode = 230;
const float CIRCLE_RADIUS_ClassicMode = 20;//Square's small circles
const float Small_CIRCLE_RADIUS_ClassicMode = 30;// Small solid circle
const float OUTER_RADIUS_ClassicMode = 190.0f;
const float INNER_RADIUS_ClassicMode = 160.0f;
const float Second_OUTER_RADIUS_ClassicMode = 145.0f;
const float Second_INNER_RADIUS_ClassicMode = 125.0f;
const float Third_OUTER_RADIUS_ClassicMode = 110.0f;
const float Third_INNER_RADIUS_ClassicMode = 95.0f;
const int sleepDur=0,sleepDuration=500000;
bool ballIsMoving_ClassicMode = false,ballFirstMove_ClassicMode= true,allowedSpacePress_ClassicMode= true,allowedMouseClick_ClassicMode= true,GameOver_ClassicMode= false,mode1Shape2_ClassicMode=false,starFlag_ClassicMode[2]={false,false};
int ballColorIndex=1,starScaleIndex=0;
double ballMoves=0,ballPos=0,angle[2]= {0,0},mode1YPos[2]= {0,0},starScale[2]={0.8,1},switchColorYPos[2]= {0,0};
const char* jumpSound = "GAME  SOUND/jump.wav";
int mode1Shape[2]= {1,0}, maxShapes=1,frames=0,score_ClassicMode=0;
bool scoreChangedOnce[2] = {true,true},changeColorFlag[2]={false, false},changeBallColorIndexOnce=false,loseSoundOnce=false;
int gameScreen=0;
bool isPaused=false;
bool soundOff=false, musicOff=false,pKeyPressed=false,switchColorSoundOnce[2]= {false,false};
int mainMenuAngle=0,resumeCounter=0,bestScore,gameOverFramesCounter=0,replayScaleIndex=0,tempCounter=1;
const int firstYPos=750,SCFirstYPos=510;
double replayScale[2]={2.25,2.5};
GLuint textureID;

int main(int argc, char* argv[])
{
    if (!gameThemeMusicEngine)
        gameThemeMusicEngine = createIrrKlangDevice();
    gameThemeMusicEngine->play2D("GAME  SOUND/Color Switch - Game Theme Music.mp3",true,false,true);
    gameThemeMusicEngine->setSoundVolume(0.5);
    InitGraphics(argc, argv);
    cleanupSound();
    return 0;
}

void initSound(const char *soundFile)
{
    if (!soundEffectsEngine)
        soundEffectsEngine = createIrrKlangDevice();
    soundEffectsEngine->setSoundVolume(1);
    soundEffectsEngine->play2D(soundFile, false,false, true);
}
void cleanupSound()
{
    if (soundEffectsEngine)
        soundEffectsEngine->drop(); // Release the sound engine
    if (gameThemeMusicEngine)
        gameThemeMusicEngine->drop();
    if (starSoundEngine)
        starSoundEngine->drop();
    if (activeSound)
        activeSound->drop();
}
void SetTransformations()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-960, 960, -540, 540);
    glMatrixMode(GL_MODELVIEW);
}
void InitGraphics(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE | GLUT_DEPTH | GLUT_STENCIL);
    int windowPosX = (1920 - int(WINDOW_WIDTH)) / 2;
    int windowPosY = (1080 - int(WINDOW_HEIGHT)) / 2;

    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    glutInitWindowPosition(windowPosX,windowPosY);
    glutCreateWindow("Switch Color");

    //to enable alpha of glColor4f(R,G,B,Alfa);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    // Enable multisampling for smoother edges
    glEnable(GL_MULTISAMPLE);
    glutKeyboardFunc(KeyPress_ClassicMode);
    glutKeyboardUpFunc(keyUp_ClassicMode);
    glutMouseFunc(mouseClick_ClassicMode);
    initCircles();
    glutDisplayFunc(OnDisplay);
    glutTimerFunc(0,Timer,0);
    glutMainLoop();
}
void Timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(10, Timer, value);
}
void mouseClick_ClassicMode(int button, int state, int x, int y) {
    double openglX = (x / WINDOW_WIDTH)*1920 - 960;
    double openglY = 540 - (y / WINDOW_HEIGHT)*1080;

    if (button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&gameScreen!=MAIN_SCREEN)
    {
        if(openglX>=900-30&&openglX<=900+30&&openglY>=500-30&&openglY<=500+30)
            gameScreen=MAIN_SCREEN;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN  && gameScreen==CLASSIC_MODE )
    {
        if(openglX>=820-30&&openglX<=820+30&&openglY>=500-30&&openglY<=500+30)
            isPaused=!isPaused,pKeyPressed= true;
        else if(openglX>=900-30&&openglX<=900+30&&openglY>=420-30&&openglY<=420+30)
            soundOff=!soundOff;
        else if(openglX>=900-30&&openglX<=900+30&&openglY>=340-30&&openglY<=340+30)
        {
            musicOff = !musicOff;
            if (musicOff)
                gameThemeMusicEngine->stopAllSounds();
            else
                gameThemeMusicEngine->play2D("GAME  SOUND/Color Switch - Game Theme Music.mp3",true,false,true);
        }

        else if( allowedMouseClick_ClassicMode && !isPaused)
        {
            if (!(openglX >= 900 - 40 && openglX <= 900 + 40 && openglY >= 500 - 40 && openglY <= 500 + 40)) {
                if (!soundOff) { initSound(jumpSound); }
                ballFirstMove_ClassicMode = false;
                ballMoves = 0;
                cout << "mouse clicked" << endl;
                ballIsMoving_ClassicMode = true;
            }
        }
    }
    else if (button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&gameScreen==MAIN_SCREEN)
    {
        if (openglX>=-47.5&&openglX<=47.5&&openglY>=-47.5+25&&openglY<=47.5+25) gameScreen=CLASSIC_MODE;
        else if (openglX>=-150-40&&openglX<=-150+40&&openglY>=-150-40&&openglY<=-150+40) soundOff=!soundOff;
        else if (openglX>=-40&&openglX<=40&&openglY>=-150-40&&openglY<=-150+40)
        {
            musicOff = !musicOff;
            if (musicOff)
                gameThemeMusicEngine->stopAllSounds();
            else
                gameThemeMusicEngine->play2D("GAME  SOUND/Color Switch - Game Theme Music.mp3",true,false,true);
        }
        else if (openglX>=150-40&&openglX<=150+40&&openglY>=-150-40&&openglY<=-150+40) gameScreen=GAME_CONTROLS;
    }
    else if (button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN&&gameScreen==GAME_OVER_SCREEN)
    {
       /* cout<<(openglX>=-40*replayScale[replayScaleIndex]&&openglX<=40*replayScale[replayScaleIndex]
        &&openglY>=(-250-40*replayScale[replayScaleIndex])&&openglY<=(-250+40*replayScale[replayScaleIndex]))<<"\n";*/
        if (openglX>=-40*replayScale[replayScaleIndex]&&openglX<=40*replayScale[replayScaleIndex]
            &&openglY>=(-250-40*replayScale[replayScaleIndex])&&openglY<=(-250+40*replayScale[replayScaleIndex]))
        {
            ///Initialization
            {
                ballIsMoving_ClassicMode = false;
                ballFirstMove_ClassicMode= true;
                allowedSpacePress_ClassicMode= true;
                allowedMouseClick_ClassicMode= true;
                mode1Shape2_ClassicMode=false;
                starFlag_ClassicMode[0]=false;
                starFlag_ClassicMode[1]=false;
                starScaleIndex=0;
                ballMoves=0;
                ballPos=0;
                angle[0]= 0;
                angle[1]=0;
                mode1YPos[0]= 0;
                mode1YPos[1]=0;
                mode1Shape[0]=1;
                mode1Shape[1]=0;
                maxShapes=1;
                frames=0;
//        score_ClassicMode=0;
                scoreChangedOnce[0]=scoreChangedOnce[1] =true;
                changeColorFlag[0]= false;
                changeColorFlag[1]= false;
                changeBallColorIndexOnce= false;
//        colorChanged= false;
                isPaused=false;
                ballColorIndex=random()%100%4;
                resumeCounter=0;
                pKeyPressed=false;
                switchColorYPos[0]=switchColorYPos[1]=0;
                switchColorSoundOnce[0]=switchColorSoundOnce[1]=false;
                initCircles();
            }
            loseSoundOnce=false;
            score_ClassicMode=0;
            GameOver_ClassicMode=false;
            gameScreen = CLASSIC_MODE;
        }
        else if (openglX>=-150-40&&openglX<=-150+40&&openglY>=-250-40&&openglY<=-250+40) soundOff=!soundOff;
        else if (openglX>=150-40&&openglX<=150+40&&openglY>=-250-40&&openglY<=-250+40)
        {
            musicOff = !musicOff;
            if (musicOff)
                gameThemeMusicEngine->stopAllSounds();
            else
                gameThemeMusicEngine->play2D("GAME  SOUND/Color Switch - Game Theme Music.mp3",true,false,true);
        }
    }
//    cout<<x<<" "<<y<<"\n"<<openglX<<" "<<openglY<<"\n\n";

}
void keyUp_ClassicMode(unsigned char key, int x, int y) {
    if (key == 32) {
        allowedSpacePress_ClassicMode = false;
        cout << "space released" << endl;
    }
}
void KeyPress_ClassicMode(unsigned char key, int x, int y)
{
    if (allowedSpacePress_ClassicMode && key == 32 && !isPaused && gameScreen==CLASSIC_MODE)
    {
        if (!soundOff)
        { initSound(jumpSound); }
        ballFirstMove_ClassicMode= false;
        ballMoves=0;
        cout << "SPACE pressed" << endl;
        ballIsMoving_ClassicMode = true;
    }
    else if (key==27)
    {
        if (gameScreen==MAIN_SCREEN)
            exit(0);
        else
        {
            gameScreen = MAIN_SCREEN;
        }
        cout<<"ESC pressed\n";
    }
    else if(key == 'P'|| key == 'p')
    {
        isPaused = !isPaused;
        pKeyPressed=true;
        cout<<"P pressed\n";
    }
    else if (key=='M'||key=='m')
    {
        musicOff = !musicOff;
        cout<<"M pressed\n";
        if (musicOff)
            gameThemeMusicEngine->stopAllSounds();
        else
            gameThemeMusicEngine->play2D("GAME  SOUND/Color Switch - Game Theme Music.mp3",true,false,true);
    }
    else if (key=='N'||key=='n')
        soundOff=!soundOff,cout<<"N pressed\n";
    else if (key==13) {
        cout<<"ENTER pressed\n";
        if (gameScreen==GAME_OVER_SCREEN)
        {
            ///Initialization
            {
                ballIsMoving_ClassicMode = false;
                ballFirstMove_ClassicMode= true;
                allowedSpacePress_ClassicMode= true;
                allowedMouseClick_ClassicMode= true;
                mode1Shape2_ClassicMode=false;
                starFlag_ClassicMode[0]=false;
                starFlag_ClassicMode[1]=false;
                starScaleIndex=0;
                ballMoves=0;
                ballPos=0;
                angle[0]= 0;
                angle[1]=0;
                mode1YPos[0]= 0;
                mode1YPos[1]=0;
                mode1Shape[0]=1;
                mode1Shape[1]=0;
                maxShapes=1;
                frames=0;
//        score_ClassicMode=0;
                scoreChangedOnce[0]=scoreChangedOnce[1] =true;
                changeColorFlag[0]= false;
                changeColorFlag[1]= false;
                changeBallColorIndexOnce= false;
//        colorChanged= false;
                isPaused=false;
                ballColorIndex=random()%100%4;
                resumeCounter=0;
                pKeyPressed=false;
                switchColorYPos[0]=switchColorYPos[1]=0;
                switchColorSoundOnce[0]=switchColorSoundOnce[1]=false;
                initCircles();
            }
            loseSoundOnce=false;
            score_ClassicMode=0;
            GameOver_ClassicMode=false;
        }
        gameScreen = CLASSIC_MODE;
    }
    else if (key=='C'||key=='c'&&gameScreen==MAIN_SCREEN)
        gameScreen=GAME_CONTROLS,cout<<"C pressed\n";

}

void loadTexture(const char* filename)
{
    glEnable(GL_TEXTURE_2D);
    int width, height, channels;

    // Load image using stb_image
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha); // Convert to RGBA format

    if (image == nullptr) {
        cout << "Error loading image: " << filename << endl;
        return;
    }

    // Generate OpenGL texture
    glGenTextures(1, &textureID);
//    glActiveTexture(textureID);
    glBindTexture(GL_TEXTURE_2D, textureID); // the same as  glActiveTexture(textureID);

    // Set texture parameters (e.g., filtering, wrapping)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR);

    // Upload the image to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    // Free the image data after uploading to OpenGL
    stbi_image_free(image);
}

float mapToOrthoX_ClassicMode(float x) {
    return x - 960.0f;  // Map to range [-960, 960]
}
float mapToOrthoY_ClassicMode(float y) {
    return 540.0f - y;  // Map to range [-540, 540]
}

bool detectCollision_ClassicMode(double x1, double y1, double x2, double y2, double threshold)
{
    double distance= sqrt(pow((x2-x1),2)+ pow((y2-y1),2));
    // cout << "distance: " << distance << endl;
    // cout<<"ballPos: "<<y2<<"\n";
    // cout<<"mode1YPos[i]: "<<y1<<"\n\n";
    return distance<= threshold;
}
void drawCircle_ClassicMode(float cx, float cy, float r) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 100; i++) {
        float ang= float (i) * 2.0f * PI / 100;
        glVertex2f(cx + cos(ang) * r, cy + sin(ang) * r);
    }
    glEnd();
}
void drawBall_ClassicMode()
{
    switch (ballColorIndex)
    {
        case Cyan:
            glColor3f(0.2314f, 0.8784f, 0.9529f); // cyan
            break;
        case Purple:
            glColor3f(0.584, 0.039, 0.992); // Purple
            break;
        case Pink:
            glColor3f(0.9686f, 0.0235f, 0.4431f); // Pink
            break;
        default:
            glColor3f(1.0, 0.839, 0.055); //yellow
    }
    drawCircle_ClassicMode(0.0f, 0.0f, BALL_RADIUS);
}

void drawArc_ClassicMode(float centerX, float centerY, float radius, float startAngle, float endAngle) {
    glBegin(GL_POLYGON);
    for (int i = startAngle; i <= endAngle; i++) {
        float angle = i * PI / 180.0f;
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        glVertex2f(x, y);
    }
    glVertex2f(centerX, centerY); // Close the arc
    glEnd();
}

//draw circles square
void Square_Side_Cyan_ClassicMode()
{
    glColor3f(0.2314f, 0.8784f, 0.9529f); // cyan
    for (int i = 0; i < NUM_CIRCLES_PER_SIDE_ClassicMode; i++) {
        float x = -SIDE_LENGTH_ClassicMode / 2.0f + i * (SIDE_LENGTH_ClassicMode / (NUM_CIRCLES_PER_SIDE_ClassicMode - 1));
        float y = SIDE_LENGTH_ClassicMode / 2.0f;
        drawCircle_ClassicMode(x, y, CIRCLE_RADIUS_ClassicMode);
    }
}
void Square_Side_Purple_ClassicMode()
{
    glColor3f(0.584, 0.039, 0.992); // Purple
    for (int i = 0; i < NUM_CIRCLES_PER_SIDE_ClassicMode; i++) {
        float x = SIDE_LENGTH_ClassicMode / 2.0f;
        float y = SIDE_LENGTH_ClassicMode / 2.0f - i * (SIDE_LENGTH_ClassicMode / (NUM_CIRCLES_PER_SIDE_ClassicMode - 1));
        drawCircle_ClassicMode(x, y, CIRCLE_RADIUS_ClassicMode);
    }
}
void Square_Side_Pink_ClassicMode()
{
    glColor3f(0.9686f, 0.0235f, 0.4431f); // Pink
    for (int i = 0; i < NUM_CIRCLES_PER_SIDE_ClassicMode; i++) {
        float x = SIDE_LENGTH_ClassicMode / 2.0f - i * (SIDE_LENGTH_ClassicMode / (NUM_CIRCLES_PER_SIDE_ClassicMode - 1));
        float y = -SIDE_LENGTH_ClassicMode / 2.0f;
        drawCircle_ClassicMode(x, y, CIRCLE_RADIUS_ClassicMode);
    }
}
void Square_Side_Yellow_ClassicMode()
{
    glColor3f(1.0, 0.839, 0.055); //yellow
    for (int i = 0; i < NUM_CIRCLES_PER_SIDE_ClassicMode-1; i++) {
        float x = -SIDE_LENGTH_ClassicMode / 2.0f;
        float y = -SIDE_LENGTH_ClassicMode / 2.0f + i * (SIDE_LENGTH_ClassicMode / (NUM_CIRCLES_PER_SIDE_ClassicMode - 1));
        drawCircle_ClassicMode(x, y, CIRCLE_RADIUS_ClassicMode);
    }
}

// biggest circle
void Yellow_partCircle_ClassicMode()
{
    glColor3f(1.0, 0.839, 0.055);
    drawArc_ClassicMode(0.0f, 0.0f, OUTER_RADIUS_ClassicMode, 45, 135); // 90 degrees arc
}
void Cyan_partCircle_ClassicMode()
{
    glColor3f(0.2314f, 0.8784f, 0.9529f); // cyan
    drawArc_ClassicMode(0.0f, 0.0f, OUTER_RADIUS_ClassicMode, -45, 45); // 90 degrees arc
}
void pink_partCircle_ClassicMode()
{
    glColor3f(0.9686f, 0.0235f, 0.4431f); // Pink
    drawArc_ClassicMode(0.0f, 0.0f, OUTER_RADIUS_ClassicMode, -135, -45); // 90 degrees arc
}
void Purple_partCircle_ClassicMode()
{
    glColor3f(0.584, 0.039, 0.992); // Purple
    drawArc_ClassicMode(0.0f, 0.0f, OUTER_RADIUS_ClassicMode, 135, 225); // 90 degrees arc
}
void draw_outer_Circle_ClassicMode()
{
    Yellow_partCircle_ClassicMode();
    Cyan_partCircle_ClassicMode();
    pink_partCircle_ClassicMode();
    Purple_partCircle_ClassicMode();

}
void draw_inner_circle_ClassicMode ()
{
    glColor3f(28/255.0f,28/255.0f,28/255.0f);
    drawCircle_ClassicMode(0.0f, 0.0f, INNER_RADIUS_ClassicMode);
}

//middle circle
void Yellow_partCircle2_ClassicMode()
{
    glColor3f(1.0, 0.839, 0.055);
    drawArc_ClassicMode(0.0f, 0.0f, Second_OUTER_RADIUS_ClassicMode, 45, 135); // 90 degrees arc
}
void Cyan_partCircle2_ClassicMode()
{
    glColor3f(0.2314f, 0.8784f, 0.9529f); // cyan
    drawArc_ClassicMode(0.0f, 0.0f, Second_OUTER_RADIUS_ClassicMode, -45, 45); // 90 degrees arc
}
void pink_partCircle2_ClassicMode()
{
    glColor3f(0.9686f, 0.0235f, 0.4431f); // Pink
    drawArc_ClassicMode(0.0f, 0.0f, Second_OUTER_RADIUS_ClassicMode, -135, -45); // 90 degrees arc
}
void Purple_partCircle2_ClassicMode()
{
    glColor3f(0.584, 0.039, 0.992); // Purple
    drawArc_ClassicMode(0.0f, 0.0f, Second_OUTER_RADIUS_ClassicMode, 135, 225); // 90 degrees arc
}
void outer_second_circle_ClassicMode()
{
    Yellow_partCircle2_ClassicMode();
    Cyan_partCircle2_ClassicMode();
    pink_partCircle2_ClassicMode();
    Purple_partCircle2_ClassicMode();
}
void draw_inner_circle2_ClassicMode ()
{
    glColor3f(28/255.0f,28/255.0f,28/255.0f);
    drawCircle_ClassicMode(0.0f, 0.0f, Second_INNER_RADIUS_ClassicMode);

}

//smallest circle
void Yellow_partCircle3_ClassicMode()
{
    glColor3f(1.0, 0.839, 0.055);
    drawArc_ClassicMode(0.0f, 0.0f, Third_OUTER_RADIUS_ClassicMode, 45, 135); // 90 degrees arc
}
void Cyan_partCircle3_ClassicMode()
{
    glColor3f(0.2314f, 0.8784f, 0.9529f); // cyan
    drawArc_ClassicMode(0.0f, 0.0f, Third_OUTER_RADIUS_ClassicMode, -45, 45); // 90 degrees arc
}
void pink_partCircle3_ClassicMode()
{
    glColor3f(0.9686f, 0.0235f, 0.4431f); // Pink
    drawArc_ClassicMode(0.0f, 0.0f, Third_OUTER_RADIUS_ClassicMode, -135, -45); // 90 degrees arc
}
void Purple_partCircle3_ClassicMode()
{
    glColor3f(0.584, 0.039, 0.992); // Purple
    drawArc_ClassicMode(0.0f, 0.0f, Third_OUTER_RADIUS_ClassicMode, 135, 225); // 90 degrees arc
}
void outer_third_circle_ClassicMode()
{
    Yellow_partCircle3_ClassicMode();
    Cyan_partCircle3_ClassicMode();
    pink_partCircle3_ClassicMode();
    Purple_partCircle3_ClassicMode();
}
void draw_inner_circle3_ClassicMode ()
{
    glColor3f(28/255.0f,28/255.0f,28/255.0f);
    drawCircle_ClassicMode(0.0f, 0.0f, Third_INNER_RADIUS_ClassicMode);

}

//change color circle
void Purple_partCircleSmall_ClassicMode()
{
    glColor3f(0.584, 0.039, 0.992); // Purple
    drawArc_ClassicMode(0.0f, 0.0f, Small_CIRCLE_RADIUS_ClassicMode, 0,90);
}
void Yellow_partCircleSmall_ClassicMode()
{
    glColor3f(1.0, 0.839, 0.055);
    drawArc_ClassicMode(0.0f, 0.0f, Small_CIRCLE_RADIUS_ClassicMode, 90,180);
}
void pink_partCircleSmall_ClassicMode()
{
    glColor3f(0.9686f, 0.0235f, 0.4431f); // Pink
    drawArc_ClassicMode(0.0f, 0.0f, Small_CIRCLE_RADIUS_ClassicMode, 270,360);

}
void Cyan_partCircleSmall_ClassicMode()
{
    glColor3f(0.2314f, 0.8784f, 0.9529f); // cyan
    drawArc_ClassicMode(0.0f, 0.0f, Small_CIRCLE_RADIUS_ClassicMode, 180,270);
}
void draw_small_circle_ClassicMode()
{
    Purple_partCircleSmall_ClassicMode();
    pink_partCircleSmall_ClassicMode();
    Cyan_partCircleSmall_ClassicMode();
    Yellow_partCircleSmall_ClassicMode();



}

//2 Circles
//C1
void Yellow_partCircle_2Circles_ClassicMode()
{
    glColor3f(1.0, 0.839, 0.055); //yellow
    drawArc_ClassicMode(-130, 0.0f, 130, 45, 135);
}
void Cyan_partCircle_2Circles_ClassicMode()
{
    glColor3f(0.2314f, 0.8784f, 0.9529f); // Cyan
    drawArc_ClassicMode(-130, 0.0f, 130, -45, 45); // 90 degrees arc
}
void pink_partCircle_2Circles_ClassicMode()
{
    glColor3f(0.9686f, 0.0235f, 0.4431f);  // Magenta ,pink
    drawArc_ClassicMode(-130, 0.0f, 130, -135, -45); // 90 degrees arc
}
void Purple_partCircle_2Circles_ClassicMode()
{
    glColor3f(0.584, 0.039, 0.992);  // Purple
    drawArc_ClassicMode(-130, 0.0f, 130, 135, 225); // 90 degrees arc
}
void drawOuterCircle_2Circles_ClassicMode()
{
    Yellow_partCircle_2Circles_ClassicMode();
    Cyan_partCircle_2Circles_ClassicMode();
    pink_partCircle_2Circles_ClassicMode();
    Purple_partCircle_2Circles_ClassicMode();

}
void draw_inner_circle_2Circles_ClassicMode()
{
    glColor3f(28/255.0f,28/255.0f,28/255.0f);
    drawCircle_ClassicMode(-130, 0.0f, 105);
}
//C12
void Yellow_partCircle_side_2Circles_ClassicMode()
{
    glColor3f(1.0, 0.839, 0.055); //yellow
    drawArc_ClassicMode(130, 0.0f, 130, 45, 135); // 90 degrees arc
}
void Cyan_partCircle_side_2Circles_ClassicMode()
{
    glColor3f(0.2314f, 0.8784f, 0.9529f); // cyan
    drawArc_ClassicMode(130, 0.0f, 130, 135, 225); // 90 degrees arc
}
void pink_partCircle_side_2Circles_ClassicMode()
{
    glColor3f(0.9686f, 0.0235f, 0.4431f); // Magenta ,pink
    drawArc_ClassicMode(130, 0.0f, 130, -135, -45); // 90 degrees arc
}
void Purple_partCircle_side_2Circles_ClassicMode()
{
    glColor3f(0.5f, 0.0f, 1.0f);  // Purple
    drawArc_ClassicMode(130, 0.0f, 130, -45, 45); // 90 degrees arc

}
void drawOuterCircleSide_2Circles_ClassicMode()
{
    Yellow_partCircle_side_2Circles_ClassicMode();
    Cyan_partCircle_side_2Circles_ClassicMode();
    pink_partCircle_side_2Circles_ClassicMode();
    Purple_partCircle_side_2Circles_ClassicMode();

}
void drawInnerCircleSide_2Circles_ClassicMode ()
{
    glColor3f(28/255.0f,28/255.0f,28/255.0f);
    drawCircle_ClassicMode(130, 0.0f, 105);
}

void star() {
    glPushMatrix();
    glTranslatef(270, -40, 0);
    glScalef(0.5, 0.5, 1.0);
    {
        glColor3f(1, 1, 1);
        glBegin(GL_TRIANGLES);
        glVertex2d(mapToOrthoX_ClassicMode(539.500), mapToOrthoY_ClassicMode(378.500));
        glVertex2d(mapToOrthoX_ClassicMode(518.500), mapToOrthoY_ClassicMode(420.500));
        glVertex2d(mapToOrthoX_ClassicMode(561.500), mapToOrthoY_ClassicMode(420.500));

        glEnd();
        glBegin(GL_TRIANGLES);


        glVertex2d(mapToOrthoX_ClassicMode(482.500), mapToOrthoY_ClassicMode(420));
        glVertex2d(mapToOrthoX_ClassicMode(514.500), mapToOrthoY_ClassicMode(455));
        glVertex2d(mapToOrthoX_ClassicMode(530.500), mapToOrthoY_ClassicMode(413));

        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2d(mapToOrthoX_ClassicMode(504.000), mapToOrthoY_ClassicMode(487.500));
        glVertex2d(mapToOrthoX_ClassicMode(510.500), mapToOrthoY_ClassicMode(444.000));
        glVertex2d(mapToOrthoX_ClassicMode(544.000), mapToOrthoY_ClassicMode(470.000));

        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2d(mapToOrthoX_ClassicMode(575.500), mapToOrthoY_ClassicMode(489));
        glVertex2d(mapToOrthoX_ClassicMode(534.000), mapToOrthoY_ClassicMode(468));
        glVertex2d(mapToOrthoX_ClassicMode(569.500), mapToOrthoY_ClassicMode(445));

        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2d(mapToOrthoX_ClassicMode(599.000), mapToOrthoY_ClassicMode(420.000));
        glVertex2d(mapToOrthoX_ClassicMode(554.500), mapToOrthoY_ClassicMode(414.000));
        glVertex2d(mapToOrthoX_ClassicMode(567.000), mapToOrthoY_ClassicMode(453.500));

        glEnd();
        glBegin(GL_POLYGON);

        glVertex2d(mapToOrthoX_ClassicMode(523.000), mapToOrthoY_ClassicMode(418));
        glVertex2d(mapToOrthoX_ClassicMode(558.500), mapToOrthoY_ClassicMode(418));
        glVertex2d(mapToOrthoX_ClassicMode(568.500), mapToOrthoY_ClassicMode(450));
        glVertex2d(mapToOrthoX_ClassicMode(538.500), mapToOrthoY_ClassicMode(469));
        glVertex2d(mapToOrthoX_ClassicMode(513.500), mapToOrthoY_ClassicMode(447));

        glEnd();
    }

    glPopMatrix();
}

void print(int x, int y, string word)
{
    glColor3f(1, 1, 1);
    glRasterPos2f(x, y);
    int len = word.length();
    for (int i = 0; i < len; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, word[i]);

}
string int_to_str(long long score, bool scoreWord) {
    string s = "";
    for (; score > 0; score /= 10)
    {
        s += (score % 10) + 48;
    }
    if (s.length() == 0) s = 48;
    reverse(s.begin(), s.end());
    scoreWord? s="Score: "+s:s;
    return s ;
}

struct MovingCircle {
    float x, y;
    float speed;
    int side; // 0 = Top, 1 = Right, 2 = Bottom, 3 = Left
    int color;
};
MovingCircle circles[TOTAL_CIRCLES];
void initCircles() {
    float step = SQUARE_SIDE_LENGTH * 2 / (SQUARE_NUM_CIRCLES_PER_SIDE - 1);
    int index = 0;

    for (int i = 0; i < SQUARE_NUM_CIRCLES_PER_SIDE-1; i++) {
        circles[index++] = { -SQUARE_SIDE_LENGTH + i * step, SQUARE_SIDE_LENGTH, SQUARE_CIRCLES_SPEED, 0,Cyan }; // Top side
    }
    for (int i = 0; i < SQUARE_NUM_CIRCLES_PER_SIDE-1; i++) {
        circles[index++] = { SQUARE_SIDE_LENGTH, SQUARE_SIDE_LENGTH - i * step, SQUARE_CIRCLES_SPEED, 1,Purple }; // Right side
    }
    for (int i = 0; i < SQUARE_NUM_CIRCLES_PER_SIDE-1; i++) {
        circles[index++] = { SQUARE_SIDE_LENGTH - i * step, -SQUARE_SIDE_LENGTH, SQUARE_CIRCLES_SPEED, 2,Pink }; // Bottom side
    }
    for (int i = 0; i < SQUARE_NUM_CIRCLES_PER_SIDE-1; i++) {
        circles[index++] = { -SQUARE_SIDE_LENGTH, -SQUARE_SIDE_LENGTH + i * step, SQUARE_CIRCLES_SPEED, 3,Yellow }; // Left side
    }
}
void updateCircles() {
    for (auto& circle : circles) {
        switch (circle.side)
        {
            case 0: // Moving right (Top)
                circle.x += circle.speed;
                if (circle.x >= SQUARE_SIDE_LENGTH) {
                    circle.side = 1;
                    circle.x = SQUARE_SIDE_LENGTH;
                }
                break;
            case 1: // Moving down (Right)
                circle.y -= circle.speed;
                if (circle.y <= -SQUARE_SIDE_LENGTH) {
                    circle.side = 2;
                    circle.y = -SQUARE_SIDE_LENGTH;
                }
                break;
            case 2: // Moving left (Bottom)
                circle.x -= circle.speed;
                if (circle.x <= -SQUARE_SIDE_LENGTH) {
                    circle.side = 3;
                    circle.x = -SQUARE_SIDE_LENGTH;
                }
                break;
            case 3: // Moving up (Left)
                circle.y += circle.speed;
                if (circle.y >= SQUARE_SIDE_LENGTH) {
                    circle.side = 0;
                    circle.y = SQUARE_SIDE_LENGTH;
                }
                break;
        }
    }
}
void drawMovingCircles() {
    for (const auto& circle : circles) {
        switch (circle.color)
        {
            case Cyan:
                glColor3f(0.2314f, 0.8784f, 0.9529f); // cyan
                break;
            case Purple:
                glColor3f(0.584, 0.039, 0.992); // Purple
                break;
            case Pink:
                glColor3f(0.9686f, 0.0235f, 0.4431f); // Pink
                break;
            default:
                glColor3f(1.0, 0.839, 0.055); //yellow
        }
        drawCircle_ClassicMode(circle.x, circle.y, CIRCLE_RADIUS_IN_SQUARE);
    }
}

void drawPauseResumeIcon()
{
    if (!isPaused)
    {
        ///Resume icon at the middle
        {
            if (resumeCounter < 25)
            {
                loadTexture(
                        "/home/sama/SwitchColor_Project/cmake-build-debug/Textures/middle resume.png");
                glPushMatrix();
                glTranslated(0, 0, 0);
                glColor4f(1, 1, 1, 0.7);
                glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(-100, -100);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2f(100, -100);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2f(100, 100);
                glTexCoord2f(0.0f, 1.0f);
                glVertex2f(-100, 100);
                glEnd();
                glPopMatrix();
                glDisable(GL_TEXTURE_2D);
            }
            resumeCounter <= 25 ? resumeCounter++ : resumeCounter;
        }
    }
    else
    {
        ///Pause icon at the middle
        {
            resumeCounter=0;
            loadTexture("Textures/middle pause.png");
            glPushMatrix();
            glTranslated(0, 0, 0);
            glColor4f(1, 1, 1, 0.7);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-100, -100);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(100, -100);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(100, 100);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-100, 100);
            glEnd();
            glPopMatrix();
            glDisable(GL_TEXTURE_2D);
        }
    }
}
void classicMode()
{
    if (!GameOver_ClassicMode)
    {
        print(-900, 500-12, int_to_str(score_ClassicMode));
        for (int i = 0; i < maxShapes; ++i)
        {
            glPushMatrix();
            glTranslated(0, firstYPos, 0);
            glPushMatrix();
            glTranslated(0, mode1YPos[i], 0);
            //switch (shape)
            {
                switch (mode1Shape[i]) {
                    case 0: {
                        glPushMatrix();
                        glRotated(-angle[i]+45, 0, 0, 1);
                        Square_Side_Cyan_ClassicMode();
                        Square_Side_Purple_ClassicMode();
                        Square_Side_Pink_ClassicMode();
                        Square_Side_Yellow_ClassicMode();
                        glPopMatrix();
                        //check collision
                        {
                            if ((static_cast<int>(angle[i])%360>=0&&static_cast<int>(angle[i])%360<=89))
                            {
                                if((detectCollision_ClassicMode(0,mode1YPos[i]+firstYPos-(100),0,ballPos,15)&&ballColorIndex!=Pink)
                                ||(detectCollision_ClassicMode(0,mode1YPos[i]+firstYPos+(100),0,ballPos,15)&&ballColorIndex!=Cyan))
                                    GameOver_ClassicMode = true,usleep(sleepDur);
                            }
                            else if ((static_cast<int>(angle[i])%360>=90&&static_cast<int>(angle[i])%360<=179))
                            {
                                if ((detectCollision_ClassicMode(0,mode1YPos[i]+firstYPos-(100),0,ballPos,15)&&ballColorIndex!=Purple)
                                ||(detectCollision_ClassicMode(0,mode1YPos[i]+firstYPos+(100),0,ballPos,15)&&ballColorIndex!=Yellow))
                                    GameOver_ClassicMode = true,usleep(sleepDur);
                            }
                            else if ((static_cast<int>(angle[i])%360>=180&&static_cast<int>(angle[i])%360<=269))
                            {
                                if ((detectCollision_ClassicMode(0,mode1YPos[i]+firstYPos-(100),0,ballPos,15)&&ballColorIndex!=Cyan)
                                ||(detectCollision_ClassicMode(0,mode1YPos[i]+firstYPos+(100),0,ballPos,15)&&ballColorIndex!=Pink))
                                    GameOver_ClassicMode = true,usleep(sleepDur);
                            }
                            else if ((static_cast<int>(angle[i])%360>=270&&static_cast<int>(angle[i])%360<=359))
                            {
                                if ((detectCollision_ClassicMode(0,mode1YPos[i]+firstYPos-(100),0,ballPos,15)&&ballColorIndex!=Yellow)
                                ||(detectCollision_ClassicMode(0,mode1YPos[i]+firstYPos+(100),0,ballPos,15)&&ballColorIndex!=Purple))
                                    GameOver_ClassicMode = true,usleep(sleepDur);
                            }
                        }
                        /*{
                            double radAngle=(-angle[i]+45)*M_PI/180;
                            if ((static_cast<int>(angle[i])%360>=0&&static_cast<int>(angle[i])%360<=89))
                            {
                                if((detectCollision_ClassicMode((100*sin(radAngle)),mode1YPos[i]+firstYPos-(100*cos(radAngle)),0,ballPos,SQUARE_SIDE_LENGTH)&&ballColorIndex!=Pink)
                                   ||(detectCollision_ClassicMode((100*sin(radAngle)),mode1YPos[i]+firstYPos+(100*cos(radAngle)),0,ballPos,SQUARE_SIDE_LENGTH)&&ballColorIndex!=Cyan))
                                    GameOver_ClassicMode = true,usleep(sleepDur);
                            }
                            else if ((static_cast<int>(angle[i])%360>=90&&static_cast<int>(angle[i])%360<=179))
                            {
                                if ((detectCollision_ClassicMode((100*sin(radAngle)),mode1YPos[i]+firstYPos-(100*cos(radAngle)),0,ballPos,SQUARE_SIDE_LENGTH)&&ballColorIndex!=Purple)
                                    ||(detectCollision_ClassicMode((100*sin(radAngle)),mode1YPos[i]+firstYPos+(100*cos(radAngle)),0,ballPos,SQUARE_SIDE_LENGTH)&&ballColorIndex!=Yellow))
                                    GameOver_ClassicMode = true,usleep(sleepDur);
                            }
                            else if ((static_cast<int>(angle[i])%360>=180&&static_cast<int>(angle[i])%360<=269))
                            {
                                if ((detectCollision_ClassicMode((100*sin(radAngle)),mode1YPos[i]+firstYPos-(100*cos(radAngle)),0,ballPos,SQUARE_SIDE_LENGTH)&&ballColorIndex!=Cyan)
                                    ||(detectCollision_ClassicMode((115*sin(radAngle)),mode1YPos[i]+firstYPos+(100*cos(radAngle)),0,ballPos,SQUARE_SIDE_LENGTH)&&ballColorIndex!=Pink))
                                    GameOver_ClassicMode = true,usleep(sleepDur);
                            }
                            else if ((static_cast<int>(angle[i])%360>=270&&static_cast<int>(angle[i])%360<=359))
                            {
                                if ((detectCollision_ClassicMode((100*sin(radAngle)),mode1YPos[i]+firstYPos-(100*cos(radAngle)),0,ballPos,SQUARE_SIDE_LENGTH)&&ballColorIndex!=Yellow)
                                    ||(detectCollision_ClassicMode((100*sin(radAngle)),mode1YPos[i]+firstYPos+(100*cos(radAngle)),0,ballPos,SQUARE_SIDE_LENGTH)&&ballColorIndex!=Purple))
                                    GameOver_ClassicMode = true,usleep(sleepDur);
                            }
                        }*/
                        break;
                    }
                    case 1: {
                        glPushMatrix();
                        glRotated(angle[i] + 45, 0, 0, 1);
                        draw_outer_Circle_ClassicMode();
                        draw_inner_circle_ClassicMode();
                        glPopMatrix();
                        //check collision
                        {
                            if ((static_cast<int>(angle[i]) % 360 >= 0 && static_cast<int>(angle[i]) % 360 <= 89)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (190), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Purple)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (190), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Cyan))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 90 &&
                                        static_cast<int>(angle[i]) % 360 <= 179)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (190), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Yellow)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (190), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Pink))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 180 &&
                                        static_cast<int>(angle[i]) % 360 <= 269)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (190), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Cyan)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (190), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Purple))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 270 &&
                                        static_cast<int>(angle[i]) % 360 <= 359)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (190), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Pink)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (190), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Yellow))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            }
                        }
                        break;
                    }
                    case 2:
                    {
                        glPushMatrix();
                        glRotated(angle[i] + 45, 0, 0, 1);
                        outer_second_circle_ClassicMode();
                        draw_inner_circle2_ClassicMode();
                        glPopMatrix();
                        //check collision
                        {
                            if ((static_cast<int>(angle[i]) % 360 >= 0 && static_cast<int>(angle[i]) % 360 <= 89)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (145), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Purple)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (145), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Cyan))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 90 &&
                                        static_cast<int>(angle[i]) % 360 <= 179)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (145), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Yellow)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (145), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Pink))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 180 &&
                                        static_cast<int>(angle[i]) % 360 <= 269)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (145), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Cyan)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (145), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Purple))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 270 &&
                                        static_cast<int>(angle[i]) % 360 <= 359)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (145), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Pink)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (145), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Yellow))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            }
                        }
                        break;
                    }
                    case 3: {
                        glPushMatrix();
                        glRotated(angle[i] + 45, 0, 0, 1);
                        outer_third_circle_ClassicMode();
                        draw_inner_circle3_ClassicMode();
                        glPopMatrix();
                        //check collision
                        {
                            if ((static_cast<int>(angle[i]) % 360 >= 0 && static_cast<int>(angle[i]) % 360 <= 89)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (110), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Purple)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (110), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Cyan))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            }
                            else if ((static_cast<int>(angle[i]) % 360 >= 90 &&
                                      static_cast<int>(angle[i]) % 360 <= 179)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (110), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Yellow)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (110), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Pink))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            }
                            else if ((static_cast<int>(angle[i]) % 360 >= 180 &&
                                      static_cast<int>(angle[i]) % 360 <= 269)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (110), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Cyan)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (110), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Purple))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            }
                            else if ((static_cast<int>(angle[i]) % 360 >= 270 &&
                                      static_cast<int>(angle[i]) % 360 <= 359)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (110), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Pink)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (110), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Yellow))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            }
                        }
                        break;
                    }
                    case 4: {
                        glPushMatrix();
                        glRotated(angle[i] + 45, 0, 0, 1);
                        draw_outer_Circle_ClassicMode();
                        draw_inner_circle_ClassicMode();
                        glPopMatrix();
                        //check collision
                        {
                            if ((static_cast<int>(angle[i]) % 360 >= 0 && static_cast<int>(angle[i]) % 360 <= 89)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (190), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Purple)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (190), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Cyan))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 90 &&
                                        static_cast<int>(angle[i]) % 360 <= 179)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (190), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Yellow)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (190), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Pink))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 180 &&
                                        static_cast<int>(angle[i]) % 360 <= 269)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (190), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Cyan)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (190), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Purple))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 270 &&
                                        static_cast<int>(angle[i]) % 360 <= 359)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (190), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Pink)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (190), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Yellow))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            }
                        }

                        glPushMatrix();
                        glRotated(angle[i] * -(0.75) + 45, 0, 0, 1);
                        outer_second_circle_ClassicMode();
                        draw_inner_circle2_ClassicMode();
                        glPopMatrix();
                        //check collision
                        {
                            if ((static_cast<int>(angle[i]) % 480 >= 0 &&
                                 static_cast<int>(angle[i]) % 480 <= 119)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (145), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Pink)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (145), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Yellow))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 480 >= 120 &&
                                        static_cast<int>(angle[i]) % 480 <= 239)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (145), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Cyan)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (145), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Purple))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 480 >= 240 &&
                                        static_cast<int>(angle[i]) % 480 <= 359)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (145), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Yellow)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (145), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Pink))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 480 >= 360 &&
                                        static_cast<int>(angle[i]) % 480 <= 480)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (145), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Purple)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (145), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Cyan))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            }
                        }
                        break;
                    }
                    case 5: {
                        glPushMatrix();
                        glRotated(angle[i] + 45, 0, 0, 1);
                        draw_outer_Circle_ClassicMode();
                        draw_inner_circle_ClassicMode();
                        glPopMatrix();
                        //check collision
                        {
                            if ((static_cast<int>(angle[i]) % 360 >= 0 && static_cast<int>(angle[i]) % 360 <= 89)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (190), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Purple)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (190), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Cyan))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 90 &&
                                        static_cast<int>(angle[i]) % 360 <= 179)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (190), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Yellow)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (190), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Pink))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 180 &&
                                        static_cast<int>(angle[i]) % 360 <= 269)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (190), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Cyan)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (190), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Purple))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 270 &&
                                        static_cast<int>(angle[i]) % 360 <= 359)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (190), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Pink)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (190), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Yellow))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            }
                        }

                        glPushMatrix();
                        glRotated(-angle[i] * 0.75 + 45, 0, 0, 1);
                        outer_second_circle_ClassicMode();
                        draw_inner_circle2_ClassicMode();
                        glPopMatrix();
                        //check collision
                        {
                            if ((static_cast<int>(angle[i]) % 480 >= 0 &&
                                 static_cast<int>(angle[i]) % 480 <= 119)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (145), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Pink)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (145), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Yellow))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 480 >= 120 &&
                                        static_cast<int>(angle[i]) % 480 <= 239)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (145), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Cyan)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (145), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Purple))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 480 >= 240 &&
                                        static_cast<int>(angle[i]) % 480 <= 359)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (145), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Yellow)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (145), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Pink))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 480 >= 360 &&
                                        static_cast<int>(angle[i]) % 480 <= 480)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (145), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Purple)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (145), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Cyan))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            }
                        }

                        glPushMatrix();
                        glRotated(angle[i]/**0.5*/+ 45, 0, 0, 1);
                        outer_third_circle_ClassicMode();
                        draw_inner_circle3_ClassicMode();
                        glPopMatrix();
                        //check collision
                        {
                            if ((static_cast<int>(angle[i]) % 360 >= 0 && static_cast<int>(angle[i]) % 360 <= 89)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (110), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Purple)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (110), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Cyan))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 90 &&
                                        static_cast<int>(angle[i]) % 360 <= 179)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (110), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Yellow)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (110), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Pink))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 180 &&
                                        static_cast<int>(angle[i]) % 360 <= 269)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (110), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Cyan)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (110), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Purple))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 270 &&
                                        static_cast<int>(angle[i]) % 360 <= 359)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos - (110), 0, ballPos,
                                                                 THRESHOLD) && ballColorIndex != Pink)
                                    || (detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos + (110), 0, ballPos,
                                                                    THRESHOLD) && ballColorIndex != Yellow))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            }
                        }

                        break;
                    }
                    case 6: {
                        glPushMatrix();
                        glTranslated(-130, 0, 0);
                        glRotated(angle[i] + 45, 0, 0, 1);
                        glTranslated(130, 0, 0);
                        drawOuterCircle_2Circles_ClassicMode();
                        draw_inner_circle_2Circles_ClassicMode();
                        glPopMatrix();

                        glPushMatrix();
                        glTranslated(130, 0, 0);
                        glRotated(-angle[i] - 45, 0, 0, 1);
                        glTranslated(-130, 0, 0);
                        drawOuterCircleSide_2Circles_ClassicMode();
                        drawInnerCircleSide_2Circles_ClassicMode();
                        glPopMatrix();

                        //check collision
                        {
                            if ((static_cast<int>(angle[i]) % 360 >= 0 && static_cast<int>(angle[i]) % 360 <= 89)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos, 0, ballPos, THRESHOLD) &&
                                     ballColorIndex != Pink))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 90 &&
                                        static_cast<int>(angle[i]) % 360 <= 179)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos, 0, ballPos, THRESHOLD) &&
                                     ballColorIndex != Purple))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 180 &&
                                        static_cast<int>(angle[i]) % 360 <= 269)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos, 0, ballPos, THRESHOLD) &&
                                     ballColorIndex != Yellow))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            } else if ((static_cast<int>(angle[i]) % 360 >= 270 &&
                                        static_cast<int>(angle[i]) % 360 <= 359)) {
                                if ((detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos, 0, ballPos, THRESHOLD) &&
                                     ballColorIndex != Cyan))
                                    GameOver_ClassicMode = true, usleep(sleepDur);
                            }
                        }
                        break;
                    }
                    default: {
                        drawMovingCircles();
                        if (mode1Shape[i-1]!=7 && !isPaused)
                            updateCircles();
                        //check collision
                        for (const auto &circle: circles) {
                            if (circle.side == 2 || circle.side == 0) {
                                if ((((detectCollision_ClassicMode(circle.x,
                                                                   mode1YPos[i] + firstYPos + SQUARE_SIDE_LENGTH, 0,
                                                                   ballPos, THRESHOLD)) && circle.side == 0)
                                     || ((detectCollision_ClassicMode(circle.x,
                                                                      mode1YPos[i] + firstYPos - SQUARE_SIDE_LENGTH, 0,
                                                                      ballPos, THRESHOLD)) && circle.side == 2))
                                    && circle.color != ballColorIndex) {

                                    GameOver_ClassicMode = true, usleep(sleepDur);
                                }
                            }
                        }
                        break;
                    }
                }
                //Star
                {
                    if (!detectCollision_ClassicMode(0, mode1YPos[i] + firstYPos, 0, ballPos, 10) &&
                        !starFlag_ClassicMode[i])
                    {
                        glPushMatrix();
                        glScaled(starScale[starScaleIndex], starScale[starScaleIndex], 1);
                        glTranslated(-60, -6, 0);
                        star();
                        glPopMatrix();

                    } else {
                        starFlag_ClassicMode[i] = true;

                    }
                    if (starFlag_ClassicMode[i] && scoreChangedOnce[i])
                    {
                        score_ClassicMode++;
                        if (!starSoundEngine)
                            starSoundEngine = createIrrKlangDevice();
                        if (!soundOff)
                        {
                            activeSound = starSoundEngine->play2D("GAME  SOUND/star.wav",
                                    false, false, true);
                        }
                        scoreChangedOnce[i]=false;
                    }
                }
                //Switch color
                {
                    glPushMatrix();
                    glTranslated(0, SCFirstYPos, 0);
                    glTranslated(0, switchColorYPos[i]-mode1YPos[i], 0);
                    if (!detectCollision_ClassicMode(0, switchColorYPos[i] + firstYPos + SCFirstYPos, 0, ballPos, 30) &&
                        !changeColorFlag[i])
                        draw_small_circle_ClassicMode();
                    else {
                        changeColorFlag[i] = true;
                        //                                colorChanged = true;
                    }
                    if (changeColorFlag[i])
                    {
                        if (!colorSW)
                            colorSW= createIrrKlangDevice();
                        if (!soundOff&&!switchColorSoundOnce[i]) {
                            colorSW->play2D("GAME  SOUND/colorswitch.wav",
                                    false);
                            switchColorSoundOnce[i]= true;
                        }
                        if (!changeBallColorIndexOnce)
                            ballColorIndex = random() % 4, changeBallColorIndexOnce = true;
                    }
                    glPopMatrix();
                }
            }
            glPopMatrix();
            glPopMatrix();
            if (!isPaused)
            {
                angle[i]+= 1;
                if (ballIsMoving_ClassicMode || (mode1YPos[i] + firstYPos <= ballPos&&!mode1Shape2_ClassicMode) ||
                    (mode1YPos[i] + /*firstYPos - 400*/250 >= ballPos&&!mode1Shape2_ClassicMode) || (mode1Shape2_ClassicMode&&mode1YPos[1]>mode1YPos[0]))
                {
                    /*cout << ballIsMoving_ClassicMode << " " << (mode1YPos[i] + 650 <= ballPos) << " "
                         << (mode1YPos[i] + 650 - 350 >= ballPos) << endl;*/
                    if ((ballPos <= mode1YPos[i] + firstYPos+100 && mode1YPos[i] + firstYPos <= 0)||(mode1YPos[1]+firstYPos-ballPos<=250&&mode1YPos[1] + firstYPos>= ballPos&&mode1Shape2_ClassicMode)/*||(mode1YPos[i] + 650 - 500 <= ballPos&&!ballIsMoving_ClassicMode)*/)
                        continue;
                    else {
                        mode1YPos[i] -= 2;
                        switchColorYPos[i]=mode1YPos[i];
//                        cout<<(mode1YPos[i]+650-ballPos<=400&&mode1YPos[i] + 650>= ballPos)<<mode1YPos[i]+650-ballPos<<"\n";
                    }

                }
                if ((mode1YPos[i] + firstYPos <= -270)&& !mode1Shape2_ClassicMode)
                {
                    mode1Shape[i + 1] = (rand() % 8 + rand() % 8) % 8/*(++tempCounter) %8*/;
                    maxShapes++;
                    mode1Shape2_ClassicMode = true;
                }
                if (mode1YPos[i] + firstYPos < -750)
                {
                    /*if (changeBallColorIndexOnce)
                    {
                        mode1YPos[i] = 0;
                        starFlag_ClassicMode[i] = false;
                        angle[i] = 0;
                        changeColorFlag[i] = changeColorFlag[i + 1];
                        maxShapes--;
                        scoreChangedOnce[i] = true;
                        changeBallColorIndexOnce = false;
                        mode1Shape2_ClassicMode = (maxShapes==1?  0:1);
                        //                    colorChanged = false;
                    }
                    else*/
                    {
                        mode1YPos[i] = mode1YPos[i + 1];
                        starFlag_ClassicMode[i] = starFlag_ClassicMode[i + 1];
                        mode1Shape[i] = mode1Shape[i + 1];
                        angle[i] = angle[i + 1];
                        changeColorFlag[i] = changeColorFlag[i + 1];
                        maxShapes--;
                        scoreChangedOnce[i] = scoreChangedOnce[i + 1];
                        scoreChangedOnce[i + 1] = true;
                        changeBallColorIndexOnce = false;
                        mode1Shape2_ClassicMode = false;
                        mode1YPos[i + 1] = mode1YPos[i + 1] = angle[i + 1] = 0;
                        starFlag_ClassicMode[i + 1] = changeColorFlag[i + 1] = false;
                        switchColorYPos[i]=switchColorYPos[i+1];
                        switchColorYPos[i+1]=0;
                        switchColorSoundOnce[i]=switchColorSoundOnce[i+1];
                        switchColorSoundOnce[i+1]=false;
                        //                    colorChanged = false;
                    }
                }
            }
        }

        if (!isPaused)
        {
            if (frames % 20 == 0) starScaleIndex = (starScaleIndex + 1) % 2;
            frames++;
        }
        glPushMatrix();
        glTranslated(0.0f, ballPos, 0.0f);
        drawBall_ClassicMode();
        glPopMatrix();
        if (!isPaused)
        {
            if (ballIsMoving_ClassicMode) {
                if (ballMoves < 8&&ballPos<=540-BALL_RADIUS*2) ballMoves += 1, ballPos += 15;
                else ballIsMoving_ClassicMode = false;
                allowedSpacePress_ClassicMode = false;
                allowedMouseClick_ClassicMode = false;
            } else {
                allowedMouseClick_ClassicMode = true;
                allowedSpacePress_ClassicMode = true;
                if (!ballFirstMove_ClassicMode && ballPos > -540 + BALL_RADIUS)
                    ballPos -= 5;
            }
        }
        if(pKeyPressed)
        {
            drawPauseResumeIcon();
        }
        ///ICONS
        {
            ///Sound Icon
            {
                loadTexture("Textures/sound.png");
                glPushMatrix();
                glTranslated(900, 420, 0);
                glColor4f(1, 1, 1, 1);
                glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(-30, -30);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(30, -30);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(30, 30);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(-30, 30);
                glEnd();
                glPopMatrix();
                glDisable(GL_TEXTURE_2D);
                if (soundOff)
                {
                    glPushMatrix();
                    glTranslated(900, 420, 0);
                    glRotated(45, 0, 0, 1);
                    glLineWidth(3);
                    glBegin(GL_LINES);
                    glColor4f(1, 0, 0, 1);
                    glVertex2d(0, 30);
                    glVertex2d(0, -30);
                    glEnd();
                    glPopMatrix();
                }
            }
            ///Music Icon
            {
                loadTexture("Textures/Music.png");
                glPushMatrix();
                glTranslated(900, 340, 0);
                glColor4f(1, 1, 1, 1);
                glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f); glVertex2f(-30, -30);
                glTexCoord2f(1.0f, 0.0f); glVertex2f(30, -30);
                glTexCoord2f(1.0f, 1.0f); glVertex2f(30, 30);
                glTexCoord2f(0.0f, 1.0f); glVertex2f(-30, 30);
                glEnd();
                glPopMatrix();
                glDisable(GL_TEXTURE_2D);
                if (musicOff)
                {
                    glPushMatrix();
                    glTranslated(900, 340, 0);
                    glRotated(45, 0, 0, 1);
                    glLineWidth(3);
                    glBegin(GL_LINES);
                    glColor4f(1, 0, 0, 1);
                    glVertex2d(0, 30);
                    glVertex2d(0, -30);
                    glEnd();
                    glPopMatrix();
                }
                glLineWidth(1);
            }
            ///Resume Icon && Pause icon
            {
                ///Resume
                if (!isPaused)
                {
                    loadTexture(
                            "Textures/Resume 2.png");
                    glPushMatrix();
                    glTranslated(820, 500, 0);
                    glColor4f(1, 1, 1, 1);
                    glBegin(GL_POLYGON);
                    glTexCoord2f(0.0f, 0.0f); glVertex2f(-30, -30);
                    glTexCoord2f(1.0f, 0.0f); glVertex2f(30, -30);
                    glTexCoord2f(1.0f, 1.0f); glVertex2f(30, 30);
                    glTexCoord2f(0.0f, 1.0f); glVertex2f(-30, 30);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);
                }
                    ///Pause icon at the middle
                else
                {
                    resumeCounter=0;
                    loadTexture("Textures/Pause 2.png");
                    glPushMatrix();
                    glTranslated(820, 500, 0);
                    glColor4f(1, 1, 1, 1);
                    glBegin(GL_POLYGON);
                    glTexCoord2f(0.0f, 0.0f); glVertex2f(-30, -30);
                    glTexCoord2f(1.0f, 0.0f); glVertex2f(30, -30);
                    glTexCoord2f(1.0f, 1.0f); glVertex2f(30, 30);
                    glTexCoord2f(0.0f, 1.0f); glVertex2f(-30, 30);
                    glEnd();
                    glPopMatrix();
                    glDisable(GL_TEXTURE_2D);
                }
            }

        }

    }
    else {
        usleep(sleepDuration);
        if (!loseSoundOnce && !soundOff)
        {
            ISoundEngine * classicModeLoseSound=createIrrKlangDevice();
            classicModeLoseSound->play2D("GAME  SOUND/dead.wav",false);
            loseSoundOnce=true;
        }
        /*ballIsMoving_ClassicMode = false;
        ballFirstMove_ClassicMode= true;
        allowedSpacePress_ClassicMode= true;
        allowedMouseClick_ClassicMode= true;
        mode1Shape2_ClassicMode=false;
        starFlag_ClassicMode[0]=false;
        starFlag_ClassicMode[1]=false;
        ballColorIndex=1;
        starScaleIndex=0;
        ballMoves=0;
        ballPos=0;
        angle[0]= 0;
        angle[1]=0;
        mode1YPos[0]= 0;
        mode1YPos[1]=0;
        mode1Shape[0]=1;
        mode1Shape[1]=0;
        maxShapes=1;
        frames=0;
        score_ClassicMode=0;
        scoreChangedOnce[0] = scoreChangedOnce[1]=true;
        changeColorFlag[0]= false;
        changeColorFlag[1]= false;
        changeBallColorIndexOnce= false;*/
//        colorChanged= false;
        bestScore=max(bestScore,score_ClassicMode);
        gameScreen=GAME_OVER_SCREEN;
       /* isPaused=false;
        loseSoundOnce=false;
        initCircles();*/
        //don't forget to set it to false in main menu
    }

//    cout<<score_ClassicMode<<"\n";
    // cout << "ballPos: " << ballPos << endl;
}
void gameOverScreen()
{
/// Game Over Text
    {
        glPushMatrix();
        loadTexture("Textures/game over.png");
        glTranslated(0, 350, 0);
        glColor4f(1, 1, 1, 1);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-177, -83);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(177, -83);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(177, 83);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-177, 83);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
    ///Score
    {
        glPushMatrix();
        glTranslated(0,-40,0);
        glPushMatrix();
        glTranslated(0, 200, 0);
        loadTexture( "Textures/SCORE.jpeg");
        glColor4f(1, 1, 1, 1);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-177, -25);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(177, -25);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(177, 25);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-177, 25);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        glPushMatrix();
        glScalef(0.4, 0.4, 1);
        glTranslated(-40,275,0);
        char strScore= int_to_str(score_ClassicMode,false)[0];
        glRasterPos2f(0.0f, 0.0f);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, strScore);
        glPopMatrix();
        glPopMatrix();
    }
    ///High Score
    {
        glPushMatrix();
        glTranslated(0,-190,0);
        glPushMatrix();
        glTranslated(0, 200, 0);
        loadTexture( "Textures/BEST SCORE.jpeg");
        glColor4f(1, 1, 1, 1);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-177, -25);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(177, -25);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(177, 25);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-177, 25);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPushMatrix();
        glScalef(0.4, 0.4, 1);
        glTranslated(-40,275,0);
        char strScore= int_to_str(bestScore,false)[0];
        glRasterPos2f(0.0f, 0.0f);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, strScore);
        glPopMatrix();

        glPopMatrix();
    }
    ///Replay
    {
        loadTexture("Textures/Replay3.png");
        glPushMatrix();
        glTranslated(0, -250, 0);
        glScaled(replayScale[replayScaleIndex],replayScale[replayScaleIndex],1);
        glColor4f(1, 1, 1, 1);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-40, -40);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(40, -40);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(40, 40);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-40, 40);
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        gameOverFramesCounter++;
        replayScaleIndex=(gameOverFramesCounter%30==0? (replayScaleIndex+1)%2:replayScaleIndex);
    }
    ///Sound
    {
        loadTexture("Textures/sound.png");
        glPushMatrix();
        glTranslated(-150, -250, 0);
        glColor4f(1, 1, 1, 1);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-40, -40);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(40, -40);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(40, 40);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-40, 40);
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        if (soundOff)
        {
            glPushMatrix();
            glTranslated(-150, -250, 0);
            glRotated(45,0,0,1);
            glLineWidth(3);
            glBegin(GL_LINES);
            glColor4f(1,0,0,1);
            glVertex2d(0,40);
            glVertex2d(0,-40);
            glEnd();
            glPopMatrix();
        }
    }
    ///Music
    {
        loadTexture("Textures/Music.png");
        glPushMatrix();
        glTranslated(150, -250, 0);
        glColor4f(1, 1, 1, 1);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-40, -40);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(40, -40);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(40, 40);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-40, 40);
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        if (musicOff)
        {
            glPushMatrix();
            glTranslated(150, -250, 0);
            glRotated(45,0,0,1);
            glLineWidth(3);
            glBegin(GL_LINES);
            glColor4f(1,0,0,1);
            glVertex2d(0,40);
            glVertex2d(0,-40);
            glEnd();
            glPopMatrix();
        }
    }

}

void mainMenu()
{
    ///Initialization
    {
        ballIsMoving_ClassicMode = false;
        ballFirstMove_ClassicMode= true;
        allowedSpacePress_ClassicMode= true;
        allowedMouseClick_ClassicMode= true;
        mode1Shape2_ClassicMode=false;
        starFlag_ClassicMode[0]=false;
        starFlag_ClassicMode[1]=false;
        starScaleIndex=0;
        ballMoves=0;
        ballPos=0;
        angle[0]= 0;
        angle[1]=0;
        mode1YPos[0]= 0;
        mode1YPos[1]=0;
        mode1Shape[0]=1;
        mode1Shape[1]=0;
        maxShapes=1;
        frames=0;
        score_ClassicMode=0;
        scoreChangedOnce[0]=scoreChangedOnce[1] =true;
        changeColorFlag[0]= false;
        changeColorFlag[1]= false;
        changeBallColorIndexOnce= false;
//        colorChanged= false;
        isPaused=false;
        loseSoundOnce=false;
        GameOver_ClassicMode= false;
        ballColorIndex=random()%100%4;
        resumeCounter=0;
        pKeyPressed=false;
        switchColorYPos[0]=switchColorYPos[1]=0;
        switchColorSoundOnce[0]=switchColorSoundOnce[1]=false;
        initCircles();
    }
/// Switch Color Text
    {
        glPushMatrix();
        loadTexture("Textures/switch color text.png");
        glTranslated(0, 250, 0);
        glColor4f(1, 1, 1, 1);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-177, -83);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(177, -83);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(177, 83);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-177, 83);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
    ///Start Game
    {
        glPushMatrix();
        glTranslated(0, 25, 0);

        glPushMatrix();
        glScaled(0.5, 0.5, 1);
        {
            glPushMatrix();
            glRotated(mainMenuAngle *1.5 + 45, 0, 0, 1);
            draw_outer_Circle_ClassicMode();
            draw_inner_circle_ClassicMode();
            glPopMatrix();
            glPushMatrix();
            glRotated(-mainMenuAngle * 1.25 + 45, 0, 0, 1);
            outer_second_circle_ClassicMode();
            draw_inner_circle2_ClassicMode();
            glPopMatrix();
            glPushMatrix();
            glRotated(mainMenuAngle + 45, 0, 0, 1);
            outer_third_circle_ClassicMode();
            draw_inner_circle3_ClassicMode();
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        glTranslated(5,0,0);
        loadTexture( "Textures/start game.png");
        glColor4f(1, 1, 1, 1);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-40, -40);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(40, -40);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(40, 40);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-40, 40);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glPopMatrix();
    }
    ///Sound
    {
        loadTexture("Textures/sound.png");
        glPushMatrix();
        glTranslated(-150, -150, 0);
        glColor4f(1, 1, 1, 1);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-40, -40);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(40, -40);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(40, 40);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-40, 40);
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        if (soundOff)
        {
            glPushMatrix();
            glTranslated(-150, -150, 0);
            glPushMatrix();
            glRotated(45,0,0,1);
            glLineWidth(3);
            glBegin(GL_LINES);
            glColor4f(1,0,0,1);
            glVertex2d(0,40);
            glVertex2d(0,-40);
            glEnd();
            glPopMatrix();
            glPopMatrix();
        }
    }
    ///Music
    {
        loadTexture("Textures/Music.png");
        glPushMatrix();
        glTranslated(0, -150, 0);
        glColor4f(1, 1, 1, 1);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-40, -40);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(40, -40);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(40, 40);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-40, 40);
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        if (musicOff)
        {
            glPushMatrix();
            glTranslated(0, -150, 0);
            glRotated(45,0,0,1);
            glLineWidth(3);
            glBegin(GL_LINES);
            glColor4f(1,0,0,1);
            glVertex2d(0,40);
            glVertex2d(0,-40);
            glEnd();
            glPopMatrix();
        }
    }
    ///Game controls
    {
        loadTexture("Textures/keyboard-and-mouse.png");
        glPushMatrix();
        glTranslated(150, -150, 0);
        glColor4f(1, 1, 1, 1);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-25, -25);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(25, -25);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(25, 25);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-25, 25);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glLineWidth(2);
        drawCircle(0,0,40,40,255,255,255);
        glPopMatrix();
    }
    mainMenuAngle+=2;
    glLineWidth(1);
}
void gameControls()
{
    glPushMatrix();
    loadTexture("Textures/CONTROLS.png");
    glTranslated(0, 0, 0);
    glScaled(1.3,1.3,1);
    glColor4f(1, 1, 1, 1);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-408, -327);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(408, -327);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(408, 327);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-408, 327);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawCircle(double x0, double y0, double rX, double rY, double R1,double G1, double B1, double alfa1)
{
    glBegin(GL_LINE_LOOP);
    glColor4d(R1/255.0f,G1/255.0f,B1/255.0f,alfa1);
    for (int i = 0; i <= 360; ++i)
    {
        double theta=i*3.14/180;
        glVertex2d(rX*cos(theta)+x0,-rY*sin(theta)-y0);
    }
    glEnd();
}
void OnDisplay()
{
    glClearColor(28/255.0f,28/255.0f,28/255.0f,1);
    glClear(GL_COLOR_BUFFER_BIT);
    SetTransformations();
    glEnable (GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLoadIdentity();

    //drawing code

    if (gameScreen!=MAIN_SCREEN)
    {
        ///Back icon
        {
            glPushMatrix();
            glTranslated(900,500,0);
            loadTexture( "Textures/icons8-back-100.png");
            glColor4f(1, 1, 1, 1);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-40, -40);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(40, -40);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(40, 40);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-40, 40);
            glEnd();
            glDisable(GL_TEXTURE_2D);
            glPopMatrix();
        }
    }
    switch (gameScreen)
    {
        case MAIN_SCREEN: {
            mainMenu();
            break;
        }
        case CLASSIC_MODE:
        {
            classicMode();
            break;
        }
        case GAME_CONTROLS:
        {
            gameControls();
            break;
        }
        case GAME_OVER_SCREEN:
        {
            gameOverScreen();
        }
        default:;
    }

    glutSwapBuffers();
}
