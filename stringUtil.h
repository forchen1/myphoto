#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <QObject>
#include <QDebug>


class StringUtil : public QObject
{
	Q_OBJECT

public:
    static int getUint16(QByteArray data, int offset);
    static int getUint16(QByteArray data, int offset, bool bigEnd);
    static int getUint32(QByteArray data, int offset, bool bigEnd);
    static int getUint8(QByteArray data, int offset);
    static QString getStringFromDB(QByteArray data, int start, int length);

    static QMap<QString, QString> readEXIFData(QByteArray data, int start);
    static QMap<QString, QString>  readTags(QByteArray data, int tiffStart, int dirStart, QMap<int, QString> tagInfos, bool bigEnd);
    static QString readTagValue(QByteArray data, int entryOffset, int tiffStart, int dirStart, bool bigEnd);
    static QMap<QString, QString> readJpgInfo(QString filepath);
};

#endif // STRINGUTIL_H

