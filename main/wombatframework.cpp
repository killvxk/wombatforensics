#include "wombatframework.h"

WombatFramework::WombatFramework(WombatVariable* wombatvariable)
{
    wombatptr = wombatvariable;
}
WombatFramework::~WombatFramework()
{
}

void WombatFramework::OpenEvidenceImage() // open current evidence image
{
    const TSK_TCHAR** images;
    images = (const char**)malloc(wombatptr->evidenceobject.fullpathvector.size()*sizeof(char*));
    for(int i=0; i < wombatptr->evidenceobject.fullpathvector.size(); i++)
    {
        images[i] = wombatptr->evidenceobject.fullpathvector[i].c_str();
        fprintf(stderr, "fpv: %s\n", wombatptr->evidenceobject.fullpathvector[i].c_str());
    }
    wombatptr->evidenceobject.imageinfo = tsk_img_open(wombatptr->evidenceobject.itemcount, images, TSK_IMG_TYPE_DETECT, 0);
    free(images);
}

void WombatFramework::OpenVolumeSystem() // open current volume system
{
    wombatptr->evidenceobject.volinfo = tsk_vs_open(wombatptr->evidenceobject.imageinfo, 0, TSK_VS_TYPE_DETECT);
    if(wombatptr->evidenceobject.volinfo == NULL)
        fprintf(stderr, "no volume, create dummy one.");
}

void WombatFramework::GetVolumeSystemName() // get the volume system name
{
    if(wombatptr->evidenceobject.volinfo == NULL)
        wombatptr->volumeobject.name = "Dummy Volume";
    else
        wombatptr->volumeobject.name = QString::fromUtf8(tsk_vs_type_todesc(wombatptr->evidenceobject.volinfo->vstype));
    wombatptr->volumeobjectvector.append(wombatptr->volumeobject); // may not need.
}

void WombatFramework::OpenPartitions() // open the partitions in the volume
{
    // MAY NEED TO CHECK IF VOLUME EXISTS, IF NOT THEN SHOULD GET FS FROM IMG INSTEAD OF VOL.
    //fprintf(stderr, "Number of Partitions: %d\n", wombatptr->evidenceobject.volinfo->part_count);
    if(wombatptr->evidenceobject.volinfo == NULL)
    {
        wombatptr->evidenceobject.fsinfovector.push_back(tsk_fs_open_img(wombatptr->evidenceobject.imageinfo, 0, TSK_FS_TYPE_DETECT));
    }
    else
    {
        fprintf(stderr, "partitioncount: %i\n", wombatptr->evidenceobject.volinfo->part_count);
        if(wombatptr->evidenceobject.volinfo->part_count > 0)
        {
            for(uint32_t i=0; i < wombatptr->evidenceobject.volinfo->part_count; i++)
            {
                wombatptr->evidenceobject.partinfovector.push_back(tsk_vs_part_get(wombatptr->evidenceobject.volinfo, i));
                TSK_FS_INFO* tmpfsinfo = tsk_fs_open_vol(wombatptr->evidenceobject.partinfovector[i], TSK_FS_TYPE_DETECT);
                if(tmpfsinfo != NULL)
                {
                    fprintf(stderr, "Not Null FS info found\n");
                    wombatptr->evidenceobject.fsinfovector.push_back(tsk_fs_open_vol(wombatptr->evidenceobject.partinfovector[i], TSK_FS_TYPE_DETECT));
                }
                else
                {
                    fprintf(stderr, "NULL fs info found\n");
                }
            }
        }
    }
}
// DEFINITELY DON'T NEED THIS FUNCTION RIGHT NOW.
void WombatFramework::OpenFileSystems() // open the filesystems in the image or partition
{
    // may not need this function since i can open them when the partitions are added w/o having to reloop it.
}

