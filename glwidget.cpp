#include <iostream>
#include "glwidget.h"
#include "obj.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include "QGLFramebufferObject"
#include "resourceloader.h"
#include "particles/particleemitter.h"
#include "assert.h"

using namespace std;

static const int MAX_FPS = 120;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent), m_timer(this), m_fps(0.0f), m_prevFps(0.0f), m_increment(0), m_font("Deja Vu Sans Mono", 8, 4),
	m_defaultModel("/course/cs123/data/mesh/sponzaLab.obj"), m_captureMouse(true), m_shouldRotate(false), m_useVbo(false)
{
	//this->loadTexture("textures/grass.jpg");
    // Set up the camera
    m_camera.center = Vector3(0.f, 50.f, 0.f);
    m_cameraTarget = m_camera.center;
    m_camera.up = Vector3(0.f, 1.f, 0.f);
    m_camera.zoom = 3.5f;
    m_camera.theta = M_PI * 1.5f, m_camera.phi = -0.2f;
    m_camera.fovy = 60.f;
    m_numObjs = 0;
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
	m_numTrees = 40;
	m_treeRadius = 5.0;
	for(unsigned int i = 0 ; i < m_numTrees; i++) {
		m_treeAngles.append(i*2*(3.1415926)/float(m_numTrees) + urand(-0.01, 0.01));
		m_treeSizes.append(urand(0.9, 1.1)*40);
	}
	m_timer.start(1000.0f / MAX_FPS);
}

GLWidget::~GLWidget()
{

	// Reallocate the framebuffers with the new window dimensions
	foreach (QGLFramebufferObject *fbo, m_framebufferObjects)
	{
		delete fbo;
	}
	foreach (QGLShaderProgram *shader, m_shaderPrograms)
	{
		delete shader;
	}
	foreach (QFile *file, m_fileList)
	{
		delete file;
	}
}

/**
    Called once at GL initialization time to setup OpenGL states
   */
void GLWidget::initializeGL()
{
    // Enable depth testing, so that objects are occluded based on depth instead of drawing order
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    // Enable back-face culling, meaning only the front side of every face is rendered
	//glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  //  glDisable(GL_DITHER);
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
	GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat position[] = { 0.0f, 50.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

    // Set up material properties
    GLfloat shiny = 50;
	GLfloat ambientMat[] = {0.1f, 0.1f, 0.1f, 0.0f};
	GLfloat diffuseMat[] = { 1.0f, 1.0f, 1.0, 0.0f };
	GLfloat specularMat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shiny);

	this->createFramebufferObjects(this->width(), this->height());
    //set up shaders
    createShaderPrograms();
    // Set the screen color when the color buffer is cleared (in RGBA format)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    addObjects();


    glEnable(GL_LIGHTING);// Set up material properties
	glEnable(GL_LIGHT0);
    // TODO: Put any additional initialization code here
}
/**
  Allocate framebuffer objects.

  @param width: the viewport width
  @param height: the viewport height
 **/
void GLWidget::createFramebufferObjects(int width, int height)
{
	// Allocate the main framebuffer object for rendering the scene to
        m_framebufferObjects["fbo_0"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::Depth,GL_TEXTURE_2D, GL_RGB16F_ARB);
        //m_framebufferObjects["fbo_0"]->format().setSamples(16);
	//FBO for doing edge detect
	m_framebufferObjects["fbo_1"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::NoAttachment, GL_TEXTURE_2D, GL_RGB16F_ARB);
	m_framebufferObjects["fbo_2"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::NoAttachment, GL_TEXTURE_2D, GL_RGB16F_ARB);
}

