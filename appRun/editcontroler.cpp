#include "editcontroler.h"

void editControler::buildRCREdit(QGraphicsItem *parent, QGraphicsScene *scene)
{
    RCSlot.name = new QGraphicsTextItem(QString("RC Robot"));
    RCSlot.name->setParent(dynamic_cast<QObject *>(parent));
    RCSlot.name->setPos(50, 320);
    RCSlot.name->setDefaultTextColor(Qt::black);

    RCSlot.sensorText = new QGraphicsTextItem(QString("Set sensor lenght"));
    RCSlot.sensorText->setParent(dynamic_cast<QObject *>(parent));
    RCSlot.sensorText->setPos(RCSlot.name->x(), RCSlot.name->y() + 40);
    RCSlot.sensorText->setDefaultTextColor(Qt::black);

    RCSlot.sensorInput = new QLineEdit;
    RCSlot.sensorInput->setPlaceholderText("0-100");
    RCSlot.sensorValidator = new QIntValidator(0, 100);
    RCSlot.sensorInput->setValidator(RCSlot.sensorValidator);
    RCSlot.sensorInput->setGeometry(RCSlot.name->x(), RCSlot.name->y() + 80, 100, 30); // Adjust position and size as needed

    RCSlot.buildRCRobot = new gameButton(QString("build RCRobot"), RCSlot.name->x(), RCSlot.name->y() + 120, 100, 30, parent);

    scene->addItem(RCSlot.name);
    scene->addItem(RCSlot.sensorText);
    scene->addWidget(RCSlot.sensorInput);
    // magic bracho
    scene->addItem(RCSlot.buildRCRobot);
}
void editControler::buildAREdit(QGraphicsItem *parent, QGraphicsScene *scene)
{
    ASlot.name = new QGraphicsTextItem(QString("Automatic Robot"));
    ASlot.name->setParent(dynamic_cast<QObject *>(parent));
    ASlot.name->setPos(50, 0);
    ASlot.name->setDefaultTextColor(Qt::black);

    ASlot.sensorText = new QGraphicsTextItem(QString("Set sensor lenght"));
    ASlot.sensorText->setParent(dynamic_cast<QObject *>(parent));
    ASlot.sensorText->setPos(ASlot.name->x(), ASlot.name->y() + 40);
    ASlot.sensorText->setDefaultTextColor(Qt::black);

    ASlot.sensorInput = new QLineEdit;
    ASlot.sensorInput->setPlaceholderText("0-100");
    ASlot.sensorValidator = new QIntValidator(0, 100);
    ASlot.sensorInput->setValidator(ASlot.sensorValidator);
    ASlot.sensorInput->setGeometry(ASlot.name->x(), ASlot.name->y() + 80, 100, 30); // Adjust position and size as needed

    ASlot.directionText = new QGraphicsTextItem(QString("Set direction"));
    ASlot.directionText->setParent(dynamic_cast<QObject *>(parent));
    ASlot.directionText->setPos(ASlot.name->x(), ASlot.name->y() + 120);
    ASlot.directionText->setDefaultTextColor(Qt::black);

    ASlot.directionInput = new QLineEdit;
    ASlot.directionInput->setPlaceholderText("-1 and 1");
    ASlot.directionValidator = new QIntValidator(-1, 1);
    ASlot.directionInput->setValidator(ASlot.directionValidator);
    ASlot.directionInput->setGeometry(ASlot.name->x(), ASlot.name->y() + 160, 100, 30);

    ASlot.spinText = new QGraphicsTextItem(QString("Set spin size"));
    ASlot.spinText->setParent(dynamic_cast<QObject *>(parent));
    ASlot.spinText->setPos(ASlot.name->x(), ASlot.name->y() + 200);
    ASlot.spinText->setDefaultTextColor(Qt::black);

    ASlot.spinInput = new QLineEdit;
    ASlot.spinInput->setPlaceholderText("10 - 180");
    ASlot.spinValidator = new QIntValidator(10, 180);
    ASlot.spinInput->setValidator(ASlot.spinValidator);
    ASlot.spinInput->setGeometry(ASlot.name->x(), ASlot.name->y() + 240, 100, 30);

    ASlot.buildARobot = new gameButton(QString("build ARobot"), ASlot.name->x(), ASlot.name->y() + 280, 100, 30, parent);

    scene->addItem(ASlot.name);
    scene->addItem(ASlot.sensorText);
    scene->addWidget(ASlot.sensorInput);
    scene->addItem(ASlot.directionText);
    scene->addWidget(ASlot.directionInput);
    scene->addItem(ASlot.spinText);
    scene->addWidget(ASlot.spinInput);
    // magic bracho
    scene->addItem(ASlot.buildARobot);
}
void editControler::buildBAREdit(QGraphicsItem *parent, QGraphicsScene *scene)
{
    BARSlot.name = new QGraphicsTextItem(QString("Edit Barriers"));
    BARSlot.name->setParent(dynamic_cast<QObject *>(parent));
    BARSlot.name->setPos(50, 480);
    BARSlot.name->setDefaultTextColor(Qt::black);
    BARSlot.bar = new barrierC();
    BARSlot.buildBarrier = new gameButton(QString("Open"), BARSlot.name->x(), BARSlot.name->y() + 40, 100, 30, parent);

    scene->addItem(BARSlot.name);
    // magic bracho
    scene->addItem(BARSlot.buildBarrier);
}

