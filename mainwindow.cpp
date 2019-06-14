#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include "iomanip"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    videoItem = new QGraphicsVideoItem;
    videoItem->setSize(QSizeF(screenGeometry.width() / 3, screenGeometry.height() / 2.25));

    qDebug() << QSizeF(screenGeometry.width() / 3, screenGeometry.height() / 2.25);

    scene = new QGraphicsScene(this);
    graphicsView = new QGraphicsView(scene);
    graphicsView->setHorizontalScrollBarPolicy (Qt :: ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy (Qt :: ScrollBarAlwaysOff);
    graphicsView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    scene->addItem(videoItem);

    playerlayout = ui->playerlayout;
    playerlayout->setMargin(0);
    playerlayout->addWidget(graphicsView);

    player->setVideoOutput(videoItem);

    //------slider
    barlayout = new QHBoxLayout();
    slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, long(player->duration()/1000) );

    labelDuration = new QLabel(this);

    barlayout->addWidget(slider);
    barlayout->addWidget(labelDuration);

    ui->barWidget->setLayout(barlayout);

    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);
    connect(slider, &QSlider::sliderMoved, this, &MainWindow::seek);

    //---------- image slot
    imageslot = new QVBoxLayout(this);
    imageslot->setMargin(0);
    imageslot->setSpacing(0);
    QPushButton* insertButton;
    for(int i = 0; i < 10; i++) {
        insertButton = new QPushButton(this);
        insertButton->setStyleSheet("background-image: url(:/images/icon/add.png); background-repeat: no-repeat; background-position: center;");
        insertButton->setFixedSize(200, 150);
        imageslot->addWidget(insertButton);
        connect(insertButton, SIGNAL(clicked()), this, SLOT(on_insertButton_clicked()));
    }
    ui->scrollAreaWidgetContents->setLayout(imageslot);

}
MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getCurrDir() {
    QDir currdir = QDir::current();

    auto path = currdir.path();
    qDebug() << path;
    return path;
}

void MainWindow::on_actionOpen_triggered()
{
    QString currDir = getCurrDir();
//    QMessageBox mss;
//    mss.setText(currDir);
//    mss.exec();
    QString filename = QFileDialog::getOpenFileName(this, "Open a File", "", "Video File (*.mp4)");


    file_path = QUrl::fromLocalFile(filename);

//    qDebug() << file_path.path();
    player->setMedia(QUrl::fromLocalFile(filename));
    graphicsView->show();
    player->play();

}

void MainWindow::on_playButton_clicked()
{
    player->play();
    ui->statusBar->showMessage("Playing");
}

void MainWindow::on_pauseButton_clicked()
{
    player->pause();
    ui->statusBar->showMessage("Paused...");
}

void MainWindow::on_StopButton_clicked()
{
    player->stop();
    ui->statusBar->showMessage("Stop!");
}

void MainWindow::on_insertButton_clicked()
{
    QPushButton* button = (QPushButton*)sender();

    if(button->icon().isNull()) {
        QString filename = QFileDialog::getOpenFileName(this, "Open a File", "", "Image File (*.jpeg)");
        slot_path = QUrl::fromLocalFile(filename);
        QString slot_file = slot_path.toString().section("/", 3);
//        slot_file.append("\'");
//        slot_file.prepend("\'");
        temlist.push_back(slot_file);
//        qDebug() << temlist;
//        qDebug() << slot_path;
        QPixmap pixmap(filename);
        QIcon icon(pixmap);
//        slotlist.push_back(filename);
        button->setIcon(icon);
        button->setIconSize(button->rect().size());
    }
    else {
        button->setIcon(QIcon());
    }

}
void MainWindow::on_actionZoom_in_triggered()
{
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    graphicsView->scale(1.2, 1.2);
}

void MainWindow::on_actionZoom_out_triggered()
{
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    graphicsView->scale(0.9,0.9);
}

void MainWindow::durationChanged(qint64 duration)
{
    m_duration = duration / 1000;
    slider->setMaximum(m_duration);
}

void MainWindow::positionChanged(qint64 progress)
{
    if( !slider->isSliderDown() )
        slider->setValue(progress / 1000);

    updateDurationInfo(progress / 1000);
}

