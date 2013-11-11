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

struct SPTProject{
    QString ProjectTitle;
    QString DeviceType;
    double ChannelType;
    QVector<double> SampleRates;
    QString OutputType;
    int FilterLength;
    QVector<double> FilterLevels;
    QVector<double> Frequencies;
    QVector<double> Magnitude;
    QVector<double> Phase;
};

struct XMLstruct{
    QXmlStreamWriter *xmlDataWriter;
    QFile *xmlDataFile;
};

struct SPTProjectInfo{
    QString Notes;
    QDateTime CreatedDate;
};

struct SPTDataset{
    QVector< QVector<double> > TimeDomain;
    QVector< QVector<double> > FrequencyDomain;
};

struct UserPrivileges{

    //eq
        bool CanUseEQ;
        bool CanAdjustQ;
        bool CanAdjustX;
        bool CanAdjustY;
        bool PostClipSelectionAvailable;
        //export
        bool CanExportDataset;
        bool CanConvertDataset;
    //projects
        bool CanCreateProject;

};



class FileRWKit : public QObject
{
    Q_OBJECT
public:
    explicit FileRWKit(QObject *parent = 0);
    QVector< QVector<double> > ReadCSV(QString filename);
    void WriteProject(SPTProject projectData);
    SPTProject ReadProject(QString Filename);
    void WriteDatasetInfo(QString dir,QString notes);
    SPTProjectInfo ReadDatasetInfo(QString dir);
    XMLstruct CreateDataset(QString Filename);
    void AppendDatatoDataset(XMLstruct writerinfo,QVector<double> data);
    void AppendDatatoDatasetFFT(XMLstruct writerinfo,QVector<double> Fdata,QVector<double> Mdata,QVector<double> Pdata);
    void CloseDataset(XMLstruct writerinfo);
    SPTDataset ReadDataset(QString Filename);
    QVector< QVector<double> > ReadDatasetTimeDomain(QString Filename);
    QVector< QVector<double> > ReadDatasetFFT(QString Filename);
    void WriteUserPrivileges(QString Filename,UserPrivileges privileges);
    UserPrivileges ReadUserPrivileges(QString Filename);
    void WriteCirrusFFT(QString Filename,QVector<double> Mag,QVector<double> Phase);
    void WriteCSRFFT(QString Filename,QVector<double> Mag,QVector<double> Phase);


private:


signals:
    
public slots:
    
};

#endif // FILERWKIT_H
