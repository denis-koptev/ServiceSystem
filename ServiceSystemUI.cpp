#include "ServiceSystemUI.hpp"
#include "ui_ServiceSystemUI.h"
#include "Coordinates.hpp"
#include <QMessageBox>
#include <iostream>
#include <ctime>
#include <cstdlib>

ServiceSystemUI::ServiceSystemUI(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::ServiceSystemUI),
    dynamicSS(0, 0, 0, 0, 0)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentWidget(ui->mainTab);
    dynamicSS = ServiceSystem(ui->srcAmountText->text().toUInt(),
                              ui->buffSizeText->text().toUInt(),
                              ui->rcvAmountText->text().toUInt(),
                              ui->srcLambda->text().toDouble(),
                              ui->rcvLambda->text().toDouble());
    dynamicUpdated = true;
}

ServiceSystemUI::~ServiceSystemUI()
{
    delete ui;
}

void ServiceSystemUI::on_automaticButton_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->automaticTab);
}

void ServiceSystemUI::on_dynamicButton_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->dynamicTab);
}

void ServiceSystemUI::on_mainButton_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->mainTab);
}

void ServiceSystemUI::on_dynamicButton_2_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->dynamicTab);
}

void ServiceSystemUI::on_mainButton_2_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->mainTab);
}

void ServiceSystemUI::on_automaticButton_2_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->automaticTab);
}

void ServiceSystemUI::on_aboutButton_clicked()
{
    QMessageBox::about(this, "About", "Service System\nAuthor: Koptev Denis, Saint-Petersburg, 2016");
}

void ServiceSystemUI::on_startAutoButton_clicked()
{
    QGraphicsScene * scene = new QGraphicsScene(ui->graphicsView);
    QPen pen(Qt::red);
    pen.setWidth(2);

    ServiceSystem::Results res;

    double p0 = 0, p1 = 0, diff = 0, precision = 0.1;
    int N = 10;

    struct result
    {
        double x, y;
        result()
            : x(0), y(0)
        {}
        result(double x_, double y_)
            : x(x_), y(y_)
        {}
    };

    std::vector<result> results;

    ui->max_y->setText("1");

    do
    {
        ServiceSystem s_system(ui->srcAmountText->text().toUInt(),
                               ui->buffSizeText->text().toUInt(),
                               ui->rcvAmountText->text().toUInt(),
                               ui->srcLambda->text().toDouble(),
                               ui->rcvLambda->text().toDouble());
        p0 = p1;
        //scene->clear();
        Coordinates crds(0, N, 0, 1);
        crds.draw(scene);
        pen.setColor(QColor(255 * N / 11000, 255 - 255 * N / 11000, 0));
        int step = std::round(N / 300);
        if (step == 0) {
            step = 1;
        }
        for (int i = 0; i < N; ++i)
        {
            s_system.make_step();
            ServiceSystem::Results lres = s_system.get_results();
            double p = 0;
            switch (ui->graphList->currentIndex())
            {
                case 0:
                {
                    p = 1. - ((double)lres.accepted / (double)lres.generated);
                    break;
                }
                case 1:
                {
                    p = lres.downtime / (ui->rcvAmountText->text().toUInt() * lres.final_time);
                    break;
                }
            }

            int px = std::round(i * crds.get_x_prop());
            int py = std::round(p * crds.get_y_prop());
            if (i % step == 0) {
                scene->addLine(px, py, px, py, pen);
            }
        }
        ui->graphicsView->setScene(scene);
        ui->graphicsView->update();

        res = s_system.get_results();
        switch (ui->graphList->currentIndex())
        {
            case 0:
            {
                p1 = 1. - ((double)res.accepted / (double)res.generated);
                break;
            }
            case 1:
            {
                p1 = res.downtime / (ui->rcvAmountText->text().toUInt() * res.final_time);
                break;
            }
        }
        results.push_back(result(N, p1));

        std::cout << "N = " << N << std::endl;
        std::cout << "P_0 = " << p0 << std::endl;
        std::cout << "P_1 = " << p1 << std::endl;
        if (p1 == 0) {
            break;
        }
        N = std::round((1.643 * 1.643 * (1 - p1)) / (p1 * precision * precision));
        if (N > 20000) {
            N = 20000;
        }
        std::cout << "New N = " << N << std::endl;
        diff = std::abs(p0 - p1);
    }
    while (diff >= precision * p0);

    Coordinates crds(0, N, 0, 1);
    pen.setWidth(10);
    int px = std::round(N * crds.get_x_prop());
    int py = std::round(p1 * crds.get_y_prop());
    scene->addLine(px, py, px, py, pen);
    scene->addLine(0, py, 0, py, pen);
    ui->graphicsView->setScene(scene);
    ui->max_x->setText(QString::number(N));

    ui->finalValue->setText("Refusal: " +
                            QString::number(1. - (double)res.accepted / (double)res.generated)
                            + "\nError: " + QString::number(std::abs(p0 - p1))
                            + "\nDowntime: " +
                            QString::number(res.downtime / (ui->rcvAmountText->text().toUInt()*res.final_time))
                            + "\nFinal time: " + QString::number(res.final_time));

    ui->finalValue->setStyleSheet("color: rgb(" + QString::number(pen.color().red())
                                  + "," + QString::number(pen.color().green())
                                  + "," + QString::number(pen.color().blue()) + ");");
}

