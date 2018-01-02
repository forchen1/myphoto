#include <QFile>
#include "stringUtil.h"

int StringUtil::getUint16(QByteArray data, int offset){
    int tmpi1 = (int) data.at(offset);
    tmpi1 = tmpi1 < 0 ? tmpi1+256 : tmpi1;
    int tmpi2 = (int) data.at(offset+1);
    tmpi2 = tmpi2 < 0 ? tmpi2+256 : tmpi2;
    int tmpoffset = tmpi1 * 256 + tmpi2;
    return tmpoffset;
}

int StringUtil::getUint16(QByteArray data, int offset, bool bigEnd){
    int tmpi1 = (int) data.at(offset);
    tmpi1 = tmpi1 < 0 ? tmpi1+256 : tmpi1;
    int tmpi2 = (int) data.at(offset+1);
    tmpi2 = tmpi2 < 0 ? tmpi2+256 : tmpi2;
    int tmpoffset = 0;
    if(bigEnd){
        tmpoffset = tmpi1 + tmpi2 * 256 ;
    }else{
        tmpoffset = tmpi1 * 256 + tmpi2;
    }
    return tmpoffset;
}

int StringUtil::getUint32(QByteArray data, int offset, bool bigEnd){
    int tmpi1 = (int) data.at(offset);
    tmpi1 = tmpi1 < 0 ? tmpi1+256 : tmpi1;
    int tmpi2 = (int) data.at(offset+1);
    tmpi2 = tmpi2 < 0 ? tmpi2+256 : tmpi2;
    int tmpi3 = (int) data.at(offset+2);
    tmpi3 = tmpi3 < 0 ? tmpi3+256 : tmpi3;
    int tmpi4 = (int) data.at(offset+3);
    tmpi4 = tmpi4 < 0 ? tmpi4+256 : tmpi4;
    int tmpoffset = 0;
    if(bigEnd){
        tmpoffset = tmpi1 + tmpi2 * 256 + tmpi3 * 256 * 256 + tmpi4 * 256 * 256 * 256 ;
    }else{
        tmpoffset = tmpi1 * 256 * 256 * 256 + tmpi2 * 256 * 256 + tmpi3 * 256 + tmpi4;
    }
    return tmpoffset;
}

int StringUtil::getUint8(QByteArray data, int offset){
    char tmp = data.at(offset);
    int marker = (int) tmp;
    marker = marker <0 ? marker+256 : marker;
    return marker;
}

QString StringUtil::getStringFromDB(QByteArray data, int start, int length) {
    QString outstr = "";
    for (int n = start; n < start+length; n++) {
        outstr += data.at(n);
    }
    return outstr;
}


QMap<QString, QString> StringUtil::readJpgInfo(QString filename)
{
    QMap<QString, QString> tags;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
         qDebug()<< "error!";
         return tags;
    }

    QByteArray dataView = file.readAll();
    if ((dataView.at(0) != (char)0xFF) || (dataView.at(1) != (char)0xD8) ){
        qDebug() << "Not a valid JPEG";
        return tags;
    }

    int offset = 2;
    int length = file.size();
    int marker = 0;


    while (offset < length) {
        if (dataView.at(offset) != (char)0xFF) {
            qDebug() << "Not a valid marker at offset " << offset << ", found: " << (char) dataView.at(offset);
            return tags; // not a valid marker, something is wrong
        }

        marker = getUint8(dataView, offset + 1 );
        // we could implement handling for other markers here,
        // but we're only looking for 0xFFE1 for EXIF data
        if (marker == 225) {
            //qDebug() << "Found 0xFFE1 marker";
            tags = readEXIFData(dataView, offset + 4); //, getUint16(dataView, offset + 2) - 2);
            length=0;
        } else {
            offset += 2 + getUint16(dataView, offset+2);
        }
    }
    file.close();
    return tags;
}

