#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include "gpxobject.h"

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
			"Copyright 2012 Marcus Ahlberg\n"
			"\n"
			"This program is currently under development.\n"
			"Please report bugs at https://bitbucket.org/aahlborg/gpxbrowser.";
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

void MainWindow::on_actionSave_As_triggered()
{
	qDebug() << "Save as selected";

	// Select file for save
	QString fileName = QFileDialog::getSaveFileName(this, "Save as");
	QFile file(fileName);

	// Open file
	if (file.open(QFile::WriteOnly | QFile::Text))
	{
		// Save to file
		GPXObject gpxObj;
		gpxObj.saveToFile(&file);
		file.close();
	}
	else
	{
		qDebug() << "Open file " << fileName << " failed!";
	}
}
