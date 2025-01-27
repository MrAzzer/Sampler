#include "audiowidget.h"
#include "soundvisualizationwidget.h"

AudioWidget::AudioWidget(QWidget *parent)
    : QWidget(parent), fileDialog(nullptr), isPlaying(false), maxDistance(10.0f) {
    setMinimumSize(900, 700);
    auto *mainLayout = new QHBoxLayout(this);
    auto *leftPanel = new QVBoxLayout;

    // File upload section
    auto *fileLayout = new QHBoxLayout;
    fileEdit = new QLineEdit;
    fileEdit->setPlaceholderText(tr("Audio File"));
    fileLayout->addWidget(fileEdit);
    fileDialogButton = new QPushButton(tr("Choose..."));
    fileLayout->addWidget(fileDialogButton);
    leftPanel->addLayout(fileLayout);

    // File tree view
    fileTreeView = new QTreeView;
    fileSystemModel = new QFileSystemModel;
    fileSystemModel->setRootPath("/Users/spikespiegel/Library/CloudStorage/OneDrive-UniwersytetWrocławski/Uwr/Sampler/Spatial-Audio-Plugin/sample");
    fileTreeView->setModel(fileSystemModel);
    fileTreeView->setRootIndex(fileSystemModel->index("/Users/spikespiegel/Library/CloudStorage/OneDrive-UniwersytetWrocławski/Uwr/Sampler/Spatial-Audio-Plugin/sample"));
    leftPanel->addWidget(fileTreeView);

    mainLayout->addLayout(leftPanel);

    // Center Panel: 3D Visualization
    auto *centerPanel = new QWidget;
    centerPanel->setStyleSheet("background-color: #333;"); // Dark background for visualization
    auto *centerLayout = new QVBoxLayout(centerPanel);

    // Add a label for the visualization
    auto *visualizationLabel = new QLabel(tr("Sound Source Visualization"));
    visualizationLabel->setAlignment(Qt::AlignCenter);
    visualizationLabel->setStyleSheet("color: white; font-size: 16px;");
    centerLayout->addWidget(visualizationLabel);

    // Add the custom visualization widget
    soundVisualizationWidget = new SoundVisualizationWidget;
    centerLayout->addWidget(soundVisualizationWidget);

    mainLayout->addWidget(centerPanel);

    // Right Panel: Knobs (Sliders)
    auto *rightPanel = new QVBoxLayout;

    // Add a label to display the selected file name
    fileNameLabel = new QLabel(tr("Selected File: None"));
    fileNameLabel->setAlignment(Qt::AlignCenter);
    fileNameLabel->setStyleSheet("color: white; font-size: 14px;");
    rightPanel->addWidget(fileNameLabel);

    rightPanel->addWidget(new QLabel(tr("Azimuth (-180 - 180 degree):")));
    azimuth = new QSlider(Qt::Horizontal);
    azimuth->setRange(-180, 180);
    rightPanel->addWidget(azimuth);

    rightPanel->addWidget(new QLabel(tr("Elevation (-90 - 90 degree):")));
    elevation = new QSlider(Qt::Horizontal);
    elevation->setRange(-90, 90);
    rightPanel->addWidget(elevation);

    rightPanel->addWidget(new QLabel(tr("Distance (0 - 10 meter):")));
    distance = new QSlider(Qt::Horizontal);
    distance->setRange(0, 1000);
    distance->setValue(100);
    rightPanel->addWidget(distance);

    rightPanel->addWidget(new QLabel(tr("Occlusion (0 - 4):")));
    occlusion = new QSlider(Qt::Horizontal);
    occlusion->setRange(0, 400);
    rightPanel->addWidget(occlusion);

    rightPanel->addWidget(new QLabel(tr("Room dimension (0 - 100 meter):")));
    roomDimension = new QSlider(Qt::Horizontal);
    roomDimension->setRange(0, 10000);
    roomDimension->setValue(1000);
    rightPanel->addWidget(roomDimension);

    rightPanel->addWidget(new QLabel(tr("Reverb gain (0-5):")));
    reverbGain = new QSlider(Qt::Horizontal);
    reverbGain->setRange(0, 500);
    reverbGain->setValue(0);
    rightPanel->addWidget(reverbGain);

    rightPanel->addWidget(new QLabel(tr("Reflection gain (0-5):")));
    reflectionGain = new QSlider(Qt::Horizontal);
    reflectionGain->setRange(0, 500);
    reflectionGain->setValue(0);
    rightPanel->addWidget(reflectionGain);

    rightPanel->addWidget(new QLabel(tr("Output mode:")));
    mode = new QComboBox;
    mode->addItem(tr("Surround"), QVariant::fromValue(QAudioEngine::Surround));
    mode->addItem(tr("Stereo"), QVariant::fromValue(QAudioEngine::Stereo));
    mode->addItem(tr("Headphone"), QVariant::fromValue(QAudioEngine::Headphone));
    rightPanel->addWidget(mode);

    animateButton = new QCheckBox(tr("Animate sound position"));
    rightPanel->addWidget(animateButton);

    playButton = new QPushButton(tr("Play Audio"));
    playButton->setEnabled(false);
    rightPanel->addWidget(playButton);

    mainLayout->addLayout(rightPanel);

    // Connect signals and slots
    connect(playButton, &QPushButton::clicked, this, &AudioWidget::playAudio);
    connect(fileEdit, &QLineEdit::textChanged, this, &AudioWidget::fileChanged);
    connect(fileDialogButton, &QPushButton::clicked, this, &AudioWidget::openFileDialog);
    connect(azimuth, &QSlider::valueChanged, this, &AudioWidget::updatePosition);
    connect(elevation, &QSlider::valueChanged, this, &AudioWidget::updatePosition);
    connect(distance, &QSlider::valueChanged, this, &AudioWidget::updatePosition);
    connect(occlusion, &QSlider::valueChanged, this, &AudioWidget::newOcclusion);
    connect(roomDimension, &QSlider::valueChanged, this, &AudioWidget::updateRoom);
    connect(reverbGain, &QSlider::valueChanged, this, &AudioWidget::updateRoom);
    connect(reflectionGain, &QSlider::valueChanged, this, &AudioWidget::updateRoom);
    connect(mode, &QComboBox::currentIndexChanged, this, &AudioWidget::modeChanged);
    connect(animateButton, &QCheckBox::toggled, this, &AudioWidget::animateChanged);

    // Initialize audio components
    room = new QAudioRoom(&engine);
    room->setWallMaterial(QAudioRoom::BackWall, QAudioRoom::Marble);
    room->setWallMaterial(QAudioRoom::FrontWall, QAudioRoom::Marble);
    room->setWallMaterial(QAudioRoom::LeftWall, QAudioRoom::Marble);
    room->setWallMaterial(QAudioRoom::RightWall, QAudioRoom::Marble);
    room->setWallMaterial(QAudioRoom::Floor, QAudioRoom::Marble);
    room->setWallMaterial(QAudioRoom::Ceiling, QAudioRoom::Marble);
    updateRoom();

    listener = new QAudioListener(&engine);
    listener->setPosition({});
    listener->setRotation({});
    engine.start();

    sound = new QSpatialSound(&engine);
    updatePosition();

    animation = new QPropertyAnimation(azimuth, "value");
    animation->setDuration(10000);
    animation->setStartValue(-180);
    animation->setEndValue(180);
    animation->setLoopCount(-1);

    // Connect signals and slots for visualization
    connect(soundVisualizationWidget, &SoundVisualizationWidget::positionChanged,
            this, &AudioWidget::handlePositionChanged);
    connect(soundVisualizationWidget, &SoundVisualizationWidget::fileDropped,
            this, &AudioWidget::handleFileDropped);

    connect(soundVisualizationWidget, &SoundVisualizationWidget::positionChanged,
            this, &AudioWidget::updateKnobs);
    connect(soundVisualizationWidget, &SoundVisualizationWidget::fileDropped,
            this, &AudioWidget::handleFileDropped);
    connect(soundSourceSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AudioWidget::onSoundSourceSelected);
    connect(soundVisualizationWidget, &SoundVisualizationWidget::playSelectedFileRequested, this, &AudioWidget::handlePlaySelectedFileRequested);


    // Enable drag on the tree view
    fileTreeView->setDragEnabled(true);
    fileTreeView->viewport()->setAcceptDrops(true);
    fileTreeView->setDropIndicatorShown(true);
    fileTreeView->setDragDropMode(QAbstractItemView::DragOnly);

    soundSourceSelector = new QComboBox;
    rightPanel->addWidget(new QLabel(tr("Select Sound Source:")));
    rightPanel->addWidget(soundSourceSelector);

    // Connect the ComboBox to update knobs
    connect(soundSourceSelector, &QComboBox::currentTextChanged, this, [this](const QString &id) {
        QVector3D pos = soundVisualizationWidget->getSoundPosition(id);
        this->updateKnobs(id, pos); // Use 'this->' to call the member function
    });

    // Add "Play All Files" button
    playAllButton = new QPushButton("Play All Files", this);
    connect(playAllButton, &QPushButton::clicked, this, &AudioWidget::playAllFiles);
    rightPanel->addWidget(playAllButton);

    // Connect the "Play Selected File" signal
    connect(soundVisualizationWidget, &SoundVisualizationWidget::playSelectedFileRequested,
            this, &AudioWidget::handlePlaySelectedFileRequested);


    // Setup visualization
    setupVisualization();
}

