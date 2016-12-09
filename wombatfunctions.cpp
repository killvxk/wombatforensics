#include "wombatfunctions.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

std::string GetTime()
{
    struct tm *newtime;
    time_t aclock;

    time(&aclock);   // Get time in seconds
    newtime = localtime(&aclock);   // Convert time to struct tm form 
    char timeStr[64];
    snprintf(timeStr, 64, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d",
        newtime->tm_mon+1,newtime->tm_mday,newtime->tm_year % 100, 
        newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

    return timeStr;
}

void LogMessage(QString logmsg)
{
    QString tmpstring = QDateTime::currentDateTime().toString(QString("MM/dd/yyyy hh:mm:ss t"));
    //msglog->append(QString(tmpstring + " " + logmsg));
    logfile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
    logfile.write(QString(tmpstring + "\t" + logmsg + "\n").toStdString().c_str());
    logfile.close();
}

char* TskTimeToStringUTC(time_t time, char buf[128])
{
    buf[0] = '\0';
    if (time <= 0) {
        strncpy(buf, "", 128);
    }
    else {
        //struct tm *tmTime = localtime(&time);
        struct tm *tmTime = gmtime(&time);

        snprintf(buf, 128, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", (int) tmTime->tm_year + 1900, (int) tmTime->tm_mon + 1, (int) tmTime->tm_mday, tmTime->tm_hour, (int) tmTime->tm_min, (int) tmTime->tm_sec);//, tzname[(tmTime->tm_isdst == 0) ? 0 : 1]);
    }
    return buf;
}

unsigned long long GetChildCount(int type, unsigned long long address, unsigned long long parimgid, unsigned long long parfsid)
{
    unsigned long long tmpcount = 0;
    /*
    QSqlQuery childquery(fcasedb);
    QString querystring = "SELECT COUNT(id) FROM data WHERE parid = ?";
    if(type < 4)
        querystring += " AND objtype < 5";
    else
    {
        querystring += " AND objtype >= 5 AND parfsid = ?";
    }
    if(type != 1)
        querystring += " AND parimgid = ?";
    childquery.prepare(querystring);
    childquery.addBindValue(address);
    if(type >= 4)
        childquery.addBindValue(parfsid);
    if(type != 1)
        childquery.addBindValue(parimgid);
    if(childquery.exec())
    {
        childquery.next();
        tmpcount = childquery.value(0).toULongLong();
    }
    childquery.finish();
    */
    return tmpcount;
}

bool FileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

bool ProcessingComplete()
{
    if(filesfound == filesprocessed && filesfound > 0)
        return true;
    
    return false;
}
/*
void ProcessFile(QVector<QString> tmpstrings, QVector<unsigned long long> tmpints, FileData adsdata, bool adsbool)
{
    FileData tmpdata;
    tmpdata.type = tmpints[0];
    tmpdata.name = tmpstrings[0];
    tmpdata.paraddr = tmpints[1];
    tmpdata.path = tmpstrings[1];
    tmpdata.atime = tmpints[2];
    tmpdata.ctime = tmpints[3];
    tmpdata.crtime = tmpints[4];
    tmpdata.mtime = tmpints[5];
    tmpdata.size = tmpints[6];
    tmpdata.addr = tmpints[7];
    tmpdata.evid = currentevidenceid;
    tmpdata.fsid = tmpints[8];
    tmpdata.mftattrid = 0;
    //mutex.lock();
    filedatavector.append(tmpdata);
    if(adsbool == true)
    {
        filedatavector.append(adsdata);
    }
    //mutex.unlock();
}
*/
TSK_WALK_RET_ENUM GetBlockAddress(TSK_FS_FILE* tmpfile, TSK_OFF_T off, TSK_DADDR_T addr, char* buf, size_t size, TSK_FS_BLOCK_FLAG_ENUM flags, void *ptr)
{
    if(off < 0)
    {
        // remove compile warning
    }
    if(buf)
    {
        // remove compile warning
    }
    if(ptr)
    {
        // remove compile warning
    }

    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_HFS_DETECT)
    {
        blockstring += QString::number(addr) + "|";
    }
    else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_ISO9660_DETECT)
    {
        // iso is already done in the previous function
    }
    else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_FAT_DETECT || tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        blockstring += QString::number(addr) + "|";
    }
    else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_YAFFS2_DETECT)
    {
        if(flags & TSK_FS_BLOCK_FLAG_CONT)
        {
            blockstring += QString::number(addr) + "|";
        }
    }
    else if(tmpfile->name != NULL)
    {
        if((strcmp(tmpfile->name->name, "$FAT1") == 0) || (strcmp(tmpfile->name->name, "$FAT2") == 0) || (strcmp(tmpfile->name->name, "$MBR") == 0) || (strcmp(tmpfile->name->name, "$OprhanFiles") == 0))
        {
            blockstring += QString::number(addr) + "|";
        }
    }
    else
    {
        if(flags & TSK_FS_BLOCK_FLAG_CONT)
        {
            int i, s;
            for(i = 0, s = (int) size; s > 0; s -= tmpfile->fs_info->block_size, i++)
            {
                if(addr)
                {
                    blockstring += QString::number(addr + i) + "|";
                }
            }
        }
    }
    return TSK_WALK_CONT;
}

