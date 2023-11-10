#ifndef IOPDSCONTROLLER_H
    #define IOPDSCONTROLLER_H

    #include <QObject>

class IOpdsController : public QObject
{
    Q_OBJECT
public:
    virtual ~IOpdsController() noexcept = default;

signals:

};

#endif // IOPDSCONTROLLER_H
