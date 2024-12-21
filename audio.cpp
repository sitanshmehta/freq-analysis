#include "audio.h"
#include <QFile>
#include <QUrl>
#include <QDebug>

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
        qDebug() << "HERE";
    } else {
        qDebug() << "NO";
    }

    return true;
}

QString Audio::check_file_type(const QFile &file)
{
    QByteArray ftyp;
    for(int i = 0; i < 4; i++){
        ftyp = file.readLine();
        process_line(ftyp);
    }

    if (ftyp == "M4A " || ftyp == "M4B ") {
        return "M4A";
    } else if (ftyp == "isom" || ftyp == "mp42") {
        return "MP4";
    }
    return "Unknown";
}




