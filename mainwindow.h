#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QFileDialog>
#include <QProgressBar>
#include <QSlider>
#include <QFrame>
#include <QVBoxLayout>
#include <QIcon>
#include <QPixmap>
#include <QPushButton>
#include <QObject>
#include <QLabel>
#include <QTime>
#include <QDir>
#include <QList>
#include <QScrollArea>
#include <QGraphicsView>
#include <QGraphicsVideoItem>
#include <QWidget>
#include <QDesktopWidget>
#include <QProcess>
#include <QMessageBox>
#include <QFileInfo>
#include <QStackedLayout>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_actionOpen_triggered();

    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_StopButton_clicked();

    void on_insertButton_clicked();

    void on_actionZoom_in_triggered();

    void on_actionZoom_out_triggered();

    void durationChanged(qint64 duration);

    void positionChanged(qint64 progress);

    void seek(int seconds);

    void on_mat_value_2_valueChanged(int arg1);
    
    void on_startButton_clicked();
    
    void on_maskButton_clicked();

    void on_imageButton_clicked();

    QString getCurrDir();

    void on_actionDrag_triggered();

    void on_actionSave_triggered();

signals:
    void clicked();


private:
    void make_button();
    void show_cropiamges(QList<QPushButton *> list ,QString dir);
    void updateDurationInfo(qint64 currentInfo);

    Ui::MainWindow *ui;
    QWidget* barWidget;
    QMediaPlayer* player;
    QGraphicsView* graphicsView;
    QGraphicsScene* scene;
    QGraphicsVideoItem* videoItem;
    QVideoWidget* videoWidget;
    QLabel* labelDuration;
    QSlider* slider;
    QVBoxLayout* playerlayout;
    QVBoxLayout* boxlayout;
    QHBoxLayout* barlayout;
    QVBoxLayout* imageslot;
    QIcon* icon;
    QPushButton* button;
    QScrollArea* scrollArea;
    QString show_cropdir;
    QStringList show_cropimages;
    QList<QPushButton *> show_croplist;
//    QList<QPushButton *> show_croplist1;
    QStringList slotlist;
    QUrl file_path;
    QUrl slot_path;
    QStringList temlist;
    QString savename;
    QString savename_final;

    int value=30;

    qint64 m_duration;
};

#endif // MAINWINDOW_H
