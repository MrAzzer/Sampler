#include "soundvisualizationwidget.h"
#include <QPainter>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QLinearGradient>
#include <QVBoxLayout>
#include <QLabel>
#include <cmath>

SoundVisualizationWidget::SoundVisualizationWidget(QWidget *parent)
    : QWidget(parent), cubeSize(200.0f), rotateX(0), rotateY(0), rotateZ(0), m_selectedId("") {
    setAcceptDrops(true);
    setStyleSheet("background-color: #444; color: white;");
    setMinimumSize(600, 500);
    createRotationControls();
}

void SoundVisualizationWidget::createRotationControls() {
    QWidget *controls = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(controls);

    auto createSlider = [&](const QString &label, QSlider *&slider, float *rotationVar) {
        QWidget *group = new QWidget;
        QVBoxLayout *groupLayout = new QVBoxLayout(group);
        groupLayout->addWidget(new QLabel(label));
        slider = new QSlider(Qt::Horizontal);
        slider->setRange(0, 360);
        slider->setStyleSheet("QSlider { height: 30px; }");
        groupLayout->addWidget(slider);
        layout->addWidget(group);

        connect(slider, &QSlider::valueChanged, this, [this, rotationVar](int value) {
            *rotationVar = value;
            update();
        });
    };

    createSlider("X Rotation", xRotSlider, &rotateX);
    createSlider("Y Rotation", yRotSlider, &rotateY);
    createSlider("Z Rotation", zRotSlider, &rotateZ);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addStretch(2);
    mainLayout->addWidget(controls);
}

void SoundVisualizationWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set up viewport dimensions
    const QRect viewport = rect().adjusted(50, 50, -50, -50);
    const float aspect = static_cast<float>(viewport.width()) / viewport.height();

    // Set up projection matrix
    QMatrix4x4 projection;
    projection.perspective(45.0f, aspect, 0.1f, 1000.0f);

    // Set up view matrix (camera)
    QMatrix4x4 view;
    view.translate(0, 0, -cubeSize * 2.0f);

    // Set up model matrix
    QMatrix4x4 model;
    model.translate(cubeSize/2, cubeSize/2, cubeSize/2);
    model.rotate(rotateX, 1, 0, 0);
    model.rotate(rotateY, 0, 1, 0);
    model.rotate(rotateZ, 0, 0, 1);
    model.translate(-cubeSize/2, -cubeSize/2, -cubeSize/2);

    QMatrix4x4 mvp = projection * view * model;

    // Draw elements
    drawCube(painter, mvp, viewport);
    drawSoundSources(painter, mvp, viewport);
    drawAxisLabels(painter);
}

void SoundVisualizationWidget::drawCube(QPainter &painter, const QMatrix4x4 &mvp, const QRect &viewport) {
    painter.setPen(QPen(Qt::white, 2));

    // Define cube vertices
    const float s = cubeSize;
    QVector<QVector3D> vertices = {
        {0, 0, 0}, {s, 0, 0}, {s, s, 0}, {0, s, 0},  // Front
        {0, 0, s}, {s, 0, s}, {s, s, s}, {0, s, s}   // Back
    };

    // Project vertices
    QVector<QPointF> screenPoints;
    for (const auto &v : vertices) {
        QVector4D proj = mvp.map(QVector4D(v, 1.0f));
        proj /= proj.w();
        QPointF p(
            viewport.left() + (proj.x() + 1) * 0.5 * viewport.width(),
            viewport.top() + (1 - (proj.y() + 1) * 0.5) * viewport.height()
        );
        screenPoints << p;
    }

    // Define edges and draw
    const QVector<QPair<int, int>> edges = {
        {0,1}, {1,2}, {2,3}, {3,0}, {4,5}, {5,6}, {6,7}, {7,4},
        {0,4}, {1,5}, {2,6}, {3,7}
    };

    for (const auto &edge : edges) {
        painter.drawLine(screenPoints[edge.first], screenPoints[edge.second]);
    }
}

void SoundVisualizationWidget::drawSoundSources(QPainter &painter, const QMatrix4x4 &mvp, const QRect &viewport) {
    foreach (const QString &id, soundSources.keys()) {
        QVector3D pos = soundSources[id];
        QVector4D proj = mvp.map(QVector4D(pos, 1.0f));
        proj /= proj.w();

        QPointF screenPos(
            viewport.left() + (proj.x() + 1) * 0.5 * viewport.width(),
            viewport.top() + (1 - (proj.y() + 1) * 0.5) * viewport.height()
        );

        float depth = (proj.z() + 1) / 2;
        int size = 10 + static_cast<int>(20 * (1 - depth));

        painter.setPen(QPen(id == m_selectedId ? Qt::yellow : Qt::red, 2));
        painter.setBrush(id == m_selectedId ? Qt::yellow : Qt::red);
        painter.drawEllipse(screenPos, size, size);
    }
}