//============================================================LEFT============================================================
void AudioWidget::setFile(const QString &file)
{
    fileEdit->setText(file);
}

void AudioWidget::playAudio()
{
    if (soundSources.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No audio files added for playback."));
        return;
    }

    if (isPlaying) {
        // Stop all sounds
        foreach (QSpatialSound *sound, soundSources) {
            sound->stop();
        }
        isPlaying = false;
        playButton->setText(tr("Play Audio"));
    } else {
        // Play all sounds with their 3D effects
        foreach (QSpatialSound *sound, soundSources) {
            sound->play();
        }
        isPlaying = true;
        playButton->setText(tr("Stop Audio"));
    }
}

//==========================================================Center===========================================================
void AudioWidget::updatePosition() {
    foreach (const QString &id, soundSources.keys()) {
        QVector3D pos = soundVisualizationWidget->getSoundPosition(id);
        QSpatialSound* sound = soundSources[id];

        // Calculate distance attenuation
        float dist = pos.length();
        sound->setVolume(qBound(0.0f, 1.0f - (dist / maxDistance), 1.0f));

        // Apply 3D positioning
        sound->setPosition(pos);

        // Update occlusion based on walls
        updateOcclusion(sound, pos);
    }
}
void AudioWidget::newOcclusion()
{
    sound->setOcclusionIntensity(occlusion->value() / 100.);
}

