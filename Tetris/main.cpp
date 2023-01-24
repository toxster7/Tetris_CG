#include <Windows.h>

#include <GL/glut.h>                

#include<GL/GL.h>

#include <GL/GLU.h>                    

#include <cmath>

#include <fstream>

#include <iostream>

#include <MMSystem.h>

#include <time.h>

#include<SOIL2\SOIL2.h>                

#define SOIL_CHECK_FOR_GL_ERRORS

using namespace std;

int record = 0;                        //record

int record_saved = 0;                   

int  width = 700, height = 400;        //window

float ggg = 0, ggg1 = 0, ggg2 = 0;  //camera

short tip = 0;                        //figure

GLuint    num[10];                    // numbers texture 

GLuint    texture[3];                    //texture

bool FULEKRAN = false;                    //fullscreen

bool PSUSE = 0;                        //pause

bool KLAVIATURA[256] = { 0 };        //keyboard

short Drawin1[50][50];                //field

short Drawin2[50][50];                //field copy

short grav = 500, grav1 = 300;        //falling speed

bool t1 = 0, TBL = 0;                //record desk   

float i = 0;                        

float DeltaTime = 0;                //time to draw the scene

void ZS_L();

void ZS_R();

//Soil2
GLuint loadTexture(char const* path) {

    GLuint textureID = 0;

    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrComponents;

    unsigned char* data = SOIL_load_image(path, &width, &height, &nrComponents, 0);

    if (data) {

        GLenum format;
        if (nrComponents == 3)    format = GL_RGB;

        else    if (nrComponents == 4)  format = GL_RGBA;

        else     format = GL_RED;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        SOIL_free_image_data(data);

    }else {

        std::cout << "Texture failed to load at path: " << path << std::endl;

        SOIL_free_image_data(data);

    }
    return textureID;
}

GLvoid LoadGLTextures(int i, char* fbmp){

    texture[i] = loadTexture(fbmp);
}

GLvoid LoadGLTextures2(int i, char* fbmp){

    num[i] = loadTexture(fbmp);

}

struct  Pixel{

    float R, G, B;
};

//==========================================FOG
void tuman(bool ON_OFF, int fogfilter, float intensivnost, Pixel color_tumana)
{
    if (ON_OFF)

    {
        GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR };                                
        GLfloat fogColor[4] = { color_tumana.R,color_tumana.G, color_tumana.B, 1.0f };    
        glFogi(GL_FOG_MODE, fogMode[fogfilter]);                                        
        glFogfv(GL_FOG_COLOR, fogColor);                                                
        glFogf(GL_FOG_DENSITY, intensivnost);                                           
        glHint(GL_FOG_HINT, GL_DONT_CARE);                                               
        glFogf(GL_FOG_START, 1.0f);                                                           
        glFogf(GL_FOG_END, 5.0f);                                                        
}

    if (ON_OFF)
        glEnable(GL_FOG);
    else
        glDisable(GL_FOG);
}

//===============================
void key_S();


void SAVES_P(char* SV){

    ofstream jjj(SV);

    jjj << ggg << endl << ggg1 << endl << ggg2 << endl << record;

    jjj.close();
}

void SAVES_OPEN(char* SV){

    ifstream jjj(SV);

    jjj >> ggg >> ggg1 >> ggg2 >> record;

    jjj.close();
}

bool blok_z = 0;
bool blok_x = 0;
float SH = 0.5;
int gggg = 0;;

short Joni = 0;

//=================================================================================================

bool Tablo();

void DSinhron(){//===============arrays sync

    {
        short temp_1 = -8;
        short temp_2 = 0;
        while (temp_2 <= 15) {
            while (temp_1 <= 8){

                if (Drawin2[temp_1 + 8][temp_2] > 0) { Drawin1[temp_1 + 8][temp_2] = Drawin2[temp_1 + 8][temp_2]; }

                temp_1 += 1;
            }
            temp_1 = -8;

            temp_2++;
        }
    }

    {

        {

            short temp_1 = -8;

            short temp_2 = 0;

            while (temp_2 <= 15) {

                while (temp_1 <= 8)

                {

                    Drawin2[temp_1 + 8][temp_2] = 0;

                    temp_1 += 1;

                }

                temp_1 = -8;

                temp_2++;

            }

            //Drawin2

        }

    }

    //Drawin2

}//=======================Drawin1

short TEmp_Disply[50][50];
short CUTIC222 = 0;

