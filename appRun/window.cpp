#include "window.h"

window::window()
{
    // set up the screen
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1024, 768);

    // set up the scene
    welcomeScene = new QGraphicsScene();
    welcomeScene->setSceneRect(0, 0, 1024, 768);
    setScene(welcomeScene);
}

window::~window()
{
    delete welcomeScene;
    delete editScene;
}

void window::mainWindow()
{
    gameButton *newGameButton = new gameButton(QString("new"), 400, 275, 200, 50);
    connect(newGameButton, SIGNAL(clicked()), this, SLOT(editWindowSignal()));
    welcomeScene->addItem(newGameButton);

    gameButton *loadGameButton = new gameButton(QString("new but blue"), 400, 475, 200, 50);
    QBrush brush(Qt::blue);
    loadGameButton->setBrush(brush);
    connect(loadGameButton, SIGNAL(clicked()), this, SLOT(editWindowSignal()));
    welcomeScene->addItem(loadGameButton);
}
void window::editWindowSignal()
{
    // set up the scene
    editScene = new QGraphicsScene();
    editScene->setSceneRect(0, 0, 1024, 768);
    setScene(editScene);

    QGraphicsRectItem *playground = new QGraphicsRectItem(0, 0, PLAY_W, PLAY_H);
    playground->setPos(PLAY_X, PLAY_Y);
    QPen pen;
    pen.setColor(Qt::white); // Set the border color (replace with your desired color)
    pen.setWidth(4);         // Set the border width (replace with your desired width)
    playground->setPen(pen);
    editScene->addItem(playground);

    QGraphicsRectItem *iPanel = new QGraphicsRectItem(0, 0, IPANEL_W, IPANEL_H);
    iPanel->setPos(IPANEL_X, IPANEL_Y);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::white);
    iPanel->setBrush(brush);
    pen.setColor(Qt::white);
    iPanel->setPen(pen);
    editScene->addItem(iPanel);

    QGraphicsRectItem *pPanel = new QGraphicsRectItem(0, 0, PPANEL_W, PPANEL_H);
    pPanel->setPos(PPANEL_X, PPANEL_Y);
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green);
    pen.setColor(Qt::green);
    pPanel->setPen(pen);
    pPanel->setBrush(brush);
    editScene->addItem(pPanel);

    QGraphicsRectItem *rPanel = new QGraphicsRectItem(0, 0, RPANEL_W, RPANEL_H);
    rPanel->setPos(RPANEL_X, RPANEL_Y);
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::red);
    rPanel->setBrush(brush);
    pen.setColor(Qt::red);
    rPanel->setPen(pen);
    editScene->addItem(rPanel);

    gameButton *robotAButton = new gameButton(QString("Auto Robot"), 50, 50, 80, 20, iPanel);
    connect(robotAButton, SIGNAL(clicked()), this, SLOT(editWindowSignal()));
    editScene->addItem(robotAButton);

    gameButton *robotRButton = new gameButton(QString("RC robot"), 50, 300, 80, 20, iPanel);
    connect(robotRButton, SIGNAL(clicked()), this, SLOT(editWindowSignal()));
    editScene->addItem(robotRButton);

    gameButton *barrierButton = new gameButton(QString("Barrier"), 50, 550, 80, 20, iPanel);
    connect(barrierButton, SIGNAL(clicked()), this, SLOT(editWindowSignal()));
    editScene->addItem(barrierButton);

    barrierC *edgeTop = new barrierC(0, 0, PLAY_W, 1, playground);
    pen.setColor(Qt::black);
    edgeTop->setPen(pen);
    editScene->addItem(edgeTop);

    barrierC *edgeBottom = new barrierC(0, PLAY_H, PLAY_W, 1, playground);
    edgeBottom->setPen(pen);
    editScene->addItem(edgeBottom);

    barrierC *edgeLeft = new barrierC(0, 0, 1, PLAY_H, playground);

    edgeLeft->setPen(pen);
    editScene->addItem(edgeLeft);

    barrierC *edgeRight = new barrierC(PLAY_W, 0, 1, PLAY_H, playground);
    edgeRight->setPen(pen);
    editScene->addItem(edgeRight);

    barrierC *bar1 = new barrierC(250, 250, 100, 100, playground);
    pen.setColor(Qt::magenta);
    bar1->setPen(pen);
    editScene->addItem(bar1);

    barrierC *bar2 = new barrierC(50, 50, 100, 100, playground);
    bar2->setPen(pen);
    editScene->addItem(bar2);

    Robot *rob1 = new Robot(150, 150, 50, playground);
    QBrush brushRob(Qt::blue);
    brushRob.setColor(Qt::darkMagenta);
    rob1->setBrush(brushRob);

    Robot *rob2 = new Robot(450, 450, 50, playground);
    brushRob.setColor(Qt::yellow);
    rob2->setBrush(brushRob);
    Robot *rob3 = new Robot(450, 100, 50, playground);
    brushRob.setColor(Qt::green);
    rob3->setBrush(brushRob);

    QTimer *timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), rob1, SLOT(move()));
    QObject::connect(timer, SIGNAL(timeout()), rob2, SLOT(move()));
    QObject::connect(timer, SIGNAL(timeout()), rob3, SLOT(move()));
    timer->start(30);
    editScene->addItem(rob1);
    editScene->addItem(rob2);
    editScene->addItem(rob3);
}
