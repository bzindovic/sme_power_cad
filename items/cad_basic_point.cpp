#include "cad_basic_point.h"

CAD_basic_point::CAD_basic_point() : CADitem(CADitem::Basic_Point)
{
    this->description = "Basic|Point";

    wizardParams.insert(QObject::tr("Position x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Position y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Position z"), QVariant::fromValue(0.0));
}

QList<CADitem::ItemType> CAD_basic_point::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_basic_point::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    qDebug() << imageFileName;

    image.load(imageFileName, "PNG");

    return image;
}

void CAD_basic_point::calculate()
{

}

void CAD_basic_point::processWizardInput()
{
    position.setX(wizardParams.value(QObject::tr("Position x")).toDouble());
    position.setY(wizardParams.value(QObject::tr("Position y")).toDouble());
    position.setZ(wizardParams.value(QObject::tr("Position z")).toDouble());
}