QString GetFilePermissions(TSK_FS_META* tmpmeta)
{
    QString tmpstring = "----------";
    tmpstring.replace(0, 1, tsk_fs_meta_type_str[tmpmeta->type][0]);
    if(tmpmeta->mode & TSK_FS_META_MODE_IRUSR)
        tmpstring.replace(1, 1, "r");
    if(tmpmeta->mode & TSK_FS_META_MODE_IWUSR)
        tmpstring.replace(2, 1, "w");
    if(tmpmeta->mode & TSK_FS_META_MODE_ISUID)
    {
        if(tmpmeta->mode & TSK_FS_META_MODE_IXUSR)
            tmpstring.replace(3, 1, "s");
        else
            tmpstring.replace(3, 1, "S");
    }
    else if(tmpmeta->mode & TSK_FS_META_MODE_IXUSR)
        tmpstring.replace(3, 1, "x");
    if(tmpmeta->mode & TSK_FS_META_MODE_IRGRP)
        tmpstring.replace(4, 1, "r");
    if(tmpmeta->mode & TSK_FS_META_MODE_IWGRP)
        tmpstring.replace(5, 1, "w");
    if(tmpmeta->mode && TSK_FS_META_MODE_ISGID)
    {
        if(tmpmeta->mode & TSK_FS_META_MODE_IXGRP)
            tmpstring.replace(6, 1, "s");
        else
            tmpstring.replace(6, 1, "S");
    }
    else if(tmpmeta->mode & TSK_FS_META_MODE_IXGRP)
        tmpstring.replace(6, 1, "x");
    if(tmpmeta->mode & TSK_FS_META_MODE_IROTH)
        tmpstring.replace(7, 1, "r");
    if(tmpmeta->mode & TSK_FS_META_MODE_IWOTH)
        tmpstring.replace(8, 1, "w");
    if(tmpmeta->mode & TSK_FS_META_MODE_ISVTX) // sticky bit
    {
        if(tmpmeta->mode & TSK_FS_META_MODE_IXOTH)
            tmpstring.replace(9, 1, "t");
        else
            tmpstring.replace(9, 1, "T");
    }
    else if(tmpmeta->mode & TSK_FS_META_MODE_IXOTH)
        tmpstring.replace(9, 1, "x");
    return tmpstring;
}

void FileMap(FileData &filedata)
{
    QFile filefile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".f" + filedata.addr);
    filefile.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&filefile);
    out << filedata.name << "," << filedata.path << "," << filedata.type << "," << filedata.paraddr << "," << filedata.atime << "," << filedata.ctime << "," << filedata.crtime << "," << filedata.mtime << "," << filedata.size << "," << filedata.addr << "," << filedata.evid << "," << filedata.fsid << "," << filedata.mftattrid << "," << filedata.mimetype;
    filefile.close();
    isignals->ProgUpd();
    //emit isignals->FinishSql();

}

