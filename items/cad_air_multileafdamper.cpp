/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

#include "cad_air_multileafdamper.h"

CAD_air_multiLeafDamper::CAD_air_multiLeafDamper() : CADitem(CADitemTypes::Air_MultiLeafDamper)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a",  200.0);
    wizardParams.insert("b",  300.0);
    wizardParams.insert("l", 1000.0);
    wizardParams.insert("fe",  10.0);
    wizardParams.insert("ff",  10.0);
    wizardParams.insert("s",    1.0);
    wizardParams.insert("n",   10);


    main_duct = new CAD_basic_duct();
    flange_duct_left = new CAD_basic_duct();
    flange_duct_right = new CAD_basic_duct();
    function = new CAD_basic_box();

    subItems.append(main_duct);
    subItems.append(flange_duct_left);
    subItems.append(flange_duct_right);
    subItems.append(function);

    processWizardInput();
    calculate();
}

CAD_air_multiLeafDamper::~CAD_air_multiLeafDamper()
{

}

QList<CADitemTypes::ItemType> CAD_air_multiLeafDamper::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Air_CanvasFlange);
    flangable_items.append(CADitemTypes::Air_Duct);
    flangable_items.append(CADitemTypes::Air_DuctEndPlate);
    flangable_items.append(CADitemTypes::Air_DuctBaffleSilencer);
    flangable_items.append(CADitemTypes::Air_DuctFireDamper);
    flangable_items.append(CADitemTypes::Air_DuctTeeConnector);
    flangable_items.append(CADitemTypes::Air_DuctTransitionRectRect);
    flangable_items.append(CADitemTypes::Air_DuctTransitionRectRound);
    flangable_items.append(CADitemTypes::Air_DuctTurn);
    flangable_items.append(CADitemTypes::Air_DuctVolumetricFlowController);
    flangable_items.append(CADitemTypes::Air_DuctYpiece);
    flangable_items.append(CADitemTypes::Air_Filter);
    flangable_items.append(CADitemTypes::Air_HeatExchangerAirAir);
    flangable_items.append(CADitemTypes::Air_HeatExchangerWaterAir);
    flangable_items.append(CADitemTypes::Air_MultiLeafDamper);
    return flangable_items;
}

QImage CAD_air_multiLeafDamper::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_air_multiLeafDamper::iconPath()
{
    return ":/icons/cad_air/cad_air_multileafdamper.svg";
}

QString CAD_air_multiLeafDamper::domain()
{
    return "Air";
}

QString CAD_air_multiLeafDamper::description()
{
    return "Air|Mulit leaf damper";
}

