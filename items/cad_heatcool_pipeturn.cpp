#include "cad_heatcool_pipeturn.h"

CAD_heatcool_pipeTurn::CAD_heatcool_pipeTurn() : CADitem(CADitem::HeatCool_PipeTurn)
{
    this->description = "Heat/Cool|Pipe turn";

}

QList<CADitem::ItemType> CAD_heatcool_pipeTurn::flangable_items()
{
    QList<CADitem::ItemType> flangable_items;

    return flangable_items;
}

QImage CAD_heatcool_pipeTurn::wizardImage()
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

void CAD_heatcool_pipeTurn::calculate()
{

}

void CAD_heatcool_pipeTurn::processWizardInput()
{

}
