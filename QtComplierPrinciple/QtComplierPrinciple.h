#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtComplierPrinciple.h"

class QtComplierPrinciple : public QMainWindow
{
    Q_OBJECT

public:
    QtComplierPrinciple(QWidget *parent = nullptr);
    ~QtComplierPrinciple();

private:
    Ui::QtComplierPrincipleClass ui;
};
