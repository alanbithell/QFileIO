#include "filerwkit.h"

FileRWKit::FileRWKit(QObject *parent) :
    QObject(parent)
{
}

QVector< QVector<double> > FileRWKit::ReadCSV(QString filename)
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

SPTProject FileRWKit::ReadProject(QString Filename){
    SPTProject projectData;

    QFile *xmlFile = new QFile(Filename);
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "error";

    QXmlStreamReader xml(xmlFile);

    while(!xml.atEnd() && !xml.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument)
            continue;

        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {
            //qDebug() << xml.name();
            if(xml.name() == "ProjectTitle") {
                //qDebug() << xml.readElementText();
                projectData.ProjectTitle=xml.readElementText();
                continue;
            }
            if(xml.name() == "DeviceType") {
                //qDebug() << xml.readElementText();
                projectData.DeviceType=xml.readElementText();
                continue;
            }
            if(xml.name() == "ChannelType") {
                //qDebug() << xml.readElementText();
                projectData.ChannelType=xml.readElementText().toDouble();
                continue;
            }
            if(xml.name() == "SampleRates") {
                //qDebug() << xml.readElementText();
                QString sr = xml.readElementText();
                QStringList srl = sr.split(",");
                QVector<double>srld(srl.size());
                for(int i=0;i<srl.size();i++){
                    srld[i]= (srl.at(i)).toDouble();
                }

                projectData.SampleRates=srld;
                continue;
            }
            if(xml.name() == "OutputType") {
                //qDebug() << xml.readElementText();
                projectData.OutputType=xml.readElementText();
                continue;
            }
            if(xml.name() == "FilterLength") {
                //qDebug() << xml.readElementText();
                projectData.FilterLength=xml.readElementText().toInt();
                continue;
            }
            if(xml.name() == "FilterLevels") {
                //qDebug() << xml.readElementText();
                QString fl = xml.readElementText();
                QStringList fll = fl.split(",");
                QVector<double>flld(fll.size());
                for(int i=0;i<fll.size();i++){
                    flld[i]= (fll.at(i)).toDouble();
                }
                projectData.FilterLevels=flld;
                qDebug() <<flld;
                continue;
            }
            if(xml.name() == "FrequencyData") {
                //qDebug() << xml.readElementText();
                QString f = xml.readElementText();
                QStringList fl = f.split(",");
                QVector<double>fld(fl.size());
                for(int i=0;i<fl.size();i++){
                    fld[i]= (fl.at(i)).toDouble();
                }
                projectData.Frequencies=fld;
                qDebug() << projectData.Frequencies.size();
                continue;
            }
            if(xml.name() == "MagnitudeData") {
                //qDebug() << xml.readElementText();
                QString m = xml.readElementText();
                QStringList ml = m.split(",");
                QVector<double>mld(ml.size());
                for(int i=0;i<ml.size();i++){
                    mld[i]= (ml.at(i)).toDouble();
                }
                projectData.Magnitude=mld;
                qDebug() << projectData.Magnitude.size();
                continue;
            }
            if(xml.name() == "PhaseData") {
                //qDebug() << xml.readElementText();
                QString p = xml.readElementText();
                QStringList pl = p.split(",");
                QVector<double>pld(pl.size());
                for(int i=0;i<pl.size();i++){
                    pld[i]= (pl.at(i)).toDouble();
                }
                projectData.Phase=pld;
                qDebug() << projectData.Phase.size();
                continue;
            }
        }
    }
    /* Error handling. */
    if(xml.hasError())
        qDebug() << "QXSRExample::parseXML"<< xml.errorString();

    //resets its internal state to the initial state.
    xml.clear();



xmlFile->close();
return projectData;
}

