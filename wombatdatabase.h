#ifndef WOMBATDATABASE_H
#define WOMBATDATABASE_H

#include "wombatinclude.h"
#include "wombatvariable.h"
#include "wombatproperties.h"
#include "wombatfunctions.h"

class WombatDatabase : public QObject
{
    Q_OBJECT
public:
    WombatDatabase(WombatVariable* wombatvarptr);
    void CreateAppDB(void);
    void CreateCaseDB(void);
    void CreateLogDB(void);
    void OpenCaseDB(void);
    void OpenAppDB(void);
    void OpenLogDB(void);
    void CloseAppDB(void);
    void CloseCaseDB(void);
    void CloseLogDB(void);
    void InsertEvidenceObject(void);
    void GetEvidenceObject(void);
    void GetEvidenceObjects(void);
    void InsertVolumeObject(void);
    void InsertPartitionObjects(void);
    void InsertFileSystemProperties(int curfsid, TSK_FS_INFO* curfsinfo);
    void GetObjectValues(void);
    //QString GetFileSystemLabel(TSK_FS_INFO* curfsinfo);

    ~WombatDatabase();

    int ReturnCaseCount(void);
    void InsertCase();
    void ReturnCaseNameList(void);
    void ReturnEvidenceNameList(void);
    void ReturnCaseID(void);
    void ReturnFileSystemObjectList(int curid);
    void ReturnObjectPropertyList(void);
    void RemoveEvidence(void);

    QList<QSqlRecord> GetSqlResults(QString query, QVariantList invalues);
    int InsertSqlGetID(QString query, QVariantList invalues);
    void InsertSql(QString query, QVariantList invalues);
    void InsertSql(QString query);
    int ReturnSqlRowsAffected(QString query, QVariantList invalues);
    int GetEvidenceFileCount(void);
    int ReturnFileSystemOffset(int fsid);

    WombatProperties* wombatprop;

signals:
    void DisplayError(QString errorNumber, QString errorType, QString errorValue);
private:
    WombatVariable* wombatptr;
};

#endif // WOMBATDATABASE_H
