#include <iostream>
#include "glwidget.h"
#include "obj.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

using namespace std;

static const int MAX_FPS = 120;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent), m_timer(this), m_fps(0.0f), m_prevFps(0.0f), m_increment(0), m_font("Deja Vu Sans Mono", 8, 4),
    m_defaultModel("/course/cs123/data/mesh/sponzaLab.obj"), m_captureMouse(true), m_shouldRotate(true), m_useVbo(false)
{
    // Set up the camera
    m_camera.center = Vector3(0.f, 50.f, 0.f);
    m_camera.up = Vector3(0.f, 1.f, 0.f);
    m_camera.zoom = 3.5f;
    m_camera.theta = M_PI * 1.5f, m_camera.phi = -0.2f;
    m_camera.fovy = 60.f;
    m_numObjs = 0;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));

    m_timer.start(1000.0f / MAX_FPS);
}

GLWidget::~GLWidget()
{

}

/**
    Called once at GL initialization time to setup OpenGL states
   */
void GLWidget::initializeGL()
{
    // Enable depth testing, so that objects are occluded based on depth instead of drawing order
    glEnable(GL_DEPTH_TEST);

    // Enable back-face culling, meaning only the front side of every face is rendered
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Specify that the front face is represented by vertices in counterclockwise order (this is the default)
    glFrontFace(GL_CCW);

    // Enable color materials with ambient and diffuse lighting terms
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Set up global (ambient) lighting
    GLfloat global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    // Set up GL_LIGHT0 with a position and lighting properties
    GLfloat ambientLight[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // Set up material properties
    GLfloat shiny = 50;
    GLfloat ambientMat[] = {0.0f, 0.0f, 0.0f, 0.0f};
    GLfloat diffuseMat[] = { 0.0f, 1.0f, 0.0, 0.0f };
    GLfloat specularMat[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shiny);

    // Set the screen color when the color buffer is cleared (in RGBA format)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    addObjects();


    glEnable(GL_LIGHTING);// Set up material properties
    glEnable(GL_LIGHT0);
    // TODO: Put any additional initialization code here
}

/**
    This is called every time the screen is repainted.
    */
void GLWidget::paintGL()
{
    int width = this->width();
    int height = this->height();
    applyPerspectiveCamera(width, height);

    //fps stuff
    int fpsTime = m_clock.elapsed();
    m_fps = 1000.f / (fpsTime - m_prevTime);
    m_prevTime = fpsTime;

    // Clear the color and depth buffers to the current glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //glColor3f(.5,.5,1);
    glPushMatrix();
    if (m_shouldRotate) m_camera.theta += .005; ;
    if (m_useVbo){
        objects.at(0).vboDraw();
        //m_obj.vboDraw();
    }else{
;
        glEnable(GL_NORMALIZE);
        glMatrixMode(GL_MODELVIEW);
        objects.at(0).draw();
        glPushMatrix();
        glTranslatef(0.0, 0.0, 15.0);
        glScalef(.2, .2, .2);
        objects.at(1).draw();
        glPopMatrix();

        //m_obj.draw();
    }
    glPopMatrix();

    paintText();
}

/**
    Called when the screen gets resized.
    The camera is updated when the screen resizes because the aspect ratio may change.
  */
void GLWidget::resizeGL(int width, int height)
{
    // Set the viewport to fill the screen
    glViewport(0, 0, width, height);
}

void GLWidget::addObjects(){
    OBJ dragon;
	if (! dragon.read("models/dragon.obj")){
        std::cout << "it didnt load! call a ta over" << endl;
        exit(1);

    }
    OBJ lego;
	if (! lego.read("models/lego.obj")){
        std::cout << "it didnt load! call a ta over" << endl;
        exit(1);

    }
    objects.append(dragon);
    objects.append(lego);
   // m_obj=dragon;
}

/**
  Specifies to Qt what to do when the widget needs to be updated.
  We only want to repaint the onscreen objects.
**/
void GLWidget::tick()
{
    update();
}

/**
  Draws text for the FPS
 **/
void GLWidget::paintText()
{
    glColor3f(1.f, 1.f, 1.f);

    // Combine the previous and current framerate
    if (m_fps >= 0 && m_fps < 1000)
    {
       m_prevFps *= 0.95f;
       m_prevFps += m_fps * 0.05f;
    }

    // QGLWidget's renderText takes xy coordinates, a string, and a font
    renderText(10, 20, "FPS: " + QString::number((int) (m_prevFps)), m_font);
}

void GLWidget::fileOpen()
{/*

    m_captureMouse = false;
    QString path = QFileDialog::getOpenFileName(this, "Open", "/course/cs123/data/mesh");
    if (path.isEmpty()){
        cout << "path was empty\n";
        return;
    }


    if (! m_obj.read(path))
    {
        QString text = QString("Could not load from \"%1\". Reloading default model").arg(path);
        cout << (char*) text.toStdString().c_str() << "\n";
        QMessageBox::information(this, "Error", text, QMessageBox::Ok);
    } else{
        //BoundingBox box = *m_obj.getBoundingBox();
        //set camera center right here
        m_camera.center = Vector3(m_obj.boundingBox.maxX * 1.5,  (m_obj.boundingBox.maxY + m_obj.boundingBox.minY) /2, m_obj.boundingBox.maxZ * 1.5);
        m_camera.lookAt(m_obj.boundingBox.center - m_camera.center);
    }
    m_captureMouse = true;
    m_prevTime = m_clock.elapsed();*/
}

void GLWidget::toggleRotate()
{
    m_shouldRotate = !m_shouldRotate;
}

void GLWidget::toggleVboDraw(){
    m_useVbo = !m_useVbo;
}

/**
  Called to switch to a perspective OpenGL camera.

  @param width: the viewport width
  @param height: the viewport height
**/
void GLWidget::applyPerspectiveCamera(float width, float height)
{
    float ratio = ((float) width) / height;
    Vector3 dir(-Vector3::fromAngles(m_camera.theta, m_camera.phi));
    Vector3 eye(m_camera.center - dir * m_camera.zoom);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera.fovy, ratio, 0.1f, 100000.f);
    gluLookAt(eye.x, eye.y, eye.z, eye.x + dir.x, eye.y + dir.y, eye.z + dir.z,
              m_camera.up.x, m_camera.up.y, m_camera.up.z);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
  Called when the mouse is dragged.  Rotates the camera based on mouse movement.
**/
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_captureMouse){
        Vector2 pos(event->x(), event->y());
        if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
        {
            m_camera.mouseMove(pos - m_prevMousePos);
        }
        m_prevMousePos = pos;
    }
}

/**
  Record a mouse press position.
 **/
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_captureMouse){
        m_prevMousePos.x = event->x();
        m_prevMousePos.y = event->y();
    }
}


/**
  Called when the mouse wheel is turned.  Moves the camera forward and back.
**/
void GLWidget::wheelEvent(QWheelEvent *event)
{
    if (m_captureMouse && event->orientation() == Qt::Vertical)
    {
        m_camera.mouseWheel(event->delta() );
    }
}

