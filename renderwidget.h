// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __RENDERWIDGET_H_
#define __RENDERWIDGET_H_

#include <QGLFunctions>
#include <QGLWidget>
#include <QPoint>
#include <QPointF>
#include <QString>
#include <QImage>
#include <QMap>
#include <QVariant>
#include <QTimerEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QResizeEvent>
#include <QTimerEvent>
#include <QScopedPointer>
#include <QGLShaderProgram>

class RenderWidgetPrivate;

class RenderWidget : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT
public:
    explicit RenderWidget(QWidget* parent = NULL);
    ~RenderWidget();

    virtual QSize minimumSizeHint(void) const { return QSize(240, 160); }
    virtual QSize sizeHint(void) const  { return QSize(640, 480); }
    void setShaderSources(const QString& vs, const QString& fs);
    void setVertexShaderSource(const QString&);
    const QString& vertexShaderSource(void) const;
    void setFragmentShaderSource(const QString&);
    const QString& fragmentShaderSource(void) const;
    const QString& imageFileName(void) const;
    bool loadImage(const QString& fileName);
    const QImage& image(void) const;
    void setImage(const QImage& = QImage());
    void setChannel(int index, const uchar* data, int w, int h);
    void setChannel(int index, const uchar* data, int length);
    void setUniformValue(const QString& name, int value);
    void setUniformValue(const QString& name, double value);
    void setUniformValue(const QString& name, bool value);
    void setUniformValue(const QString& name, const QColor& value);
    void setUniforms(const QMap<QString, QVariant>&);
    const QMap<QString, QVariant>& uniforms(void) const;
    void updateUniforms(void);
    void clearUniforms(void);
    QImage resultImage(void);
    double scale(void) const;
    void goLive(void);
    void stopCode(void);
    void updateViewport(void);
    QString glVersionString(void) const;
    bool isTimerActive(void) const;
    QGLShaderProgram* shaderProgram(void);
    bool build(void);

public slots:
    void setChannel(int index, const QImage& = QImage());
    void setScale(double factor);
    void fitImageToWindow(void);
    void resizeToOriginalImageSize(void);
    void enableAlpha(bool enabled = true);
    void enableImageRecycling(bool enabled = true);
    void enableInstantUpdate(bool enabled = true);
    void setBackgroundColor(const QColor&);
    void feedbackOneFrame(void);
    void setTimerActive(bool);
    void clampToBorder(bool);

signals:
    void ready(void);
    void frameReady(void);
    void vertexShaderError(QString);
    void fragmentShaderError(QString);
    void linkerError(QString);
    void linkingSuccessful(void);
    void imageDropped(const QImage&);
    void fpsChanged(double);
    void mousePosChanged(const QPointF&);

protected:
    void resizeEvent(QResizeEvent*);
    void initializeGL(void);
    void resizeGL(int, int);
    void paintGL(void);
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);
    void timerEvent(QTimerEvent*);

private: // methods
    void buildProgram(const QString& vs, const QString& fs);
    void makeImageFBO(void);
    void configureTexture(void);
    void configureAudioTexture(int index);
    void updateViewport(const QSize&);
    void updateViewport(int w, int h);
    void scrollBy(const QPoint&);
    void startMotion(const QPointF& velocity);
    void stopMotion(void);

    static const int MaxWaveLength = 2048;

private:

    QScopedPointer<RenderWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(RenderWidget)
    Q_DISABLE_COPY(RenderWidget)
};

#endif // __RENDERWIDGET_H_