TSK_WALK_RET_ENUM FileEntries(TSK_FS_FILE* tmpfile, const char* tmppath, void* tmpptr)
{
    /*
     *
     *            // PUT FILES IN THE SPARSE FILE NOW.
            QFile evidfile(wombatvariable.tmpmntpath + wombatvariable.evidenceobject.name + ".evid");
            //QFile evidfile(wombatvariable.caseobject.dirpath + wombatvariable.evidenceobject.name + ".evid");
            evidfile.open(QIODevice::Append | QIODevice::Text);
            QTextStream out(&evidfile);
            out << (int)readimginfo->itype << "," << (unsigned long long)readimginfo->size << "," << (int)readimginfo->sector_size;
            for(unsigned int i=0; i < wombatvariable.evidenceobject.itemcount; i++)
                out << QString::fromStdString(wombatvariable.evidenceobject.fullpathvector[i]) << "," << i+1;
            evidfile.close();

     *
     *
     */ 
    FileData tmpdata;
    filesfound++;
    isignals->ProgUpd();
    if(tmpptr != NULL)
    {
        //LogMessage("TmpPtr got a value somehow");
    }

    if(tmpfile->name != NULL) tmpdata.name = QString(tmpfile->name->name);
    else tmpdata.name = QString("unknown.dat");
    tmpdata.path = QString("/") + QString(tmppath);

    if(tmpfile->name != NULL)
    {
        tmpdata.type = (unsigned long long)tmpfile->name->type;
        tmpdata.paraddr = (unsigned long long)tmpfile->name->par_addr;
    }
    else
    {
        tmpdata.type = 0;
        tmpdata.type = 0;
    }
    if(tmpfile->meta != NULL)
    {
        tmpdata.atime = (unsigned long long)tmpfile->meta->atime;
        tmpdata.ctime = (unsigned long long)tmpfile->meta->ctime;
        tmpdata.crtime = (unsigned long long)tmpfile->meta->crtime;
        tmpdata.mtime = (unsigned long long)tmpfile->meta->mtime;
        tmpdata.size = (unsigned long long)tmpfile->meta->size;
        tmpdata.addr = (unsigned long long)tmpfile->meta->addr;
    }
    else
    {
        tmpdata.atime = 0;
        tmpdata.ctime = 0;
        tmpdata.crtime = 0;
        tmpdata.mtime = 0;
        tmpdata.size = 0;
        tmpdata.addr = 0;
    }
    tmpdata.evid = currentevidenceid;
    tmpdata.fsid = currentfilesystemid;
    tmpdata.mftattrid = 0;
    char magicbuffer[1024];
    tsk_fs_file_read(tmpfile, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);
    QMimeDatabase mimedb;
    QMimeType mimetype = mimedb.mimeTypeForData(QByteArray((char*)magicbuffer));
    tmpdata.mimetype = mimetype.name();
    /*
     *    readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, secprocobj.address);
    char magicbuffer[1024];
    tsk_fs_file_read(readfileinfo, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);


    // Begin Mime Type Determination
    QMimeDatabase mimedb;
    QMimeType mimetype = mimedb.mimeTypeForData(QByteArray((char*)magicbuffer));
    secprocobj.mimetype = mimetype.name();

     *
     */ 
    filedatavector.append(tmpdata);

    FileData adsdata;
    unsigned long long adssize = 0;
    TSK_OFF_T curmftentrystart = 0;
    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        NTFS_INFO* ntfsinfo = (NTFS_INFO*)tmpfile->fs_info;
        int recordsize = 0;
        if(ntfsinfo->fs->mft_rsize_c > 0)
        {
            recordsize = ntfsinfo->fs->mft_rsize_c * ntfsinfo->fs->csize * tsk_getu16(tmpfile->fs_info->endian, ntfsinfo->fs->ssize);
        }
        else
            recordsize = 1 << -ntfsinfo->fs->mft_rsize_c;
        if(tmpfile->meta != NULL)
            curmftentrystart = tsk_getu16(tmpfile->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(tmpfile->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize * tmpfile->meta->addr + 20;
        else
            curmftentrystart = tsk_getu16(tmpfile->fs_info->endian, ntfsinfo->fs->ssize) * ntfsinfo->fs->csize * tsk_getu64(tmpfile->fs_info->endian, ntfsinfo->fs->mft_clust) + recordsize + 20;
        char startoffset[2];
        tsk_fs_read(tmpfile->fs_info, curmftentrystart, startoffset, 2);
        uint16_t teststart = startoffset[1] * 256 + startoffset[0];
        adssize = (unsigned long long)teststart;
    }
    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        if(strcmp(tmpfile->name->name, ".") != 0)
        {
            if(strcmp(tmpfile->name->name, "..") != 0)
            {
                int cnt, i;
                cnt = tsk_fs_file_attr_getsize(tmpfile);
                for(i = 0; i < cnt; i++)
                {
                    char type[512];
                    const TSK_FS_ATTR* fsattr = tsk_fs_file_attr_get_idx(tmpfile, i);
                    adssize += 24;
                    adssize += (unsigned long long)fsattr->size;
                    if(ntfs_attrname_lookup(tmpfile->fs_info, fsattr->type, type, 512) == 0)
                    {
                        if(QString::compare(QString(type), "$DATA", Qt::CaseSensitive) == 0)
                        {
                            if(QString::compare(QString(fsattr->name), "") != 0 && QString::compare(QString(fsattr->name), "$I30", Qt::CaseSensitive) != 0)
                            {
                                adsdata.type = (unsigned long long)tmpfile->name->type;
                                adsdata.paraddr = (unsigned long long)tmpfile->meta->addr;
                                adsdata.name = QString(":") + QString(fsattr->name);
                                adsdata.path = QString("/") + QString(tmppath);
                                adsdata.atime = 0;
                                adsdata.ctime = 0;
                                adsdata.crtime = 0;
                                adsdata.mtime = 0;
                                adsdata.evid = currentevidenceid;
                                adsdata.fsid = currentfilesystemid;
                                adsdata.size = (unsigned long long)fsattr->size;
                                adsdata.addr = adssize - (unsigned long long)fsattr->size + 16;
                                adsdata.mimetype = mimetype.name();
                                adsdata.mftattrid = (unsigned long long)fsattr->id; // STORE attr id in this variable in the db.
                                filedatavector.append(adsdata);
                            }
                        }
                    }
                }
            }
        }
    }
    return TSK_WALK_CONT;
}

