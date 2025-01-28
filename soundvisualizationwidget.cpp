#include "soundvisualizationwidget.h"
#include <QPainter>

SoundVisualizationWidget::SoundVisualizationWidget(QWidget *parent)
    : QWidget(parent), soundX(0), soundY(0), soundZ(0)
{
    setStyleSheet("background-color: #444;");
    setMinimumSize(200, 200);  // Ensure the widget has a minimum size
}

void SoundVisualizationWidget::setSoundPosition(float x, float y, float z)
{
    soundX = x;
    soundY = y;
    soundZ = z;
    update(); // Trigger a repaint
}

void SoundVisualizationWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw a simple 2D representation of the sound source
    int centerX = width() / 2;
    int centerY = height() / 2;

    // Draw the listener (represented by a blue circle)
    painter.setBrush(Qt::blue);
    painter.drawEllipse(QPoint(centerX, centerY), 10, 10);  // Listener radius is 10

    // Scale and constrain the sound source position
    float maxRangeX = static_cast<float>(width()) / 2 - 10;  // Max range considering the listener radius
    float maxRangeY = static_cast<float>(height()) / 2 - 10; // Max range considering the listener radius

    // Scale the sound position to the visualization space, keeping it within bounds
    int soundPosX = centerX + static_cast<int>(qBound(-maxRangeX, soundX * 10, maxRangeX));  // Constrain within the widget width
    int soundPosY = centerY - static_cast<int>(qBound(-maxRangeY, soundY * 10, maxRangeY)); // Constrain within the widget height

    // Draw the sound source (represented by a red circle)
    painter.setBrush(Qt::red);
    painter.drawEllipse(QPoint(soundPosX, soundPosY), 10, 10);  // Sound source radius is 10
}
