// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include <QtCore/QDebug>

#include "camera_win.h"


class WebcamPrivate
{
public:
    WebcamPrivate(void)
    {
    }

    QImage frame;
};

Webcam::Webcam(int id, QObject* parent)
    : QObject(parent)
    , d_ptr(new WebcamPrivate)
{
    if (id >= 0)
        open(id);
}

Webcam::~Webcam()
{
    close();
}

bool Webcam::isOpen(void) const
{
    return true; // XXX
}

bool Webcam::open(int)
{
    return true;
}

void Webcam::close(void)
{
    // TODO
}

const QImage& Webcam::getFrame(void) const
{
    return d_ptr->frame;
}

void Webcam::getRawFrame(const uchar*& data, int& w, int& h) const
{
    if (data)
        data = 0;
    w = -1;
    h = -1;
}

QSize Webcam::frameSize(void) const
{
    return d_ptr->frame.size();
}

void Webcam::setSize(const QSize&)
{
    // TODO
}
