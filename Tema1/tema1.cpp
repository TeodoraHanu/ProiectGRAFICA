#include <stdlib.h>
#include "glut.h"
#include "GL.H"
#include "GLU.H"
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "GLAux.h"

float wingAngle = 0.0;
float wingSpeed = 0.2;
float wingAmplitude = 5.0;
float clAngle = 0.0;
GLuint textureId1;

void createFont() {
    static const int fontWidth = 9;
    static const int fontHeight = 15;


    GLuint fontList = glGenLists(1);
    glNewList(fontList, GL_COMPILE);

    for (int i = 32; i < 128; i++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, i);
    }

    glEndList();
}

void renderText(const char* text, float x, float y) {
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 0.0);
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
    glEnable(GL_LIGHTING);
}

GLuint incarcaTextura(const char* s)
{
    GLuint textureId = 0;
    AUX_RGBImageRec* pImagineTextura = auxDIBImageLoad(s);

    if (pImagineTextura != NULL)
    {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, pImagineTextura->sizeX, pImagineTextura->sizeY,
            0, GL_RGB, GL_UNSIGNED_BYTE, pImagineTextura->data);
    }
    if (pImagineTextura)
    {
        if (pImagineTextura->data) {
            free(pImagineTextura->data);
        }
        free(pImagineTextura);
    }
    return textureId;
}

void initLighting() {
    GLfloat light_position[] = { 50.0, 100.0, 50.0, 0.0 };
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 0.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_DEPTH_TEST);
}

void init(void) {
    glClearColor(0.0, 0.0, 1.0, 0.0);
    textureId1 = incarcaTextura("wing.bmp");
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
    initLighting();
    createFont();

}

void drawBezierCurve(GLfloat controlPoints[4][3]) {
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &controlPoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 30; i++) {
        glEvalCoord1f((GLfloat)i / 30.0);
    }
    glEnd();
    glDisable(GL_MAP1_VERTEX_3);
}

