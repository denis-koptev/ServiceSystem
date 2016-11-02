#ifndef SERVICESYSTEMUII_HPP
#define SERVICESYSTEMUII_HPP

#include <QMainWindow>
#include "ServiceSystem.hpp"

namespace Ui {
class ServiceSystemUI;
}

class ServiceSystemUI : public QMainWindow
{
        Q_OBJECT

    public:
        explicit ServiceSystemUI(QWidget * parent = 0);
        ~ServiceSystemUI();

    private slots:
        void on_automaticButton_clicked();

        void on_dynamicButton_clicked();

        void on_mainButton_clicked();

        void on_dynamicButton_2_clicked();

        void on_mainButton_2_clicked();

        void on_automaticButton_2_clicked();

        void on_aboutButton_clicked();

        void on_startAutoButton_clicked();

        void on_nextStepButton_clicked();

        void on_updateButton_clicked();

    private:
        Ui::ServiceSystemUI * ui;
        ServiceSystem dynamicSS;
        bool dynamicUpdated;
};

#endif // SERVICESYSTEMUII_HPP
