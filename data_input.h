#ifndef DATA_INPUT_H
#define DATA_INPUT_H

#include <QVector>

class DataInput
{
public:
    enum DataType {
        NONE,
        AUDIO,
        IMAGE
    };

    DataInput() = default;
    ~DataInput() = default;

    virtual QVector<float> get_data() const = 0;
    virtual DataInput::DataType get_data_type() const = 0;
};

#endif // DATA_INPUT_H