void GenerateThumbnails()
{
    /*
    QSqlQuery filequery(fcasedb);
    filequery.prepare("SELECT objectid, parimgid, parfsid, address FROM data WHERE objecttype = 5 AND filemime LIKE '%image/%';");
    if(filequery.exec())
    {
        while(filequery.next())
        {
            const TSK_TCHAR** imagepartspath;
            unsigned long long objectid = 0;
            objectid = filequery.value(0).toULongLong();
            /
            QSqlQuery thumbquery(thumbdb);
            thumbquery.prepare("SELECT COUNT(thumbid) FROM thumbs WHERE objectid = ?;");
            thumbquery.addBindValue(objectid);
            thumbquery.exec();
            thumbquery.next();
            if(thumbquery.value(0).toInt() == 0)
            {
                TSK_IMG_INFO* readimginfo;
                TSK_FS_INFO* readfsinfo;
                TSK_FS_FILE* readfileinfo;
                // Open Parent Image
                std::vector<std::string> pathvector;
                pathvector.clear();
                QSqlQuery imgquery(fcasedb);
                imgquery.prepare("SELECT fullpath FROM dataruns WHERE objectid = ? ORDER BY seqnum;");
                imgquery.bindValue(0, filequery.value(1).toULongLong());
                if(imgquery.exec())
                {
                    while(imgquery.next())
                    {
                        pathvector.push_back(imgquery.value(0).toString().toStdString());
                    }
                }
                imgquery.finish();
                imagepartspath = (const char**)malloc(pathvector.size()*sizeof(char*));
                for(uint i=0; i < pathvector.size(); i++)
                {
                    imagepartspath[i] = pathvector.at(i).c_str();
                }
                readimginfo = tsk_img_open(pathvector.size(), imagepartspath, TSK_IMG_TYPE_DETECT, 0);
                free(imagepartspath);
                //OpenParentFileSystem
                QSqlQuery fsquery(fcasedb);
                fsquery.prepare("SELECT byteoffset FROM data where objectid = ?;");
                fsquery.bindValue(0, filequery.value(2).toULongLong());
                fsquery.exec();
                fsquery.next();
                readfsinfo = tsk_fs_open_img(readimginfo, fsquery.value(0).toULongLong(), TSK_FS_TYPE_DETECT);
                fsquery.finish();
                //OpenFile
                readfileinfo = tsk_fs_file_open_meta(readfsinfo, NULL, filequery.value(3).toULongLong());
                ThumbFile(readfileinfo, objectid);    
                tsk_fs_file_close(readfileinfo);
                tsk_fs_close(readfsinfo);
                tsk_img_close(readimginfo);
            }
            thumbquery.finish();
        }
    }
    filequery.finish();
    */
}