/**
    This is called every time the screen is repainted.
    */
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int width = this->width();
    int height = this->height();


    //fps stuff
    int fpsTime = m_clock.elapsed();
    m_fps = 1000.f / (fpsTime - m_prevTime);
    m_prevTime = fpsTime;


	if (abs((m_camera.center - m_cameraTarget).length()) <=1) {
		m_camera.center = m_cameraTarget;
	} else {
		m_camera.center -= (m_camera.center - m_cameraTarget)/1.4;
	}

    // Clear the color and depth buffers to the current glClearColor



	m_fire.updateParticles();
	//render normally
   m_framebufferObjects["fbo_0"]->bind();
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   applyPerspectiveCamera(width, height);

   renderScene(width, height);
   m_framebufferObjects["fbo_0"]->release();

   //run edge detect on the normal render (this is to avoid edges on the toony areas, which looks sloppy
   m_framebufferObjects["fbo_1"]->bind();
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   applyOrthogonalCamera(width, height);
   glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_0"]->texture());
   m_shaderPrograms["edge_detect"]->bind();
   m_shaderPrograms["edge_detect"]->setUniformValue("iwidth", float(this->width()));
   m_shaderPrograms["edge_detect"]->setUniformValue("iheight", float(this->height()));
   renderTexturedQuad(width, height);
   m_shaderPrograms["edge_detect"]->release();
   glBindTexture(GL_TEXTURE_2D, 0);
   m_framebufferObjects["fbo_1"]->release();

   //do a toon-shaded rendering
   m_framebufferObjects["fbo_0"]->bind();
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   applyPerspectiveCamera(width, height);
   glEnable(GL_TEXTURE_CUBE_MAP);
   glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
   glCallList(m_skybox);
   glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
   glDisable(GL_TEXTURE_CUBE_MAP);
   m_shaderPrograms["toon"]->bind();
   m_rain.updateParticles();
   m_rain.drawParticles();
   renderScene(width, height);
   m_shaderPrograms["toon"]->release();
   m_framebufferObjects["fbo_0"]->release();

   //combine them
   m_framebufferObjects["fbo_2"]->bind();
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   applyOrthogonalCamera(width, height);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_0"]->texture());
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_1"]->texture());
   m_shaderPrograms["combine"]->bind();
   m_shaderPrograms["combine"]->setUniformValue("tex1", 1);
   m_shaderPrograms["combine"]->setUniformValue("tex2", 2);
   renderTexturedQuad(width, height);
   m_shaderPrograms["combine"]->release();
   glBindTexture(GL_TEXTURE_2D, 0);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, 0);
   m_framebufferObjects["fbo_2"]->release();

   //now paint it to the screen
   applyOrthogonalCamera(width, height);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_2"]->texture());
   renderTexturedQuad(width, height);
   glBindTexture(GL_TEXTURE_2D, 0);
    paintText();
}

void GLWidget::renderScene(int width, int height)
{

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

    glEnable(GL_NORMALIZE);
    glMatrixMode(GL_MODELVIEW);
	//m_shaderPrograms["toon"]->bind();               //bind shader
   //bind shader
	//draw fire
	glPushMatrix();
	glScalef(30,30,30);
	m_fire.drawParticles();
	glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 0.0, 15.0);
   glScalef(10, 10, 10);
   glRotatef(160, 0, 1, 0);
    objects.at(1).draw();
    glPopMatrix();
    glPushMatrix();
    glScalef(0.6,0.6,0.6);
    for(unsigned int h=0;h<m_numTrees;h++) {
            glPushMatrix();
            glScalef(m_treeSizes[h], m_treeSizes[h], m_treeSizes[h]);
            glTranslatef(m_treeRadius*cos(m_treeAngles[h]),0.0,m_treeRadius*sin(m_treeAngles[h]));
            objects.at(2).draw();
            glPopMatrix();
    }
	glPopMatrix();
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(2.8,3.8,0);
	glScalef(6.3,6.3,6.3);
	objects.at(3).draw();
	glPopMatrix();
	//renderGround();

    paintText();
	glPopMatrix();
	//m_shaderPrograms["toon"]->release();            //unbind shader
	//glDisable(GL_CULL_FACE);
   // glDisable(GL_DEPTH_TEST);
}

void GLWidget::renderGround() {
	float extent = 100;//this comes from the skybox
	bindGroundTexture();
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0,-1.2,0);
	//glColor3f(0,1,0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-extent, 0, -extent);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(-extent, 0, extent);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(extent, 0, extent);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(extent, 0, -extent);
	glPopMatrix();
	glEnd();
	releaseGroundTexture();
}

