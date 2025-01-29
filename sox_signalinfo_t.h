#include <sox.h>
#include <sndfile.h>
#include <QMessageBox>
#include <QString>

void AudioWidget::processAudio(const QString &saveLocation)
{
    // Open input file using libsndfile
    SF_INFO sfinfo;
    SNDFILE *infile = sf_open(inputFile.toStdString().c_str(), SFM_READ, &sfinfo);
    if (!infile) {
        QMessageBox::warning(this, tr("Error"), tr("Unable to open input file."));
        return;
    }

    // Prepare SoX signal information
    sox_signalinfo_t signalInfo;
    signalInfo.rate = sfinfo.samplerate;       // Sampling rate from SF_INFO
    signalInfo.channels = sfinfo.channels;     // Channels from SF_INFO
    signalInfo.precision = 16;                 // 16-bit audio
    signalInfo.encoding = SOX_ENCODING_SIGN2;  // Signed 2's complement encoding

    // Create SoX format structure
    sox_format_t *out = sox_open_write(saveLocation.toStdString().c_str(), &signalInfo, nullptr, nullptr, nullptr, nullptr);
    if (!out) {
        sf_close(infile);
        QMessageBox::warning(this, tr("Error"), tr("Unable to open the output file for writing."));
        return;
    }

    // Buffers for reading and writing audio data
    const int bufferSize = 1024;
    short inputBuffer[bufferSize];
    short outputBuffer[bufferSize];

    // Read from input file and write to output file using SoX
    sf_count_t readCount;
    while ((readCount = sf_read_short(infile, inputBuffer, bufferSize)) > 0) {
        // Process the audio (you can modify this block to apply effects)
        for (int i = 0; i < readCount; ++i) {
            outputBuffer[i] = inputBuffer[i];  // Simple pass-through, modify for effects
        }

        // Write to SoX output file
        if (sox_write(out, outputBuffer, readCount) != readCount) {
            sf_close(infile);
            sox_close(out);
            QMessageBox::warning(this, tr("Error"), tr("Error writing to output file."));
            return;
        }
    }

    // Close files
    sf_close(infile);
    sox_close(out);

    // Notify the user
    QMessageBox::information(this, tr("Success"), tr("Audio file processed successfully."));
}