void FileRWKit::WriteProject(SPTProject projectData){
    QString filename = QDir::homePath();
    filename.append("/Documents/SPTLiteTuner/Projects/");
    filename.append(projectData.ProjectTitle);
    filename.append("_");
    filename.append(QDateTime::currentDateTime().toString("hh-mm-ss ddd MMMM d yyyy"));
    filename.append(".sptpro");
    qDebug() << filename;
    QFile *xmlFile = new QFile(filename);
    if (!xmlFile->open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "error";

    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(xmlFile);
    xmlWriter->setAutoFormatting(true);
    xmlWriter->writeStartDocument();

    xmlWriter->writeStartElement("Root");

        xmlWriter->writeStartElement("ProjectSettings");

            xmlWriter->writeTextElement("ProjectTitle", projectData.ProjectTitle);
            xmlWriter->writeTextElement("DeviceType", projectData.DeviceType);
            xmlWriter->writeTextElement("ChannelType", QString::number(projectData.ChannelType));
            QString sr;
            for(int i=0;i<projectData.SampleRates.size();i++){
                sr.append(QString::number(projectData.SampleRates[i]));
                if(i<projectData.SampleRates.size()-1){
                    sr.append(",");
                }
            }
            xmlWriter->writeTextElement("SampleRates", sr);
            xmlWriter->writeTextElement("OutputType", projectData.OutputType);
            xmlWriter->writeTextElement("FilterLength", QString::number(projectData.FilterLength));
            QString FL;
            for(int i=0;i<projectData.FilterLevels.size();i++){
                FL.append(QString::number(projectData.FilterLevels[i]));
                if(i<projectData.FilterLevels.size()-1){
                    FL.append(",");
                }
            }
            xmlWriter->writeTextElement("FilterLevels", FL);

        xmlWriter->writeEndElement(); // ProjectSettings

        xmlWriter->writeStartElement("ProjectData");

            QString Freq;
            for(int i=0;i<projectData.Frequencies.size();i++){
                Freq.append(QString::number(projectData.Frequencies[i]));
                if(i<projectData.Frequencies.size()-1){
                    Freq.append(",");
                }
            }
            xmlWriter->writeTextElement("FrequencyData", Freq);
            QString Mag;
            for(int i=0;i<projectData.Magnitude.size();i++){
                Mag.append(QString::number(projectData.Magnitude[i]));
                if(i<projectData.Magnitude.size()-1){
                    Mag.append(",");
                }
            }
            xmlWriter->writeTextElement("MagnitudeData", Mag);
            QString Phase;
            for(int i=0;i<projectData.Phase.size();i++){
                Phase.append(QString::number(projectData.Phase[i]));
                if(i<projectData.Phase.size()-1){
                    Phase.append(",");
                }
            }
            xmlWriter->writeTextElement("PhaseData", Phase);

        xmlWriter->writeEndElement(); // ProjectData

    xmlWriter->writeEndElement(); //Root

    xmlWriter->writeEndDocument();

    xmlFile->close();
    qDebug() << "File Written";

}


void FileRWKit::WriteDatasetInfo(QString dir,QString notes){
    QString filename = dir;
    filename.append("/info.xml");

    QFile *xmlFile = new QFile(filename);
    if (!xmlFile->open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "error";

    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(xmlFile);
    xmlWriter->setAutoFormatting(true);
    xmlWriter->writeStartDocument();

    xmlWriter->writeStartElement("Root");

    xmlWriter->writeTextElement("Notes",notes);
    xmlWriter->writeTextElement("CreatedDate",QDateTime::currentDateTime().toString("hh:mm:ss ddd MMMM d yyyy"));

    xmlWriter->writeEndElement(); //Root

    xmlWriter->writeEndDocument();

    xmlFile->close();
    qDebug() << "File Written";

}

SPTProjectInfo FileRWKit::ReadDatasetInfo(QString dir){
    SPTProjectInfo output;
    SPTProject projectData;

    QFile *xmlFile = new QFile(dir);
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "error";

    QXmlStreamReader xml(xmlFile);

    while(!xml.atEnd() && !xml.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument)
            continue;

        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {
            //qDebug() << xml.name();
            if(xml.name() == "Notes") {
                //qDebug() << xml.readElementText();
                output.Notes=xml.readElementText();
                continue;
            }
            if(xml.name() == "CreatedDate") {
                //qDebug() << xml.readElementText();
                output.CreatedDate= QDateTime::fromString( xml.readElementText(),"hh:mm:ss ddd MMMM d yyyy");
                continue;
            }
        }
    }
    /* Error handling. */
    if(xml.hasError())
        qDebug() << "QXSRExample::parseXML"<< xml.errorString();

    //resets its internal state to the initial state.
    xml.clear();

    xmlFile->close();
    return output;
}

XMLstruct FileRWKit::CreateDataset(QString Filename){

    QString filename = Filename;
    filename.append("/data.xml");

    QFile *xmlDataFile;
    QXmlStreamWriter *xmlDataWriter;

    xmlDataFile = new QFile(filename);
    if (!xmlDataFile->open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "error";

    xmlDataWriter = new QXmlStreamWriter(xmlDataFile);
    xmlDataWriter->setAutoFormatting(true);
    xmlDataWriter->writeStartDocument();
    xmlDataWriter->writeStartElement("Root");

    qDebug() << "File Created";

    XMLstruct info;
    info.xmlDataFile=xmlDataFile;
    info.xmlDataWriter=xmlDataWriter;

    return info;
}

void FileRWKit::AppendDatatoDataset(XMLstruct writerinfo,QVector<double> data){

     QString Freq;
     for(int i=0;i<data.size();i++){
         Freq.append(QString::number(data[i]));
         if(i<data.size()-1){
             Freq.append(",");
         }
     }
     writerinfo.xmlDataWriter->writeTextElement("CorrectionData", Freq);
     qDebug() << "Data Written";
}

void FileRWKit::AppendDatatoDatasetFFT(XMLstruct writerinfo,QVector<double> Fdata,QVector<double> Mdata,QVector<double> Pdata){

     QString Freq;
     for(int i=0;i<Fdata.size();i++){
         Freq.append(QString::number(Fdata[i]));
         if(i<Fdata.size()-1){
             Freq.append(",");
         }
     }
     writerinfo.xmlDataWriter->writeTextElement("FrequencyData", Freq);

     QString Mag;
     for(int i=0;i<Mdata.size();i++){
         Mag.append(QString::number(Mdata[i]));
         if(i<Mdata.size()-1){
             Mag.append(",");
         }
     }
     writerinfo.xmlDataWriter->writeTextElement("MagnitudeData", Mag);

     QString Phase;
     for(int i=0;i<Pdata.size();i++){
         Phase.append(QString::number(Pdata[i]));
         if(i<Pdata.size()-1){
             Phase.append(",");
         }
     }
     writerinfo.xmlDataWriter->writeTextElement("PhaseData", Phase);
     qDebug() << "Data Written";
}

void FileRWKit::CloseDataset(XMLstruct writerinfo){

    writerinfo.xmlDataWriter->writeEndElement(); //Root

    writerinfo.xmlDataWriter->writeEndDocument();

    writerinfo.xmlDataFile->close();
    qDebug() << "File Closed";

}

SPTDataset FileRWKit::ReadDataset(QString Filename){
    SPTDataset output;
    output.TimeDomain = ReadDatasetTimeDomain(Filename);
    output.FrequencyDomain = ReadDatasetFFT(Filename);

    return output;
}

QVector< QVector<double> > FileRWKit::ReadDatasetTimeDomain(QString Filename){

    QVector< QVector<double> > output(10);
    int counter=0;
    qDebug() <<Filename;
    QFile *xmlFile = new QFile(Filename);
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "error";

    QXmlStreamReader xml(xmlFile);

    while(!xml.atEnd() && !xml.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument)
            continue;

        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {
            //qDebug() << xml.name();
            if(xml.name() == "CorrectionData") {
                //qDebug() << xml.readElementText();
                QStringList list =xml.readElementText().split(",");
                output[counter].resize(list.length());
                for(int i=0;i<list.length();i++){
                    output[counter][i]=list[i].toDouble();
                }
                counter++;
                continue;
            }
        }
    }
    /* Error handling. */
    if(xml.hasError())
        qDebug() << "QXSRExample::parseXML"<< xml.errorString();

    //resets its internal state to the initial state.
    xml.clear();

    xmlFile->close();

    output.resize(counter);

    return output;
}

QVector< QVector<double> > FileRWKit::ReadDatasetFFT(QString Filename){

    qDebug() << Filename;

    QVector< QVector<double> > output(10);
    QVector<int> indexes(10);

    for(int i=0;i<10;i++){
        indexes[i]=-1;
    }

    int counter=0;
    qDebug() <<Filename;
    QFile *xmlFile = new QFile(Filename);
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "error";

    QXmlStreamReader xml(xmlFile);

    while(!xml.atEnd() && !xml.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument)
            continue;

        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {
            //qDebug() << xml.name();
            if((xml.name() == "FrequencyData")) {
                //qDebug() << xml.readElementText();
                QStringList list =xml.readElementText().split(",");
                output[counter].resize(list.length());
                for(int i=0;i<list.length();i++){
                    output[counter][i]=list[i].toDouble();
                }
                indexes[counter]=0;
                counter++;
                continue;
            }
            if((xml.name() == "MagnitudeData")) {
                //qDebug() << xml.readElementText();
                QStringList list =xml.readElementText().split(",");
                output[counter].resize(list.length());
                for(int i=0;i<list.length();i++){
                    output[counter][i]=list[i].toDouble();
                }
                indexes[counter]=1;
                counter++;
                continue;
            }
            if((xml.name() == "PhaseData")) {
                //qDebug() << xml.readElementText();
                QStringList list =xml.readElementText().split(",");
                output[counter].resize(list.length());
                for(int i=0;i<list.length();i++){
                    output[counter][i]=list[i].toDouble();
                }
                indexes[counter]=2;
                counter++;
                continue;
            }
        }
    }
    /* Error handling. */
    if(xml.hasError())
        qDebug() << "QXSRExample::parseXML"<< xml.errorString();

    //resets its internal state to the initial state.
    xml.clear();

    xmlFile->close();

    output.resize(counter);
    indexes.resize(counter);

    QVector< QVector<double> > RealOutput(counter);
    for(int i=0;i<counter;i++){
        if(indexes[i]==0){
            RealOutput[0]=output[i];
        }
        if(indexes[i]==1){
            RealOutput[1]=output[i];
        }
        if(indexes[i]==2){
            RealOutput[2]=output[i];
        }
    }

    return RealOutput;
}