void RANDI1()//====================Randomly creates figure

{
    CUTIC222 = 0;
    int dhs = rand() % 5;  //random 0 to 4 (figure type)

    if (dhs == 0) { Drawin2[8][15] = 1; Drawin2[9][15] = 1; Drawin2[10][15] = 1; tip = 0; }

    else                                    //figure 0 line

        if (dhs == 1) { Drawin2[8][15] = 1; tip = 1; }

        else                                                                                // 1 small cube

            if (dhs == 2) { Drawin2[8][15] = 1; Drawin2[9][15] = 1; Drawin2[8][14] = 1;  Drawin2[9][14] = 1; tip = 2; }

            else                //2 Z

                if (dhs == 3) {

                    Drawin2[8][14] = 1; Drawin2[9][14] = 1; Drawin2[10][14] = 1;  Drawin2[9][15] = 1; Drawin2[9][16] = 1; tip = 3;//figure 3
                }
                else {

                    dhs = rand() % 2;

                    if (dhs == 0) {
                        Drawin2[7][15] = 1; Drawin2[8][15] = 1; Drawin2[8][14] = 1;  Drawin2[9][14] = 1;; tip = 5;
                    }
                    else {

                        Drawin2[8][15] = 1; Drawin2[9][15] = 1; Drawin2[9][14] = 1;  Drawin2[10][14] = 1;; tip = 6;                //4
                    }
                }
}

void Onul2()

{
    {//===============reset arrays

        short temp_1 = -8;
        short temp_2 = 0;
        while (temp_2 <= 15) {

            while (temp_1 <= 8){
                Drawin2[temp_1 + 8][temp_2] = 0;

                temp_1 += 1;
            }
            temp_1 = -8;

            temp_2++;
        }
    }//=======================Drawin1
}

//===========================turn over the figure

void  Obrt()
{
    {
        short temp_1 = -8; short temp_2 = 0;

        while (temp_2 <= 15) {
            while (temp_1 <= 8) {

                if (Drawin2[temp_1 + 8][temp_2] > 0 && temp_1 > -7 && temp_1 < 7 && temp_2 < 13) {

                    if (tip == 6) {

                        Onul2();

                        if (CUTIC222 == 0) {

                            Drawin2[temp_1 + 8 + 1][temp_2] = 1; Drawin2[temp_1 + 8 + 1][temp_2 + 1] = 1; Drawin2[temp_1 + 8][temp_2 + 1] = 1;    Drawin2[temp_1 + 8][temp_2 + 2] = 1; 

                            CUTIC222 = 1;
                        }

                        else
                            if (CUTIC222 == 1) {

                                Drawin2[temp_1 + 8 + 1][temp_2 + 1] = 1; Drawin2[temp_1 + 8][temp_2 + 1] = 1; Drawin2[temp_1 + 8][temp_2] = 1;    Drawin2[temp_1 + 8 - 1][temp_2] = 1; 

                                CUTIC222 = 0;

                            }

                        return;

                    }

                    if (tip == 5) {

                        Onul2();

                        if (CUTIC222 == 0)
                        {
                            Drawin2[temp_1 + 8 + 1][temp_2] = 1; Drawin2[temp_1 + 8 + 1][temp_2 + 1] = 1; Drawin2[temp_1 + 8][temp_2] = 1;    Drawin2[temp_1 + 8][temp_2 - 1] = 1; 

                            CUTIC222 = 1;
                        }
                        else
                            if (CUTIC222 == 1) {

                                Drawin2[temp_1 + 8 - 1][temp_2 + 1] = 1; Drawin2[temp_1 + 8][temp_2 + 1] = 1; Drawin2[temp_1 + 8][temp_2] = 1;    Drawin2[temp_1 + 8 + 1][temp_2] = 1; 

                                CUTIC222 = 0;
                            }
                        return;
                    }

                    if (tip == 0) {

                        if (CUTIC222 == 0) {

                            Onul2();

                            Drawin2[temp_1 + 8 + 1][temp_2 - 1] = 1; Drawin2[temp_1 + 8 + 1][temp_2] = 1;  Drawin2[temp_1 + 8 + 1][temp_2 + 1] = 1;  

                            CUTIC222 = 1;
                        }
                        else {

                            Onul2();

                            Drawin2[temp_1 + 8 - 1][temp_2] = 1; Drawin2[temp_1 + 8][temp_2] = 1;  Drawin2[temp_1 + 8 + 1][temp_2] = 1;  

                            CUTIC222 = 0;
                        }
                        return;
                    }

                    if (tip == 3) {

                        if (CUTIC222 == 0) {
                            Onul2();

                            Drawin2[temp_1 + 8][temp_2] = 1; Drawin2[temp_1 + 8][temp_2 + 1] = 1;  Drawin2[temp_1 + 8][temp_2 + 2] = 1; Drawin2[temp_1 + 8 - 1][temp_2 + 1] = 1;

                            CUTIC222 = 1;
                        }

                        else

                            if (CUTIC222 == 1) {

                                Onul2();

                                Drawin2[temp_1 + 8][temp_2] = 1; Drawin2[temp_1 + 8 + 1][temp_2] = 1;  Drawin2[temp_1 + 8 + 2][temp_2] = 1;           Drawin2[temp_1 + 8 + 1][temp_2 - 1] = 1;

                                CUTIC222 = 2;
                            }

                            else if (CUTIC222 == 2) {
                                Onul2();

                                Drawin2[temp_1 + 8][temp_2] = 1; Drawin2[temp_1 + 8][temp_2 + 1] = 1;  Drawin2[temp_1 + 8][temp_2 + 2] = 1;           Drawin2[temp_1 + 8 + 1][temp_2 + 1] = 1;

                                CUTIC222 = 3;

                            }
                            else if (CUTIC222 == 3) {

                                Onul2();

                                Drawin2[temp_1 + 8 - 1][temp_2] = 1; Drawin2[temp_1 + 8 + 1 - 1][temp_2] = 1;  Drawin2[temp_1 + 8 + 2 - 1][temp_2] = 1;           Drawin2[temp_1 + 8][temp_2 + 1] = 1;

                                CUTIC222 = 0;
                            }
                        return;
                    }
                }

                temp_1 += 1;
            }
            temp_1 = -8;
            temp_2++;
        }

    }

    //====================turn over;
}


