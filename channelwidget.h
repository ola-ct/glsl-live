// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __CHANNELWIDGET_H_
#define __CHANNELWIDGET_H_

#include <QObject>
#include <QWidget>
#include <QPaintEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QScopedPointer>

class ChannelWidgetPrivate;

class ChannelWidget : public QWidget
{
    Q_OBJECT
public:
    enum Type {
        None = 0,
        Auto,
        Image,
        Video,
        Sound,
        Data2D,
        Data3D
    };

    explicit ChannelWidget(int index, QWidget* parent = NULL);
    ~ChannelWidget();
    QSize minimumSizeHint(void) const { return QSize(40, 40); }
    QSize sizeHint(void) const { return QSize(80, 80); }

    int heightForWidth(int w) const { return w; }

    void load(const QString& filename, Type = Auto);
    int index(void) const;

    static QString channelName(int);

protected:
    void paintEvent(QPaintEvent*);
    void dragEnterEvent(QDragEnterEvent*);
    void dragLeaveEvent(QDragLeaveEvent*);
    void dropEvent(QDropEvent*);

signals:
    void imageDropped(int index, const QImage&);

public slots:

private slots:
    void showContextMenu(const QPoint&);

private: // variables
    QScopedPointer<ChannelWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ChannelWidget)
    Q_DISABLE_COPY(ChannelWidget)
};

#endif // __CHANNELWIDGET_H_