void AudioWidget::modeChanged()
{
    engine.setOutputMode(mode->currentData().value<QAudioEngine::OutputMode>());
}

void AudioWidget::fileChanged(const QString &file)
{
    if (QFile::exists(file)) {
        sound->setSource(QUrl::fromLocalFile(file));
        sound->setSize(5);
        sound->setLoops(QSpatialSound::Infinite);
        playButton->setEnabled(true);
    } else {
        playButton->setEnabled(false);
    }
}

void AudioWidget::openFileDialog()
{
    if (fileDialog == nullptr) {
        const QString dir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
        fileDialog = new QFileDialog(this, tr("Open Audio File"), dir);
        fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
        const QStringList mimeTypes{"audio/mpeg", "audio/aac", "audio/x-ms-wma",
                                    "audio/x-flac+ogg", "audio/x-wav"};
        fileDialog->setMimeTypeFilters(mimeTypes);
        fileDialog->selectMimeTypeFilter(mimeTypes.constFirst());
    }

    if (fileDialog->exec() == QDialog::Accepted)
        fileEdit->setText(fileDialog->selectedFiles().constFirst());
}

void AudioWidget::updateRoom()
{
    const float d = roomDimension->value();
    room->setDimensions(QVector3D(d, d, 400));
    room->setReflectionGain(float(reflectionGain->value()) / 100);
    room->setReverbGain(float(reverbGain->value()) / 100);
}

