#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gpxobject.h"
#include "tileprovider.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QtAlgorithms>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	updateTileProviderList();
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
		qDebug() << "MainWindow: Open file " << fileName << " failed!";
	}
}


void MainWindow::on_actionTileProvider_triggered()
{
	QAction * tileProviderMenuItem = static_cast<QAction *>(sender());
	int tileProviderIndex = tileProviderMenuItems_.indexOf(tileProviderMenuItem);

	if (tileProviderIndex < 0)
	{
		qDebug() << "MainWindow: Error: TileProvider not found";
		return;
	}

	// Set tile provider
	MapView * mapView = static_cast<MapView *>(ui->centralWidget);
	mapView->setActiveTileProvider(tileProviderIndex);

	// Debug
	TileProviderInfoList * tileProviders = mapView->getTileProviders();
	qDebug() << "MainWindow: Tile provider " << tileProviders->at(tileProviderIndex)->name << " selected";
}

void MainWindow::updateTileProviderList()
{
	qDebug() << "MainWindow: updateTileProviderList";
	MapView * mapView = static_cast<MapView *>(ui->centralWidget);
	TileProviderInfoList * tileProviders = mapView->getTileProviders();

	ui->menuMap->clear();
	qDeleteAll(tileProviderMenuItems_);
	tileProviderMenuItems_.clear();

	for (int i = 0; i < tileProviders->size(); ++i)
	{
		QAction * newMenuItem = new QAction(tileProviders->at(i)->name, ui->menuMap);
		connect(newMenuItem, SIGNAL(triggered()), this, SLOT(on_actionTileProvider_triggered()));
		tileProviderMenuItems_.append(newMenuItem);
		ui->menuMap->addAction(newMenuItem);
	}
}
