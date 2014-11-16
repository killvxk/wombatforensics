#include "xbytearray.h"

XByteArray::XByteArray()
{
    //_oldSize = -99;
    addressnumbers = 4;
    addressoffset = 0;
    slicesize = 819200; // default loaded size is three slices (1600*512)
}

int XByteArray::AddressOffset()
{
    return addressoffset;
}

void XByteArray::SetAddressOffset(int offset)
{
    addressoffset = offset;
}

int XByteArray::AddressWidth()
{
    return addressnumbers;
}

void XByteArray::SetAddressWidth(int width)
{
    if ((width >= 0) and (width<=6))
    {
        addressnumbers = width;
    }
}

QByteArray & XByteArray::data()
{
    return _data;
}

void XByteArray::SetData(QByteArray d)
{
    _data = d;
    //_changedData = QByteArray(data.length(), char(0));
}
/*
bool XByteArray::dataChanged(int i)
{
    return bool(_changedData[i]);
}

QByteArray XByteArray::dataChanged(int i, int len)
{
    return _changedData.mid(i, len);
}

void XByteArray::setDataChanged(int i, bool state)
{
    _changedData[i] = char(state);
}

void XByteArray::setDataChanged(int i, const QByteArray & state)
{
    int length = state.length();
    int len;
    if ((i + length) > _changedData.length())
        len = _changedData.length() - i;
    else
        len = length;
    _changedData.replace(i, len, state);
}
*/
int XByteArray::RealAddressNumbers()
{
    // is addressNumbers wide enought?
    QString test = QString("%1").arg(_data.size() + addressoffset, addressnumbers, 16, QChar('0'));
    realaddressnumbers = test.size();
    return realaddressnumbers;
}

int XByteArray::size()
{
    return _data.size();
}
/*
QByteArray & XByteArray::insert(int i, char ch)
{
    _data.insert(i, ch);
    _changedData.insert(i, char(1));
    return _data;
}

QByteArray & XByteArray::insert(int i, const QByteArray & ba)
{
    _data.insert(i, ba);
    _changedData.insert(i, QByteArray(ba.length(), char(1)));
    return _data;
}

QByteArray & XByteArray::remove(int i, int len)
{
    _data.remove(i, len);
    _changedData.remove(i, len);
    return _data;
}

QByteArray & XByteArray::replace(int index, char ch)
{
    _data[index] = ch;
    _changedData[index] = char(1);
    return _data;
}

QByteArray & XByteArray::replace(int index, const QByteArray & ba)
{
    int len = ba.length();
    return replace(index, len, ba);
}

QByteArray & XByteArray::replace(int index, int length, const QByteArray & ba)
{
    int len;
    if ((index + length) > _data.length())
        len = _data.length() - index;
    else
        len = length;
    _data.replace(index, len, ba.mid(0, len));
    _changedData.replace(index, len, QByteArray(len, char(1)));
    return _data;
}
*/
QChar XByteArray::AsciiChar(int index)
{
    char ch = _data[index];
    if ((ch < 0x20) or (ch > 0x7e))
            ch = '.';
    return QChar(ch);
}
/*
QString XByteArray::ToReadableString(int start, int end)
{
    int adrWidth = RealAddressNumbers();
    if (addressnumbers > adrWidth)
        adrWidth = addressnumbers;
    if (end < 0)
        end = _data.size();

    QString result;
    for (int i=start; i < end; i += 16)
    {
        QString adrStr = QString("%1").arg(addressoffset + i, adrWidth, 16, QChar('0'));
        QString hexStr;
        QString ascStr;
        for (int j=0; j<16; j++)
        {
            if ((i + j) < _data.size())
            {
                hexStr.append(" ").append(_data.mid(i+j, 1).toHex());
                ascStr.append(AsciiChar(i+j));
            }
        }
        result += adrStr + " " + QString("%1").arg(hexStr, -48) + "  " + QString("%1").arg(ascStr, -17) + "\n";
    }
    return result;
}
*/

int XByteArray::LineCount(void)
{
    return linecount;
}

int XByteArray::BlockLineCount(void)
{
    return blocklinecount;
}

bool XByteArray::OpenImage(TskObject* tskpointer)
{
    // initialize the variables
    tskptr = tskpointer;
    blocksize = tskptr->blocksize;
    imagesize = tskptr->imglength;
    sliceindex = 3;
    firstoffset = 0;
    slicestart = slicesize;
    addressoffset = 0;
    // determine if slicesize is > imagesize and adjust accordingly
    if(slicesize*3 >= imagesize)
    {
        slicesize = imagesize;
        sliceend = slicesize;
        slicestart = 0;
    }
    else
    {
        sliceend = 2*slicesize;
    }
    lastoffset = imagesize - 1;
    currentoffset = 0;
    //blocklinecount = blocksize / bytesperline;
    blocklinecount = slicesize / bytesperline;
    linecount = imagesize / bytesperline;
    slicecount = imagesize / slicesize;
    if(imagesize % slicesize != 0)
        slicecount++;
    // load the 1st three slices here...
    int retval = 0;
    LoadSlice(firstoffset, 0);
    LoadSlice(slicesize, 1);
    LoadSlice(2*slicesize, 2);
}