void AudioWidget::animateChanged(bool checked)
{
    if (checked)
        animation->start();
    else
        animation->stop();
}

void AudioWidget::handlePositionChanged(QString id, QVector3D pos) {
    if (soundSources.contains(id)) {
        QSpatialSound *sound = soundSources[id];
        sound->setPosition(pos);

        // Update spatial effects based on position
        updateOcclusion(sound, pos);
        updateDistanceAttenuation(sound, pos);
    } // Add this closing brace
}

void AudioWidget::handleFileDropped(QString filePath, QVector3D position) {
    if (QFile::exists(filePath)) {
        QString id = QString::number(qHash(filePath));

        if (!soundSources.contains(id)) {
            QSpatialSound *sound = new QSpatialSound(&engine);
            sound->setSource(QUrl::fromLocalFile(filePath));
            sound->setPosition(position);
            sound->setSize(0.5f);
            soundSources.insert(id, sound);

            // Add the new sound source to the ComboBox
            soundSourceSelector->addItem(filePath, id);
        }

        soundSources[id]->setPosition(position);
        soundVisualizationWidget->addSoundSource(id, position, filePath);
    }
}

void AudioWidget::onSoundSourceSelected(int index) {
    QString id = soundSourceSelector->itemData(index).toString();
    QVector3D pos = soundVisualizationWidget->getSoundPosition(id); // Use soundVisualizationWidget
    updateKnobs(id, pos);
}

void AudioWidget::updateOcclusion(QSpatialSound *sound, QVector3D position) {
    // Example occlusion calculation
    float occlusionValue = 0.0f;
    if (position.x() < -5.0f || position.x() > 5.0f ||
        position.y() < -5.0f || position.y() > 5.0f ||
        position.z() < -5.0f || position.z() > 5.0f) {
        occlusionValue = 1.0f; // Full occlusion if outside room bounds
    }
    sound->setOcclusionIntensity(occlusionValue);
}

void AudioWidget::updateDistanceAttenuation(QSpatialSound *sound, QVector3D position) {
    float distance = position.length();
    float volume = qBound(0.0f, 1.0f - (distance / maxDistance), 1.0f);
    sound->setVolume(volume);
}

void AudioWidget::setupVisualization() {
    // Connect the nodeSelected signal to update the file name display
    connect(soundVisualizationWidget, &SoundVisualizationWidget::nodeSelected, this, [this](const QString &filePath) {
        // Update the file name display
        fileNameLabel->setText("Selected File: " + filePath);
    });
}

void AudioWidget::handlePlaySelectedFileRequested(const QString &filePath) {
    if (soundSources.contains(filePath)) {
        QSpatialSound *sound = soundSources[filePath];
        sound->play();
    }
}

void AudioWidget::playAllFiles() {
    foreach (QSpatialSound *sound, soundSources) {
        sound->play();
    }
}

//===========================================================RIGHT=============================================================

void AudioWidget::updateKnobs(QString id, QVector3D position) {
    if (soundSources.contains(id)) {
        QSpatialSound *sound = soundSources[id];
        QVector3D pos = sound->position();
        azimuth->setValue(static_cast<int>(pos.x()));
        elevation->setValue(static_cast<int>(pos.y()));
        distance->setValue(static_cast<int>((pos.length())));
    }
}
