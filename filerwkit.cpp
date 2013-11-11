#include "filerwkit.h"

QFileIO::QFileIO(QObject *parent) :
    QObject(parent)
{
}

QVector< QVector<double> > QFileIO::ReadCSV(QString filename)
{
     QFile file(filename);
     QString lineend="";
     QVector< QVector<double> > vectors(1);
     int numlines=0;
     if (file.open(QIODevice::ReadOnly))
     {
        QTextStream in(&file);
        QString line = in.readAll(); //read one line at a time
        QStringList strings1 = line.split("\n");
        QStringList strings2 = line.split("\r");
        QStringList strings3 = line.split("\r\n");
        QStringList strings;

        if(strings3.length()>1){
           numlines=strings3.length();
           lineend="\r\n";
           strings=strings3;
        }else
        if(strings1.length()>1){
           numlines=strings1.length();
           lineend="\n";
           strings=strings1;
        }else
        if(strings2.length()>1){
           numlines=strings2.length();
           lineend="\r";
           strings=strings2;
        }
        vectors.resize(numlines);
        for(int i=0;i<numlines;i++){
            QStringList splitline = strings.at(i).split(",");
           QVector<double> a(splitline.length());
           for(int j=0;j<splitline.length();j++){
               a[j]=splitline[j].toDouble();
           }
           vectors[i]=a;
        }
     }
     file.close();

     return vectors;
}