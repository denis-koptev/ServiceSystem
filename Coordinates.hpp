#ifndef COORDINATES_HPP
#define COORDINATES_HPP

#include <QGraphicsScene>

class Coordinates
{
    public:

        Coordinates(double x_from_, double x_to_,
                    double y_from_, double y_to_);

        double get_x_prop() const;
        double get_y_prop() const;

        void draw(QGraphicsScene * scene);

    private:

        double x_prop, y_prop;
        double x_from, x_to,
               y_from, y_to;
};

#endif // COORDINATES_HPP
