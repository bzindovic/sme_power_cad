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

#include "cad_heatcool_pipeendcap.h"

#include "glwidget.h"

CAD_heatcool_pipeEndCap::CAD_heatcool_pipeEndCap() : CADitem(CADitemTypes::HeatCool_PipeEndCap)
{
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);
    wizardParams.insert("d", 150.0);
    wizardParams.insert("l", 100.0);
    wizardParams.insert("s",  10.0);
    wizardParams.insert("fe", 10.0);
    wizardParams.insert("ff", 10.0);

    flange = new CAD_basic_pipe();
    endcap_outer = new CAD_Basic_TorisphericalHeadDIN28011();
    endcap_inner = new CAD_Basic_TorisphericalHeadDIN28011();
    this->subItems.append(endcap_outer);
    this->subItems.append(endcap_inner);
    this->subItems.append(flange);

    arrayBufVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    arrayBufVertices->create();
    arrayBufVertices->setUsagePattern(QOpenGLBuffer::StaticDraw);

    indexBufFaces = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBufFaces->create();
    indexBufFaces->setUsagePattern(QOpenGLBuffer::StaticDraw);

    processWizardInput();
    calculate();
}

CAD_heatcool_pipeEndCap::~CAD_heatcool_pipeEndCap()
{

}

QList<CADitemTypes::ItemType> CAD_heatcool_pipeEndCap::flangable_items(int flangeIndex)
{
    Q_UNUSED(flangeIndex);
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::HeatCool_Adjustvalve);
    flangable_items.append(CADitemTypes::HeatCool_BallValve);
    flangable_items.append(CADitemTypes::HeatCool_Boiler);
    flangable_items.append(CADitemTypes::HeatCool_ButterflyValveBolted);
    flangable_items.append(CADitemTypes::HeatCool_ButterflyValveClamped);
    flangable_items.append(CADitemTypes::HeatCool_Chiller);
    flangable_items.append(CADitemTypes::HeatCool_Controlvalve);
    flangable_items.append(CADitemTypes::HeatCool_CoolingTower);
    flangable_items.append(CADitemTypes::HeatCool_DirtArrester);
    flangable_items.append(CADitemTypes::HeatCool_ExpansionChamber);
    flangable_items.append(CADitemTypes::HeatCool_Filter);
    flangable_items.append(CADitemTypes::HeatCool_Flange);
    flangable_items.append(CADitemTypes::HeatCool_Flowmeter);
    flangable_items.append(CADitemTypes::HeatCool_Gauge);
    flangable_items.append(CADitemTypes::HeatCool_Gauge90Degree);
    flangable_items.append(CADitemTypes::HeatCool_HeatexchangerSoldered);
    flangable_items.append(CADitemTypes::HeatCool_HeatexchangerBolted);
    flangable_items.append(CADitemTypes::HeatCool_NonReturnFlap);
    flangable_items.append(CADitemTypes::HeatCool_NonReturnValve);
    flangable_items.append(CADitemTypes::HeatCool_Pipe);
    flangable_items.append(CADitemTypes::HeatCool_PipeEndCap);
    flangable_items.append(CADitemTypes::HeatCool_PipeReducer);
    flangable_items.append(CADitemTypes::HeatCool_PipeTeeConnector);
    flangable_items.append(CADitemTypes::HeatCool_PipeTurn);
    flangable_items.append(CADitemTypes::HeatCool_PumpInline);
    flangable_items.append(CADitemTypes::HeatCool_PumpNorm);
    flangable_items.append(CADitemTypes::HeatCool_Radiator);
    flangable_items.append(CADitemTypes::HeatCool_RadiatorCompact);
    flangable_items.append(CADitemTypes::HeatCool_RadiatorFlange);
    flangable_items.append(CADitemTypes::HeatCool_RadiatorFlangeBent);
    flangable_items.append(CADitemTypes::HeatCool_RadiatorValve);
    flangable_items.append(CADitemTypes::HeatCool_SafetyValve);
    flangable_items.append(CADitemTypes::HeatCool_Sensor);
    flangable_items.append(CADitemTypes::HeatCool_StorageBoiler);
    flangable_items.append(CADitemTypes::HeatCool_Valve);
    flangable_items.append(CADitemTypes::HeatCool_Valve90Degree);
    flangable_items.append(CADitemTypes::HeatCool_WaterHeater);
    return flangable_items;
}

QImage CAD_heatcool_pipeEndCap::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");
    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_heatcool_pipeEndCap::iconPath()
{
    return ":/icons/cad_heatcool/cad_heatcool_pipeendcap.svg";
}

QString CAD_heatcool_pipeEndCap::domain()
{
    return "HeatCool";
}

QString CAD_heatcool_pipeEndCap::description()
{
    return "HeatCool|Pipe endcap";
}

