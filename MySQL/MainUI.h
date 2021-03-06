﻿#pragma once
#include <QWidget>
//#include<QSqlTableModel>
#include<QSqlRelationalTableModel>
#include "ZrMySQL.h"

namespace Ui { class MainUI; };

class MainUI : public QWidget
{
	Q_OBJECT

public:
	MainUI(QWidget *parent = Q_NULLPTR);
	~MainUI();

private:
	void init();

	Ui::MainUI *ui;
	ZrMySQL mySql;
	QSqlRelationalTableModel *theModel;

private slots:
	void on_comboDB_currentIndexChanged(int index);
	void on_btnQuery_clicked();
};
