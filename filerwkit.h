#ifndef FILERWKIT_H
#define FILERWKIT_H

#include <QObject>
#include <QVector>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QtMath>


class QFileIO : public QObject
{
    Q_OBJECT
public:
    explicit QFileIO(QObject *parent = 0);
    QVector< QVector<double> > ReadCSV(QString filename);
    
private:


signals:
    
public slots:
    
};

#endif // FILERWKIT_H
