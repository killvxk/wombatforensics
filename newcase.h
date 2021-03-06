#ifndef NEWCASE_H
#define NEWCASE_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "tskcpp.h"
#include "properties.h"
#include "wombatfunctions.h"
/*#include "libvsmbr.h"
#include "libvsgpt.h"
#include "libfsapfs.h"
#include "libfsext.h"
#include "libfshfs.h"
#include "libfsntfs.h"
#include "libfsrefs.h"
#include "libfsxfs.h"
*/

void ProcessVolume(QString evidstring);
int ParseVolume(QString estring, qint64 imgsize, QList<int> *offlist, QList<int> *sizlist);
void ParseExtendedPartition(QString estring, uint32_t primaryextendedoffset, uint32_t offset, uint32_t size, QList<int>* offlist, QList<int>* sizelist);
int GetFileSystemType(QString estring, off64_t offset);
/*
//int GetFileSystemType(QString estring);
int GetFileSystemType(QString estring, off64_t offset);
//QString GetFileSystemVolumeName(QString estring, int fstype);
QString GetFileSystemVolumeName(QString estring, int fstype, off64_t offset, size64_t psize);
*/
void InitializeEvidenceStructure(QString evidstring);
void ProcessDir(TSK_FS_INFO* fsinfo, TSK_STACK* stack, TSK_INUM_T dirinum, const char* path, int eint, int vint, int pint, QString partitionpath);

#endif // newcase.h
