#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
	void on_action_About_Gpx_Browser_triggered();

	void on_action_Quit_triggered();

	void on_actionAbout_Qt_triggered();

	void on_actionOpen_triggered();

	void on_actionSave_As_triggered();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