void editControler::buildControlsEdit(QGraphicsItem *parent, QGraphicsScene *scene)
{
    bottomSlot.up = new gameButton(QString("up"), RPANEL_W - 130, 10, 50, 50, parent);
    bottomSlot.left = new gameButton(QString("left"), RPANEL_W - 190, 10, 50, 50, parent);
    bottomSlot.right = new gameButton(QString("right"), RPANEL_W - 70, 10, 50, 50, parent);
    scene->addItem(bottomSlot.up);
    scene->addItem(bottomSlot.left);
    scene->addItem(bottomSlot.right);
}

void editControler::buildBarGrid(QGraphicsItem *parent, QGraphicsScene *scene)
{
    if (inBarEdit)
    {
        BARSlot.buildBarrier->changeText("Open");
        deleteBarGrid();
        inBarEdit = false;
        return;
    }
    BARSlot.buildBarrier->changeText("Close");
    inBarEdit = true;
    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            gameButton *tmp = new gameButton(QString("add"), 3 + i * 50, 3 + j * 50, 44, 44, parent);
            scene->addItem(tmp);
            barGrid.push_back(tmp);
            placeBar(tmp);
        }
    }
}

void editControler::placeBar(gameButton *button)
{
    QList<QGraphicsItem *> colliding_items = button->collidingItems();
    bool hit = false;
    QBrush brush(Qt::magenta);
    // if one of the colliding items is an Enemy, destroy both the bullet and the enemy
    for (int i = 0, n = colliding_items.size(); i < n; ++i)
    {
        barrierC *barrierItem = dynamic_cast<barrierC *>(colliding_items[i]);

        // Check if the dynamic_cast was successful and the object is a barrierC or its derived class
        if (barrierItem != nullptr)
        {
            button->changeText("dlt");
            brush.setColor(Qt::magenta);
            button->setBrush(brush);
            return;
        }

        Robot *robotItem = dynamic_cast<Robot *>(colliding_items[i]);

        // Check if the dynamic_cast was successful and the object is a barrierC or its derived class
        if (robotItem != nullptr)
        {
            button->changeText("NO!");

            QBrush brush(Qt::magenta);
            brush.setColor(Qt::red);
            button->setBrush(brush);
            return;
        }
    }
}

void editControler::deleteBarGrid()
{
    for (auto i : barGrid)
    {
        delete i;
    }
    barGrid.clear();
}

void editControler::refresh(QGraphicsScene *scene)
{
    for (auto i : (bottomSlot.robs))
    {
        delete i;
    }
    bottomSlot.robs.clear();
    int offset_x = 0;
    int offset_y = 0;
    int shift = 60;
    for (size_t i = 0; i < bottomSlot.activable.size(); i++)
    {
        string robName = "rob" + to_string(i + 1);
        gameButton *tmp = new gameButton(QString::fromStdString(robName), 10 + offset_x, 10 + offset_y, 50, 50, rPanel);
        bottomSlot.robs.push_back(tmp);
        scene->addItem(tmp);

        if (i % 2)
        {
            offset_y -= shift;
            offset_x += shift;
        }
        else
        {
            offset_y += shift;
        }
    }
}

void editControler::buildPlayEdit(QGraphicsItem *parent, QGraphicsScene *scene)
{
    playSlot.pause = new gameButton(QString("pause"), PPANEL_W - 70, 10, 50, 50, parent);
    playSlot.play = new gameButton(QString("play"), PPANEL_W - 130, 10, 50, 50, parent);
    playSlot.save = new gameButton(QString("save"), PPANEL_W - 190, 10, 50, 50, parent);

    scene->addItem(playSlot.pause);
    scene->addItem(playSlot.play);
    scene->addItem(playSlot.save);
}

editControler::editControler(QGraphicsScene *scene)
{
    inBarEdit = false;
    iPanel = new QGraphicsRectItem(0, 0, IPANEL_W, IPANEL_H);
    iPanel->setPos(IPANEL_X, IPANEL_Y);
    QBrush brush;
    QPen pen;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::white);
    iPanel->setBrush(brush);
    pen.setColor(Qt::white);
    iPanel->setPen(pen);
    scene->addItem(iPanel);

    pPanel = new QGraphicsRectItem(0, 0, PPANEL_W, PPANEL_H);
    pPanel->setPos(PPANEL_X, PPANEL_Y);
    brush.setStyle(Qt::SolidPattern);
    pPanel->setPen(pen);
    pPanel->setBrush(brush);
    scene->addItem(pPanel);

    rPanel = new QGraphicsRectItem(0, 0, RPANEL_W, RPANEL_H);
    rPanel->setPos(RPANEL_X, RPANEL_Y);
    brush.setStyle(Qt::SolidPattern);
    rPanel->setBrush(brush);
    rPanel->setPen(pen);
    scene->addItem(rPanel);

    buildRCREdit(iPanel, scene);
    buildAREdit(iPanel, scene);
    buildBAREdit(iPanel, scene);
    buildControlsEdit(rPanel, scene);
    buildPlayEdit(pPanel, scene);
}
