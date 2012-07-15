#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

class GPXObject;
class QAction;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
private slots:
	// Actions
	void on_action_About_Gpx_Browser_triggered();
	void on_action_Quit_triggered();
	void on_actionAbout_Qt_triggered();
	void on_actionOpen_triggered();
	void on_actionSave_As_triggered();
	void on_actionTileProvider_triggered();
	void on_actionLength_triggered();
	void on_actionDuration_triggered();

private:
	// Application global functions
	void appInit();

	// Misc functions
	void updateTileProviderList();
	void updatePathsToDraw();

	// Utility functions
	QString formatDistance(int meters);
	QString formatTimeSpan(int seconds);

	// Keeps track of tile providers in menu
	QList<QAction *> tileProviderMenuItems_;

	Ui::MainWindow *ui;

	// Document
	GPXObject * gpxObject_;
};

#endif // MAINWINDOW_H
