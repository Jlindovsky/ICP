#ifndef RCROBOT_H
#define RCROBOT_H

#include "robot.h"
#include "global.h"

#define UPARROW 16777235
#define LARROW 16777234
#define RARROW 16777236

/**
 * @brief A class representing a remote control robot.
 *
 * This class inherits from the Robot class and extends its functionality to include
 * methods for controlling the robot's movement and rotation based on user input.
 * @authors Jan Lindovský (xlindo04) Marcel Mravec(xmrave02)
 */
class RCRobot : public Robot
{
public:
    RCRobot(qreal x, qreal y, qreal w, QGraphicsRectItem *parent, int sensorIN);
    RCRobot(qreal x, qreal y, qreal w, QGraphicsRectItem *parent, int sensorIN, int angleIN);
    // void keyPressEvent(QKeyEvent * event)override;
    QJsonObject save() override;
public slots:
    void moveUp();      // override;
    void rotateLeft();  // override;
    void rotateRight(); // override;
};

#endif