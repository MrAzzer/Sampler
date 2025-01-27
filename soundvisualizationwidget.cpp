#include "soundvisualizationwidget.h"
SoundVisualizationWidget::SoundVisualizationWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #444;");
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
    // Draw a circle representing the listener
    painter.setBrush(Qt::blue);
    painter.drawEllipse(QPoint(centerX, centerY), 10, 10);
    // Draw the sound source position
    int soundPosX = centerX + static_cast<int>(soundX * 10); // Scale for visualization
    int soundPosY = centerY - static_cast<int>(soundY * 10); // Invert Y for correct orientation
    painter.setBrush(Qt::red);
    painter.drawEllipse(QPoint(soundPosX, soundPosY), 10, 10);
}