// ... keep existing drawAxisLabels, mouse events, and other methods

QPointF SoundVisualizationWidget::convertTo3DSpace(QVector3D position) const {
    QMatrix4x4 projection;
    projection.perspective(45.0f, width()/height(), 0.1f, 1000.0f);
    QMatrix4x4 view;
    view.translate(0, 0, -cubeSize * 2.0f);

    QVector4D proj = projection * view * QMatrix4x4().map(QVector4D(position, 1.0f));
    proj /= proj.w();

    return QPointF(
        (proj.x() + 1) * 0.5 * width(),
        (1 - (proj.y() + 1) * 0.5) * height()
    );
}

void SoundVisualizationWidget::drawAxisLabels(QPainter &painter) {
    painter.resetTransform();
    painter.setPen(Qt::white);
    QRect visRect = rect().adjusted(20, 20, -220, -20);

    // Draw X, Y, Z labels
    painter.drawText(visRect.center() + QPoint(visRect.width() / 2 + 10, 0), "X");
    painter.drawText(visRect.center() + QPoint(0, -visRect.height() / 2 - 10), "Y");
    painter.drawText(visRect.center() + QPoint(-40, visRect.height() / 2 + 10), "Z");
}



void SoundVisualizationWidget::mousePressEvent(QMouseEvent *event) {
    // Check if clicking near a sound source
    foreach (const QString &id, soundSources.keys()) {
        QPointF pos = convertTo3DSpace(soundSources[id]);
        if (QLineF(event->pos(), pos).length() < 20) {
            m_selectedId = id;
            emit positionChanged(id, soundSources[id]);
            emit nodeSelected(soundSourceFiles[id]); // Emit the file name of the selected node
            update();
            return;
        }
    }
    // Handle rotation if not clicking a source
    lastMousePos = event->pos();
}

void SoundVisualizationWidget::mouseMoveEvent(QMouseEvent *event) {
    QPoint delta = event->pos() - lastMousePos;
    if (event->buttons() & Qt::LeftButton) {
        rotateY += delta.x() * 0.5f; // Rotate around Y-axis (horizontal)
        rotateX += delta.y() * 0.5f; // Rotate around X-axis (vertical)
        updateSliders();
    }
    lastMousePos = event->pos();
    update();
}

void SoundVisualizationWidget::wheelEvent(QWheelEvent *event) {
    cubeSize *= (event->angleDelta().y() > 0) ? 1.1f : 0.9f;
    cubeSize = qBound(50.0f, cubeSize, 500.0f);
    update();
}

void SoundVisualizationWidget::updateSliders() {
    xRotSlider->setValue(static_cast<int>(rotateX) % 360);
    yRotSlider->setValue(static_cast<int>(rotateY) % 360);
    zRotSlider->setValue(static_cast<int>(rotateZ) % 360);
}

void SoundVisualizationWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) event->acceptProposedAction();
}

void SoundVisualizationWidget::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QPoint dropPos = event->position().toPoint();
        QVector3D position(
            (dropPos.x() - width() / 2) * cubeSize / (width() - 240),
            0,
            (height() / 2 - dropPos.y()) * cubeSize / (height() - 40)
        );
        for (const QUrl &url : mimeData->urls()) {
            QString filePath = url.toLocalFile();
            QString id = QString::number(qHash(filePath));
            addSoundSource(id, position, filePath);
            emit fileDropped(filePath, position);
        }
    }
}

void SoundVisualizationWidget::addSoundSource(const QString &id, QVector3D position, const QString &filePath) {
    soundSources.insert(id, position);
    soundSourceFiles.insert(id, filePath); // Store the file path
    update();
}

QVector3D SoundVisualizationWidget::getSoundPosition(const QString &id) const {
    return soundSources.value(id, QVector3D());
}

QMap<QString, QVector3D> SoundVisualizationWidget::getAllSoundSources() const {
    return soundSources;
}

QString SoundVisualizationWidget::getSelectedFileName() const {
    return soundSourceFiles.value(m_selectedId, "");
}