bool XByteArray::LoadSlice(off_t soffset, off_t sindex)
{
    off_t retval = 0;
    char tmpbuf[slicesize];
    retval = tsk_img_read(tskptr->readimginfo, sindex*slicesize, tmpbuf, slicesize);
    qDebug() << "retval" << retval;
    if(retval > 0)
    {
        _data.insert(sindex*slicesize, QByteArray(tmpbuf, retval));
        //slicelist.append(QByteArray(tmpbuf, retval));
        //_data.append(QByteArray(tmpbuf, retval));
        //qDebug() << "_data size" << _data.size();
    }

    return true;
}

void XByteArray::FreeSlice(int prepost, off_t sliceindex)
{
    _data.remove((sliceindex*slicesize - (prepost*slicesize)), slicesize);
    /*
    if(currentoffset == slicestart)
    {
        // free the 
    }
    */
    // NEED TO FIGURE OUT HOW TO TELL WHERE THE CURRENT OFFSET IS AT
    // IF ITS == SLICE START THEN LOAD SLICEINDEX - 1
    // IF ITS == SLICE END THEN LOAD SLICEINDEX + 1
}
void XByteArray::AdjustData(int offset, int charheight)
{
    int curoff = (offset/charheight)*bytesperline;
    qDebug() << "sidx:" << sliceindex << "ssrt:" << slicestart << "send" << sliceend << "curoff:" << curoff;
    if(sliceindex <= slicecount && sliceindex >= (sliceindex-1)*slicesize)
    {
        qDebug() << "need to remove the end slice and load the new begin slice:" << sliceindex;
        LoadSlice(0, sliceindex);
        FreeSlice(1, (sliceindex - 3));
        slicestart = (sliceindex - 1)*slicesize;
        sliceend = sliceindex*slicesize;
        sliceindex++;
    }
    if(sliceindex > 3 && sliceindex <= (sliceindex-2)*slicesize)
    {
        qDebug() << "need to remove the begin slice and load the new end slice:" << sliceindex;
        sliceindex--;
        LoadSlice(0, sliceindex - 3);
        FreeSlice(-1, sliceindex);
        slicestart = (sliceindex - 2)*slicesize;
        sliceend = (sliceindex - 1)*slicesize;
    }
    /*
    int curoff = (offset/charheight)*bytesperline;
    if(curoff <= slicestart && sliceindex > 1)
    {
        addressoffset = addressoffset - slicesize;
        sliceindex--;
        slicestart = sliceindex*slicesize - 5120;
        LoadSlice(0, sliceindex - 1);
        FreeSlice(1, sliceindex + 2);
    }
    if(curoff >= sliceend && sliceindex < (imagesize/slicesize))
    {
        addressoffset = addressoffset + slicesize;
        sliceindex++;
        sliceend = sliceindex*slicesize - 5120;
        LoadSlice(0, sliceindex + 1);
        FreeSlice(-1, sliceindex - 2);
    }*/
    /*
    //int curoff = (sliceindex-1)*slicesize + (offset/charheight)*bytesperline;
    int curoff = (offset/charheight)*bytesperline;
    //qDebug() << "curoff:" << curoff;
    //qDebug() << "scroll bar value changed:" << (offset/charheight)*bytesperline; // 1st byteoffset for each line
    if(curoff <= slicestart && sliceindex > 1)
    {
        //qDebug() << "need to remove the end slice and load the new begin slice:" << sliceindex;
        addressoffset = addressoffset - slicesize;
        sliceindex--;
        //slicestart = slicestart + slicesize/2;
        //sliceend = sliceend + slicesize/2;
        //slicestart = sliceindex*slicesize - 10*blocksize;
        //sliceend = slicestart + slicesize/2;
        qDebug() << "sidx:" << sliceindex << "ssrt:" << slicestart << "send" << sliceend << "curoff:" << curoff;
        LoadSlice(0, (sliceindex - 1));
        FreeSlice(1, sliceindex + 2);
        // set sliceindex = sliceindex - 1;
        // set the new slicestart, sliceend
        // need to free the endslice and load the beginslice
    }
    if(curoff >= sliceend && sliceindex < (imagesize/slicesize + 1))
    {
        qDebug() << "need to remove the begin slice and load the new end slice:" << sliceindex;
        addressoffset = addressoffset + slicesize;
        qDebug() << (imagesize/slicesize);
        sliceindex++;
        //slicestart = sliceindex*slicesize - 10*blocksize;
        //sliceend = slicestart + slicesize - 10*blocksize;
        sliceend = sliceend + slicesize;
        qDebug() << "sidx:" << sliceindex << "ssrt:" << slicestart << "send" << sliceend << "curoff:" << curoff;
        LoadSlice(0, (sliceindex + 1));
        FreeSlice(-1, sliceindex - 2);
        // set sliceindex = sliceindex + 1
        // set the new slicestart, sliceend
        // need to free the beginslice and load the end slice
    }
    */
}