void drawCloud() {
    glTranslatef(100.0, 50.0, -5.0);
    GLfloat cloud1[4][3] = {
        { -2.0, 0.0, 0.0 },
        { -1.5, 1.0, 0.0 },
        { -0.5, 1.0, 0.0 },
        { 0.0, 0.0, 0.0 }
    };

    GLfloat cloud2[4][3] = {
        { 0.0, 0.0, 0.0 },
        { 0.5, 1.0, 0.0 },
        { 1.5, 1.0, 0.0 },
        { 2.0, 0.0, 0.0 }
    };

    GLfloat cloud3[4][3] = {
        { -1.5, 0.0, 0.0 },
        { -1.0, 1.5, 0.0 },
        { 1.0, 1.5, 0.0 },
        { 1.5, 0.0, 0.0 }
    };

    GLfloat cloud4[4][3] = {
        { -1.0, 0.0, 0.0 },
        { -0.5, 0.8, 0.0 },
        { 0.5, 0.8, 0.0 },
        { 1.0, 0.0, 0.0 }
    };

    GLfloat cloud5[4][3] = {
        { -0.5, 0.0, 0.0 },
        { -0.2, 0.6, 0.0 },
        { 0.2, 0.6, 0.0 },
        { 0.5, 0.0, 0.0 }
    };

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glScalef(10.0f, 10.0f, 10.0f);
    drawBezierCurve(cloud1);
    drawBezierCurve(cloud2);
    drawBezierCurve(cloud3);
    drawBezierCurve(cloud4);
    drawBezierCurve(cloud5);
    glPopMatrix();

}
void drawButterfly() {


    glPushMatrix();
    glScalef(0.075, 0.35, 0.075);
    glColor3f(0.0, 0.0, 0.0);
    glutSolidCube(60.0);


    glTranslatef(0.0, 40.0, 0.0);
    glScalef(1.5, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glutSolidSphere(12.0, 20, 20);


    glTranslatef(0.0, -42.0, 0.0);
    glLineWidth(2.0);

    glBegin(GL_LINES);

    glVertex3f(-6.0, 52.0, 0.0);
    glVertex3f(-6.0, 57.0, 0.0);
    glVertex3f(-6.0, 57.0, 0.0);
    glVertex3f(-12.0, 62.0, 8.0);


    glVertex3f(6.0, 52.0, 0.0);
    glVertex3f(6.0, 57.0, 0.0);
    glVertex3f(6.0, 57.0, 0.0);
    glVertex3f(12.0, 62.0, 8.0);
    glEnd();

    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.0, -5.0, -2.4);


    glPushMatrix();
    glTranslatef(0, 5, 0);
    glScalef(0.2, 0.2, 0.2);
    glPushMatrix();
    glRotatef(wingAmplitude * sin(wingAngle), 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 10.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 10.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(100.0, -5.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(120.0, 100.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 40.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


    glPushMatrix();
    glRotatef(-wingAmplitude * sin(wingAngle), 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 10.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, -30.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(60.0, -70.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(100.0, -10.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 10.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


    glPushMatrix();
    glRotatef(-wingAmplitude * sin(wingAngle), 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 10.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 10.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-100.0, -5.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(-120.0, 100.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 40.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();


    glPushMatrix();
    glRotatef(wingAmplitude * sin(wingAngle), 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, textureId1);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 10.0, 0.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, -30.0, 0.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-60.0, -70.0, 0.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(-100.0, -10.0, 0.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 10.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();
}

void drawButterflyshd() {
    glPushMatrix();
    glScalef(0.075, 0.35, 0.075);
    glColor3f(0.0, 0.0, 0.0);
    glutSolidCube(60.0);

    glTranslatef(0.0, 40.0, 0.0);
    glScalef(1.5, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glutSolidSphere(12.0, 20, 20);


    glTranslatef(0.0, -42.0, 0.0);
    glLineWidth(2.0);

    glBegin(GL_LINES);

    glVertex3f(-6.0, 52.0, 0.0);
    glVertex3f(-6.0, 57.0, 0.0);
    glVertex3f(-6.0, 57.0, 0.0);
    glVertex3f(-12.0, 62.0, 8.0);


    glVertex3f(6.0, 52.0, 0.0);
    glVertex3f(6.0, 57.0, 0.0);
    glVertex3f(6.0, 57.0, 0.0);
    glVertex3f(12.0, 62.0, 8.0);
    glEnd();

    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.0, -5.0, -2.4);


    glPushMatrix();
    glTranslatef(0, 5, 0);
    glScalef(0.2, 0.2, 0.2);
    glPushMatrix();
    glRotatef(wingAmplitude * sin(wingAngle), 0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);
    glVertex3f(100.0, -5.0, 0.0);
    glVertex3f(120.0, 100.0, 0.0);
    glVertex3f(0.0, 40.0, 0.0);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glRotatef(-wingAmplitude * sin(wingAngle), 0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 10.0, 0.0);
    glVertex3f(0.0, -30.0, 0.0);
    glVertex3f(60.0, -70.0, 0.0);
    glVertex3f(100.0, -10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glRotatef(-wingAmplitude * sin(wingAngle), 0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);
    glVertex3f(-100.0, -5.0, 0.0);
    glVertex3f(-120.0, 100.0, 0.0);
    glVertex3f(0.0, 40.0, 0.0);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glRotatef(wingAmplitude * sin(wingAngle), 0.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 10.0, 0.0);
    glVertex3f(0.0, -30.0, 0.0);
    glVertex3f(-60.0, -70.0, 0.0);
    glVertex3f(-100.0, -10.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);
    glEnd();


    glPopMatrix();

    glPopMatrix();
    glPopMatrix();
}

void drawButterflyShadow() {

    glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);


    glColor3f(0.2f, 0.2f, 0.2f);


    GLfloat light_position[] = { 50.0f, 100.0f, 50.0f, 1.0f };
    GLfloat grPlane[] = { 0.0f, 1.0f, 1.0f, 20.0f };


    GLfloat shadowMat[16];
    GLfloat dot = grPlane[0] * light_position[0] +
        grPlane[1] * light_position[1] +
        grPlane[2] * light_position[2] +
        grPlane[3] * light_position[3];

    shadowMat[0] = dot - light_position[0] * grPlane[0];
    shadowMat[4] = -light_position[0] * grPlane[1];
    shadowMat[8] = -light_position[0] * grPlane[2];
    shadowMat[12] = -light_position[0] * grPlane[3];

    shadowMat[1] = -light_position[1] * grPlane[0];
    shadowMat[5] = dot - light_position[1] * grPlane[1];
    shadowMat[9] = -light_position[1] * grPlane[2];
    shadowMat[13] = -light_position[1] * grPlane[3];

    shadowMat[2] = -light_position[2] * grPlane[0];
    shadowMat[6] = -light_position[2] * grPlane[1];
    shadowMat[10] = dot - light_position[2] * grPlane[2];
    shadowMat[14] = -light_position[2] * grPlane[3];

    shadowMat[3] = -light_position[3] * grPlane[0];
    shadowMat[7] = -light_position[3] * grPlane[1];
    shadowMat[11] = -light_position[3] * grPlane[2];
    shadowMat[15] = dot - light_position[3] * grPlane[3];


    glPushMatrix();
    glMultMatrixf(shadowMat);


    drawButterflyshd();

    glPopMatrix();

    glPopAttrib();

}

void drawSun() {
    glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 0.0);
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();
    glTranslatef(120.0, 60.0, -5.0);
    gluSphere(quad, 10.0, 30, 30);
    glPopMatrix();
    gluDeleteQuadric(quad);
    glEnable(GL_LIGHTING);
}

void drawSphere(float radius, float angle) {

    float butterflyX = 50.0f;
    float butterflyY = 0.0f;
    float butterflyZ = -160.0f;

    float clX = butterflyX + radius * cos(angle);
    float clY = butterflyY + radius * sin(angle);
    float clZ = butterflyZ;

    glPushMatrix();
    glTranslatef(clX, clY, clZ);
    glColor3f(1.0f, 1.0f, 1.0f);

    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, 8.0, 20, 20);
    gluDeleteQuadric(quad);

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glLoadIdentity();
    gluLookAt(10.0, 20.0, 150.0,
        30.0, 10.0, -40.0,
        0.0, 1.0, 0.0);

    glRotatef(30, 0.0, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(30.0, 0.0, -130.0);
    glEnable(GL_DEPTH_TEST);
    drawButterflyShadow();
    drawButterfly();
    glDisable(GL_DEPTH_TEST);
    glPopMatrix();
    drawSphere(25.0f, clAngle);
    renderText("Teo", -45, -5);
    drawSun();
    drawCloud();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void update(int value) {
    wingAngle += wingSpeed;
    if (wingAngle > 360) {
        wingAngle -= 360;
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        clAngle += 0.1f;
        break;
    case 'd':
        clAngle -= 0.1f;
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("Fluture");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}