void PropertyFile(TSK_FS_FILE* tmpfile, unsigned long long objid, unsigned long long fsoffset, int blksize, unsigned long long parfsid)
{
    QStringList proplist;
    proplist.clear();
    if(tmpfile->name != NULL) proplist << "Short Name" << tmpfile->name->shrt_name << "Short Name for a file";
    if(tmpfile->meta != NULL)
    {
        proplist << "File Permissions" << GetFilePermissions(tmpfile->meta) << "Unix Style Permissions. r - file, d - directory, l - symbolic link, c - character device, b - block device, p - named pipe, v - virtual file created by the forensic tool; r - read, w - write, x - execute, s - set id and executable, S - set id, t - sticky bit executable, T - sticky bit. format is type|user|group|other - [rdlcbpv]|rw[sSx]|rw[sSx]|rw[tTx]";
        proplist << "User ID" << QString::number(tmpfile->meta->uid) << "User ID";
        proplist << "Group ID" << QString::number(tmpfile->meta->gid) << "Group ID";
        proplist << "Allocation Status";
        if(tmpfile->meta->flags == TSK_FS_META_FLAG_ALLOC)
            proplist << "Currently Allocated";
        else if(tmpfile->meta->flags == TSK_FS_META_FLAG_UNALLOC)
            proplist << "Currently Unallocated";
        else if(tmpfile->meta->flags == TSK_FS_META_FLAG_USED)
            proplist << "Allocated at least once";
        else if(tmpfile->meta->flags == TSK_FS_META_FLAG_UNUSED)
            proplist << "Never allocated";
        else if(tmpfile->meta->flags == TSK_FS_META_FLAG_COMP)
            proplist << "Contents are compressed";
        else
            proplist << "Unspecified";
        proplist << "allocation status for the file.";

        /*
        QSqlQuery objquery(fcasedb);
        objquery.prepare("SELECT blockaddress, filemime, filesignature, address FROM data WHERE objectid = ?;");
        objquery.bindValue(0, objid);
        objquery.exec();
        objquery.next();
        proplist << "Block Address" << objquery.value(0).toString() << "List of block addresses which contain the contents of the file";
        unsigned long long tmpoffset = 0;
        unsigned long long resoffset = 0;
        unsigned long long fileaddress = 0;
        fileaddress = objquery.value(3).toULongLong();
        proplist << "Byte Offset";
        if(objquery.value(0).toString().compare("") != 0)
            tmpoffset = objquery.value(0).toString().split("|", QString::SkipEmptyParts).at(0).toULongLong()*blksize + fsoffset;
        else
        {
            if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
            {
                QSqlQuery resquery(fcasedb);
                QStringList inputs;
                QList<unsigned long long> outputs;
                inputs << "%0x0B%" << "%0x0D%" << "%0x30%" << "%0x40%";
                for(int i=0; i < inputs.count(); i++)
                {
                    resquery.prepare("SELECT value from properties where objectid = ? and description like(?);");
                    resquery.addBindValue(parfsid);
                    resquery.addBindValue(inputs.at(i));
                    resquery.exec();
                    resquery.next();
                    outputs.append(resquery.value(0).toULongLong());
                }
                resquery.finish();
                mftrecordsize = outputs.at(3);
                resoffset = ((outputs.at(0) * outputs.at(1) * outputs.at(2)) + (outputs.at(3)*fileaddress));
                tmpoffset = resoffset + fsoffset;
            }
            else
                tmpoffset = fsoffset;
        }
        proplist << QString::number(tmpoffset) << "Byte offset for the start of the file in a block or in the MFT";
        proplist << "File Signature" << objquery.value(1).toString() << objquery.value(2).toString();
        objquery.finish();

        fcasedb.transaction();
        QSqlQuery propquery(fcasedb);
        propquery.prepare("INSERT INTO properties (objectid, name, value, description) VALUES(?, ?, ?, ?);");
        for(int i=0; i < proplist.count()/3; i++)
        {
            propquery.bindValue(0, objid);
            propquery.bindValue(1, proplist.at(3*i));
            propquery.bindValue(2, proplist.at(3*i+1));
            propquery.bindValue(3, proplist.at(3*i+2));
            propquery.exec();
        }
        fcasedb.commit();
        propquery.finish();
        */
        processphase++;
        isignals->ProgUpd();
    }
}

