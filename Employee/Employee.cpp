﻿#include "Employee.h"
#include<iostream>
#include<QFileDialog>
#include<QSqlError>
using std::cout;
using std::endl;
using std::ends;

Employee::Employee(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

bool Employee::initConnect()
{
	DB_ = QSqlDatabase::addDatabase("QSQLITE", "theSqliteConnectName");
	if (!DB_.isValid()) {
		cout <<"invalid:"<< DB_.lastError().text().toStdString() << endl;
		return false;
	}
	DB_.setDatabaseName(DBPath_);
	if (!DB_.open()) {//成功打开
		cout <<"DB open error:"<< DB_.lastError().text().toStdString() << endl;
		return false;
	}
	return true;
}

static const QString TABLENAME{ "employee" };//table name
void Employee::initModel()
{
	theModel = new QSqlTableModel(this, DB_);
	theModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//更改view数据时不自动提交
	theModel->setTable(TABLENAME);
	theModel->setSort(0, Qt::AscendingOrder);
	ui.tabView->setModel(theModel);

	theSelectionModel = new QItemSelectionModel(theModel);
	ui.tabView->setSelectionModel(theSelectionModel);
	connect(theSelectionModel, &QItemSelectionModel::currentChanged, this, &Employee::on_currentChanged);
	connect(theSelectionModel, &QItemSelectionModel::currentRowChanged, this, &Employee::on_currentRowChanged);
	theModel->select();
}

void Employee::setActState(bool ok)
{
	ui.actOpen->setEnabled(!ok);
	ui.actAppend->setEnabled(ok);
	ui.actInsert->setEnabled(ok);
	ui.actDelete->setEnabled(ok);
	ui.actPhoto->setEnabled(ok);
	ui.actClear->setEnabled(ok);
	ui.actScan->setEnabled(ok);
}

void Employee::on_currentChanged()
{

}

void Employee::on_currentRowChanged()
{

}

void Employee::on_actOpen_triggered()
{
	const QString tempPath{ QFileDialog::getOpenFileName(this,tr("选择数据库"),DBPath_,"DB(*.db3 *.db);;Other(*.*)") };
	if (tempPath.isEmpty())return;
	if (DBPath_ != tempPath) {//记忆路径
		DBPath_ = tempPath;
	}
	if (!initConnect())return;
	initModel();
	setActState(true);//enable action
}

void Employee::on_actExit_triggered()
{
	this->close();
}