void CAD_air_multiLeafDamper::calculate()
{
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    this->subItems.clear();
    subItems.append(main_duct);
    subItems.append(flange_duct_left);
    subItems.append(flange_duct_right);
    subItems.append(function);

    main_duct->wizardParams.insert("Position x", (position.x()));
    main_duct->wizardParams.insert("Position y", (position.y()));
    main_duct->wizardParams.insert("Position z", (position.z()));
    main_duct->wizardParams.insert("Angle x", (angle_x));
    main_duct->wizardParams.insert("Angle y", (angle_y));
    main_duct->wizardParams.insert("Angle z", (angle_z));
    main_duct->wizardParams.insert("l", (l));
    main_duct->wizardParams.insert("b", (b));
    main_duct->wizardParams.insert("a", (a));
    main_duct->wizardParams.insert("s", (s));
    main_duct->processWizardInput();
    main_duct->calculate();

    QVector3D position_fdl = position + matrix_rotation * QVector3D(l - fe, 0.0, 0.0);
    flange_duct_left->wizardParams.insert("Position x", (position_fdl.x()));
    flange_duct_left->wizardParams.insert("Position y", (position_fdl.y()));
    flange_duct_left->wizardParams.insert("Position z", (position_fdl.z()));
    flange_duct_left->wizardParams.insert("Angle x", (angle_x));
    flange_duct_left->wizardParams.insert("Angle y", (angle_y));
    flange_duct_left->wizardParams.insert("Angle z", (angle_z));
    flange_duct_left->wizardParams.insert("l", (fe));
    flange_duct_left->wizardParams.insert("b", (b + 2 * ff));
    flange_duct_left->wizardParams.insert("a", (a + 2 * ff));
    flange_duct_left->wizardParams.insert("s", (ff));
    flange_duct_left->processWizardInput();
    flange_duct_left->matrix_rotation = this->matrix_rotation;
    flange_duct_left->calculate();

    flange_duct_right->wizardParams.insert("Position x", (position.x()));
    flange_duct_right->wizardParams.insert("Position y", (position.y()));
    flange_duct_right->wizardParams.insert("Position z", (position.z()));
    flange_duct_right->wizardParams.insert("Angle x", (angle_x));
    flange_duct_right->wizardParams.insert("Angle y", (angle_y));
    flange_duct_right->wizardParams.insert("Angle z", (angle_z));
    flange_duct_right->wizardParams.insert("l", (fe));
    flange_duct_right->wizardParams.insert("b", (b + 2 * ff));
    flange_duct_right->wizardParams.insert("a", (a + 2 * ff));
    flange_duct_right->wizardParams.insert("s", (ff));
    flange_duct_right->processWizardInput();
    flange_duct_right->calculate();

    QVector3D position_function = position + matrix_rotation * QVector3D(l/2, b * 0.6, 0.0);
    function->wizardParams.insert("Position x", position_function.x());
    function->wizardParams.insert("Position y", position_function.y());
    function->wizardParams.insert("Position z", position_function.z());
    function->wizardParams.insert("Angle x", angle_x);
    function->wizardParams.insert("Angle y", angle_y);
    function->wizardParams.insert("Angle z", angle_z);

    function->wizardParams.insert("l", l / 3);
    function->wizardParams.insert("b", b * 0.2);
    function->wizardParams.insert("a", a / 3);

    function->processWizardInput();
    function->layer = this->layer;
    function->calculate();
    this->boundingBox.enterVertices(function->boundingBox.getVertices());

    for(int i = 0; i < n; i++)
    {
        CAD_basic_box *box = new CAD_basic_box();
        this->subItems.append(box);
        QVector3D position_box = position + matrix_rotation * QVector3D(l/2, 0.0, (a - 2*s) * (0.5 - (float)i / (float)n - 0.5 / n));
        box->wizardParams.insert("Position x", position_box.x());
        box->wizardParams.insert("Position y", position_box.y());
        box->wizardParams.insert("Position z", position_box.z());
        box->wizardParams.insert("Angle x", angle_x);
        box->wizardParams.insert("Angle y", angle_y);
        box->wizardParams.insert("Angle z", angle_z);

        box->wizardParams.insert("l", (a - 2*s) / 4 / n);
        box->wizardParams.insert("b", b - 2*s);
        box->wizardParams.insert("a", (a - 2*s) / n);

        box->processWizardInput();
        box->rotateAroundAxis(-45.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
        box->layer = this->layer;
        box->calculate();
        this->boundingBox.enterVertices(box->boundingBox.getVertices());
    }

    boundingBox.enterVertex(flange_duct_left->pos_top_1);
    boundingBox.enterVertex(flange_duct_left->pos_top_4);
    boundingBox.enterVertex(flange_duct_left->pos_bot_1);
    boundingBox.enterVertex(flange_duct_left->pos_bot_4);

    boundingBox.enterVertex(flange_duct_right->pos_top_2);
    boundingBox.enterVertex(flange_duct_right->pos_top_3);
    boundingBox.enterVertex(flange_duct_right->pos_bot_2);
    boundingBox.enterVertex(flange_duct_right->pos_bot_3);

    this->snap_vertices.append(main_duct->snap_vertices);
    this->snap_flanges.append(main_duct->snap_flanges);
}

void CAD_air_multiLeafDamper::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    s = wizardParams.value("s").toDouble();
    l = wizardParams.value("l").toDouble();
    b = wizardParams.value("b").toDouble();
    a = wizardParams.value("a").toDouble();
    ff = wizardParams.value("ff").toDouble();
    fe = wizardParams.value("fe").toDouble();
    n = wizardParams.value("n").toInt();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);
}

QMatrix4x4 CAD_air_multiLeafDamper::rotationOfFlange(quint8 num)
{
    if(num == 1)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else if(num == 2)
    {
        return matrix_rotation;
    }
    else
        return matrix_rotation;
}
