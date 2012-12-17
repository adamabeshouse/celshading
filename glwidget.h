#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QTime>
#include <QFont>
#include "camera.h"
#include "obj.h"
#include "particles/particleemitter.h"
#include "particles/rain.h"
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
	Vector3 m_cameraTarget;

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
	QList<QFile *> m_fileList;
	void createSobelKernel(GLfloat* kernel, GLfloat* offsets);
    QHash<QString, QGLShaderProgram *> m_shaderPrograms; // hash map of all shader programs
    QHash<QString, QGLFramebufferObject *> m_framebufferObjects; // hash map of all framebuffer objects
    QGLShaderProgram * newShaderProgram(const QGLContext *context, QString vertShader, QString fragShader);
	QGLShaderProgram * newFragShaderProgram(const QGLContext *context, QString fragShader);
	void renderGround();
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
        Rain m_rain;
	GLuint groundTex;
	QVector<float> m_treeAngles;
	QVector<float> m_treeSizes;
	GLuint loadTexture(const QString &filename);
	GLuint m_groundID;
	QImage m_groundTexture;
	void bindGroundTexture();
	void releaseGroundTexture();
};

#endif // GLWIDGET_H
