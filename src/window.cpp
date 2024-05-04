#include "../header/window.h"

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
    gridOpen = false;
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
    connect(loadGameButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
    welcomeScene->addItem(loadGameButton);
}
void window::setActiveRCR(RCRobot *rob)
{
    qDebug() << "activate";
    QPen peero;
    peero.setColor(Qt::green);
    if (activeRCR != nullptr)
    {
        peero.setWidth(0);
        activeRCR->setPen(peero);
    }
    peero.setWidth(4);
    activeRCR = rob;
    activeRCR->setPen(peero);
}
void window::moveUpActive()
{
    if (activeRCR)
        activeRCR->moveUp();
}
void window::rotateLeftActive()
{
    if (activeRCR)
        activeRCR->rotateLeft();
}
void window::rotateRightActive()
{
    if (activeRCR)
        activeRCR->rotateRight();
}

void window::checkARInput()
{
    auto slot = editBuilder->ASlot;
    auto sensorText = slot.sensorInput->text();
    auto directionText = slot.directionInput->text();
    auto spinText = slot.spinInput->text();

    int pos = 0;
    if (QValidator::Acceptable != slot.sensorValidator->validate(sensorText, pos))
    {
        // popup wrong input
        QMessageBox::information(
            this,
            tr("Wrong Input"),
            tr("ERROR, Wrong input. Sensor length is from 0 to 100"));
        return;
    }
    if (QValidator::Acceptable != slot.directionValidator->validate(directionText, pos))
    {
        // popup wrong input
        QMessageBox::information(
            this,
            tr("Wrong Input"),
            tr("ERROR, Wrong input. Direction is -1 or 1"));
        return;
    }
    if (QValidator::Acceptable != slot.spinValidator->validate(spinText, pos))
    {
        // popup wrong input
        QMessageBox::information(
            this,
            tr("Wrong Input"),
            tr("ERROR, Wrong input. Spin is from 10 to 180"));
        return;
    }
    editBuilder->buildARobot(playground, editScene, SPAWN_X + 25, SPAWN_Y + 25, (sensorText).toInt(), (directionText).toInt(), (spinText).toInt(),timer);
}
void window::checkRCRInput()
{
    auto slot = editBuilder->RCSlot;
    auto sensorText = slot.sensorInput->text();

    int pos = 0;
    if (QValidator::Acceptable != slot.sensorValidator->validate(sensorText, pos))
    {
        // popup wrong input
        QMessageBox::information(
            this,
            tr("Wrong Input"),
            tr("ERROR, Wrong input. Sensor length is from 0 to 100"));
        return;
    }
    editBuilder->buildRCRobot(playground, editScene, SPAWN_X + 25, SPAWN_Y + 25, (sensorText).toInt());
    editBuilder->refresh(editScene);

    for (size_t i = 0; i < editBuilder->rcRobots.size(); i++)
    {
        qDebug()<<"connect robot to button " ;//<< typeid(editBuilder->rcRobots[i]);
        connect(editBuilder->bottomSlot.robs[i], &gameButton::clicked, this, [=]()
                { setActiveRCR(editBuilder->rcRobots[i]); });
    }
}

void window::stopTimer()
{
    timer->stop();
}

void window::startTimer()
{
    if (!gridOpen)
    {
        timer->start(30);
    }
}

void window::clickGrid()
{
    gridOpen = !gridOpen;
    timer->stop();
    editBuilder->buildBarGrid(playground, editScene);
}

void window::saveGame()
{
    if (timer->isActive())
    {
        return;
    }
    QJsonArray jsonArray;

    // Iterate over each barrierC object in BARSlot.bars vector
    for (const barrierC *bar : editBuilder->BARSlot.bars)
    {
        QJsonObject barObject;
        barObject["x"] = bar->x();
        barObject["y"] = bar->y();
        ;
        barObject["type"] = "Barrier";
        jsonArray.append(barObject);
    }

    for (RCRobot *rob : editBuilder->rcRobots)
    {
        jsonArray.append(rob->save());
    }

    for (ARobot *rob : editBuilder->aRobots)
    {
        jsonArray.append(rob->save());
    }

    QJsonDocument jsonDoc(jsonArray);

    QString fileName = "game.json";

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(jsonDoc.toJson());
        file.close();
        qDebug() << "Game barriers data saved to" << fileName;
    }
    else
    {
        qDebug() << "Failed to open" << fileName << "for writing:" << file.errorString();
    }

    qDebug() << "Current working directory:" << QDir::currentPath();
}