void WombatFramework::OpenEvidenceImages() // open all evidence images.
{
    for(int j = 0; j < wombatptr->evidenceobjectvector.count(); j++)
    {
        const TSK_TCHAR** images;
        images = (const char**)malloc(wombatptr->evidenceobjectvector[j].fullpathvector.size()*sizeof(char*));
        for(int i=0; i < wombatptr->evidenceobjectvector[j].fullpathvector.size(); i++)
        {
            images[i] = wombatptr->evidenceobjectvector[j].fullpathvector[i].c_str();
        }
        wombatptr->evidenceobjectvector[j].imageinfo = tsk_img_open(wombatptr->evidenceobjectvector[j].itemcount, images, TSK_IMG_TYPE_DETECT, 0);
        free(images);
    }
}

void WombatFramework::GetBootCode() // deermine boot type and populate variable if exists otherwise populate wiht negative
{
    //Reader tmpreader;
    
    //vector<uchar> vchar(wombatptr->bootbuffer, wombatptr->bootbuffer + strlen(wombatptr->bootbuffer));
    /*
    for(int i=0; i < wombatptr->); i++)
    {
        vchar[i] = wombatptr->bootbuffer[i];
        qDebug() << vchar[i];
    }*/
    //qDebug() << vchar[510];
    //size_t bytesread = tmpreader.read(vchar, wombatptr->evidenceobject.imageinfo->sector_size);
    //qDebug() << "bytes read: " << bytesread;
    //qDebug() << "1. byte to hex: " << Translate::ByteToHex(vchar[510]);
    // LOOK AT WRITING TO BUFFER TO A FILE, STORING IT IN A READER AND THEN DOING MY TRANSLATIONS AND WHAT NOT FROM THERE...
    // get image boot sector and see what i've got.

    int retval;
    bool ok;
    //QString tmpstr = "";
    //vector<uchar> vchar;
    wombatptr->bootbuffer = NULL;
    qDebug() << "Sector Size: " << wombatptr->evidenceobject.imageinfo->sector_size;
    wombatptr->bootbuffer = new char[wombatptr->evidenceobject.imageinfo->sector_size];
    retval = tsk_img_read(wombatptr->evidenceobject.imageinfo, 0, wombatptr->bootbuffer, wombatptr->evidenceobject.imageinfo->sector_size);
    if(retval > 0)
    {
        qDebug() << "Boot Buffer Length: " << strlen(wombatptr->bootbuffer);
        vector<uchar> vchar(wombatptr->bootbuffer, wombatptr->bootbuffer + wombatptr->evidenceobject.imageinfo->sector_size);
        for(int i=0; i < retval; i++)
        {
            vchar[i] = wombatptr->bootbuffer[i];
            qDebug() << "vchar[" << i << "] " << vchar[i];
        }
        qDebug() << "vchar[510] " << vchar[510];
    }
       //qDebug() << "vchar[510]: " << vchar[510];
        // NEED TO LOOK AT READER/DATA STREAM THE BUFFER SOMEHOW...
        //unsigned char* ubuf = (unsigned char*)wombatptr->bootbuffer;
        //vector<uchar> vchar(ubuf[510], ubuf[510] + strlen((const char*)wombatptr->bootbuffer[510]));
        //qDebug() << "1. byte to hex: " << Translate::ByteToHex(wombatptr->bootbuffer[510]);
        //qDebug() << "2. byte to int: " << QString(Translate::ByteToHex(wombatptr->bootbuffer[510]));
        //Translate::ByteToBinary(tmpstr, vchar);
        //qDebug() << "3. byte to binary: " << tmpstr;
        //wombatptr->bootbytearray = QByteArray::fromRawData(wombatptr->bootbuffer, wombatptr->evidenceobject.imageinfo->sector_size);
        /*
        qDebug() << ByteArrayToHex(wombatptr->bootbytearray.mid(510,2)); // nets my signature value to compare
        if(QString::compare("55aa", ByteArrayToHex(wombatptr->bootbytearray.mid(510,2))) == 0) // its a boot sector
        {
            qDebug() << "byte to examine: " << ByteArrayToHex(wombatptr->bootbytearray.mid(510,1));
	    qDebug() << "1. byte to hex: " << SingleByteToString(wombatptr->bootbytearray.mid(510,1), 16);
	    qDebug() << "2. byte to Int: " << SingleByteToString(wombatptr->bootbytearray.mid(510,1), 10);
	    qDebug() << "3. byte to Binary: " << SingleByteToString(wombatptr->bootbytearray.mid(510,1), 2);
	    qDebug() << "4. bytearray number: " << QByteArray::number(10, 16);
	    qDebug() << "5. byte (0x10) to binary: " << SingleByteToString(QByteArray::number(10, 16), 2);
            // determine if its got a partition table
            // NEED TO CHECK THE 64 BYTE PARTITION TABLE FOR THE 1ST BYTE OF EACH 16 BYTE PARTITION ENTRY. IF ITS 80 THEN ITS BOOTABLE AND OLD, IF THE
            // SEVENTH BIT IS ACTIVE (CHECK BYTE FUNCTION) THEN ITS A BOOT PARAMETER... SO NEED TO CHECK EACH AND ALSO CHECK THE PARTITION TYPE AGAINST
            // A VALID ENTRY IN MY PARTITION TABLE MASK.
        //}
        //wombatptr->bootsectorlist << ByteArrayToHexDisplay(wombatptr->bootbytearray.mid(0,3));
        */
    //}
    /*
     *
        if(wombatvarptr->selectedobject.type == 1)
        {
            QWebElement tmpelement = ui->webView->page()->currentFrame()->documentElement().lastChild();
            tmpelement.appendInside("<div id='infotitle'>image information</div><br/>");
            tmpelement.appendInside("<table><tr><td class='property'>image type:</td><td class='pvalue'>" + QString(tsk_img_type_todesc(wombatvarptr->evidenceobject.imageinfo->itype)) + "</td></tr><tr><td class='property'>size:</td><td class='pvalue'>" + QLocale::system().toString(((int)wombatvarptr->evidenceobject.imageinfo->size)) + " bytes</td></tr><tr><td class='property'>sector size: </td><td class='pvalue'>" + QLocale::system().toString(wombatvarptr->evidenceobject.imageinfo->sector_size) + " bytes</td></tr><tr><td class='property'>sector count:</td><td class='pvalue'>" + QLocale::system().toString((int)((float)wombatvarptr->evidenceobject.imageinfo->size/(float)wombatvarptr->evidenceobject.imageinfo->sector_size)) + " sectors</td></tr><tr><td class='property'>Volume Type</td><td class='pvalue'>" + wombatvarptr->volumeobject.name + "</td></tr></table>");
            // if no boot code/partition table, then simply return info to say no boot code present.
            // OR POSSIBLY STATE WHAT THE IMAGE CONTAINS (PARTITION TABLE/SINGLE PARTITION ONLY)
            // if DOS partition table...
            // if boot code exists...
            // check for signature value at offset 510-511 to determine if its a dosmbr or something else. if its 0x55AA then its dos mbr
            // check for partition table and populate the values accordingly.
            // the fs stuff i find at jump and oem and the others are for the filesystem/partition boot sector. this isn't valid when there is an mbr.
            // need to determine if there is an mbr and then pull the partition table information from it. otherwise simply display the image info
            // and have no mbr present in first sector.
            // when you click on the partition, this is where the partition boot sector information will go.
            // MOVE ALL OF THIS CODE INTO WOMBATFRAMEWORK...
            GetDosBootCode(); // move into getbootcode above
            tmpelement.appendInside("<br/><br/><div class='tabletitle'>boot sector</div>");
            tmpelement.appendInside("<br/><table><tr><th>byte offset</th><th>value</th><th>description</th></tr><tr class='odd'><td>0-2</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[0] + "</td><td class='desc'>Jump instruction to the boot code</td></tr><tr class='even'><td>3-10</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[1] + "</td><td class='desc'>OEM name string field. This field is ignored by Microsoft operating systems</td></tr><tr class='odd'><td>11-12</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[2] + " bytes</td><td class='desc'>Bytes per sector</td></tr><tr class='even'><td>13-13</td><td class='bvalue'>" + wombatvarptr->bootsectorlist[3] + " sectors</td><td class='desc'>Seectors per cluster</td></tr><tr class='odd'><td colspan='3' class='bot'></td></tr></table>");
void WombatForensics::GetDosBootCode()
{
    int retval;
    wombatvarptr->bootbuffer = NULL;
    wombatvarptr->bootbuffer = new char[wombatvarptr->evidenceobject.imageinfo->sector_size];
    retval = tsk_img_read(wombatvarptr->evidenceobject.imageinfo, 0, wombatvarptr->bootbuffer, wombatvarptr->evidenceobject.imageinfo->sector_size);
    if(retval > 0)
    {
        wombatvarptr->bootbytearray = QByteArray::fromRawData(wombatvarptr->bootbuffer, wombatvarptr->evidenceobject.imageinfo->sector_size);
        fprintf(stderr, "oem from byte array: %s\n", QString::fromUtf8(wombatvarptr->bootbytearray.mid(3,8)).toStdString().c_str());
        wombatvarptr->bootsectorlist << ByteArrayToHexDisplay(wombatvarptr->bootbytearray.mid(0,3));
        wombatvarptr->bootsectorlist << QString::fromUtf8(wombatvarptr->bootbytearray.mid(3,8));
        wombatvarptr->bootsectorlist << ByteArrayToShortDisplay(wombatvarptr->bootbytearray.mid(11,2));
        wombatvarptr->bootsectorlist << ByteArrayToShortDisplay(wombatvarptr->bootbytearray.mid(13,1));
    }
    else
        fprintf(stderr, "filling bootbuffer failed\n");

 *
    uint64_t bytelen = 512*(seclength - 1);
    uint64_t bytestart = 512*secstart;
    char* volbuffer = NULL;
    volbuffer = new char[bytelen+512];

    {
        // need to figure out why this fails... and returns -1
        retval = TskServices::Instance().getImageFile().getSectorData(secstart, seclength-1, volbuffer);
        fprintf(stderr, "sector data return value: %i\n", retval);
        if (retval == -1)
        {
        }
    }
    if(retval > 0)
    {
        QFile tmpfile("/home/pasquale/WombatForensics/tmpfiles/volbyte.dat");
        tmpfile.open(QIODevice::WriteOnly);
        tmpfile.write(volbuffer, bytelen);
        tmpfile.close();
    }
    delete[] volbuffer;
    //delete volbuffer;

    return "/home/pasquale/WombatForensics/tmpfiles/volbyte.dat";

 *
 *
 *    // set hex (which i'll probably remove anyway since it's highlighted in same window)
    int sellength = txt.size()/2;
    QString tmptext = "Length: " + QString::number(sellength);
    selectedhex->setText(tmptext);
    // get initial bytes value and then update ascii
    std::vector<uchar> bytes;
    Translate::HexToByte(bytes, txt);
    QString ascii;
    Translate::ByteToChar(ascii, bytes);
    tmptext = "Ascii: " + ascii;
    selectedascii->setText(tmptext);
    QString strvalue;
    uchar * ucharPtr;
    // update the int entry:
    // pad right with 0x00
    int intvalue = 0;
    ucharPtr = (uchar*) &intvalue;
    memcpy(&intvalue,&bytes.begin()[0], min(sizeof(int),bytes.size()));
    strvalue.setNum(intvalue);
    tmptext = "Int: " + strvalue;
    selectedinteger->setText(tmptext);
    // update float entry;
    float fvalue;
    ucharPtr = (uchar*)(&fvalue);
    if(bytes.size() < sizeof(float) )
    {
        for(unsigned int i= 0; i < sizeof(float); ++i)
        {
            if( i < bytes.size() )
            {
                *ucharPtr++ = bytes[i];
            }
            else
            {
                *ucharPtr++ = 0x00;
            }
        }
    }
    else
    {
        memcpy(&fvalue,&bytes.begin()[0],sizeof(float));
    }
    strvalue.setNum( fvalue );
    tmptext = "Float: " + strvalue;
    selectedfloat->setText(tmptext);
    // update double
    double dvalue;
    ucharPtr = (uchar*)&dvalue;
    if(bytes.size() < sizeof(double) )
    {
        for(unsigned int i= 0; i < sizeof(double); ++i)
        {
            if( i < bytes.size() )
            {
                *ucharPtr++ = bytes[i];
            }
            else
            {
                *ucharPtr++ = 0x00;
            }
        }
    }
    else
    {
        memcpy(&dvalue,&bytes.begin()[0],sizeof(double));
    }
    strvalue.setNum( dvalue );
    tmptext = "Double: " + strvalue;
    selecteddouble->setText(tmptext);

 */ 
}

