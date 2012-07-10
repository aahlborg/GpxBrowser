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
	ui(new Ui::MainWindow),
	gpxObject_(NULL)
{
	ui->setupUi(this);

	updateTileProviderList();

	updatePathsToDraw();
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

	// Select file to open
	QString fileName = QFileDialog::getOpenFileName(this, "Open file");
	QFile file(fileName);

	// Open file
	if (file.open(QFile::ReadOnly | QFile::Text))
	{
		GPXObject * gpxObject = GPXObject::loadFromFile(&file);
		file.close();

		// Replace GPX object
		if (NULL != gpxObject_)
			delete gpxObject_;
		gpxObject_ = gpxObject;
		updatePathsToDraw();
	}
	else
	{
		qDebug() << "MainWindow: Open file " << fileName << " failed!";
	}
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

void MainWindow::updatePathsToDraw()
{
	MapView * mapView = static_cast<MapView *>(ui->centralWidget);
	mapView->clearPaths();
	mapView->clearWaypoints();

	if (NULL == gpxObject_)
		return;

	// Add tracks
	for (int i = 0; i < gpxObject_->getTracks()->size(); ++i)
	{
		qDebug() << "MainWindow: Found track";
		GPXTrack track = gpxObject_->getTracks()->at(i);
		for (int j = 0; j < track.getSegments()->size(); ++j)
		{
			qDebug() << "MainWindow: Found track segment";
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

	// Add paths
	for (int i = 0; i < gpxObject_->getRoutes()->size();++i)
	{
		qDebug() << "MainWindow: Found route";
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

	// Add waypoints
	for (int i = 0; i < gpxObject_->getWaypoints()->size(); ++i)
	{
		GPXWaypoint wpt = gpxObject_->getWaypoints()->at(i);
		QPointF coord = QPointF(wpt.getLongitude(), wpt.getLatitude());
		mapView->addWaypoint(coord);
	}
}

QString MainWindow::formatDistance(int meters)
{
	if (meters < 1000)
	{
		return QString("%1m").arg(QString::number(meters));
	}
	else
	{
		const double kilometers = meters / 1000.0;

		int decimals;
		if (kilometers < 10.0)
			decimals = 2;
		else if (kilometers < 100)
			decimals = 1;
		else
			decimals = 0;

		return QString("%1km").arg(QString::number(kilometers, 'f', decimals));
	}
}

QString MainWindow::formatTimeSpan(int seconds)
{
	const int oneMinute = 60;
	const int oneHour = 60 * oneMinute;
	const int oneDay = 24 * oneHour;
	const int oneYear = 365 * oneDay;

	const int years = seconds / oneYear;
	seconds -= years * oneYear;
	const int days = seconds / oneDay;
	seconds -= days * oneDay;
	const int hours = seconds / oneHour;
	seconds -= hours * oneHour;
	const int minutes = seconds / oneMinute;
	seconds -= minutes * oneMinute;

	QString yearsText;
	if (0 != years)
		yearsText = QString("%1y ").arg(QString::number(years));
	else
		yearsText = "";

	QString daysText;
	if (0 != days)
		daysText = QString("%1d ").arg(QString::number(days));
	else
		daysText = "";

	QString text = QString("%1%2%3:%4:%5").arg(
				yearsText).arg(
				daysText).arg(
				QString::number(hours)).arg(
				QString::number(minutes), 2, '0').arg(
				QString::number(seconds), 2, '0');
	return text;
}

void MainWindow::on_actionLength_triggered()
{
	if (NULL == gpxObject_)
	{
		QMessageBox info(this);
		info.setWindowTitle("Gpx Browser");
		info.setText("No file is open");
		info.setIcon(QMessageBox::Critical);
		info.exec();
		return;
	}

	QString message = "Length of tracks:\n";
	double totalLength = 0.0;

	for (int i = 0; i < gpxObject_->getTracks()->size(); ++i)
	{
		double trackLen = gpxObject_->getTracks()->at(i).length();
		message += QString("\nTrack %1: %2").arg(QString::number(i), formatDistance((int)trackLen));
		totalLength += trackLen;
	}

	message += QString("\n\nTotal: %1").arg(formatDistance((int)totalLength));

	QMessageBox msgBox(this);
	msgBox.setWindowTitle("Track length");
	msgBox.setText(message);
	msgBox.setIcon(QMessageBox::Information);
	msgBox.exec();
}

void MainWindow::on_actionDuration_triggered()
{
	if (NULL == gpxObject_)
	{
		QMessageBox info(this);
		info.setWindowTitle("Gpx Browser");
		info.setText("No file is open");
		info.setIcon(QMessageBox::Critical);
		info.exec();
		return;
	}

	QString message = "Duration of tracks:\n";
	double totalDuration = 0.0;

	for (int i = 0; i < gpxObject_->getTracks()->size(); ++i)
	{
		double trackTime = gpxObject_->getTracks()->at(i).duration();

		message += QString("\nTrack %1: %2").arg(
					QString::number(i)).arg(
					formatTimeSpan((int)trackTime));

		totalDuration += trackTime;
	}

	message += QString("\n\nTotal: %1").arg(formatTimeSpan((int)totalDuration));

	QMessageBox msgBox(this);
	msgBox.setWindowTitle("Track duration");
	msgBox.setText(message);
	msgBox.setIcon(QMessageBox::Information);
	msgBox.exec();
}