void onULmasT()

{
    {//===============reset Drawin1

        short temp_1 = -8;

        short temp_2 = 0;

        while (temp_2 <= 15) {

            while (temp_1 <= 8){

                TEmp_Disply[temp_1 + 8][temp_2] = 0;

                temp_1 += 1;
            }

            temp_1 = -8;

            temp_2++;

        }
    }
}


void onULmas()

{
    {
        short temp_1 = -8;

        short temp_2 = 0;

        while (temp_2 <= 15) {

            while (temp_1 <= 8){

                Drawin1[temp_1 + 8][temp_2] = 0;

                Drawin2[temp_1 + 8][temp_2] = 0;

                TEmp_Disply[temp_1 + 8][temp_2] = 0;

                temp_1 += 1;

            }

            temp_1 = -8;

            temp_2++;

        }

    }//=======================Drawin1

}


void Restart(){

    onULmas();

    TBL = 0;

    record_saved = 0;

    RANDI1();

    PSUSE = 0;

    PlaySound((char*)"Sound\\Star_Struck.wav", NULL, SND_ASYNC);  //  

}


bool Tablo()

{
    bool ewq = 0;

    {//===============reset

        short temp_1 = -8;

        short temp_2 = 0;

        while (temp_1 <= 8) {

            if (Drawin1[temp_1 + 8][15] == 1)  ewq = 1;

            temp_1 += 1;
        }
    }//=======================Drawin1

    if (ewq) {

        PlaySound((char*)"Sound\\Gong.wav", NULL, SND_ASYNC);  // 

        TBL = 1;

    }

    return ewq;

}

int gddd = 0;

//Score panel

void panel_1()

