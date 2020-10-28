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
#endif

//////////////////////////////////////

GLuint
        VaoId,
        VboId,
        ColorBufferId,
        ProgramId;


void CreateVBO(void)
{
    // varfurile
    GLfloat Vertices[] = {
            -1.0f, -0.8f, 0.0f, 1.0f,
            -0.3f,  0.5f, 0.0f, 1.0f,
            -0.2f, 0.3f, 0.0f, 1.0f,
            0.2f, 0.5f, 0.0f, 1.0f,
            0.0f, 0.95f, 0.0f, 1.0f,
            0.3f, 0.5f, 0.0f, 1.0f,
            -0.9f, -0.8f, 0.0f, 1.0f,
            -0.8f,  -0.6f, 0.0f, 1.0f,
            -0.6f,  0.5f, 0.0f, 1.0f,
            -0.6f,  0.5f, 0.0f, 1.0f,
            -0.8f,  0.0f, 0.0f, 1.0f,
            -0.6f,  -0.4f, 0.0f, 1.0f,
    };

    // culorile, ca atribute ale varfurilor
    GLfloat Colors[] = {
            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,
    };


    // se creeaza un buffer nou
    glGenBuffers(1, &VboId);
    // este setat ca buffer curent
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    // varfurile sunt "copiate" in bufferul curent
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
    FILE* f = fopen("r","./shader.vert");
    std::cout<<f;
    ProgramId=LoadShaders("./shader.vert", "./shader.frag");
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
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(1.0);
    glLineWidth (1.0);
    glDrawArrays(GL_LINE_LOOP, 0, 2);

    glDrawArrays(GL_LINE_LOOP, 2, 2);

    glDrawArrays(GL_LINE_LOOP, 4, 2);

    glDrawArrays(GL_LINE_LOOP, 6, 2);

    glDrawArrays(GL_POINTS, 9, 4);
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
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition (100,100); // pozitia initiala a ferestrei
    glutInitWindowSize(600,400); //dimensiunile ferestrei
    glutCreateWindow("Doua primitive"); // titlul ferestrei
    glewInit(); // nu uitati de initializare glew; trebuie initializat inainte de a a initializa desenarea
    Initialize( );
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Cleanup);
    glutMainLoop();


}