void window::loadFromFile()
{

    editWindowSignal();

    QString fileName = "game.json";
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open" << fileName << "for reading:" << file.errorString();
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        qDebug() << "Failed to parse JSON:" << parseError.errorString();
        return;
    }

    if (!jsonDoc.isArray())
    {
        qDebug() << "JSON document is not an array";
        return;
    }

    QJsonArray jsonArray = jsonDoc.array();

    for (const QJsonValue &value : jsonArray)
    {
        QJsonObject obj = value.toObject();
        int x = obj["x"].toInt();
        int y = obj["y"].toInt();
        int sensor = obj["sensor"].toInt();

        int spin = obj["spin"].toInt();
        int directionOfSpin = obj["direction"].toInt();
        QString type = obj["type"].toString();

        if (type == "Barrier")
        {
            editBuilder->buildBar(playground, editScene, x, y);
        }
        else if (type == "RC Robot")
        {
            editBuilder->buildRCRobot(playground, editScene, x, y, sensor);
        }
        else if (type == "Automatic Robot")
        {
            editBuilder->buildARobot(playground, editScene, x, y, sensor, directionOfSpin, spin,timer);
        }
        else
        {
            qDebug() << "wrong type of item";
            exit(EXIT_FAILURE);
        }
    }
    editBuilder->refresh(editScene);

    for (size_t i = 0; i < editBuilder->rcRobots.size(); i++)
    {
        qDebug()<<"connect robot to button " ;//<< typeid(editBuilder->rcRobots[i]);
        connect(editBuilder->bottomSlot.robs[i], &gameButton::clicked, this, [=]()
                { setActiveRCR(editBuilder->rcRobots[i]); });
    }
}

void window::editWindowSignal()
{
    // set up the scene
    editScene = new QGraphicsScene();
    editScene->clear();
    editScene->setSceneRect(0, 0, 1024, 768);
    setScene(editScene);

    playground = new QGraphicsRectItem(0, 0, PLAY_W, PLAY_H);
    playground->setPos(PLAY_X, PLAY_Y);
    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidth(4);
    playground->setPen(pen);
    editScene->addItem(playground);

    editBuilder = new editController(editScene);
    connect(editBuilder->bottomSlot.up, SIGNAL(clicked()), this, SLOT(moveUpActive()));
    connect(editBuilder->bottomSlot.right, SIGNAL(clicked()), this, SLOT(rotateRightActive()));
    connect(editBuilder->bottomSlot.left, SIGNAL(clicked()), this, SLOT(rotateLeftActive()));
    connect(editBuilder->ASlot.buildARobot, SIGNAL(clicked()), this, SLOT(checkARInput()));
    connect(editBuilder->RCSlot.buildRCRobot, SIGNAL(clicked()), this, SLOT(checkRCRInput()));
    connect(editBuilder->BARSlot.buildBarrier, SIGNAL(clicked()), this, SLOT(clickGrid()));
    connect(editBuilder->playSlot.pause, SIGNAL(clicked()), this, SLOT(stopTimer()));
    connect(editBuilder->playSlot.play, SIGNAL(clicked()), this, SLOT(startTimer()));
    connect(editBuilder->playSlot.save, SIGNAL(clicked()), this, SLOT(saveGame()));

    QBrush brush;

    barrierC *edgeTop = new barrierC(0, -1, PLAY_W, 1, playground);
    pen.setColor(Qt::black);
    edgeTop->setPen(pen);
    editScene->addItem(edgeTop);

    barrierC *edgeBottom = new barrierC(0, PLAY_H, PLAY_W, 1, playground);
    edgeBottom->setPen(pen);
    editScene->addItem(edgeBottom);

    barrierC *edgeLeft = new barrierC(-1, 0, 1, PLAY_H, playground);

    edgeLeft->setPen(pen);
    editScene->addItem(edgeLeft);

    barrierC *edgeRight = new barrierC(PLAY_W, 0, 1, PLAY_H, playground);
    edgeRight->setPen(pen);
    editScene->addItem(edgeRight);

    ARobot *Spawn = new ARobot(SPAWN_X, SPAWN_Y, 100, playground, 0, 0, 0);
    editScene->addItem(Spawn);

    timer = new QTimer();

    timer->start(30);
}