{
    if (!t1 || TBL)
    {
        //===================body

        int ss = 90;

        glColor3f(255, 255, 255);

        glBindTexture(GL_TEXTURE_2D, texture[0]);

        glBegin(GL_QUADS);

        glTexCoord2f(0, 0);        glVertex3f(ss, 0, -ss);  

        glTexCoord2f(0, 9);        glVertex3f(ss, 0, ss);  

        glTexCoord2f(9, 9);        glVertex3f(-ss, 0, ss);  

        glTexCoord2f(9, 0);        glVertex3f(-ss, 0, -ss);  

        glEnd();

        //===================body
    }

    int ss1 = 20;

    float ss1z = -10;

    glColor3f(255, 255, 255);

    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);    glVertex3f(ss1, 0, ss1z);  

    glTexCoord2f(0, 5);    glVertex3f(ss1, ss1, ss1z);  

    glTexCoord2f(5, 5);    glVertex3f(-ss1, ss1, ss1z);  

    glTexCoord2f(5, 0);    glVertex3f(-ss1, 0, ss1z);  

    glEnd();
    ss1z += 0.0005;

    glColor3f(200, 70, 70);

    if (sin(i) * 9 > 8.99){

        gddd++;

        if (gddd >= 10)      gddd = 0;
    }

    float ffr = 1;
    float record1 = record;

    float rahunoc1 = record_saved;

    float locX, locY;

    int Jff = 0;

    locX = 5;

    locY = 12.5 + cos(i / 3) / 2 - 1;

    while (Jff < 7) {

        if (rahunoc1 > 0) {

            gddd = (rahunoc1 / 10 - floor(rahunoc1 / 10)) * 10;

            rahunoc1 = rahunoc1 / 10;

        }

        else gddd = 0;

        glBindTexture(GL_TEXTURE_2D, num[gddd]);

        glBegin(GL_QUADS);


        glTexCoord2f(0, 0);                glVertex3f(5 + locY, locX, ss1z);      

        glTexCoord2f(0, -ffr);            glVertex3f(5 + locY, 8 + locX, ss1z);  

        glTexCoord2f(-ffr, -ffr);            glVertex3f(0 + locY, 8 + locX, ss1z);     

        glTexCoord2f(-ffr, 0);        glVertex3f(0 + locY, locX, ss1z);   

        glEnd();
        Jff++;

        locY -= 5;
    }

    Jff = 0;

    locX = 1;

    locY = 8 + sin(i / 3) / 2 - 1;



    while (Jff < 7) {
        if (record1 > 0)  {

            gddd = (record1 / 10 - floor(record1 / 10)) * 10;

            record1 = record1 / 10;

        }

        else gddd = 0;

        glBindTexture(GL_TEXTURE_2D, num[gddd]);

        glBegin(GL_QUADS);



        glTexCoord2f(0, 0);            glVertex3f(2 + locY, locX, ss1z); 

        glTexCoord2f(0, -ffr);        glVertex3f(2 + locY, 3 + locX, ss1z);   

        glTexCoord2f(-ffr, -ffr);    glVertex3f(0 + locY, 3 + locX, ss1z); 

        glTexCoord2f(-ffr, 0);        glVertex3f(0 + locY, locX, ss1z);  

        glEnd();

        Jff++;

        locY -= 2;

    }


    glBindTexture(GL_TEXTURE_2D, texture[2]);

    Jff = 0;
    locX = 3;

    locY = -8 + sin(i / 3) / 2 - 1;

    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);         glVertex3f(4 + locY, locX, ss1z); 

    glTexCoord2f(0, -ffr);        glVertex3f(4 + locY, 1 + locX, ss1z);  

    glTexCoord2f(-ffr, -ffr);    glVertex3f(0 + locY, 1 + locX, ss1z);  

    glTexCoord2f(-ffr, 0);        glVertex3f(0 + locY, locX, ss1z);  

    glEnd();

}


int ffff = 0;


float lostClockg = clock(); 