// BELOW FUNCTION CURRENTLY NOT USED
void WombatFramework::BuildEvidenceModel()
{
    // COME BACK TO QTCONCURRENT AS I GET FARTHER ALONG AND RESEARCH MORE
    /*
    QVector<ItemObject> itemvector;
    for(int i = 0; i < wombatptr->evidenceobjectvector.count(); i++)
    {
        itemvector.append(wombatptr->evidenceobjectvector[i]);
    }
    QFutureWatcher<void> watcher;
    watcher.setFuture(QtConcurrent::map(itemvector, &WombatFramework::OpenEvidenceImages));
    //watcher.setFuture(QtConcurrent::map(wombatptr->evidenceobjectvector, WombatFramework::OpenEvidenceImages));
    watcher.waitForFinished();
    */
    OpenEvidenceImages(); // PROBABLY PUT THIS IN A MULTI-THREAD ENVIRONMENT.
    for(int i=0; i < wombatptr->evidenceobjectvector.count(); i++) // for each evidence image info file.
    {
        if(wombatptr->evidenceobjectvector[i].imageinfo == NULL)
            fprintf(stderr, "Image didn't open. add to log file as error.\n");
        else
            fprintf(stderr, "Image %s opened. add to log file as info.\n", wombatptr->evidenceobjectvector[i].fullpathvector[0].c_str());
    }
    // NEED TO LAUNCH THIS IN A NEW THREAD TO KEEP GUI RESPONSIVE
    // NEED TO OPEN THE IMAGE - img_open.c [tsk_img_open()]
    // NEED TO GET THE METADATA FOR THE IMAGE/VOLUMES/PARTITIONS/FILES SO I CAN POPULATE THE DIRECTORY TREE INFORMATION


    // NEED TO ADD THE EVIDENCE ITEM TO THE DATABASE
    // POPULATE THE WOMBATVARPTR FOR THE EVIDENCEOBJECT VECTOR
    // NEED TO CREATE THE EVIDENCE TSK DATABASE (EXTRACT EVIDENCE ACCORDING TO MODULES)
    // NEED TO BUILD DIRMODEL AS I GO AND POPULATE DIRTREEVIEW AS I GO WITH EACH FILE
    // FOR NOW I WON'T BUILD MODULES, I'LL JUST DESIGN A MULTI-THREADED APPROACH FOR IT AND ABSTRACT TO PLUGGABLE MODULES LATER

    /*
    QFutureWatcher<void> watcher;
    std::vector<TskSchedulerQueue::task_struct* > tmpvector;
    watcher.setFuture(QtConcurrent::map(&tmpvector, &SleuthKitPlugin::TaskMap));
    //watcher.setFuture(QtConcurrent::map(&((std::vector<TskSchedulerQueue::task_struct*>)scheduler.mapvector), &SleuthKitPlugin::TaskMap));
    watcher.waitForFinished();
    */
    // QT CONCURRENT TEST
}