void AlternateDataStreamPropertyFile(TSK_FS_FILE* tmpfile, QVector<unsigned long long> adsobjid, QVector<unsigned long long> adsattrid)
{
    QStringList proplist;
    proplist.clear();
    if(tmpfile->name != NULL)
    {
        QString adsinfo = "Alternate Data Stream for " + QString::fromStdString(std::string(tmpfile->name->name));
        proplist << "Alternate Data Stream (ADS)" << adsinfo << "Alternate data stream which contains different content from what the file's standard content is.";
    }
    for(int i = 0; i < adsobjid.count(); i++)
    {
        /*
        QSqlQuery adsquery(fcasedb);
        adsquery.prepare("SELECT name, parentid, blockaddress FROM data WHERE objectid = ?;");
        adsquery.bindValue(0, adsobjid.at(i));
        adsquery.exec();
        adsquery.next();
        proplist << "Name" << adsquery.value(0).toString() << "Name for the NTFS parent file additional $Data attribute";
        proplist << "Parent Address" << adsquery.value(1).toString() << "NTFS address ID for the parent file";
        if(tmpfile->name != NULL)
            proplist << "Parent File Name" << QString(tmpfile->name->name) << "File name of the parent file";
        proplist << "Block Address" << adsquery.value(2).toString() << "List of block addresses which contain the contents of the ADS";
        proplist << "Attribute ID" << QString::number(adsattrid.at(i)) << "ID for the file's ADS attribute";
        adsquery.finish();
        fcasedb.transaction();
        QSqlQuery propquery(fcasedb);
        propquery.prepare("INSERT INTO properties (objectid, name, value, description) VALUES(?, ?, ?, ?);");
        for(int j = 0; j < proplist.count()/3; j++)
        {
            propquery.bindValue(0, adsobjid.at(i));
            propquery.bindValue(1, proplist.at(3*j));
            propquery.bindValue(2, proplist.at(3*j+1));
            propquery.bindValue(3, proplist.at(3*j+2));
            propquery.exec();
        }
        fcasedb.commit();
        propquery.finish();
        */
   }
}
/*
void BlockFile(TSK_FS_FILE* tmpfile, unsigned long long objid, QVector<unsigned long long> adsattrid)
{
    blockstring = "";
    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_HFS_DETECT || tmpfile->fs_info->ftype == TSK_FS_TYPE_ISO9660_DETECT || tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT || tmpfile->fs_info->ftype == TSK_FS_TYPE_FAT_DETECT)
    {
        if(tmpfile->fs_info->ftype == TSK_FS_TYPE_HFS_DETECT)
        {
            tsk_fs_file_walk_type(tmpfile, TSK_FS_ATTR_TYPE_HFS_DATA, HFS_FS_ATTR_ID_DATA, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
        }
        else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_ISO9660_DETECT)
        {
            iso9660_inode* dinode;
            dinode = (iso9660_inode*)tsk_malloc(sizeof(iso9660_inode));
            iso9660_inode_node* n;
            n = ((ISO_INFO*)tmpfile->fs_info)->in_list;
            while(n && (n->inum != tmpfile->meta->addr))
                n = n->next;
            if(n)
                memcpy(dinode, &n->inode, sizeof(iso9660_inode));
            int block = tsk_getu32(tmpfile->fs_info->endian, dinode->dr.ext_loc_m);
            TSK_OFF_T size = tmpfile->meta->size;
            while((int64_t)size > 0)
            {
                blockstring += QString::number(block++) + "|";
                size -= tmpfile->fs_info->block_size;
            }
        }
        else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
        {
            unsigned long long minads = 1000;
            for(int i = 0; i < adsattrid.count(); i++)
            {
                if(adsattrid.at(i) < minads)
                    minads = adsattrid.at(i);
            }
            if(tmpfile->meta != NULL)
            {
                if(tmpfile->meta->attr)
                {
                    int cnt, i;
                    cnt = tsk_fs_file_attr_getsize(tmpfile);
                    for(i = 0; i < cnt; i++)
                    {
                        const TSK_FS_ATTR* tmpattr = tsk_fs_file_attr_get_idx(tmpfile, i);
                        if(tmpattr->flags & TSK_FS_ATTR_NONRES) // non resident attribute
                        {
                            if(tmpattr->type == TSK_FS_ATTR_TYPE_NTFS_DATA && tmpattr->id < (int)minads)
                            {
                                tsk_fs_file_walk_type(tmpfile, tmpattr->type, tmpattr->id, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
                            }
                        }
                    }
                }
            }
        }
        else if(tmpfile->fs_info->ftype == TSK_FS_TYPE_FAT_DETECT)
        {
            tsk_fs_file_walk(tmpfile, (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
        }
    }
    else
        tsk_fs_file_walk(tmpfile, TSK_FS_FILE_WALK_FLAG_AONLY, GetBlockAddress, NULL);
    QSqlQuery blockquery(fcasedb);
    blockquery.prepare("UPDATE data SET blockaddress = ? WHERE objectid = ?;");
    blockquery.bindValue(0, blockstring);
    blockquery.bindValue(1, objid);
    blockquery.exec();
    blockquery.next();
    blockquery.finish();
    processphase++;
    isignals->ProgUpd();
}

void AlternateDataStreamBlockFile(TSK_FS_FILE* tmpfile, QVector<unsigned long long> adsobjid, QVector<unsigned long long> adsattrid)
{
    blockstring = "";
    if(tmpfile->fs_info->ftype == TSK_FS_TYPE_NTFS_DETECT)
    {
        for(int j=0; j < adsattrid.count(); j++)
        {
            if(tmpfile->meta != NULL)
            {
                tsk_fs_file_walk_type(tmpfile, TSK_FS_ATTR_TYPE_NTFS_DATA, adsattrid.at(j), (TSK_FS_FILE_WALK_FLAG_ENUM)(TSK_FS_FILE_WALK_FLAG_AONLY | TSK_FS_FILE_WALK_FLAG_SLACK), GetBlockAddress, NULL);
            }
            QSqlQuery blockquery(fcasedb);
            blockquery.prepare("UPDATE data SET blockaddress = ? WHERE objectid = ?;");
            blockquery.bindValue(0, blockstring);
            blockquery.bindValue(1, adsobjid.at(j));
            blockquery.exec();
            blockquery.next();
            blockquery.finish();
        }
    }
}
*/
/*
QVariant MagicFile(TSK_FS_FILE* tmpfile, unsigned long long objid)
{
    QVariant tmpvariant;
    // FILE MIME TYPE
    char magicbuffer[1024];
    //const char* mimesig;
    //const char* sigtype;
    //char* sigp1;
    //char* sigp2;
    ssize_t readlen = tsk_fs_file_read(tmpfile, 0, magicbuffer, 1024, TSK_FS_FILE_READ_FLAG_NONE);
    if(readlen > 0)
    {
        mimesig = magic_buffer(magicmimeptr, magicbuffer, readlen);
        sigp1 = strtok((char*)mimesig, ";");
        sigtype = magic_buffer(magicptr, magicbuffer, readlen);
        sigp2 = strtok((char*)sigtype, ",");
    }
    QSqlQuery mimequery(fcasedb);
    mimequery.prepare("UPDATE data SET filemime = ?, filesignature = ? WHERE objectid = ?;");
    if(readlen > 0)
    {
        tmpvariant = QVariant(QString::fromStdString(sigp2));
        mimequery.bindValue(0, QString::fromStdString(sigp1));
        mimequery.bindValue(1, QString::fromStdString(sigp2));
    }
    else
    {
        tmpvariant = QVariant(QString::fromStdString("Zero File"));
        mimequery.bindValue(0, QString("Zero File"));
        mimequery.bindValue(1, QString("Zero File"));
    }
    mimequery.bindValue(2, objid);
    mimequery.exec();
    mimequery.next();
    mimequery.finish();
    processphase++;
    isignals->ProgUpd();
    return tmpvariant;
}
*/
/*
QVariant AlternateDataStreamMagicFile(TSK_FS_FILE* tmpfile, unsigned long long adsobjid)
{
    QVariant tmpvariant;
    off_t retval = 0;
    char magicbuffer[1024];
    int chunksize = 1024;
    //const char* mimesig;
    //const char* sigtype;
    //char* sigp1;
    //char* sigp2;
    QSqlQuery adsquery(fcasedb);
    adsquery.prepare("SELECT name, fullpath, parimgid, parfsid, parentid, address, mftattrid, size, blockaddress FROM data WHERE objectid = ?;");
    adsquery.bindValue(0, adsobjid);
    adsquery.exec();
    adsquery.next();
    if(adsquery.value(7).toULongLong() < (unsigned)chunksize)
        chunksize = adsquery.value(7).toULongLong();
    if(adsquery.value(8).toString().compare("") != 0)
    {
        if(adsquery.value(8).toString().split("|", QString::SkipEmptyParts).at(0).toULongLong() == 0) // block address empty
        {
            retval = tsk_fs_file_read_type(tmpfile, TSK_FS_ATTR_TYPE_NTFS_DATA, adsquery.value(6).toInt(), 0, magicbuffer, chunksize, TSK_FS_FILE_READ_FLAG_NONE);
        }
        else // block address contains valid data
        {
            retval = tsk_fs_read_block(tmpfile->fs_info, adsquery.value(8).toString().split("|", QString::SkipEmptyParts).at(0).toULongLong(), magicbuffer, chunksize);
        }
    }
    else // blockaddress = ""
    {
        if(adsquery.value(7).toULongLong() != 0)
        {
            retval = tsk_fs_file_read_type(tmpfile, TSK_FS_ATTR_TYPE_NTFS_DATA, adsquery.value(6).toInt(), 0, magicbuffer, chunksize, TSK_FS_FILE_READ_FLAG_NONE);
        }
    }
    if(retval > 0)
    {
        mimesig = magic_buffer(magicmimeptr, magicbuffer, chunksize);
        sigp1 = strtok((char*)mimesig, ";");
        sigtype = magic_buffer(magicptr, magicbuffer, chunksize);
        sigp2 = strtok((char*)sigtype, ";");
    }
    QSqlQuery mimequery(fcasedb);
    mimequery.prepare("UPDATE data SET filemime = ?, filesignature = ? WHERE objectid = ?;");
    if(retval > 0)
    {
        tmpvariant = QVariant(QString::fromStdString(sigp2));
        mimequery.bindValue(0, QString::fromStdString(sigp1));
        mimequery.bindValue(1, QString::fromStdString(sigp2));
    }
    else
    {
        tmpvariant = QVariant(QString("Zero File"));
        mimequery.bindValue(0, QString("Zero File"));
        mimequery.bindValue(1, QString("Zero File"));
    }
    mimequery.bindValue(2,  adsobjid);
    mimequery.exec();
    mimequery.next();
    mimequery.finish();
    adsquery.finish();
    return tmpvariant;
}
*/