QMap<QString, QString> StringUtil::readEXIFData(QByteArray data, int start){

    QMap<int, QString> ExifTags;
    QMap<int, QString> TiffTags;

    ExifTags[0x0100] = "ImageWidth";
    ExifTags[0x0101] = "ImageHeight";
    ExifTags[0x8769] = "ExifIFDPointer";
    ExifTags[0x8825] = "GPSInfoIFDPointer";
    ExifTags[0xA005] = "InteroperabilityIFDPointer";
    ExifTags[0x0102] = "BitsPerSample";
    ExifTags[0x0103] = "Compression";
    ExifTags[0x0106] = "PhotometricInterpretation";
    ExifTags[0x0112] = "Orientation";
    ExifTags[0x0115] = "SamplesPerPixel";
    ExifTags[0x011C] = "PlanarConfiguration";
    ExifTags[0x0212] = "YCbCrSubSampling";
    ExifTags[0x0213] = "YCbCrPositioning";
    ExifTags[0x011A] = "XResolution";
    ExifTags[0x011B] = "YResolution";
    ExifTags[0x0128] = "ResolutionUnit";
    ExifTags[0x0111] = "StripOffsets";
    ExifTags[0x0116] = "RowsPerStrip";
    ExifTags[0x0117] = "StripByteCounts";
    ExifTags[0x0201] = "JPEGInterchangeFormat";
    ExifTags[0x0202] = "JPEGInterchangeFormatLength";
    ExifTags[0x012D] = "TransferFunction";
    ExifTags[0x013E] = "WhitePoint";
    ExifTags[0x013F] = "PrimaryChromaticities";
    ExifTags[0x0211] = "YCbCrCoefficients";
    ExifTags[0x0214] = "ReferenceBlackWhite";
    ExifTags[0x0132] = "DateTime";
    ExifTags[0x010E] = "ImageDescription";
    ExifTags[0x010F] = "Make";
    ExifTags[0x0110] = "Model";
    ExifTags[0x0131] = "Software";
    ExifTags[0x013B] = "Artist";
    ExifTags[0x8298] = "Copyright";
    ExifTags[0x9000] = "ExifVersion";
    ExifTags[0xA000] = "FlashpixVersion";
    ExifTags[0xA001] = "ColorSpace";
    ExifTags[0xA002] = "PixelXDimension";
    ExifTags[0xA003] = "PixelYDimension";
    ExifTags[0x9101] = "ComponentsConfiguration";
    ExifTags[0x9102] = "CompressedBitsPerPixel";
    ExifTags[0x927C] = "MakerNote";
    ExifTags[0x9286] = "UserComment";
    ExifTags[0xA004] = "RelatedSoundFile";
    ExifTags[0x9003] = "DateTimeOriginal";
    ExifTags[0x9004] = "DateTimeDigitized";
    ExifTags[0x9290] = "SubsecTime";
    ExifTags[0x9291] = "SubsecTimeOriginal";
    ExifTags[0x9292] = "SubsecTimeDigitized";
    ExifTags[0x829A] = "ExposureTime";
    ExifTags[0x829D] = "FNumber";
    ExifTags[0x8822] = "ExposureProgram";
    ExifTags[0x8824] = "SpectralSensitivity";
    ExifTags[0x8827] = "ISOSpeedRatings";
    ExifTags[0x8828] = "OECF";
    ExifTags[0x9201] = "ShutterSpeedValue";
    ExifTags[0x9202] = "ApertureValue";
    ExifTags[0x9203] = "BrightnessValue";
    ExifTags[0x9204] = "ExposureBias";
    ExifTags[0x9205] = "MaxApertureValue";
    ExifTags[0x9206] = "SubjectDistance";
    ExifTags[0x9207] = "MeteringMode";
    ExifTags[0x9208] = "LightSource";
    ExifTags[0x9209] = "Flash";
    ExifTags[0x9214] = "SubjectArea";
    ExifTags[0x920A] = "FocalLength";
    ExifTags[0xA20B] = "FlashEnergy";
    ExifTags[0xA20C] = "SpatialFrequencyResponse";
    ExifTags[0xA20E] = "FocalPlaneXResolution";
    ExifTags[0xA20F] = "FocalPlaneYResolution";
    ExifTags[0xA210] = "FocalPlaneResolutionUnit";
    ExifTags[0xA214] = "SubjectLocation";
    ExifTags[0xA215] = "ExposureIndex";
    ExifTags[0xA217] = "SensingMethod";
    ExifTags[0xA300] = "FileSource";
    ExifTags[0xA301] = "SceneType";
    ExifTags[0xA302] = "CFAPattern";
    ExifTags[0xA401] = "CustomRendered";
    ExifTags[0xA402] = "ExposureMode";
    ExifTags[0xA403] = "WhiteBalance";
    ExifTags[0xA404] = "DigitalZoomRation";
    ExifTags[0xA405] = "FocalLengthIn35mmFilm";
    ExifTags[0xA406] = "SceneCaptureType";
    ExifTags[0xA407] = "GainControl";
    ExifTags[0xA408] = "Contrast";
    ExifTags[0xA409] = "Saturation";
    ExifTags[0xA40A] = "Sharpness";
    ExifTags[0xA40B] = "DeviceSettingDescription";
    ExifTags[0xA40C] = "SubjectDistanceRange";
    ExifTags[0xA005] = "InteroperabilityIFDPointer";
    ExifTags[0xA420] = "ImageUniqueID";
    TiffTags = ExifTags;


    QMap<QString, QString> tags;
    if (getStringFromDB(data, start, 4) != "Exif") {
       qDebug() << "Not valid EXIF data! " << getStringFromDB(data, start, 4);
       return tags;
    }

    bool bigEnd = false;
    int tiffOffset = start + 6;
    if (getUint16(data, tiffOffset) == 0x4949) {
        bigEnd = false;
    } else if (getUint16(data, tiffOffset) == 0x4D4D) {
        bigEnd = true;
    } else {
        qDebug() << "Not valid TIFF data! (no 0x4949 or 0x4D4D)";
        return tags;
    }

    if (getUint16(data, tiffOffset+2, !bigEnd) != 0x002A) {
        qDebug() << "Not valid TIFF data! (no 0x002A)";
        return tags;
    }

    int firstIFDOffset = getUint32(data, tiffOffset+4, !bigEnd);

    if (firstIFDOffset < 0x00000008) {
        qDebug() << "Not valid TIFF data! (First offset less than 8)" << getUint32(data, tiffOffset+4, !bigEnd);
        return tags;
    }

    tags = readTags(data, tiffOffset, tiffOffset + firstIFDOffset, ExifTags,  bigEnd);
    if(tags.size()==0){
        return tags;
    }

    QString test = tags["ExifIFDPointer"];
    int testi = test.toInt();
    //qDebug() << test;
    QMap<QString, QString> exifData;
    exifData = readTags(data, tiffOffset, tiffOffset + testi, TiffTags, bigEnd);

    QMap<QString,QString>::Iterator  it;
    for(it = exifData.begin(); it != exifData.end(); ++it)
    {
        //qDebug() <<"key:"<<it.key()<<"value:"<<it.value()<<endl;
        tags.insert(it.key(), it.value());
    }

    return tags;
}


