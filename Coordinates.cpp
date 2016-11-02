#include "Coordinates.hpp"
#include <iostream>
#include <QThread>

Coordinates::Coordinates(double x_from_, double x_to_,
                         double y_from_, double y_to_)
    : x_from(x_from_), x_to(x_to_),
      y_from(y_from_), y_to(y_to_)
{
    x_prop = 970 / (x_to - x_from);
    y_prop = -590 / (y_to - y_from);
}

void Coordinates::draw(QGraphicsScene * scene)
{
    int y_size = 590;

    double x_step = (x_to - x_from) / 20;
    double y_step = (y_to - y_from) / 10;

    QPen pen(Qt::white);
    pen.setWidth(1);

    scene->addLine(0, (y_from * y_prop), 0, (y_to * y_prop), pen);
    scene->addLine((x_from * x_prop), 0, (x_to * x_prop), 0, pen);

    int actual_y_pos = y_size;
    for (double y = y_from; y <= y_to; y += y_step)
    {
        pen.setWidth(1);
        actual_y_pos = y * y_prop;
        scene->addLine(0, actual_y_pos, 5, actual_y_pos, pen);
    }

    int actual_x_pos = 0;
    for (double x = x_from; x <= x_to; x += x_step)
    {
        actual_x_pos = x * x_prop;
        pen.setWidth(1);
        scene->addLine(actual_x_pos, 0, actual_x_pos, -5, pen);
    }
}

double Coordinates::get_x_prop() const
{
    return x_prop;
}

double Coordinates::get_y_prop() const
{
    return y_prop;
}
