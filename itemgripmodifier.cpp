#include "itemgripmodifier.h"
#include "ui_itemgripmodifier.h"

#include <QToolButton>

ItemGripModifier::ItemGripModifier(ItemDB *itemDB, ItemWizard *itemWizard, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemGripModifier)
{
    ui->setupUi(this);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->hide();
    this->itemDB = itemDB;
    this->itemWizard = itemWizard;
    this->item = NULL;
    this->activeGrip = Grip_None;

    connect(this, SIGNAL(rejected()), this, SLOT(slot_rejected()));
}

ItemGripModifier::~ItemGripModifier()
{
    delete ui;
}

void ItemGripModifier::setItem(CADitem *item)
{
    this->item = item;
}

CADitem *ItemGripModifier::getItem()
{
    return this->item;
}

void ItemGripModifier::activateGrip(ItemGripModifier::ItemGripType gripType, QPoint mousePos, QVector3D scenePos)
{
    Q_UNUSED(mousePos);
    this->scenePos = scenePos;
    this->activeGrip = gripType;

    switch (gripType)
    {
    case Grip_None:
        break;
    case Grip_Append:
    {
        showAppendBox();
        break;
    }
    case Grip_Copy:
    {
        break;
    }
    case Grip_CopyMulti:
    {
        showCopyMultiBox();
        break;
    }
    case Grip_Length:
    {
        break;
    }
    case Grip_Move:
    {
        break;
    }
    case Grip_Rotate_X:
    {
        break;
    }
    case Grip_Rotate_Y:
    {
        break;
    }
    case Grip_Rotate_Z:
    {
        break;
    }
    }
}

void ItemGripModifier::finishGrip()
{
    this->item = NULL;
    this->activeGrip = Grip_None;
    hide();
    deleteWdgs(ui->gridLayout);
}

ItemGripModifier::ItemGripType ItemGripModifier::getActiveGrip()
{
    return this->activeGrip;
}

void ItemGripModifier::slot_rejected()
{
    this->item = NULL;
    this->hide();
    deleteWdgs(ui->gridLayout);
}

void ItemGripModifier::slot_button_clicked()
{
    QToolButton* button = (QToolButton*)this->sender();
    CADitem::ItemType type = (CADitem::ItemType)button->property("ItemType").toInt();
    int flangeIndex = this->item->snap_flanges.indexOf(this->scenePos) + 1;

    CADitem* newItem = itemDB->drawItem(this->item->layer->name, type);

    newItem->wizardParams.insert("Position x", QVariant::fromValue((qreal)scenePos.x()));
    newItem->wizardParams.insert("Position y", QVariant::fromValue((qreal)scenePos.y()));
    newItem->wizardParams.insert("Position z", QVariant::fromValue((qreal)scenePos.z()));
    // tbd: add flange angles
    newItem->wizardParams.insert("Angle x", QVariant::fromValue((qreal)this->item->angle_x));
    newItem->wizardParams.insert("Angle y", QVariant::fromValue((qreal)this->item->angle_y));
    newItem->wizardParams.insert("Angle z", QVariant::fromValue((qreal)this->item->angle_z));

    // Copy parameters that are parameters of the flange, but ony if they exist in the both items
    foreach (QString key, newItem->wizardParams.keys())
    {
        if ((key == "a") || (key == "b") || (key == "s") || (key == "fe") || (key == "ff") || (key == "g") || (key == "h"))
        {
            QString sourceKey = key;
            int flangeIndexOffset = 0;

            // Reverse search of indexed parameter in source flange parameters
            do
            {
                if ((flangeIndex + flangeIndexOffset) > 1)
                {
                    sourceKey = key + QString().setNum(flangeIndex + flangeIndexOffset);
                }
                else
                {
                    sourceKey = key;
                    break;
                }
                flangeIndexOffset--;
            }while(!this->item->wizardParams.keys().contains(sourceKey));


            if (this->item->wizardParams.keys().contains(sourceKey))
                newItem->wizardParams.insert(key, this->item->wizardParams.value(sourceKey));
        }
    }

    itemWizard->showWizard(newItem);

    finishGrip();
}

void ItemGripModifier::slot_button_copyMulty()
{
    CADitem* item = this->item;
    CADitem* newItem;
    qreal deltaX;
    qreal deltaY;
    qreal deltaZ;

    for (int x=0; x < copyMulti_spinBox_countX->value(); x++)
    {
        deltaX = (qreal)x * copyMulti_doubleSpinBox_distanceX->value();
        for (int y=0; y < copyMulti_spinBox_countY->value(); y++)
        {
            deltaY = (qreal)y * copyMulti_doubleSpinBox_distanceY->value();
            for (int z=0; z < copyMulti_spinBox_countZ->value(); z++)
            {
                // Do not make an in place copy of the source item
                if ((x == 0) && (y == 0) && (z == 0))
                    continue;

                deltaZ = (qreal)z * copyMulti_doubleSpinBox_distanceZ->value();
                QVector3D pos = item->position + QVector3D(deltaX, deltaY, deltaZ);

                // Copy Item
                newItem = this->itemDB->drawItem(item->layer->name, item->getType());
                newItem->wizardParams = item->wizardParams;
                newItem->wizardParams.insert("Position x", QVariant::fromValue((qreal)pos.x()));
                newItem->wizardParams.insert("Position y", QVariant::fromValue((qreal)pos.y()));
                newItem->wizardParams.insert("Position z", QVariant::fromValue((qreal)pos.z()));
                newItem->processWizardInput();
                newItem->calculate();
            }
        }
    }

    finishGrip();
    emit signal_sceneRepaintNeeded();
}