/**
  Called to switch to an orthogonal OpenGL camera.
  Useful for rending a textured quad across the whole screen.

  @param width: the viewport width
  @param height: the viewport height
**/
void GLWidget::applyOrthogonalCamera(float width, float height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.f, width, 0.f, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void GLWidget::renderTexturedQuad(int width, int height) {
	// Clamp value to edge of texture when texture index is out of bounds
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Draw the  quad
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(width, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(width, height);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(0.0f, height);
	glEnd();
}

/**
    Called when the screen gets resized.
    The camera is updated when the screen resizes because the aspect ratio may change.
  */
void GLWidget::resizeGL(int width, int height)
{
    // Set the viewport to fill the screen
    glViewport(0, 0, width, height);

	// Reallocate the framebuffers with the new window dimensions
	foreach (QGLFramebufferObject *fbo, m_framebufferObjects)
	{
		const QString &key = m_framebufferObjects.key(fbo);
		QGLFramebufferObjectFormat format = fbo->format();
		delete fbo;
		m_framebufferObjects[key] = new QGLFramebufferObject(width, height, format);
	}
}

void GLWidget::addObjects() {
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
	OBJ tree1;
	if (! tree1.read("models/simpletree.obj")){
		std::cout << "it didnt load! call a ta over" << endl;
		exit(1);

	}
	OBJ logs;
	if (! logs.read("models/logs.obj")){
		std::cout <<"it didn't load!";
		exit(1);
	}
    objects.append(dragon);
    objects.append(lego);
	objects.append(tree1);
	objects.append(logs);

	m_skybox = ResourceLoader::loadSkybox();
	loadCubeMap();

   // m_obj=dragon;
}

/**
  Load a cube map for the skybox
 **/
void GLWidget::loadCubeMap()
{

	/*fileList.append(new QFile("textures/posx.jpg"));//course/cs123/bin/textures/astra/posx.jpg"));
	fileList.append(new QFile("textures/negx.jpg"));//course/cs123/bin/textures/astra/negx.jpg"));
	fileList.append(new QFile("textures/posy.jpg"));//course/cs123/bin/textures/astra/posy.jpg"));
	fileList.append(new QFile("textures/negy.jpg"));///course/cs123/bin/textures/astra/negy.jpg"));
	fileList.append(new QFile("textures/posz.jpg"));///course/cs123/bin/textures/astra/posz.jpg"));
	fileList.append(new QFile("textures/negz.jpg"));///course/cs123/bin/textures/astra/negz.jpg"));*/
	m_fileList.append(new QFile("textures/brush2.jpg"));//posx
	m_fileList.append(new QFile("textures/brush2.jpg"));//negx
	m_fileList.append(new QFile("textures/moonclouds.jpg"));//posy
	m_fileList.append(new QFile("textures/grass.jpg"));//negy
	m_fileList.append(new QFile("textures/brush2.jpg"));//posz
	m_fileList.append(new QFile("textures/brush2.jpg"));//negz
	m_cubeMap = ResourceLoader::loadCubeMap(m_fileList);
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
	gluPerspective(m_camera.fovy, ratio, .01, 10000000.f);
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
		m_camera.mouseWheel(event->delta(), &(this->m_cameraTarget));
    }
}

void GLWidget::createShaderPrograms()
{
    const QGLContext *ctx = context();
    m_shaderPrograms["toon"] = newShaderProgram(ctx, "shaders/toon.vert", "shaders/toon.frag");
	m_shaderPrograms["edge_detect"] = newFragShaderProgram(ctx, "shaders/edge_detect.frag");
	m_shaderPrograms["combine"] = newFragShaderProgram(ctx, "shaders/combine.frag");
}
QGLShaderProgram * GLWidget::newShaderProgram(const QGLContext *context, QString vertShader, QString fragShader)
{
    QGLShaderProgram *program = new QGLShaderProgram(context);
    program->addShaderFromSourceFile(QGLShader::Vertex, vertShader);
    program->addShaderFromSourceFile(QGLShader::Fragment, fragShader);
    program->link();
    return program;
}
QGLShaderProgram * GLWidget::newFragShaderProgram(const QGLContext *context, QString fragShader)
{
	QGLShaderProgram *program = new QGLShaderProgram(context);
	program->addShaderFromSourceFile(QGLShader::Fragment, fragShader);
	program->link();
	return program;
}
GLuint GLWidget::loadTexture(const QString &filename)
{
	// Make sure the image file exists
	QFile file(filename);
	if (!file.exists())
		return -1;

	// Load the file into memory
	QImage image;
	image.load(file.fileName());
	image = image.mirrored(false, true);
	m_groundTexture = QGLWidget::convertToGLFormat(image);

	// Generate a new OpenGL texture ID to put our image into
	glGenTextures(1, &m_groundID);

	glBindTexture(GL_TEXTURE_2D, m_groundID);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, m_groundTexture.width(), m_groundTexture.height(), GL_RGBA, GL_UNSIGNED_BYTE, m_groundTexture.bits());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set coordinate wrapping options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);


	return this->m_groundID;
}

void GLWidget::bindGroundTexture() {
	// Make the texture we just created the new active texture

	glBindTexture(GL_TEXTURE_2D, m_groundID);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, m_groundTexture.width(), m_groundTexture.height(), GL_RGBA, GL_UNSIGNED_BYTE, m_groundTexture.bits());

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void GLWidget::releaseGroundTexture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}


