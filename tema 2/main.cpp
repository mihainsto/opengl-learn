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

GLuint
        VaoId,
        VboId,
        ColorBufferId,
        ProgramId,
        myMatrixLocation,
        matrScaleLocation,
        matrTranslLocation,
        matrRotlLocation,
        codColLocation,
        poligonDLocation;


glm::mat4 myMatrix, resizeMatrix, matrTransl, matrScale, matrRot;


int codCol;
float PI=3.141592, angle=0;
float tx=0; float ty=0;
int width=1000, height=1000;

void displayMatrix ( )
{
    for (int ii = 0; ii < 4; ii++)
    {
        for (int jj = 0; jj < 4; jj++)
            cout <<  myMatrix[ii][jj] << "  " ;
        cout << endl;
    };
    cout << "\n";

};

void CreateVBO(void)
{
    // varfurile
    GLfloat Vertices[] = {
            // fundal
            -1990.0f, -1990.0f, 0.0f, 1.0f,
            1990.0f,  -1990.0f, 0.0f, 1.0f,
            1990.0f, 1990.0f, 0.0f, 1.0f,
            -1990.0f, 1990.0f, 0.0f, 1.0f,
            // dreptunghi 800 x 1000
            1000.0f,  -800.0f, 0.0f, 1.0f,
            -1000.0f, -800.0f, 0.0f, 1.0f,
            -1000.0f,  800.0f, 0.0f, 1.0f,
            1000.0f,  800.0f, 0.0f, 1.0f,
            // poligon p1 convex
            -300.0f,  -400.0f, 0.0f, 1.0f,
            -600.0f, -400.0f, 0.0f, 1.0f,
            -600.0f,  0.0f, 0.0f, 1.0f,
            -300.0f,  0.0f, 0.0f, 1.0f,
            // poligon p2 concav
            300.0f,  400.0f, 0.0f, 1.0f,
            600.0f,  0.0f, 0.0f, 1.0f,
            600.0f, 400.0f, 0.0f, 1.0f,
            300.0f,  0.0f, 0.0f, 1.0f,

    };



    // culorile varfurilor din colturi
    GLfloat Colors[] = {
            1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
    };


    // se creeaza un buffer nou
    glGenBuffers(1, &VboId);
    // este setat ca buffer curent
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    // punctele sunt "copiate" in bufferul curent
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    // se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);
    // se activeaza lucrul cu atribute; atributul 0 = pozitie
    glEnableVertexAttribArray(0);
    //
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // un nou buffer, pentru culoare
    glGenBuffers(1, &ColorBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
    // atributul 1 =  culoare
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);


}
void DestroyVBO(void)
{


    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ColorBufferId);
    glDeleteBuffers(1, &VboId);

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
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului
    CreateVBO();
    CreateShaders();
}
void RenderFunction(void)
{

    resizeMatrix= glm::scale(glm::mat4(1.0f), glm::vec3(1.f/width, 1.f/height, 1.0));
    // Aplicam o rotatie intre p1 si p2
    matrRot=glm::rotate(glm::mat4(1.0f), -0.5f, glm::vec3(0.0, 0.0, 1.0));

    glClear(GL_COLOR_BUFFER_BIT);
    myMatrix=resizeMatrix;


    // matricea de redimensionare (pentru elementele "fixe")
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
    // desenare puncte din colturi si axe
    codCol = 0;
    int curentIndex = 0;
    glUniform1i(codColLocation, codCol);
    //Fundal
    glDrawArrays(GL_POLYGON, curentIndex, 4);
    curentIndex += 4;

    // Matricea pentru elementele care isi schimba pozitia

    myMatrix=matrRot * resizeMatrix ;
    // displayMatrix ();


    // matricele de scalare si de translatie
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);

    codColLocation = glGetUniformLocation(ProgramId, "codCol");

    poligonDLocation = glGetUniformLocation(ProgramId, "poligonD");

    codCol=3;
    glUniform1i(codColLocation, codCol);
    glUniform1i(poligonDLocation, 1);
    // Poligon 800 1000 scalat cu shader
    glDrawArrays(GL_POLYGON, curentIndex, 4);

    glUniform1i(poligonDLocation, 0);
    codCol=2;
    glUniform1i(codColLocation, codCol);
    // Poligon 800 1000
    glDrawArrays(GL_POLYGON, curentIndex, 4);
    curentIndex += 4;
    codCol=3;
    glUniform1i(codColLocation, codCol);

    // Poligon P1 convex
    glDrawArrays(GL_POLYGON, curentIndex, 4);
    curentIndex += 4;
    codCol=1;
    glUniform1i(codColLocation, codCol);
    // Poligon P2 concav
    glDrawArrays(GL_POLYGON, curentIndex, 4);

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
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowPosition (100,100);
    glutInitWindowSize(800,600);
    glutCreateWindow("Tema 2");
    glewInit();
    Initialize( );
    glutDisplayFunc(RenderFunction);
    glutIdleFunc(RenderFunction);
    glutCloseFunc(Cleanup);
    glutMainLoop();


}
