// Copyright (c) 2012 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include <QtCore/QDebug>
#include "scriptrunner.h"
#include "glclass.h"
#include "imageclass.h"
#include "util.h"

class ScriptRunnerPrivate {
public:
    ScriptRunnerPrivate(RenderWidget* renderWidget)
        : scriptEngine(new QScriptEngine)
        , renderWidget(renderWidget)
    { /* ... */ }
    ~ScriptRunnerPrivate()
    {
        safeDelete(scriptEngine);
    }
    QScriptEngine* scriptEngine;
    RenderWidget* renderWidget;
    QString scriptSource;
    QScriptValue onFrame;
};

ScriptRunner::ScriptRunner(RenderWidget* renderWidget)
    : d_ptr(new ScriptRunnerPrivate(renderWidget))
{
    Q_D(ScriptRunner);
    // set up scripting engine
    GLClass::Init(d->renderWidget, d->scriptEngine);
    ImageClass::Init(d->scriptEngine);
}

ScriptRunner::~ScriptRunner()
{
    /* ... */
}

const QScriptEngine* ScriptRunner::engine(void) const
{
    return d_ptr->scriptEngine;
}

QScriptEngine* ScriptRunner::engine(void)
{
    return d_ptr->scriptEngine;
}

void ScriptRunner::execute(const QString& source)
{
    d_ptr->scriptSource = source;
    run();
}

void ScriptRunner::onFrame(void)
{
    Q_D(ScriptRunner);
    if (d->onFrame.isFunction())
        d->onFrame.call();
}

void ScriptRunner::run(void)
{
    Q_D(ScriptRunner);
    QScriptValue globals = d->scriptEngine->globalObject();
    globals.setProperty("onFrame", QScriptValue::UndefinedValue);
    globals.setProperty("onMousePosChanged", QScriptValue::UndefinedValue);
    globals.setProperty("W", d->renderWidget->width());
    globals.setProperty("H", d->renderWidget->height());
    const QScriptValue& result = d->scriptEngine->evaluate(d->scriptSource);
    if (result.isError()) {
        emit debug(tr("%1 (line: %2)").arg(result.toString()).arg(result.property("lineNumber").toInt32()));
    }
    else if (d->scriptEngine->hasUncaughtException()) {
        emit debug(d->scriptEngine->uncaughtException().toString());
        QStringListIterator ex(d->scriptEngine->uncaughtExceptionBacktrace());
        while (ex.hasNext())
            emit debug(QString("  %1").arg(ex.next()));
    }
    d->onFrame = globals.property("onFrame");
    if (!d->onFrame.isFunction())
        emit debug(tr("`onFrame()` not defined. Ignoring function."));
}
