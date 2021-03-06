﻿#include "MainUI.h"
#include "ui_MainUI.h"
#include<QSqlRelationalDelegate>

MainUI::MainUI(QWidget *parent)
	: QWidget(parent),mySql("conMySQL"),\
	theModel(nullptr)
{
	ui = new Ui::MainUI();
	ui->setupUi(this);
	init();
}

MainUI::~MainUI()
{
	delete ui;
}

void MainUI::init()
{
	mySql.createDB("test");
	theModel = new QSqlRelationalTableModel(this, mySql.getDataBase());

	ui->tableView->setModel(theModel);
	ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	//ui->tableView->setColumnHidden(0, true);//id 列隐藏
	ui->tableView->resizeColumnsToContents();//大小适配文本
	//ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//无法编辑

	//设置外键的代理后,界面显示为可选择的枚举,否则编辑时为city name的QString,但是
	//实际只接受student address的int值,直接更改QString值无效
	ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));

	QHeaderView *header{ ui->tableView->horizontalHeader() };
	header->setStretchLastSection(true);//最后一列拉伸,不显示空白列
}

void MainUI::on_comboDB_currentIndexChanged(int index)
{
	QString tableName;
	switch (index) {
	case 0:
		tableName = "classes";
		break;
	case 1:
		tableName = "pet";
		break;
	case 2:
		tableName = "student";
		break;
	case 3:
		tableName = "students";
		break;
	case 4:
		tableName = "user";
		break;
	}
	theModel->setTable(tableName);
	if (tableName == "student") {
		theModel->setRelation(3, QSqlRelation{ "city","id","name" });
		theModel->setHeaderData(3, Qt::Horizontal, "theCity");
	}
	theModel->setSort(0, Qt::AscendingOrder);//index 1 name排序
	//theModel->setFilter("id=2");//设置where条件
	//theModel->setHeaderData(1, Qt::Horizontal, "aName");//手动指定列名
	//theModel->setHeaderData(2, Qt::Horizontal, "theAge");//默认用数据库指定的名字
	theModel->select();
}

void MainUI::on_btnQuery_clicked()
{
	const auto s{ ui->textEdit->toPlainText() };
	mySql.query(s);
}
