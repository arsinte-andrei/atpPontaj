#ifndef ATPDB_H
#define ATPDB_H

#include <QMap>
#include <QVariant>
#include <QSqlDatabase>
#include <QObject>
#include <QSqlQuery>
#include "atpini.h"

class atpDb : public QObject, public QSqlDatabase{
        Q_OBJECT
    private:
        QSqlDatabase mydb, myImportDb;
        bool openStatus;
        atpIni *atpIniSettings;
        QString dbPath, dbType;

    public:
        atpDb(QObject *parent = 0);
        static bool atp_create_new_atpdb(QString dbPath);
        static void createViews();

    public slots:
        QSqlQuery atpSelect(QString connName, QString sqlQuerry, QMap<QString, QVariant> *myArray = NULL);
        QSqlQuery atpInsert(QString connName, QString tableName, QMap<QString, QVariant> *data );
        QSqlQuery atpUpdate(QString connName, QString tableName, QMap<QString, QVariant> *data, QString whereTo);
        QSqlQuery atpDelete(QString connName, QString tableName, QString whereTo);

        QSqlDatabase mainDatabase();
        QSqlDatabase importDatabase();

        bool setupNewImportConnection(QString dbImportPath, QString connName = "importConnection");

        bool setupNewConnection(QString connName = "mainConnection");
        bool isOpen();
        bool isDriverAvailable();
        void setDataBasePath(QString newPath);

    signals:
        void dbRefresh(bool);
        void atpSelected(QString, QSqlQuery);
        void atpInserted(QString, QSqlQuery);
        void atpUpdated(QString, QSqlQuery);
        void atpDeleted(QString, QSqlQuery);
};

#endif // ATPDB_H