void CAD_heatcool_pipeEndCap::calculate()
{
    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);

    flange->wizardParams.insert("Position x", position.x());
    flange->wizardParams.insert("Position y", position.y());
    flange->wizardParams.insert("Position z", position.z());
    flange->wizardParams.insert("Angle x", angle_x);
    flange->wizardParams.insert("Angle y", angle_y);
    flange->wizardParams.insert("Angle z", angle_z);
    flange->wizardParams.insert("l", fe);
    flange->wizardParams.insert("d", d + 2 * ff);
    flange->wizardParams.insert("s",  ff);
    flange->layer = this->layer;
    flange->processWizardInput();
    flange->calculate();

    endcap_outer->wizardParams.insert("Position x", position.x());
    endcap_outer->wizardParams.insert("Position y", position.y());
    endcap_outer->wizardParams.insert("Position z", position.z());
    endcap_outer->wizardParams.insert("Angle x", angle_x);
    endcap_outer->wizardParams.insert("Angle y", angle_y);
    endcap_outer->wizardParams.insert("Angle z", angle_z);
    endcap_outer->wizardParams.insert("d", d);   // Durchmesser
    qreal h = l - 0.1937742252 * d; // h - (1-sqrt(0.65)) * d
    endcap_outer->wizardParams.insert("h", h);     // Höhe
    endcap_outer->layer = this->layer;
    endcap_outer->processWizardInput();
    endcap_outer->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    endcap_outer->calculate();

    endcap_inner->wizardParams.insert("Position x", position.x());
    endcap_inner->wizardParams.insert("Position y", position.y());
    endcap_inner->wizardParams.insert("Position z", position.z());
    endcap_inner->wizardParams.insert("Angle x", angle_x);
    endcap_inner->wizardParams.insert("Angle y", angle_y);
    endcap_inner->wizardParams.insert("Angle z", angle_z);
    endcap_inner->wizardParams.insert("d", d - 2 * s);   // Durchmesser
    h = l - 0.2 * (d - 2*s); // h - (1-sqrt(0.65)) * d
    endcap_inner->wizardParams.insert("h", h);     // Höhe
    endcap_inner->layer = this->layer;
    endcap_inner->processWizardInput();
    endcap_inner->rotateAroundAxis(90.0, QVector3D(0.0, 1.0, 0.0), angle_x, angle_y, angle_z);
    endcap_inner->calculate();

    //swap normal direction for inner torispherical head
    static GLushort indicesFacesTH[350];
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            indicesFacesTH[35 * i + 2 * j] = 16 * i + j + 16;
            indicesFacesTH[35 * i + 2 * j + 1] = 16 * i + j;
        }
        indicesFacesTH[35 * i + 32] = 16 * i + 16;
        indicesFacesTH[35 * i + 33] = 16 * i;
        indicesFacesTH[35 * i + 34] = 0xABCD;
    }
    endcap_inner->indexBufFaces->bind();
    endcap_inner->indexBufFaces->allocate(indicesFacesTH, sizeof(indicesFacesTH));

    QVector3D vertices[32];
    for(int k = 0; k < 16; k++)
        vertices[k] = position + matrix_rotation * QVector3D(0.0, sin(k * PI * 0.125) * d/2, cos(k * PI *0.125) * d/2);
    for(int k = 0; k < 16; k++)
        vertices[16 + k] = position + matrix_rotation * QVector3D(0.0, sin(k * PI * 0.125) * (d/2-s), cos(k * PI *0.125) * (d/2-s));

    GLushort indicesFaces[] =
    {
        16, 0, 17, 1, 18, 2, 19, 3, 20, 4, 21, 5, 22, 6, 23, 7, 24, 8, 25, 9, 26, 10, 27, 11, 28, 12, 29, 13, 30, 14, 31, 15, 16, 0
    };



    arrayBufVertices->bind();
    arrayBufVertices->allocate(vertices, sizeof(vertices));

    indexBufFaces->bind();
    indexBufFaces->allocate(indicesFaces, sizeof(indicesFaces));


    foreach(CADitem *item, subItems)
        this->boundingBox.enterVertices(item->boundingBox.getVertices());
    this->snap_flanges.append(position);
}

void CAD_heatcool_pipeEndCap::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();


    d = wizardParams.value("d").toDouble();
    l = wizardParams.value("l").toDouble();
    s = wizardParams.value("s").toDouble();
    fe = wizardParams.value("fe").toDouble();
    ff = wizardParams.value("ff").toDouble();

    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

}

QMatrix4x4 CAD_heatcool_pipeEndCap::rotationOfFlange(quint8 num)
{
    if(num == 1)
    {
        QMatrix4x4 m;
        m.setToIdentity();
        m.rotate(180.0, 0.0, 0.0, 1.0);
        return matrix_rotation * m;
    }
    else
        return matrix_rotation;
}

void CAD_heatcool_pipeEndCap::paint(GLWidget *glwidget)
{
    QColor color_brush_tmp = getColorBrush();

    glwidget->glEnable(GL_PRIMITIVE_RESTART);
    glwidget->glPrimitiveRestartIndex(0xABCD);

    arrayBufVertices->bind();
    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

    if (glwidget->render_solid)
    {
        glwidget->setPaintingColor(color_brush_tmp);

        indexBufFaces->bind();
        glwidget->glDrawElements(GL_TRIANGLE_STRIP, indexBufFaces->size(), GL_UNSIGNED_SHORT, 0);

        indexBufFaces->release();
    }

    arrayBufVertices->release();
}