void MainWindow::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if( currentInfo || m_duration ) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
                    currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
                    m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    labelDuration->setText(tStr);
    labelDuration->setStyleSheet({"color:black;"});
}

void MainWindow::seek(int seconds) {
    player->setPosition(seconds*1000);
}

void MainWindow::on_mat_value_2_valueChanged(int arg1)
{
//    arg1 = ui->mat_value_2->value();
    value = arg1;
    qDebug() << arg1;

}
void MainWindow::on_startButton_clicked()
{
    QString currDir = getCurrDir();
    QString cropdir = currDir + "/Crop_images/";
    QString modeldir = currDir + "/cfg/tiny-yolo-voc-2c.cfg";
    QString gifname = currDir + "/icon/Spinner.gif";

    QString filename = file_path.path();
//    qDebug() << filename;
    QString py_program(currDir + "/python");
    QProcess process;
    QStringList args = QStringList() << currDir+"/processing_video.py" << filename << cropdir << modeldir << gifname;

//    py_process.startDetached(py_program, args);

    ui->maskButton->setEnabled(true);
    int exitCode = process.execute(py_program, args);
//    qDebug() << exitCode;


   delete ui->cropAreaWidgetContents->layout();
   make_button();


}

void MainWindow::on_maskButton_clicked()
{
    QString currDir = getCurrDir();


    QString templatename = temlist.join(",");
    QString py_program(currDir + "/python");
    QString cropdir = currDir + "/Crop_images/";
    QString filename = file_path.path();
    QString gifname = currDir + "/icon/Spinner.gif";
    QString templatetxt =currDir + "/templateimg.txt";

//    QMessageBox m;
//    m.setText(filename);
//    m.exec();
//    QString savename = currDir + "/video-masked.mp4";

    //template matching
    QProcess templateprocess;
    QStringList args1 = QStringList() << currDir +"/pattern2.py" << filename << templatename << gifname << templatetxt;


    int exitCode1 = templateprocess.execute(py_program, args1);
//    qDebug() << "exit1";
//    qDebug() << exitCode1;

    //masking
    QProcess process;
    QStringList args2 = QStringList() << currDir + "/masking.py" << cropdir << filename << savename << QString::number(value)<<templatetxt;
    
    int exitCode2 = process.execute(py_program, args2);
//    qDebug() << "exit2";
//    qDebug() << exitCode2;
    if(exitCode2==0)
    {
        QMessageBox msgBox;
        msgBox.setText("Complete!");
        msgBox.setInformativeText("Do you want to check a video?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        switch(ret) {
        case QMessageBox::Yes :
            player->setMedia(QUrl::fromLocalFile(savename));
            graphicsView->show();
            player->play();
            break;
        case QMessageBox::No :
            break;
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Caution! The program was interrupted");
        msgBox.setInformativeText("Do you want to check a video?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        switch(ret) {
        case QMessageBox::Yes :
            player->setMedia(QUrl::fromLocalFile(savename));
            graphicsView->show();
            player->play();
            break;
        case QMessageBox::No :
            break;
        }
    }


    delete ui->cropAreaWidgetContents->layout();
    qDebug() << "layout삭제";

    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel;
    label->setText("No Images");
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    label->setGeometry(0, 0, 891, 300);
    label->setStyleSheet("color : black; font-size : 15pt;");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
    ui->cropAreaWidgetContents->setLayout(layout);
    layout->setMargin(0);

    ui->actionDrag->setEnabled(true);
}

void MainWindow::make_button(){
    qDebug() << "make 시작";

    //----------- cropimage slot
    delete ui->cropAreaWidgetContents->layout();
    QGridLayout* show_cropslot = new QGridLayout(this);
    show_cropslot->setMargin(0);
    show_cropslot->setSpacing(0);
    QPushButton* imageButton;
    QString currDir = getCurrDir();
    show_cropdir = currDir + "/Crop_images/";

    QDir cdir(show_cropdir);
    show_croplist = QList<QPushButton *>();

    show_cropimages = cdir.entryList(QStringList() <<"*.jpeg" <<"*.JPEG",QDir::Files);

    int c = show_cropimages.count()/7 + 1;
    int cnt = 1;

    qDebug() << show_cropimages.count();
    for(int i=0; i<c; i++) {
        for(int j=0; j<7; j++){
            imageButton = new QPushButton(this);
            imageButton->setFixedSize(120, 90);
            show_croplist.push_back(imageButton);
            show_cropslot->addWidget(imageButton,i,j,1,1);
            connect(imageButton, SIGNAL(clicked()), this, SLOT(on_imageButton_clicked()));

            cnt += 1;
        }
    }
    show_cropiamges(show_croplist, show_cropdir);

    ui->cropAreaWidgetContents->setLayout(show_cropslot);
}

void MainWindow::show_cropiamges(QList<QPushButton *> list ,QString dir){
    show_croplist = list;
    show_cropdir = dir;
    QDir cdir(show_cropdir);
    show_cropimages = cdir.entryList(QStringList() <<"*.jpeg" <<"*.JPEG",QDir::Files);

    qDebug() << "show_image call";
    qDebug() << show_cropimages.count();
    for(int i=0; i<show_cropimages.count(); i++){
        QPixmap croppixmap(show_cropdir + show_cropimages.at(i));
        QIcon cropicon(croppixmap);
        qDebug() << show_cropimages.at(i);
        qDebug() << croppixmap.isNull();
        show_croplist.value(i)->setIcon(cropicon);
        show_croplist.value(i)->setIconSize(show_croplist.value(i)->rect().size());
        show_croplist.value(i)->setObjectName(show_cropdir + show_cropimages.at(i));
    }
}

void MainWindow::on_imageButton_clicked(){

    QPushButton* imagebutton = (QPushButton*)sender();
    QString currDir = getCurrDir();
    QString filename = imagebutton->objectName();

    qDebug() << filename;

    QString py_program(currDir + "/python");
    QProcess deleteprocess;
    QStringList args = QStringList() << currDir+"/crop_delete.py" <<  filename ;

    int exitCode = deleteprocess.execute(py_program,args);
    qDebug() << exitCode;


    show_cropiamges(show_croplist, show_cropdir);
}

void MainWindow::on_actionDrag_triggered()
{
    QString currDir = getCurrDir();

    savename_final = QFileDialog::getSaveFileName(this, "Save file", "", "Video(*.mp4)");

    QString cropdir = currDir + "/Crop_images/";

//    QString filename = currDir + "/video-masked.mp4";
    QString filename = savename;
//    QString savename = currDir + "/video-masked-complete.mp4";

    qDebug() << savename_final;
    qDebug() << filename;

    QString py_program(currDir + "/python");
    QProcess process;
    QStringList args = QStringList() << currDir + "/manual_blur.py" << filename << savename_final;

//    int exitCode = process.startDetached(py_program, args);


    QMessageBox msgBox;

    msgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
    msgBox.setFixedSize(242, 103);
    msgBox.move(100,100);
    msgBox.setText("Keyboard Operation");
    msgBox.setInformativeText("\n[Spacebar] : Play & Pause \n\n[w] : Edit mode \n\n[esc] : quit  ");

    msgBox.setWindowTitle("help");
    msgBox.exec();


    int exitCode = process.execute(py_program, args);


    if(exitCode==0)
    {
        QMessageBox msgBox;
        msgBox.setText("Complete!");
        msgBox.exec();


        QMessageBox msgBox2;
        msgBox2.setText("Complete!");
        msgBox2.setInformativeText("Do you want to check a video?");
        msgBox2.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
        msgBox2.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox2.exec();

        switch(ret) {
        case QMessageBox::Yes :
            player->setMedia(QUrl::fromLocalFile(savename_final));
            graphicsView->show();
            player->play();
            break;
        case QMessageBox::No :
            break;
        }

    }
    else if(exitCode==1)
    {

        QMessageBox msgBox;
        msgBox.setText("Sorry. Please try again!");
        msgBox.exec();
    }
    qDebug() << exitCode;
}

void MainWindow::on_actionSave_triggered()
{
    savename = QFileDialog::getSaveFileName(this, "Save file", "", "Video(*.mp4)");
    ui->startButton->setEnabled(true);

}