void ThumbFile(TSK_FS_FILE* tmpfile, unsigned long long objid)
{
    /*
    QSqlQuery thumbquery(thumbdb);
    thumbquery.prepare("SELECT COUNT(thumbid) FROM thumbs WHERE objectid = ?;");
    thumbquery.addBindValue(objid);
    thumbquery.exec();
    thumbquery.next();
    if(thumbquery.value(0).toInt() == 0)
    {
        if(tmpfile->meta != NULL)
        {
            QByteArray thumbdata;
            QImage thumbimage;
            QBuffer thumbuf(&thumbdata);
            QImage origimage;
            char imagebuffer[tmpfile->meta->size];
            ssize_t imglen = tsk_fs_file_read(tmpfile, 0, imagebuffer, tmpfile->meta->size, TSK_FS_FILE_READ_FLAG_NONE);
            bool imageloaded = origimage.loadFromData(QByteArray::fromRawData(imagebuffer, imglen));
            if(imageloaded)
            {
                thumbimage = origimage.scaled(QSize(320, 320), Qt::KeepAspectRatio, Qt::FastTransformation);
                thumbuf.open(QIODevice::WriteOnly);
                thumbimage.save(&thumbuf, "PNG");
                thumbdata = thumbdata.toBase64();
                QSqlQuery imgquery(thumbdb);
                imgquery.prepare("INSERT INTO thumbs(objectid, thumbblob) VALUES(?, ?);");
                imgquery.bindValue(0, objid);
                imgquery.bindValue(1, QString(thumbdata));
                imgquery.exec();
                imgquery.next();
                imgquery.finish();
            }
        }
    }
    */
}

