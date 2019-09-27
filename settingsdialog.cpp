#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "settings.h"

#include <QSqlQuery>
#include <QSqlResult>
#include "savetodbdialog.h"
#include "loadfromdb.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ui->KernelRadiusBox->setValue(Settings::kernelRadius);
    int index = 0;
    if(Settings::filter == Settings::Filters::Gaus)
        index = 1;
    else if (Settings::filter == Settings::Filters::Sobel)
        index = 2;
    ui->FilterBox->setCurrentIndex(index);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_Ok_button_clicked()
{
    Settings::kernelRadius = ui->KernelRadiusBox->value();
    //auto selectedFilter = ui->comboBox->itemData(ui->comboBox->currentIndex()).toString();
    switch (ui->FilterBox->currentIndex()) {
    case 0:{
        Settings::filter = Settings::Filters::None;
        break;
    }
    case 1:{
        Settings::filter = Settings::Filters::Gaus;
        break;
    }
    case 2:{
        Settings::filter = Settings::Filters::Sobel;
        break;
    }
    default:
        Settings::filter = Settings::Filters::None;
    }
    this->close();
}

void SettingsDialog::on_SaveToDbButton_clicked()
{
    SaveToDbDialog* saveToDb = new SaveToDbDialog(this);
    connect(saveToDb, SIGNAL(saveToDb(QString, QString)),
            this, SLOT(saveToDb(QString, QString)));
    saveToDb->show();
    saveToDb->exec();
}

void SettingsDialog::on_LoadFromDbButton_clicked()
{
    auto db = static_cast<MainWindow*>(parent())->getDb();
    QSqlQuery *query = new QSqlQuery(db);
    query->exec("Select Id, Configurations.Name, Description, Configurations.AttrId, Value from Configurations "
                "join Attributes on Configurations.AttrId = Attributes.AttrId");
    QVector<QVector<QVariant>> configurations;
    while(query->next()){
        int i = 0;
        for (int j = 0; j < 5; j++) {
            configurations[i][j] = query->value(j);
        }
        i++;
    }
   // Settings::kernelRadius = query->value(0).toInt();
    //ui->KernelRadiusBox->setValue(Settings::kernelRadius);

    LoadFromDb* loadFromDb = new LoadFromDb(this);
    loadFromDb->setInfoForList(configurations);
    loadFromDb->show();
}

void SettingsDialog::saveToDb(QString name, QString desc)
{
    auto db = static_cast<MainWindow*>(parent())->getDb();
    QSqlQuery *query = new QSqlQuery(db);
    query->prepare("Insert into Attributes (Value, Name) values(?, ?)");
    query->bindValue(1, QString::number(ui->KernelRadiusBox->value()));
    query->bindValue(2, "radius");
    query->exec();
    query->exec("Select max(AttrId) from Attributes");
    query->next();
    int maxAttrId = query->value(0).toInt();
    query->prepare("Insert into Configurations (Name, Description, AttrId) values (?, ?, ?)");
    query->bindValue(1, name);
    query->bindValue(2, desc);
    query->bindValue(3, maxAttrId);
}
