// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QMainWindow>
#include <QCloseEvent>
#include <QByteArray>
#include <QScopedPointer>
#include <QColor>
#include <QImage>
#include <QList>
#include <QString>
#include <QStringList>
#include <QAction>
#include <QMenu>

#include "project.h"


namespace Ui {
class MainWindow;
}

class MainWindowPrivate;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();

public slots:
    void debug(const QString& message);

protected:
    void closeEvent(QCloseEvent*);
#ifdef WITH_WINDOWS_MEDIA_FOUNDATION
    bool nativeEvent(const QByteArray&, void*, long*);
#endif

private slots:
    void initAfterGL(void);
    void about(void);
    void aboutQt(void);
    void showHelp(void);
    void updateWindowTitle(void);
    void badVertexShaderCode(const QString&);
    void badFragmentShaderCode(const QString&);
    void linkerError(const QString&);
    void successfullyLinkedShader(void);
    void parseShadersForParameters();
    void processShaderChange(void);
    void updateShaderSources(void);
    void openRecentProject(void);
    void appendToRecentFileList(const QString& fileName, const QString& listName, QMenu* menu, QAction* actions[]);
    void newProject(void);
    void openProject(void);
    void openProject(const QString& filename);
    void saveProject(void);
    void saveProject(const QString& filename);
    void saveProjectAs(void);
    void closeProject(void);
    void loadImage(void);
    void loadImage(const QString& filename);
    void valueChanged(int);
    void valueChanged(double);
    void valueChanged(bool);
    void valueChanged(const QColor&);
    void acceptColor(void);
    void rejectColor(void);
    void imageDropped(const QImage&);
    void imageDropped(int index, const QImage&);
    void frameDropped(const uchar* data, int w, int h, int index, Project::SourceSelector);
    void frameDropped(const uchar* data, int length, int index, Project::SourceSelector);
    void setCamReady(int);
    void reloadImage(void);
    void saveImageSnapshot(void);
    void batchProcess(void);
    void chooseBackgroundColor(void);
    void setTimerActive(bool);
    void setFPS(double);
    void setMousePos(const QPointF&);
    void processScriptChange(void);
    void executeScript(void);

private: //methods
    void saveSettings(void);
    void restoreSettings(void);

private:
    Ui::MainWindow *ui;

    QScopedPointer<MainWindowPrivate> d_ptr;
    Q_DECLARE_PRIVATE(MainWindow)
    Q_DISABLE_COPY(MainWindow)
};

#endif // __MAINWINDOW_H_