void ServiceSystemUI::on_nextStepButton_clicked()
{
    static Request min_rq, buff_rq;
    static uint min_pos = 0, curr_pos = 0, pop_num = 0, rcvs_busy = 0;
    static double min_time = 0;
    static uint step_num = 0;

    if (!dynamicUpdated)
    {
        min_pos = 0;
        curr_pos = 0;
        pop_num = 0;
        rcvs_busy = 0;
        min_time = 0;
        step_num = 0;
        dynamicUpdated = true;
    }

    switch (step_num)
    {
        case 0:
        {
            min_rq = std::min_element(dynamicSS.sources.begin(), dynamicSS.sources.end(),
                                      [](const Source & lhs, const Source & rhs)
            {
                return (lhs.get_request().get_time_in() < rhs.get_request().get_time_in());
            }
                                     )->get_request();

            min_pos = min_rq.get_src_num();
            min_time = min_rq.get_time_in();

            ui->statusLabel->setText("Earliest request search");
            ui->srcStatusMessage->setText("Request #" + QString::number(min_rq.get_src_num()) + "."
                                          + QString::number(min_rq.get_rq_num()) + " found with time: "
                                          + QString::number(min_rq.get_time_in()));
            ui->srcStatusMessage->setStyleSheet("color: rgb(0,255,0); background-color: rgb(58,58,58)");
            ui->buffStatusMessage->setStyleSheet("color: rgb(150,150,150); background-color: rgb(58,58,58)");
            ui->rcvStatusMessage->setStyleSheet("color: rgb(150,150,150); background-color: rgb(58,58,58)");
            break;
        }

        case 1:
        {
            dynamicSS.sources.at(min_pos).generate();
            Request gen_rq = dynamicSS.sources.at(min_pos).get_request();

            ui->statusLabel->setText("Request generation");
            ui->srcStatusMessage->setText("Request #" + QString::number(gen_rq.get_src_num()) + "."
                                          + QString::number(gen_rq.get_rq_num()) + " generated at time: "
                                          + QString::number(gen_rq.get_time_in()));
            ui->srcStatusMessage->setStyleSheet("color: rgb(0,255,0); background-color: rgb(58,58,58)");
            ui->buffStatusMessage->setStyleSheet("color: rgb(150,150,150); background-color: rgb(58,58,58)");
            ui->rcvStatusMessage->setStyleSheet("color: rgb(150,150,150); background-color: rgb(58,58,58)");
            break;
        }

        case 2:
        {
            QString message = "Requests in buffer: " +
                              QString::number(dynamicSS.buffer.get_current_size()) +
                              "\nTaken from buffer: \n";

            rcvs_busy = 0;
            curr_pos = dynamicSS.rcv_pos;
            while (dynamicSS.buffer.get_current_size() != 0 &&
                    rcvs_busy != dynamicSS.receivers.size())
            {
                if (!dynamicSS.receivers.at(curr_pos).is_busy(min_time))
                {
                    buff_rq = dynamicSS.buffer.get();

                    message += "#" + QString::number(buff_rq.get_rq_num()) +
                               "." + QString::number(buff_rq.get_src_num()) + " (time: "
                               + QString::number(buff_rq.get_time_in()) + "; rcv #"
                               + QString::number(curr_pos) + ")\n";

                    dynamicSS.receivers.at(curr_pos).push(min_time);
                    dynamicSS.sources.at(buff_rq.get_src_num()).accept();
                    dynamicSS.rcv_pos = dynamicSS.next_pos(curr_pos, (uint)dynamicSS.receivers.size());
                }
                curr_pos = dynamicSS.next_pos(curr_pos, (uint)dynamicSS.receivers.size());

                rcvs_busy = 0;
                std::for_each(dynamicSS.receivers.begin(), dynamicSS.receivers.end(),
                              [](const Receiver & rcv)
                {
                    if (rcv.is_busy(min_time))
                    {
                        ++rcvs_busy;
                    }
                });
            }
            dynamicSS.rcv_pos = curr_pos;

            for (int i = 0; i < dynamicSS.receivers.size(); ++i)
            {
                dynamicSS.receivers.at(i).upd_downtime(min_time);
            }

            ui->statusLabel->setText("Buffer extraction");
            ui->buffStatusMessage->setText(message + "Requests in buffer: " +
                                           QString::number(dynamicSS.buffer.get_current_size()) +
                                           "\nBusy receivers: " + QString::number(rcvs_busy));
            ui->srcStatusMessage->setStyleSheet("color: rgb(150,150,150); background-color: rgb(58,58,58)");
            ui->buffStatusMessage->setStyleSheet("color: rgb(0,255,0); background-color: rgb(58,58,58)");
            ui->rcvStatusMessage->setStyleSheet("color: rgb(150,150,150); background-color: rgb(58,58,58)");
            break;
        }

        case 3:
        {
            ui->statusLabel->setText("Pushing to buffer");
            QString message = "Trying to push request #" +
                              QString::number(min_rq.get_src_num()) + "." +
                              QString::number(min_rq.get_rq_num()) + "\n";

            bool pushed = dynamicSS.buffer.push(min_rq);
            if (!pushed)
            {
                message += "Failed to push. Buffer is busy.\n";
                pop_num = dynamicSS.buffer.pop();
                message += "Request from source #" + QString::number(pop_num) + " was popped.\n";
                dynamicSS.sources.at(pop_num).decline();
                dynamicSS.buffer.push(min_rq);
            }
            message += "Successfully pushed";
            ui->rcvStatusMessage->setText(message);
            ui->srcStatusMessage->setStyleSheet("color: rgb(150,150,150); background-color: rgb(58,58,58)");
            ui->buffStatusMessage->setStyleSheet("color: rgb(150,150,150); background-color: rgb(58,58,58)");
            ui->rcvStatusMessage->setStyleSheet("color: rgb(0,255,0); background-color: rgb(58,58,58)");
            break;
        }

        case 4:
        {
            ui->statusLabel->setText("Current parameters");
            QString src_message = "From sources:\n";
            QString rcv_message = "From receivers:\n";
            QString buff_message = "Inside:\n";
            for (uint i = 0; i < dynamicSS.sources.size(); ++i)
            {
                src_message += "Source #" + QString::number(i) + ":\n   accepted = "
                               + QString::number(dynamicSS.sources.at(i).get_accepted()) + ";\n    declined = "
                               + QString::number(dynamicSS.sources.at(i).get_declined()) + ";\n";
            }
            for (uint i = 0; i < dynamicSS.receivers.size(); ++i)
            {
                rcv_message += "Source #" + QString::number(i) + ":\n   downtime = "
                               + QString::number(dynamicSS.receivers.at(i).get_downtime())
                               + "\n   worktime = " + QString::number(dynamicSS.receivers.at(i).get_worktime()) + ";\n";
            }
            for (uint i = 0; i < dynamicSS.buffer.get_current_size(); ++i)
            {
                buff_message += "#" + QString::number(dynamicSS.buffer.at(i).get_src_num())
                                + "." + QString::number(dynamicSS.buffer.at(i).get_rq_num()) + "\n";
            }
            ui->srcStatusMessage->setText(src_message);
            ui->rcvStatusMessage->setText(rcv_message);
            ui->buffStatusMessage->setText(buff_message);
            ui->srcStatusMessage->setStyleSheet("color: rgb(0,255,0); background-color: rgb(58,58,58)");
            ui->buffStatusMessage->setStyleSheet("color: rgb(0,255,0); background-color: rgb(58,58,58)");
            ui->rcvStatusMessage->setStyleSheet("color: rgb(0,255,0); background-color: rgb(58,58,58)");
            break;
        }

        default:
            step_num = -1;

    }

    ++step_num;
    if (step_num > 4) {
        step_num = 0;
    }
    std::cout << "step_num: " << step_num << std::endl;

}

void ServiceSystemUI::on_updateButton_clicked()
{
    dynamicSS = ServiceSystem(ui->srcAmountText->text().toUInt(),
                              ui->buffSizeText->text().toUInt(),
                              ui->rcvAmountText->text().toUInt(),
                              ui->srcLambda->text().toDouble(),
                              ui->rcvLambda->text().toDouble());
    dynamicUpdated = false;

}
