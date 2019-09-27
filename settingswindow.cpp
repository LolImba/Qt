#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "settings.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_spinBox_valueChanged(int radius)
{
    Settings::kernelRadius = radius;
}
