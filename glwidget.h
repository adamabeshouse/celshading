#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QTime>
#include <QFont>
#include "camera.h"
#include "obj.h"
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
    void paintGL();
    void resizeGL(int width, int height);
    void paintText();
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void applyPerspectiveCamera(float width, float height);
    void addObjects();

protected slots:
    void tick();


private:
    //camera m_camera;
    OrbitCamera m_camera;
    Vector2 m_prevMousePos;
    QTimer m_timer;
    QTime m_clock;
    float m_fps;
    float m_prevFps;
    float m_increment;
    float m_prevTime;
    QHash<QString, QGLShaderProgram *> m_shaderPrograms; // hash map of all shader programs
    QHash<QString, QGLFramebufferObject *> m_framebufferObjects; // hash map of all framebuffer objects
    QGLShaderProgram * newShaderProgram(const QGLContext *context, QString vertShader, QString fragShader);
    void createShaderPrograms();
    OBJ m_obj;
    QVector<OBJ> objects;         //NEW added for scene
    int m_numObjs;            //NEW added for scene
    QFont m_font;
    QString m_defaultModel;
    bool m_captureMouse;
    bool m_shouldRotate;
    bool m_useVbo;
};

#endif // GLWIDGET_H