void FileRWKit::WriteUserPrivileges(QString Filename, UserPrivileges privileges){


    QFile *xmlFile = new QFile(Filename);
    if (!xmlFile->open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "error";

    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(xmlFile);
    xmlWriter->setAutoFormatting(true);
    xmlWriter->writeStartDocument();

    xmlWriter->writeStartElement("Root");

    xmlWriter->writeStartElement("EQ");
    xmlWriter->writeTextElement("CanUseEQ", QString::number((int) privileges.CanUseEQ));
    xmlWriter->writeTextElement("CanAdjustQ",QString::number((int) privileges.CanAdjustQ));
    xmlWriter->writeTextElement("CanAdjustX",QString::number((int) privileges.CanAdjustX));
    xmlWriter->writeTextElement("CanAdjustY",QString::number((int) privileges.CanAdjustY));
    xmlWriter->writeTextElement("PostClipSelectionAvailable",QString::number((int) privileges.PostClipSelectionAvailable));
    xmlWriter->writeEndElement(); //EQ

    xmlWriter->writeStartElement("Export");
    xmlWriter->writeTextElement("CanExportDataset",QString::number((int) privileges.CanExportDataset));
    xmlWriter->writeTextElement("CanConvertDataset",QString::number((int) privileges.CanConvertDataset));
    xmlWriter->writeEndElement(); //Export

    xmlWriter->writeStartElement("Projects");
    xmlWriter->writeTextElement("CanCreateProject",QString::number((int) privileges.CanCreateProject));
    xmlWriter->writeEndElement(); //Projects

    xmlWriter->writeEndElement(); //Root

    xmlWriter->writeEndDocument();

    xmlFile->close();
    qDebug() << "File Written";

}

UserPrivileges FileRWKit::ReadUserPrivileges(QString Filename){

    UserPrivileges UP;

    QFile *xmlFile = new QFile(Filename);
    if (!xmlFile->open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "error";

    QXmlStreamReader xml(xmlFile);

    /*
    struct EQ{
        bool CanUseEQ;
        bool CanAdjustQ;
        bool CanAdjustX;
        bool CanAdjustY;
        bool PostClipSelectionAvailable;
    };
    struct Export{
        bool CanExportDataset;
        bool CanConvertDataset;
    };
    struct Projects{
        bool CanCreateProject;
    };
     */

    while(!xml.atEnd() && !xml.hasError())
    {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument)
            continue;

        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {

            if(xml.name() == "CanUseEQ") {
                //qDebug() << xml.readElementText();
                UP.CanUseEQ =(bool)xml.readElementText().toInt();
                continue;
            }

            if(xml.name() == "CanAdjustQ") {
                //qDebug() << xml.readElementText();
                UP.CanAdjustQ =(bool)xml.readElementText().toInt();
                continue;
            }

            if(xml.name() == "CanAdjustX") {
                //qDebug() << xml.readElementText();
                UP.CanAdjustX =(bool)xml.readElementText().toInt();
                continue;
            }

            if(xml.name() == "CanAdjustY") {
                //qDebug() << xml.readElementText();
                UP.CanAdjustY =(bool)xml.readElementText().toInt();
                continue;
            }

            if(xml.name() == "PostClipSelectionAvailable") {
                //qDebug() << xml.readElementText();
                UP.PostClipSelectionAvailable =(bool)xml.readElementText().toInt();
                continue;
            }

            if(xml.name() == "CanExportDataset") {
                //qDebug() << xml.readElementText();
                UP.CanExportDataset =(bool)xml.readElementText().toInt();
                continue;
            }

            if(xml.name() == "CanConvertDataset") {
                //qDebug() << xml.readElementText();
                UP.CanConvertDataset =(bool)xml.readElementText().toInt();
                continue;
            }

            if(xml.name() == "CanCreateProject") {
                //qDebug() << xml.readElementText();
                UP.CanCreateProject =(bool)xml.readElementText().toInt();
                continue;
            }
        }
    }
    /* Error handling. */
    if(xml.hasError())
        qDebug() << "QXSRExample::parseXML"<< xml.errorString();

    //resets its internal state to the initial state.
    xml.clear();

    xmlFile->close();

    return UP;
}

void FileRWKit::WriteCirrusFFT(QString Filename, QVector<double> Mag, QVector<double> Phase){
    QFile *xmlFile = new QFile(Filename);
    if (!xmlFile->open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "error";

    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(xmlFile);
    xmlWriter->setAutoFormatting(true);
    xmlWriter->writeStartDocument();

    xmlWriter->writeStartElement("Root");

    double temp;

    QString MagString;
    for(int i=0;i<Mag.size();i++){
        temp=pow(10,Mag[i]/20.0);
        MagString.append(QString::number(temp));
        if(i<Mag.size()-1){
            MagString.append(",");
        }
    }
    xmlWriter->writeTextElement("Mag",MagString);

    QString PhaseString;
    for(int i=0;i<Phase.size();i++){

        temp = cos(Phase[i]);
        if(temp==1)
            temp=0.999999;
        PhaseString.append(QString::number(temp));
        PhaseString.append('\n');

        temp = sin(Phase[i]);
        if(temp==1)
            temp=0.999999;
        PhaseString.append(QString::number(temp));

        if(i<Phase.size()-1){
            PhaseString.append('\n');
        }
    }
    xmlWriter->writeTextElement("Phase",PhaseString);

    xmlWriter->writeEndElement(); //Root

    xmlWriter->writeEndDocument();

    xmlFile->close();
    qDebug() << "File Written";
}

void FileRWKit::WriteCSRFFT(QString Filename, QVector<double> Mag, QVector<double> Phase){
    QFile *xmlFile = new QFile(Filename);
    if (!xmlFile->open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "error";

    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(xmlFile);
    xmlWriter->setAutoFormatting(true);
    xmlWriter->writeStartDocument();

    xmlWriter->writeStartElement("Root");

    double temp;

    QString MagString;
    for(int i=0;i<Mag.size();i++){
        temp=pow(10,Mag[i]/20.0);
        if(temp>=1)
            temp=0.999999;
        MagString.append(QString::number(temp));
        if(i<Mag.size()-1){
            MagString.append(",");
            MagString.append('\n');
        }
    }
    xmlWriter->writeTextElement("Mag",MagString);

    QString RealString;
    for(int i=0;i<Phase.size();i++){
        temp = cos(Phase[i]);
        if(temp>=1)
            temp=0.999999;
        RealString.append(QString::number(temp));
        if(i<Phase.size()-1){
            RealString.append(",");
            RealString.append('\n');
        }
    }

    QString ImaginaryString;
    for(int i=0;i<Phase.size();i++){
        temp = sin(Phase[i]);
        if(temp>=1)
            temp=0.999999;
        ImaginaryString.append(QString::number(temp));

        if(i<Phase.size()-1){
            ImaginaryString.append(",");
            ImaginaryString.append('\n');
        }
    }

    xmlWriter->writeTextElement("Real",RealString);

    xmlWriter->writeTextElement("Imaginary",ImaginaryString);

    xmlWriter->writeEndElement(); //Root

    xmlWriter->writeEndDocument();

    xmlFile->close();
    qDebug() << "File Written";
}