void ItemGripModifier::deleteWdgs(QLayout *layout)
{
    QLayoutItem *item;
    while ((item = layout->takeAt(0)))
    {
        if (item->layout())
        {
            deleteWdgs(item->layout());
            delete item->layout();
        }
        if (item->widget())
        {
            delete item->widget();
        }
        delete item;
    }

    this->layout()->removeItem(ui->gridLayout);
    ui->gridLayout->deleteLater();
    ui->gridLayout = new QGridLayout(this);
    ((QVBoxLayout*)this->layout())->insertLayout(1, ui->gridLayout);
}

void ItemGripModifier::showAppendBox()
{
    QList<CADitem::ItemType> flangable_items = item->flangable_items();

    deleteWdgs(ui->gridLayout);
    ui->label->setText(tr("Choose new item"));

    int buttonCount = flangable_items.count();
    int columnCount = sqrt(buttonCount);
    int column = 0;
    int row = 0;

    foreach(CADitem::ItemType type, flangable_items)
    {
        QIcon icon = itemDB->getIconByItemType(type, QSize(64, 64));
        QToolButton* button = new QToolButton(this);
        button->setMinimumSize(74, 74);
        button->setMaximumSize(74, 74);
        button->setFocusPolicy(Qt::NoFocus);
        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        button->setIconSize(QSize(64, 64));
        button->setIcon(icon);
        button->setProperty("ItemType", QVariant((int)type));

        connect(button, SIGNAL(clicked()), this, SLOT(slot_button_clicked()));


        ui->gridLayout->addWidget(button, row, column);
        column++;
        if (column == columnCount)
        {
            column = 0;
            row++;
        }
    }

    this->move(QCursor::pos());
    this->show();
}

void ItemGripModifier::showCopyMultiBox()
{
    deleteWdgs(ui->gridLayout);
    ui->label->setText(tr("Multi Copy"));

    copyMulti_spinBox_countX = new QSpinBox(this);
    copyMulti_doubleSpinBox_distanceX = new QDoubleSpinBox(this);
    copyMulti_spinBox_countY = new QSpinBox(this);
    copyMulti_doubleSpinBox_distanceY = new QDoubleSpinBox(this);
    copyMulti_spinBox_countZ = new QSpinBox(this);
    copyMulti_doubleSpinBox_distanceZ = new QDoubleSpinBox(this);

    copyMulti_doubleSpinBox_distanceX->setMinimum(-10e+20);
    copyMulti_doubleSpinBox_distanceX->setMaximum( 10e+20);
    copyMulti_doubleSpinBox_distanceY->setMinimum(-10e+20);
    copyMulti_doubleSpinBox_distanceY->setMaximum( 10e+20);
    copyMulti_doubleSpinBox_distanceZ->setMinimum(-10e+20);
    copyMulti_doubleSpinBox_distanceZ->setMaximum( 10e+20);
    copyMulti_spinBox_countX->setMinimum(1);
    copyMulti_spinBox_countX->setMaximum(INT_MAX);
    copyMulti_spinBox_countY->setMinimum(1);
    copyMulti_spinBox_countY->setMaximum(INT_MAX);
    copyMulti_spinBox_countZ->setMinimum(1);
    copyMulti_spinBox_countZ->setMaximum(INT_MAX);

    ui->gridLayout->addWidget(new QLabel(tr("Count X")),    0, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Distance X")), 1, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Count Y")),    2, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Distance Y")), 3, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Count Z")),    4, 0);
    ui->gridLayout->addWidget(new QLabel(tr("Distance Z")), 5, 0);

    ui->gridLayout->addWidget(copyMulti_spinBox_countX,          0, 1);
    ui->gridLayout->addWidget(copyMulti_doubleSpinBox_distanceX, 1, 1);
    ui->gridLayout->addWidget(copyMulti_spinBox_countY,          2, 1);
    ui->gridLayout->addWidget(copyMulti_doubleSpinBox_distanceY, 3, 1);
    ui->gridLayout->addWidget(copyMulti_spinBox_countZ,          4, 1);
    ui->gridLayout->addWidget(copyMulti_doubleSpinBox_distanceZ, 5, 1);

    QToolButton* button_cancel = new QToolButton(this);
    button_cancel->setText(tr("Cancel"));
    button_cancel->setFocusPolicy(Qt::NoFocus);
    button_cancel->setMinimumSize(150, 40);
    button_cancel->setMaximumSize(200, 40);
    button_cancel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(button_cancel, SIGNAL(clicked()), this, SLOT(slot_rejected()));
    ui->gridLayout->addWidget(button_cancel, 6, 0);

    QToolButton* button_ok = new QToolButton(this);
    button_ok->setText(tr("Ok"));
    button_ok->setFocusPolicy(Qt::NoFocus);
    button_ok->setMinimumSize(150, 40);
    button_ok->setMaximumSize(200, 40);
    button_ok->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(button_ok, SIGNAL(clicked()), this, SLOT(slot_button_copyMulty()));
    ui->gridLayout->addWidget(button_ok, 6, 1);

    this->move(QCursor::pos());
    this->show();
}