//Scene ============================================================================
bool DrawGLScene()     {

    //=======================================================================Camera Position
    glRotatef(20, 1, 0, 0);   

    glTranslatef(0, -7, ggg1);

    glRotatef(ggg, 1, 0, 0);

    glTranslatef(0, 2.4 + ggg2, sin(i / 5));

    glRotatef(sin(i / 2), sin(i / 3), 1.0f, 0.0f);

    Pixel tumanColor = { 0.8, 0.6, 1 };

    tuman(true, 1, 0.03 + sin(i / 10) / 500, tumanColor);

    glDisable(GL_TEXTURE_GEN_S);

    glDisable(GL_TEXTURE_GEN_T);

    if (TBL) { panel_1(); } //draw Score panel

    else {

        if (t1)   panel_1();

        glBegin(GL_QUADS);

        glVertex3f(-10.0, 0000.1, -6);

        glVertex3f(-10, 0000.1, -4);

        glVertex3f(10, 0000.1, -4);

        glVertex3f(10, 0000.1, -6);

        glEnd();


        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

        glEnable(GL_TEXTURE_GEN_S);

        glEnable(GL_TEXTURE_GEN_T);


        if (Tablo()) return 0;


        //Gravitation

        if (grav <= 0)

        {
            grav = grav1;

            {
                short temp_1 = -8;  short temp_2 = 0; while (temp_2 <= 15) {

                    while (temp_1 <= 8) { TEmp_Disply[temp_1 + 8][temp_2 - 1] = Drawin2[temp_1 + 8][temp_2]; temp_1 += 1; }

                    temp_1 = -8; temp_2++;

                } }

            { short temp_1 = -8;  short temp_2 = 0; while (temp_2 <= 15) {

                while (temp_1 <= 8)
                {
                    Drawin2[temp_1 + 8][temp_2] = TEmp_Disply[temp_1 + 8][temp_2]; TEmp_Disply[temp_1 + 8][temp_2] = 0; temp_1 += 1;

                } temp_1 = -8; temp_2++;

            } }

        }

        else

        {
            grav -= DeltaTime;
        }
        glColor3f(0, 0, 150);

        {//==============draw array 1&2

            short temp_1 = -8; short temp_2 = 0;

            while (temp_2 <= 15)

            {
                while (temp_1 <= 8)
                {

                    if (Drawin1[temp_1 + 8][temp_2] > 0) {

                        glPushMatrix(); glTranslatef(temp_1, temp_2 + 0.5, -5);

                        float ss1 = 0.5;

                        glBindTexture(GL_TEXTURE, texture[3]);

                        glutSolidCube(1);

                        glPopMatrix();
                    }

                    if (Drawin2[temp_1 + 8][temp_2] > 0) { glPushMatrix(); glTranslatef(temp_1, temp_2 + 0.5, -5); glutSolidCube(1); glPopMatrix(); }
                    temp_1 += 1;

                }

                temp_1 = -8;

                temp_2++;

            }

        }//=======================Drawin1


        //====================blue cubes

        glColor3f(0, 0, 255);



        glPushMatrix();

        glTranslatef(10, 15, -3);

        glutSolidCube(1);

        glTranslatef(-20, 0, 0);

        glutSolidCube(1);

        glPopMatrix();

        //====================blue cubes;

         //===================red coloumns 

        { glColor3f(255, 0, 0);

        float TeMping = 0.5;

        while (TeMping < 20)

        {
            glPushMatrix();

            glTranslatef(-9, TeMping, -4);

            glutSolidCube(1);

            glPopMatrix();

            glPushMatrix();

            glTranslatef(9, TeMping, -4);

            glutSolidCube(1);

            glPopMatrix();

            TeMping += 1.5;

            TeMping += (sin(i / 5) + 0.5) / 4;

        }  }

        { glColor3f(255, 0, 0);

        float TeMping = 0.5;

        while (TeMping < 20)

        {
            glPushMatrix();

            glTranslatef(-9, TeMping, -6);

            glutSolidCube(1);

            glPopMatrix();

            glPushMatrix();

            glTranslatef(9, TeMping, -6);

            glutSolidCube(1);

            glPopMatrix();

            TeMping += 1.5;

            TeMping += (sin(i / 5) + 0.5) / 4;

        }  }

        //===================red coloumns;



        glDisable(GL_TEXTURE_GEN_S);

        glDisable(GL_TEXTURE_GEN_T);


        //===================grass

        int ss = 90;

        glColor3f(255, 255, 255);

        glBindTexture(GL_TEXTURE_2D, texture[0]);

        glBegin(GL_QUADS);

        glTexCoord2f(0, 0);         glVertex3f(ss, 0, -ss);

        glTexCoord2f(0, 9);         glVertex3f(ss, 0, ss);

        glTexCoord2f(9, 9);         glVertex3f(-ss, 0, ss);

        glTexCoord2f(9, 0);         glVertex3f(-ss, 0, -ss);

        glEnd();



        //====================interaction falling figures with laying figures

        bool OB_VZ = 0;

        {

            short temp_1 = -8; short temp_2 = 0;

            while (temp_2 <= 15) {
                while (temp_1 <= 8) {

                    if (Drawin2[temp_1 + 8][temp_2] > 0) { if (temp_2 == 0) { OB_VZ = 1; }         if (Drawin1[temp_1 + 8][temp_2 - 1] > 0) { OB_VZ = 1; } }

                    temp_1 += 1;
                }
                temp_1 = -8;

                temp_2++;
            }

        }

        if (OB_VZ) {

            DSinhron();

            RANDI1();

            PlaySound((char*)"Sound\\Sirrah.wav", NULL, SND_ASYNC);  //   

        }

        //====================;
        //===================================================================eliminate line

        bool JJ = 0; {

            short temp_1 = -8; short temp_2 = 0;

            while (temp_2 <= 15) {

                while (temp_1 <= 8) {

                    if (Drawin1[temp_1 + 8][temp_2] > 0) {}

                    else JJ = 1;

                    temp_1 += 1;
                }



                if (!JJ) {

                    PlaySound((char*)"Sound\\Fomalhaut.wav", NULL, SND_ASYNC); record_saved++; if (record_saved > record) record = record_saved;

                    temp_1 = -8;

                    while (temp_1 <= 8)

                    {
                        if (Drawin1[temp_1 + 8][temp_2] > 0) { Drawin1[temp_1 + 8][temp_2] = 0; };

                        temp_1 += 1;

                    }

                    short tempK[50][50];

                    int vbn = temp_2;//-------------------------------------------

                    temp_1 = -8;

                    while (temp_2 <= 15) {

                        while (temp_1 <= 8) {
                            if (Drawin1[temp_1 + 8][temp_2 + 1] > 0) { Drawin1[temp_1 + 8][temp_2] = Drawin1[temp_1 + 8][temp_2 + 1];    Drawin1[temp_1 + 8][temp_2 + 1] = 0; };

                            temp_1++;
                        }

                        temp_1 = -8;

                        temp_2++;

                    }
                    temp_2 = vbn;
                }

                JJ = 0;

                temp_1 = -8;

                temp_2++;

            }

        }

        //===================================================================

    }
    return true;
}