QMap<QString, QString>  StringUtil::readTags(QByteArray data, int tiffStart, int dirStart, QMap<int, QString> tagInfos,  bool bigEnd) {
    int entries = getUint16(data, dirStart, !bigEnd);
    QMap<QString, QString> tags;
    int entryOffset = 0;
    QString tag="";
    int i;

    if(entries==0){
        return tags;
    }

    for (i=0;i<entries;i++) {
        //qDebug() << "i" << i;
        entryOffset = dirStart + i*12 + 2;
        tag = tagInfos[getUint16(data, entryOffset, !bigEnd)];
        //if (!tag && debug) console.log("Unknown tag: " + file.getUint16(entryOffset, !bigEnd));
        if(tag.length()==0){
            //qDebug() << "Unknown tag: " << getUint16(data, entryOffset, !bigEnd);
        }else{
            QString valuetest = readTagValue(data, entryOffset, tiffStart, dirStart, bigEnd);
            //qDebug() << "tag" << tag << "valuetest" << valuetest;
            tags[tag] = readTagValue(data, entryOffset, tiffStart, dirStart, bigEnd);
        }
    }
    return tags;
}

QString StringUtil::readTagValue(QByteArray data, int entryOffset, int tiffStart, int dirStart, bool bigEnd){
    int type = getUint16(data, entryOffset+2, !bigEnd);
    int numValues = getUint32(data, entryOffset+4, !bigEnd);
    int valueOffset = getUint32(data, entryOffset+8, !bigEnd) + tiffStart;
    int offset=0;
//        vals, val, n,
//        numerator, denominator;

    switch (type) {
        case 1: // byte, 8-bit unsigned int
        case 7: // undefined, 8-bit byte, value depending on field
            if (numValues == 1) {
                int result = getUint8(data, entryOffset + 8);
                return result;
            } else {
                offset = numValues > 4 ? valueOffset : (entryOffset + 8);
//                vals = [];
//                for (n=0;n<numValues;n++) {
//                    vals[n] = file.getUint8(offset + n);
//                }
//                return vals;
//                qDebug() << "case 7";
                return "";
            }
        case 2: // ascii, 8-bit byte
            offset = numValues > 4 ? valueOffset : (entryOffset + 8);
            return getStringFromDB(data, offset, numValues-1);
        case 3: // short, 16 bit int
            if (numValues == 1) {
                int result = getUint16(data, entryOffset + 8, !bigEnd);
                return QString::number(result, 10);
            } else {
                offset = numValues > 2 ? valueOffset : (entryOffset + 8);
//                vals = [];
//                for (n=0;n<numValues;n++) {
//                    vals[n] = file.getUint16(offset + 2*n, !bigEnd);
//                }
//                return vals;
//                qDebug() << "case 3";
                return "";
            }

        case 4: // long, 32 bit int
            if (numValues == 1) {
                int result = getUint32(data, entryOffset + 8, !bigEnd);
                return QString::number(result, 10);
            } else {
//                vals = [];
//                for (n=0;n<numValues;n++) {
//                    vals[n] = file.getUint32(valueOffset + 4*n, !bigEnd);
//                }
//                return vals;
//                qDebug() << "case 4";
                return "";
            }

        case 5:    // rational = two long values, first is numerator, second is denominator
//            if (numValues == 1) {
//                numerator = file.getUint32(valueOffset, !bigEnd);
//                denominator = file.getUint32(valueOffset+4, !bigEnd);
//                val = new Number(numerator / denominator);
//                val.numerator = numerator;
//                val.denominator = denominator;
//                return val;
//            } else {
//                vals = [];
//                for (n=0;n<numValues;n++) {
//                    numerator = file.getUint32(valueOffset + 8*n, !bigEnd);
//                    denominator = file.getUint32(valueOffset+4 + 8*n, !bigEnd);
//                    vals[n] = new Number(numerator / denominator);
//                    vals[n].numerator = numerator;
//                    vals[n].denominator = denominator;
//                }
//                return vals;
//            }
//            qDebug() << "case 5";
            return "";

        case 9: // slong, 32 bit signed int
            if (numValues == 1) {
                int result = getUint32(data, entryOffset + 8, !bigEnd);
                return QString::number(result, 10);
            } else {
//                vals = [];
//                for (n=0;n<numValues;n++) {
//                    vals[n] = file.getInt32(valueOffset + 4*n, !bigEnd);
//                }
//                return vals;
//                qDebug() << "case 9";
                return "";
            }

        case 10: // signed rational, two slongs, first is numerator, second is denominator
//            if (numValues == 1) {
//                return file.getInt32(valueOffset, !bigEnd) / file.getInt32(valueOffset+4, !bigEnd);
//            } else {
//                vals = [];
//                for (n=0;n<numValues;n++) {
//                    vals[n] = file.getInt32(valueOffset + 8*n, !bigEnd) / file.getInt32(valueOffset+4 + 8*n, !bigEnd);
//                }
//                return vals;
//            }
//            qDebug() << "case 10";
            return "";
    }
    return "";
}