QVariant HashFile(TSK_FS_FILE* tmpfile, unsigned long long objid)
{
    QVariant tmpvariant;
    TSK_FS_HASH_RESULTS hashresults;
    uint8_t retval = tsk_fs_file_hash_calc(tmpfile, &hashresults, TSK_BASE_HASH_MD5);
    if(retval == 0)
    {
        char sbuf[17];
        int sint = 0;
        for(int i=0; i < 16; i++)
        {
            sint = sprintf(sbuf+(2*i), "%02X", hashresults.md5_digest[i]);
        }
        /*
        QSqlQuery hashquery(fcasedb);
        hashquery.prepare("UPDATE data SET md5 = ? where objectid = ?;");
        if(sint > 0)
        {
            hashquery.bindValue(0, QString(sbuf));
            tmpvariant = QVariant(QString(sbuf));
        }
        else
        {
            hashquery.bindValue(0, QString(""));
            tmpvariant = QVariant(QString(""));
        }
        hashquery.bindValue(1, objid);
        hashquery.exec();
        hashquery.next();
        hashquery.finish();
        */
    }
    return tmpvariant;
}

void cnid_to_array(uint32_t cnid, uint8_t array[4])
{
    array[3] = (cnid >> 0) & 0xff;
    array[2] = (cnid >> 8) & 0xff;
    array[1] = (cnid >> 16) & 0xff;
    array[0] = (cnid >> 24) & 0xff;
}

QImage MakeThumb(const QString &img)
{
    QByteArray ba = QByteArray::fromBase64(QByteArray::fromStdString(img.toStdString()));
    if(ba.length() > 0)
        return QImage::fromData(ba, "PNG").scaled(QSize(thumbsize, thumbsize), Qt::KeepAspectRatio, Qt::FastTransformation);
    else
        return QImage(":/bar/missingthumb").scaled(QSize(thumbsize, thumbsize), Qt::KeepAspectRatio, Qt::FastTransformation);
}

void SqlMap(FileData &filedata)
{
    //QMutexLocker locker(&mutex);
    /*
    QSqlQuery fquery(fcasedb);
    //wombattableschema << "CREATE TABLE data(objectid INTEGER PRIMARY KEY, objecttype INTEGER, type INTEGER, name TEXT, fullpath TEXT, address INTEGER, parentid INTEGER, parimgid INTEGER, parfsid INTEGER, ctime INTEGER, crtime INTEGER, atime INTEGER, mtime INTEGER, md5 TEXT NOT NULL DEFAULT "", filemime TEXT, known INTEGER, checked INTEGER NOT NULL DEFAULT 0, mftattrid INTEGER NOT NULL DEFAULT 0);";
    fquery.prepare("INSERT INTO data(objtype, type, name, fullpath, size, addr, parid, parimgid, parfsid, ctime, crtime, atime, mtime, filemime, mftattrid) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
    if(filedata.mftattrid == 0)
    {
        fquery.bindValue(0, 5);
        filesprocessed++;
        processphase++;
    }
    else
        fquery.bindValue(0, 6);
    fquery.bindValue(1, filedata.type);
    fquery.bindValue(2, filedata.name);
    fquery.bindValue(3, filedata.path);
    fquery.bindValue(4, filedata.size);
    fquery.bindValue(5, filedata.addr);
    fquery.bindValue(6, filedata.paraddr);
    fquery.bindValue(7, filedata.evid);
    fquery.bindValue(8, filedata.fsid);
    fquery.bindValue(9, filedata.ctime);
    fquery.bindValue(10, filedata.crtime);
    fquery.bindValue(11, filedata.atime);
    fquery.bindValue(12, filedata.mtime);
    fquery.bindValue(13, filedata.mimetype);
    fquery.bindValue(14, filedata.mftattrid);
    /*
    fquery.bindValue(4, filedata.paraddr);
    fquery.bindValue(4, filedata.path);
    fquery.bindValue(6, filedata.ctime);
    fquery.bindValue(9, filedata.size);
    fquery.bindValue(10, filedata.addr);
    //fquery.bindValue(14, filedata.mimetype.split("/").at(0));
    */
    /*
    fquery.exec();
    //fquery.next();
    isignals->ProgUpd();
    //emit isignals->FinishSql();
    fquery.finish();
    */
}