void on_paint()//   DrawGLScene(  );
{
    glLoadIdentity();            

    DrawGLScene(); 


    glMatrixMode(GL_PROJECTION);

    glClearColor(0.8, 0.6, 1, 1);               

    glViewport(0, 0, width, height); 

    glLoadIdentity();//скидання положення

    gluPerspective(60, (GLfloat)width / height, 1, 100);

    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void ZS_R(){
    {
        bool KR = 0;
        {//=======================Drawin1\

            short temp_2 = 0;

            while (temp_2 <= 15){

                if (Drawin2[16][temp_2] > 0) { KR = 1; }
                temp_2++;
            }
         }//=======================Drawin1\

        if (!KR){

            onULmasT();

            //TEmp_Disply

            {//===============move the figure
                short temp_1 = -8;

                short temp_2 = 0;

                while (temp_2 <= 15){

                    while (temp_1 <= 8) {

                        if (Drawin2[temp_1 + 8][temp_2] > 0)  {

                            TEmp_Disply[temp_1 + 8 + 1][temp_2] = 1; Drawin2[temp_1 + 8][temp_2] = 0;
                         }
                        temp_1 += 1;
                    }

                    temp_1 = -8;

                    temp_2++;
                }
            }//=======================Drawin1

            {//===============

                short temp_1 = -8;

                short temp_2 = 0;

                while (temp_2 <= 15) {

                    while (temp_1 <= 8) {
                        {
                            Drawin2[temp_1 + 8][temp_2] = TEmp_Disply[temp_1 + 8][temp_2];
                        }
                        temp_1 += 1;

                    }
                    temp_1 = -8;

                    temp_2++;
                }

            }//=======================Drawin1

        }

        onULmasT();

    }

}
void  Obrt();

void on_keyboard(unsigned char key, int x, int y) //event listner keyboard---------------------------------------------------------------
{}

//------------------------------------------------------------------------------------------------------

void on_size(int w, int h)
{
    width = w;

    height = h;

    if (height == 0) height = 1;                           
}

float ClockOld;

void on_timer(int value) //Timer

{

    DeltaTime = clock() - ClockOld;        //time wasted on drawing and calculating

    ClockOld = clock();                    //reset timet

    i = (float)clock() / 500.0f; 

    key_S(); 


    if (KLAVIATURA[80]) if (PSUSE) PSUSE = 0; else PSUSE = 1;//pause

    if (KLAVIATURA[122]) {

        Sleep(50);

        if (!FULEKRAN) { glutFullScreen(); ShowCursor(0); FULEKRAN = 1; }

        else { glutReshapeWindow(700, 400); ShowCursor(1); glutPositionWindow(50, 50); FULEKRAN = 0; }

    }//F1 

    if (KLAVIATURA[27]) { SAVES_P((char*)"SAVE\\SAVE_1.txt"); exit(0); } //ESC 

    if (KLAVIATURA[32]) t1 = 1; else t1 = 0;                  


    if (!PSUSE)  {

        //´controls

        if (KLAVIATURA[38] || KLAVIATURA[87]) { Obrt();        Sleep(100); }  //tuen over

        if (KLAVIATURA[39] || KLAVIATURA[68]) { ZS_R();        Sleep(100); }  //move right

        if (KLAVIATURA[37] || KLAVIATURA[65]) { ZS_L();        Sleep(100); }  //move left

        if (KLAVIATURA[40] || KLAVIATURA[83]) { grav -= DeltaTime * 5; }  //Speed up

        if (KLAVIATURA[82]) { Restart();  Sleep(200); }                      //R reset


        on_paint(); 

    }

    glutTimerFunc(0, on_timer, 0); 
}

void ZS_L()//<<

{
    {
        bool KR = 0;

        {//=======================Drawin1\

            short temp_2 = 0;

            while (temp_2 <= 15)

            {
                if (Drawin2[0][temp_2] > 0) { KR = 1; }

                temp_2++;

            }

        }//=======================Drawin1\

        onULmasT();

        //TEmp_Disply

        if (!KR) {
             {

                short temp_1 = -8;

                short temp_2 = 0;

                while (temp_2 <= 15) {

                    while (temp_1 <= 8) {

                        if (Drawin2[temp_1 + 8][temp_2] > 0)



                        {

                            TEmp_Disply[temp_1 + 8 - 1][temp_2] = 1; Drawin2[temp_1 + 8][temp_2] = 0;

                        }

                        temp_1 += 1;

                    }

                    temp_1 = -8;

                    temp_2++;

                }

            }//=======================Drawin1\


            {

                short temp_1 = -8;

                short temp_2 = 0;

                while (temp_2 <= 15) {

                    while (temp_1 <= 8)  {

                       {

                            Drawin2[temp_1 + 8][temp_2] = TEmp_Disply[temp_1 + 8][temp_2];

                        }

                        temp_1 += 1;

                    }

                    temp_1 = -8;

                    temp_2++;

                }
            }
        }//=======================Drawin1

        onULmasT();

    }//<<

}


void key_S(){

    short KEY_temp = 0;

    while (KEY_temp + 1 < 256){

        KEY_temp++;

        if (GetAsyncKeyState(KEY_temp)) KLAVIATURA[KEY_temp] = 1;

        else
            KLAVIATURA[KEY_temp] = 0;

    }
}


void RANDI1();

void  main(int* argcp, char** argv)

{

    srand(time(0));                                 

    int a = 0;

    argcp = &a;

    glutInit(argcp, NULL);                                        

    glutCreateWindow("3D Tetris");        

    glEnable(GL_TEXTURE_2D);       

    glShadeModel(GL_SMOOTH);     

    glEnable(GL_DEPTH_TEST);                                   

    glutDisplayFunc(on_paint);                                   

    glutReshapeFunc(on_size);                                  

    glutKeyboardFunc(on_keyboard);                              

    if (FULEKRAN) { glutFullScreen(); ShowCursor(0); }           

    SAVES_OPEN((char*)"SAVE\\SAVE_1.txt");                       



    LoadGLTextures(0, (char*)"textur\\tr1.bmp");                     

    LoadGLTextures(1, (char*)"textur\\K1.bmp");

    LoadGLTextures(2, (char*)"textur\\num\\rek.bmp");

    LoadGLTextures2(0, (char*)"textur\\num\\0.bmp");

    LoadGLTextures2(1, (char*)"textur\\num\\1.bmp");

    LoadGLTextures2(2, (char*)"textur\\num\\2.bmp");

    LoadGLTextures2(3, (char*)"textur\\num\\3.bmp");

    LoadGLTextures2(4, (char*)"textur\\num\\4.bmp");

    LoadGLTextures2(5, (char*)"textur\\num\\5.bmp");

    LoadGLTextures2(6, (char*)"textur\\num\\6.bmp");

    LoadGLTextures2(7, (char*)"textur\\num\\7.bmp");

    LoadGLTextures2(8, (char*)"textur\\num\\8.bmp");

    LoadGLTextures2(9, (char*)"textur\\num\\9.bmp");

    Restart();                                                    

    glutTimerFunc(10, on_timer, 0);                               

    glutMainLoop();                                                

    return;
}
