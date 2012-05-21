

#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "glwidget.h"
#include "qtlogo.h"
#include <GL/glu.h>
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif




GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    logo = 0;
    xRot = 0;
    yRot = 0;
    zRot = 0;

    
    camara_x = camara_y = 0;
    camara_z = -5;
    ejes_modo = 0;
    

    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
}



GLWidget::~GLWidget()
{
}



QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}



QSize GLWidget::sizeHint() const

{
    return QSize(400, 400);
}


void GLWidget::camara_mover(eEje eje, bool positivo)
{
    switch(eje)
    {
    case eje_x:
        positivo? camara_x += paso: camara_x -= paso;
        break;
    case eje_y:
        positivo? camara_y += paso: camara_y -= paso;
        break;
    case eje_z:
        positivo? camara_z += paso: camara_z -= paso;
        break;
    }
    updateGL();
}
void GLWidget::cambiar_funcion(Figuras tipo, QList<double> constantes)
{
    delete logo;
    QtLogo::constantes = constantes;
    switch(tipo)
    {
    case esfera:
        logo = new QtLogo(this,&func_esfera);
        break;
    case elipsoide:
        logo = new QtLogo(this,&func_elipsoide);
        break;
    case cilindro:
        logo = new QtLogo(this,&func_cilindro);
        break;
    case hiperboloide_1hoja:
        logo = new QtLogo(this,&func_hiperboloide_1hoja);
        break;
    case hiperboloide_2hojas:
        logo = new QtLogo(this,&func_hiperboloide_2hojas);
        break;
    case paraboloide_hiperbolico:
        logo = new QtLogo(this,&func_paraboloide_hiperbolico);
        break;
    case paraboloide_eliptico:
        logo = new QtLogo(this,&func_paraboloide_eliptico);
        break;
    case cono_eliptico:
        logo = new QtLogo(this,&func_cono_eliptico);
        break;
    default:
        logo = new QtLogo(this,&func_esfera);
    }
    logo->setColor(qtGreen.dark());
    updateGL();
}

void GLWidget::cambiar_modo_ejes(int modo)
{
    if(modo < 0)
    {
        return;
    }
    ejes_modo = (unsigned int)modo;
    updateGL();
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}


void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot)
    {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}


void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot)
    {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot)
    {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}


void GLWidget::initializeGL()
{
    qglClearColor(qtPurple.dark());

    
    QtLogo::constantes.clear();
    QtLogo::constantes.append(0.5);
    QtLogo::constantes.append(0.5);
    QtLogo::constantes.append(0.5);
    QtLogo::constantes.append(0.25);

    logo = new QtLogo(this, &func_esfera);
    logo->setColor(qtGreen.dark());

    glEnable(GL_DEPTH_TEST);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}



void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef CROSSEYE
    
    int side = qMin(width()/2, height());
    glViewport((width() - 2*side) / 2, (height() - side) / 2, side, side);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30,1,1,15);
    glMatrixMode(GL_MODELVIEW);
#endif

    glLoadIdentity();
    glTranslatef(camara_x, camara_y, camara_z);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    logo->draw();
    
    pintar_ejes(ejes_modo);

#ifdef CROSSEYE
    
    glViewport((width() - 2*side) / 2+side, (height() - side) / 2, side, side);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30,1,1,15);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslatef(camara_x+0.1, camara_y, camara_z);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    logo->draw();
    
    pintar_ejes(ejes_modo);
#endif

}



void GLWidget::resizeGL(int width, int height)
{
#ifndef CROSSEYE
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(30,1,1,15);
#endif
    glMatrixMode(GL_MODELVIEW);
}



void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}



void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    }
    else if (event->buttons() & Qt::RightButton)
    {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}



void GLWidget::pintar_ejes(unsigned int modo)
{
    GLfloat color[4];
    switch(modo)
    {
    case 0:
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
        color[3] = 1;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
        glBegin(GL_LINES);
        
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.2f, 0.0f);
        glVertex3f(0.9f, 0.1f, 0.0f);
        glVertex3f(0.9f, 0.2f, 0.0f);
        glVertex3f(1.0f, 0.1f, 0.0f);
        
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.1f, 1.0f, 0.0f);
        glVertex3f(0.12f, 0.97f, 0.0f);
        glVertex3f(0.14f, 1.0f, 0.0f);
        glVertex3f(0.12f, 0.97f, 0.0f);
        glVertex3f(0.12f, 0.97f, 0.0f);
        glVertex3f(0.12f, 0.9f, 0.0f);
        
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.2f, 1.0f);
        glVertex3f(0.0f, 0.2f, 0.9f);
        glVertex3f(0.0f, 0.2f, 0.9f);
        glVertex3f(0.0f, 0.1f, 1.0f);
        glVertex3f(0.0f, 0.1f, 1.0f);
        glVertex3f(0.0f, 0.1f, 0.9f);
        glEnd();
        break;
    case 1:
        glEnable(GL_CULL_FACE);
        
        color[0] = 1;
        color[1] = 0;
        color[2] = 0;
        color[3] = 1;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
        glBegin(GL_QUADS);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        
        color[0] = 0;
        color[1] = 1;
        color[2] = 0;
        color[3] = 1;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(0.0f, 1.0f, 1.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        
        color[0] = 0;
        color[1] = 0;
        color[2] = 1;
        color[3] = 1;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glEnd();
        glDisable(GL_CULL_FACE);
        break;
    default:
        break;
    }
}
