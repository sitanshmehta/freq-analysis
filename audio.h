#ifndef AUDIO_H
#define AUDIO_H

#include <QVector>
#include <data_input.h>
#include <QObject>
#include <QAudioBuffer>
#include <QFile>

class Audio : public QObject, public DataInput
{
    Q_OBJECT

public:
    explicit Audio(QObject *parent = nullptr);
    ~Audio();

    Q_INVOKABLE QVector<float> get_data() const override;
    Q_INVOKABLE DataInput::DataType get_data_type() const override;

    Q_INVOKABLE bool load_from_file(const QString &path);
    QString check_file_type(QFile &file);
    bool process_M_four_A(QFile &file);
    void convert_buff_to_float(const QAudioBuffer &buffer);

private:
    QVector<float> data;

};

#endif // AUDIO_H
