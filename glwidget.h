#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QTime>
#include <QFont>
#include "camera.h"
#include "obj.h"
#include "particles/particleemitter.h"
class QGLShaderProgram;
class QGLFramebufferObject;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void updateCamera();
public slots:
    void fileOpen();
    void toggleRotate();
    void toggleVboDraw();

protected:
    void initializeGL();
    void renderScene(int width, int height);
    void paintGL();
    void resizeGL(int width, int height);
    void paintText();
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void applyPerspectiveCamera(float width, float height);
    void addObjects();
	void renderTexturedQuad(int width, int height);
	void applyOrthogonalCamera(float width, float height);

protected slots:
    void tick();


private:
    //camera m_camera;
    OrbitCamera m_camera;
    Vector2 m_prevMousePos;
    QTimer m_timer;
    QTime m_clock;
	GLuint m_skybox;
	GLuint m_cubeMap;
    float m_fps;
    float m_prevFps;
    float m_increment;
    float m_prevTime;
	void loadCubeMap();
    QHash<QString, QGLShaderProgram *> m_shaderPrograms; // hash map of all shader programs
    QHash<QString, QGLFramebufferObject *> m_framebufferObjects; // hash map of all framebuffer objects
    QGLShaderProgram * newShaderProgram(const QGLContext *context, QString vertShader, QString fragShader);
	QGLShaderProgram * newFragShaderProgram(const QGLContext *context, QString fragShader);
	void createFramebufferObjects(int width, int height);
    void createShaderPrograms();
    OBJ m_obj;
    QVector<OBJ> objects;         //NEW added for scene
    int m_numObjs;            //NEW added for scene

    QFont m_font;
    QString m_defaultModel;
    bool m_captureMouse;
    bool m_shouldRotate;
    bool m_useVbo;
	unsigned int m_numTrees;
	float m_treeRadius;
	ParticleEmitter m_fire;
	ParticleEmitter m_fire2;
	QVector<float> m_treeAngles;
	QVector<float> m_treeSizes;
};

#endif // GLWIDGET_H
