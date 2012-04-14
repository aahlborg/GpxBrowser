#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_action_About_Gpx_Browser_triggered()
{
	QString text = "Gpx Browser Alpha\n"
			"\n"
			"This program is currently under development.\n"
			"Please report bugs to gpxbrowser@aahlborg.se.\n"
			"Copyright 2012 Marcus Ahlberg";
	QMessageBox::about(this, "About Gpx Browser", text);
}

void MainWindow::on_actionAbout_Qt_triggered()
{
	QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_action_Quit_triggered()
{
	qApp->quit();
}

void MainWindow::on_actionOpen_triggered()
{
	qDebug() << "Open selected";
}
