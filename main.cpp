#ifdef WIN32
#include <windows.h>
#include <GL/glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "Linha.h"
#include "Objeto3D.h"
#include "Ponto.h"

#include <iostream>

Objeto3D o;

void DefineLuz()
{
    // Define cores para um objeto dourado
    float luz_ambiente[] {0.4, 0.4, 0.4};
    float luz_difusa[] {0.7, 0.7, 0.7};
    float luz_especular[] {0.9, 0.9, 0.9};
    float posicao_luz[] {2.0, 3.0, 0.0};  // Posição da Luz
    float especularidade[] {1.0, 1.0, 1.0};

    // ****************  Fonte de Luz 0

    glEnable(GL_COLOR_MATERIAL);

    // Habilita o uso de iluminação
    glEnable(GL_LIGHTING);

    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luz_ambiente);
    // Define os parametros da luz número Zero
    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
    glEnable(GL_LIGHT0);

    // Ativa o "Color Tracking"
    glEnable(GL_COLOR_MATERIAL);

    // Define a reflectancia do material
    glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);

    // Define a concentração do brilho.
    // Quanto maior o valor do Segundo parametro, mais
    // concentrado será o brilho. (Valores válidos: de 0 a 128)
    glMateriali(GL_FRONT, GL_SHININESS, 51);
}

void PosicUser()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Configura a matriz da projeção perspectiva (FOV, proporção da tela, distância do mínimo antes do clipping, distância máxima antes do clipping
    // https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
    gluPerspective(60, 16/9, 0.01, 50); // Projecao perspectiva
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Especifica a matriz de transformação da visualização
    // As três primeiras variáveis especificam a posição do observador nos eixos x, y e z    # As três próximas especificam o ponto de foco nos eixos x, y e z
    // As três últimas especificam o vetor up
    // https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/gluLookAt.xml
    gluLookAt(-2, 6, -8, 0, 0, 0, 0, 1.0, 0);
}

void DesenhaLadrilho()
{
    glColor3f(0.5, 0.5, 0.5); // desenha QUAD preenchido
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-0.5, 0.0, -0.5);
    glVertex3f(-0.5, 0.0, 0.5);
    glVertex3f(0.5, 0.0, 0.5);
    glVertex3f(0.5, 0.0, -0.5);
    glEnd();

    glColor3f(1, 1, 1); // desenha a borda da QUAD
    glBegin(GL_LINE_STRIP);
    glNormal3f(0, 1, 0);
    glVertex3f(-0.5, 0.0, -0.5);
    glVertex3f(-0.5, 0.0, 0.5);
    glVertex3f(0.5, 0.0, 0.5);
    glVertex3f(0.5, 0.0, -0.5);
    glEnd();
}

void DesenhaPiso()
{
    glPushMatrix();
    glTranslated(-20, -1, -10);
    for (int x = -20; x < 20; x++)
    {
        glPushMatrix();
        for (int z = -20; z < 20; z++)
        {

            DesenhaLadrilho();
            glTranslated(0, 0, 1);
        }
        glPopMatrix();
        glTranslated(1, 0, 0);
    }
    glPopMatrix();
}

void DesenhaCubo()
{
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslated(0, 0.5, 0);
    glutSolidCube(1);

    glColor3f(0.5, 0.5, 0);
    glTranslated(0, 0.5, 0);
    glRotatef(90, -1, 0, 0);
    glRotatef(45, 0, 0, 1);
    glutSolidCone(1, 1, 4, 4);
    glPopMatrix();
}

void desenha()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    DesenhaPiso();
    // DesenhaCubo();
    o.Desenha();
    o.DesenhaWireframe();
    // o.DesenhaVertices();

    glutSwapBuffers();
}    

// to do: problema, só roda uma vez 
void teclado(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'r':
            o.setRotation(1, 0, 0, o.getRotationAngle() + 2);

            glutPostRedisplay();

            break;
    }    
}

void init()
{    
    glClearColor(0.5, 0.5, 0.9, 1.0);
    glClearDepth(1.0);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    o = Objeto3D();
    o.LoadFile("dude.obj");

    DefineLuz();
    PosicUser();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    // Define o modelo de operacao da GLUT
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

    // Especifica o tamnho inicial em pixels da janela GLUT
    glutInitWindowSize(640, 480);

    // Especifica a posição de início da janela
    glutInitWindowPosition(100, 100);

    // Cria a janela passando o título da mesma como argumento
    glutCreateWindow("Computacao Grafica - 3D");

    // Função responsável por fazer as inicializações
    init();

    // Registra a funcao callback de redesenho da janela de visualizacao
    glutDisplayFunc(desenha);

    // Registra a funcao callback para tratamento das teclas ASCII
    glutKeyboardFunc(teclado);

    // Inicia o processamento e aguarda interacoes do usuario
    glutMainLoop();

    return 0;
}