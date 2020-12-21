// A simple introductory program; its main window contains a static picture
// of a triangle, whose three vertices are red, green and blue.  The program
// illustrates viewing with default viewing parameters only.

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <fstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include "loadShaders.h"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#endif

using namespace std;

//////////////////////////////////////
// identificatori
GLuint
        VaoId,
        VboId,
        EboId,
        ColorBufferId,
        ProgramId,
        myMatrixLocation,
        viewLocation,
        projLocation,
        matrRotlLocation,
        codColLocation,
        depthLocation;

GLuint texture;

int codCol;
float PI=3.141592;

// matrice utilizate
glm::mat4 myMatrix, matrRot;

// elemente pentru matricea de vizualizare
float Obsx=0.0, Obsy=0.0, Obsz=-300.f;
float Refx=0.0f, Refy=0.0f;
float Vx=0.0;
glm::mat4 view;

// elemente pentru matricea de proiectie
float width=800, height=600, xwmin=-200.f, xwmax=200, ywmin=-200, ywmax=200, znear=100, zfar=-100, fov=20;
float znear1 = 300, zfar1 = -300;
glm::mat4 projection;




void displayMatrix ( )
{
    for (int ii = 0; ii < 4; ii++)
    {
        for (int jj = 0; jj < 4; jj++)
            cout <<  myMatrix[ii][jj] << "  " ;
        cout << endl;
    };

};

void processNormalKeys(unsigned char key, int x, int y)
{

    switch (key) {
        case 'l' :
            Vx += 0.1;
            break;
        case 'r' :
            Vx -= 0.1;
            break;
        case '+' :
            //znear += 10;
            //zfar += 10;
            Obsz+=10;
            break;
        case '-' :
            //znear -= 10;
            //zfar -= 10;
            Obsz-=10;
            break;

    }
    if (key == 27)
        exit(0);
}
void processSpecialKeys(int key, int xx, int yy) {

    switch (key) {
        case GLUT_KEY_LEFT :
            Obsx-=20;
            break;
        case GLUT_KEY_RIGHT :
            Obsx+=20;
            break;
        case GLUT_KEY_UP :
            Obsy+=20;
            break;
        case GLUT_KEY_DOWN :
            Obsy-=20;
            break;
    }
}

void CreateVBO(void)
{
    // varfurile
    GLfloat Vertices[] = {
            -50.0f,  -50.0f, -50.0f, 1.0f,  0.0f, 1.0f, 0.0f,
            50.0f,  -50.0f,  -50.0f, 1.0f,   0.0f, 0.9f, 0.0f,
            50.0f,  50.0f,  -50.0f, 1.0f,    0.0f, 0.6f, 0.0f,
            -50.0f,  50.0f, -50.0f, 1.0f,   0.0f, 0.2f, 0.0f,
            0.0f,  0.0f, 100.0f, 1.0f,  1.0f, 0.0f, 0.0f,


    };

    // indicii pentru varfuri
    GLubyte Indices[ ]={

            1, 2, 0,   2, 0, 3,  //  Fata "de jos"
            0, 1, 4,   1, 2, 4,
            2, 3, 4,   3, 0, 4,
            0, 1, 2, 3,
            0, 4,
            1, 4,
            2, 4,


    };

    // se creeaza un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
    glGenVertexArrays(1, &VaoId);
    // se creeaza un buffer nou (atribute)
    glGenBuffers(1, &VboId);
    // se creeaza un buffer nou (indici)
    glGenBuffers(1, &EboId);

    // legarea VAO
    glBindVertexArray(VaoId);

    // legarea buffer-ului "Array"
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    // punctele sunt "copiate" in bufferul curent
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    // legarea buffer-ului "Element" (indicii)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    // indicii sunt "copiati" in bufferul curent
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    // se activeaza lucrul cu atribute; atributul 0 = pozitie
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),(GLvoid*)0);

    // se activeaza lucrul cu atribute; atributul 1 = culoare
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),(GLvoid*)(4 * sizeof(GLfloat)));

}
void DestroyVBO(void)
{
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &VboId);
    glDeleteBuffers(1, &EboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);

}


void CreateShaders(void)
{
    ProgramId=LoadShaders("shader.vert", "shader.frag");
    glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}

void Initialize(void)
{

    myMatrix = glm::mat4(1.0f);

    matrRot=glm::rotate(glm::mat4(1.0f), PI/8, glm::vec3(0.0, 0.0, 1.0));

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului


}
void RenderFunction(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);



    // se schimba pozitia observatorului
    glm::vec3 Obs = glm::vec3 (Obsx, Obsy, Obsz);

    // pozitia punctului de referinta
    Refx=Obsx; Refy=Obsy;
    glm::vec3 PctRef = glm::vec3(Refx, Refy, 800.0f);

    // verticala din planul de vizualizare

    glm::vec3 Vert =  glm::vec3(Vx, 1.0f, 0.0f);


    view = glm::lookAt(Obs, PctRef, Vert);

    myMatrix=view;
    //  displayMatrix();

    projection = glm::perspective(45.0f, 1.0f, znear, zfar);
    myMatrix = glm::mat4(1.0f);

    CreateVBO();


    // myMatrix= matrRot;
    CreateShaders();
    // variabile uniforme pentru shaderul de varfuri
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
    viewLocation = glGetUniformLocation(ProgramId, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE,  &view[0][0]);
    projLocation = glGetUniformLocation(ProgramId, "projection");
    glUniformMatrix4fv(projLocation, 1, GL_FALSE,  &projection[0][0]);

    GLint depthNearLoc = glGetUniformLocation(ProgramId, "gl_DepthRange.near");
    GLint depthFarLoc = glGetUniformLocation(ProgramId, "gl_DepthRange.far");
    GLint depthDiffLoc = glGetUniformLocation(ProgramId, "gl_DepthRange.diff");

    glUniform1f (depthNearLoc, -1.0);
    glUniform1f (depthFarLoc , 2.0);
    glUniform1f (depthDiffLoc, 1.0);

    codColLocation=glGetUniformLocation(ProgramId, "codCol");

    // Fetele piramidei
    codCol=0;
    glUniform1i(codColLocation, codCol);
    glDrawElements(GL_TRIANGLES, 18,  GL_UNSIGNED_BYTE, 0);

    // Eliberare memorie si realocare resurse
    DestroyVBO ( );
    DestroyShaders ( );
    //
    glutSwapBuffers();
    glFlush ( );
}
void Cleanup(void)
{
    DestroyShaders();
    DestroyVBO();
}

int main(int argc, char* argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowPosition (100,100);
    glutInitWindowSize(1200,900);
    glutCreateWindow("Piramida");
    glewInit();
    Initialize( );
    glutDisplayFunc(RenderFunction);
    glutIdleFunc(RenderFunction);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutCloseFunc(Cleanup);
    glutMainLoop();

}