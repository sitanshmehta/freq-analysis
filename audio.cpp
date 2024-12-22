#include "audio.h"
#include <QFile>
#include <QUrl>
#include <QDebug>
#include <QAudioDecoder>
#include <QAudioBuffer>
#include <QEventLoop>

Audio::Audio(QObject *parent)
    : QObject(parent)
{}

Audio::~Audio(){}


QVector<float> Audio::get_data() const
{
    return data;
}

DataInput::DataType Audio::get_data_type() const
{
    return DataInput::DataType::AUDIO;
}

bool Audio::load_from_file(const QString &path)
{
    QUrl url(path);
    QString local_path = url.toLocalFile();

    QFile file(local_path);

    if(!file.open(QIODevice::ReadOnly)){
        qWarning() << "File not open";
        return false;
    }

    QString file_type = check_file_type(file);

    if(file_type == "M4A"){
        qDebug() << "Processed: " << process_M_four_A(file);
    }

    return true;
}

QString Audio::check_file_type(QFile &file)
{
    QByteArray ftyp;

    file.seek(8); //skip the first 8 bytes
    ftyp = file.read(4);

    if (ftyp == "M4A " || ftyp == "M4B ") {
        return "M4A";
    } else if (ftyp == "isom" || ftyp == "mp42") {
        return "MP4";
    }
    return "";
}

/*
bool Audio::process_M_four_A(QFile &file)
{
    if (!file.isOpen()) {
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open file for M4A processing.";
            return false;
        }
    }

    // Use QAudioDecoder to read audio samples asynchronously
    QAudioDecoder decoder;
    decoder.setSource(file.fileName());

    // Temporary event loop to wait for decoding
    QEventLoop loop;

    // Connect signals
    connect(&decoder, &QAudioDecoder::bufferReady, this, [this, &decoder]() {
        // Retrieve next decoded buffer
        QAudioBuffer buffer = decoder.read();
        // Convert buffer data to float samples
        convert_buff_to_float(buffer);
    });

    connect(&decoder, &QAudioDecoder::finished, &loop, &QEventLoop::quit);

    connect(&decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error),
            [&](QAudioDecoder::Error error){

        qWarning() << "Decoder error:" << error << decoder.errorString();
        loop.quit();
    });

    // Start decoding
    decoder.start();
    loop.exec(); // Wait until finished or error

    return true;
}
*/


bool Audio::process_M_four_A(QFile &file)
{
    // 1. Open the file if not already open
    if (!file.isOpen()) {
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open file for M4A processing.";
            return false;
        }
    }

    // 2. Create the decoder and set the source
    QAudioDecoder decoder;
    decoder.setSource(file.fileName());

    // 3. Create an event loop
    QEventLoop loop;

    // 4. Connect signals properly

    // a) Buffer ready: read data and convert it
    connect(&decoder, &QAudioDecoder::bufferReady,
            this, [this, &decoder]() {
                QAudioBuffer buffer = decoder.read();
                convert_buff_to_float(buffer);  // Your function that appends float samples
            });

    // b) Finished: quit the event loop
    connect(&decoder, &QAudioDecoder::finished,
            &loop, &QEventLoop::quit);

    // c) Error signal: log and quit
    //    - Use &decoder (not decoder)
    //    - Capture by reference to avoid copying
    connect(&decoder,
            QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error),
            &loop, [&](QAudioDecoder::Error error) {
                qWarning() << "Decoder error:" << error << decoder.errorString();
                loop.quit();
            });

    // 5. Start decoding and block until finished or error
    decoder.start();
    loop.exec();

    return true; // Success (unless you added more error checks)
}

void Audio::convert_buff_to_float(const QAudioBuffer &buffer)
{
    const QAudioFormat format = buffer.format();
    QAudioFormat::SampleFormat sf = format.sampleFormat(); // enum: Int16, Int32, Float, etc.
    int totalSamples = buffer.sampleCount();               // total samples
    int channelCount = format.channelCount();

    // Example for 16-bit signed integer
    if (sf == QAudioFormat::Int16) {
        const qint16 *dataPtr = buffer.constData<qint16>();
        for (int i = 0; i < totalSamples; ++i) {
            float sample = dataPtr[i] / 32768.0f; // normalize [-32768,32767] -> [-1,1]
            this->data.push_back(sample);
        }
    }
    // Example for 32-bit signed integer
    else if (sf == QAudioFormat::Int32) {
        const qint32 *dataPtr = buffer.constData<qint32>();
        for (int i = 0; i < totalSamples; ++i) {
            // Optionally normalize if needed, e.g. dataPtr[i] / (float)(1 << 31)
            float sample = static_cast<float>(dataPtr[i]) / 2147483648.0f;
            this->data.push_back(sample);
        }
    }
    // Example for 32-bit float PCM
    else if (sf == QAudioFormat::Float) {
        const float *dataPtr = buffer.constData<float>();
        for (int i = 0; i < totalSamples; ++i) {
            this->data.push_back(dataPtr[i]);
        }
    }
    else {
        qWarning() << "Unsupported format:" << sf
                   << " sampleRate=" << format.sampleRate()
                   << " channelCount=" << channelCount;
    }
}


