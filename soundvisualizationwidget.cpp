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

    // Define visualization area
    QRect visRect = rect().adjusted(20, 20, -220, -20);

    // Set up transformation matrix
    QMatrix4x4 transform;
    transform.translate(visRect.center().x(), visRect.center().y()); // Move to the center of the screen

    // Rotate around the CUBE'S CENTER (origin)
    transform.rotate(rotateX, 1.0f, 0.0f, 0.0f); // Rotate around X-axis
    transform.rotate(rotateY, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis
    transform.rotate(rotateZ, 0.0f, 0.0f, 1.0f); // Rotate around Z-axis

    // Apply the transformation to the painter
    painter.setTransform(transform.toTransform());

    // Draw the cube and sound sources
    drawCube(painter);
    drawSoundSources(painter);
    drawAxisLabels(painter);
}
void SoundVisualizationWidget::drawCube(QPainter &painter) {
    painter.setPen(QPen(Qt::white, 2));

    // Define cube vertices relative to the center
    const float s = cubeSize / 2; // Half the size of the cube
    QVector<QVector3D> vertices = {
        {-s, -s, -s}, {s, -s, -s}, {s, s, -s}, {-s, s, -s}, // Front face
        {-s, -s, s}, {s, -s, s}, {s, s, s}, {-s, s, s}     // Back face
    };

    // Edges connecting the vertices
    QVector<QPair<int, int>> edges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Front face edges
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Back face edges
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Connecting edges
    };

    // Draw all edges
    for (const auto &edge : edges) {
        QVector3D v1 = vertices[edge.first];
        QVector3D v2 = vertices[edge.second];
        QPointF p1 = convertTo3DSpace(v1);
        QPointF p2 = convertTo3DSpace(v2);
        painter.drawLine(p1, p2);
    }
}

void SoundVisualizationWidget::drawSoundSources(QPainter &painter) {
    foreach (const QString &id, soundSources.keys()) {
        QVector3D pos = soundSources[id];
        QPointF screenPos = convertTo3DSpace(pos);

        // Highlight selected source
        bool isSelected = (id == m_selectedId);
        QColor color = isSelected ? Qt::yellow : Qt::red;

        // Depth-based size
        float depthFactor = 1.0f - (pos.z() + cubeSize / 2) / cubeSize;
        int size = 10 + static_cast<int>(20 * depthFactor);

        // Draw with halo effect
        painter.setPen(QPen(color.darker(200), 2));
        painter.setBrush(color);
        painter.drawEllipse(screenPos, size, size);
    }
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

QPointF SoundVisualizationWidget::convertTo3DSpace(QVector3D position) const {
    QRect visRect = rect().adjusted(20, 20, -220, -20);

    // Map 3D coordinates to 2D screen coordinates
    float x = visRect.center().x() + position.x() * visRect.width() / cubeSize;
    float y = visRect.center().y() - position.z() * visRect.height() / cubeSize; // Use Z for depth

    return QPointF(x, y);
}

void SoundVisualizationWidget::mousePressEvent(QMouseEvent *event) {
    // Check if clicking near a sound source
    foreach (const QString &id, soundSources.keys()) {
        QPointF pos = convertTo3DSpace(soundSources[id]);
        if (QLineF(event->pos(), pos).length() < 20) {
            m_selectedId = id;
            emit positionChanged(id, soundSources[id]);
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
            addSoundSource(id, position);
            emit fileDropped(filePath, position);
        }
    }
}

void SoundVisualizationWidget::addSoundSource(const QString &id, QVector3D position) {
    soundSources.insert(id, position);
    update();
}

QVector3D SoundVisualizationWidget::getSoundPosition(const QString &id) const {
    return soundSources.value(id, QVector3D());
}

QMap<QString, QVector3D> SoundVisualizationWidget::getAllSoundSources() const {
    return soundSources;
}
