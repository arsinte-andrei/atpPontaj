#include "atpini.h"
#include <QDebug>

atpIni::atpIni(QObject *parent) : QSettings(parent) {
	mySettings = new QSettings("atp.ini", QSettings::IniFormat);
}

/***************************************
 ***************DATABASE****************
 ***************************************/

//DataBase path
QString atpIni::get_dbPath(){
	return mySettings->value("DB/Path").toString();
}

void atpIni::set_dbPath(QString value){
	mySettings->setValue("DB/Path", value);
}

//Database type
QString atpIni::get_dbType(){
	return mySettings->value("DB/Type", "QSQLITE").toString();
}

void atpIni::set_dbType(QString value){
    mySettings->setValue("DB/Type", value);
}
