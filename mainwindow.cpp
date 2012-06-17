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

	gpxObject_ = new GPXObject();
	MapView * mapView = static_cast<MapView *>(ui->centralWidget);
	mapView->clearPaths();
	for (int i = 0; i < gpxObject_->getTracks()->size(); ++i)
	{
		GPXTrack track = gpxObject_->getTracks()->at(i);
		for (int j = 0; j < track.getSegments()->size(); ++j)
		{
			GPXPath trkSeg = track.getSegments()->at(j);
			const QVector<GPXWaypoint> * waypoints = trkSeg.getWaypoints();

			QVector<QPointF> path;
			for (int i = 0; i < waypoints->size(); ++i)
			{
				QPointF coord = QPointF(waypoints->at(i).getLongitude(), waypoints->at(i).getLatitude());
				path.append(coord);
			}
			mapView->addPath(path);
		}
	}
	for (int i = 0; i < gpxObject_->getRoutes()->size();++i)
	{
		GPXRoute route = gpxObject_->getRoutes()->at(i);
		const QVector<GPXWaypoint> * waypoints = route.getPath().getWaypoints();

		QVector<QPointF> path;
		for (int i = 0; i < waypoints->size(); ++i)
		{
			QPointF coord = QPointF(waypoints->at(i).getLongitude(), waypoints->at(i).getLatitude());
			path.append(coord);
		}
		mapView->addPath(path);
	}
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

	// Check active provider only
	for (int i = 0; i < tileProviderMenuItems_.size(); ++i)
	{
		tileProviderMenuItems_.at(i)->setChecked(false);
	}
	tileProviderMenuItems_.at(tileProviderIndex)->setChecked(true);

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
		// Set a shortcut, Ctrl+1-0
		if (i < 10)
		{
			newMenuItem->setShortcut(QKeySequence(QString("Ctrl+%1").arg((i + 1) % 10)));
		}
		// Select active provider
		newMenuItem->setCheckable(true);
		if (mapView->activeTileProvider() == i)
		{
			newMenuItem->setChecked(true);
		}
		connect(newMenuItem, SIGNAL(triggered()), this, SLOT(on_actionTileProvider_triggered()));
		tileProviderMenuItems_.append(newMenuItem);
		ui->menuMap->addAction(newMenuItem);
	}
}
