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

#include "cad_electrical_busbarwithtapoffpoints2row.h"
#include "glwidget.h"

CAD_electrical_busbarwithtapoffpoints2row::CAD_electrical_busbarwithtapoffpoints2row() : CADitem(CADitemTypes::Electrical_BusbarWithTapoffPoints2Row)
{
    busbar = new CAD_basic_box();
    this->subItems.append(busbar);
    wizardParams.insert("Position x", 0.0);
    wizardParams.insert("Position y", 0.0);
    wizardParams.insert("Position z", 0.0);
    wizardParams.insert("Angle x", 0.0);
    wizardParams.insert("Angle y", 0.0);
    wizardParams.insert("Angle z", 0.0);

    wizardParams.insert("a", 100.0);
    wizardParams.insert("a2", 20.0);
    wizardParams.insert("b", 20.0);
    wizardParams.insert("l", 1000.0);
    wizardParams.insert("l1", 200.0);
    wizardParams.insert("l2", 0.0);
    wizardParams.insert("l3", 100.0);
    wizardParams.insert("l4", 100.0);
    wizardParams.insert("n", 5);
    //    arrayBufVertices = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    //    arrayBufVertices.create();
    //    arrayBufVertices.setUsagePattern(QOpenGLBuffer::StaticDraw);

    //    indexBufFaces = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    //    indexBufFaces.create();
    //    indexBufFaces.setUsagePattern(QOpenGLBuffer::StaticDraw);

    //    indexBufLines = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    //    indexBufLines.create();
    //    indexBufLines.setUsagePattern(QOpenGLBuffer::StaticDraw);

    processWizardInput();
    calculate();
}

CAD_electrical_busbarwithtapoffpoints2row::~CAD_electrical_busbarwithtapoffpoints2row()
{
    //    arrayBufVertices.destroy();
    //    indexBufFaces.destroy();
    //    indexBufLines.destroy();
}

QList<CADitemTypes::ItemType> CAD_electrical_busbarwithtapoffpoints2row::flangable_items()
{
    QList<CADitemTypes::ItemType> flangable_items;
    flangable_items.append(CADitemTypes::Electrical_BusbarWithoutTapoffPoints);
    flangable_items.append(CADitemTypes::Electrical_BusbarWithTapoffPoints1Row);
    flangable_items.append(CADitemTypes::Electrical_BusbarWithTapoffPoints2Row);
    
    return flangable_items;
}

QImage CAD_electrical_busbarwithtapoffpoints2row::wizardImage()
{
    QImage image;
    QFileInfo fileinfo(__FILE__);
    QString imageFileName = fileinfo.baseName();
    imageFileName.prepend(":/itemGraphic/");
    imageFileName.append(".png");

    image.load(imageFileName, "PNG");

    return image;
}

QString CAD_electrical_busbarwithtapoffpoints2row::iconPath()
{
    return ":/icons/cad_electrical/cad_electrical_busbarwithtapoffpoints2row.svg";
}

QString CAD_electrical_busbarwithtapoffpoints2row::domain()
{
    return "Electrical";
}

QString CAD_electrical_busbarwithtapoffpoints2row::description()
{
    return "Electrical|Busbar With Tapoff Points 2 Row";
}

void CAD_electrical_busbarwithtapoffpoints2row::calculate()
{
    matrix_rotation.setToIdentity();
    matrix_rotation.rotate(angle_x, 1.0, 0.0, 0.0);
    matrix_rotation.rotate(angle_y, 0.0, 1.0, 0.0);
    matrix_rotation.rotate(angle_z, 0.0, 0.0, 1.0);

    boundingBox.reset();

    this->snap_flanges.clear();
    this->snap_center.clear();
    this->snap_vertices.clear();

    this->snap_basepoint = (position);
    this->subItems.clear();
    this->subItems.append(busbar);

    QVector3D position_bus = position + matrix_rotation * QVector3D(l/2, b/2, 0.0);
    busbar->wizardParams.insert("Position x", position_bus.x());
    busbar->wizardParams.insert("Position y", position_bus.y());
    busbar->wizardParams.insert("Position z", position_bus.z());
    busbar->wizardParams.insert("Angle x", angle_x);
    busbar->wizardParams.insert("Angle y", angle_y);
    busbar->wizardParams.insert("Angle z", angle_z);

    busbar->wizardParams.insert("l", l);
    busbar->wizardParams.insert("b", b);
    busbar->wizardParams.insert("a", a);
    busbar->layer = this->layer;
    busbar->processWizardInput();
    busbar->calculate();

    this->snap_flanges.append(position);
    this->snap_flanges.append(position + matrix_rotation * QVector3D(l, 0.0, 0.0));
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j <=1; j++)
        {
            QVector3D position_box = position + matrix_rotation * QVector3D(l3 + 0.5 * l2 + i * l1 + j * 0.5 * l1, (1-j) * b, 0.0);
            this->snap_flanges.append(position_box);
            CAD_basic_box * box = new CAD_basic_box();
            box->wizardParams.insert("Position x", position_box.x());
            box->wizardParams.insert("Position y", position_box.y());
            box->wizardParams.insert("Position z", position_box.z());
            box->wizardParams.insert("Angle x", angle_x);
            box->wizardParams.insert("Angle y", angle_y);
            box->wizardParams.insert("Angle z", angle_z);

            box->wizardParams.insert("l", l2);
            box->wizardParams.insert("b", 0.0);
            box->wizardParams.insert("a", a2);
            box->layer = this->layer;
            box->processWizardInput();
            box->calculate();
            this->subItems.append(box);
        }
    }
    this->snap_center = busbar->snap_center;
    this->snap_vertices = busbar->snap_vertices;
    this->boundingBox = busbar->boundingBox;
}

void CAD_electrical_busbarwithtapoffpoints2row::processWizardInput()
{
    position.setX(wizardParams.value("Position x").toDouble());
    position.setY(wizardParams.value("Position y").toDouble());
    position.setZ(wizardParams.value("Position z").toDouble());
    angle_x = wizardParams.value("Angle x").toDouble();
    angle_y = wizardParams.value("Angle y").toDouble();
    angle_z = wizardParams.value("Angle z").toDouble();

    a = wizardParams.value("a").toDouble();
    a2 = wizardParams.value("a2").toDouble();
    b = wizardParams.value("b").toDouble();
    l = wizardParams.value("l").toDouble();
    l1 = wizardParams.value("l1").toDouble();
    l2 = wizardParams.value("l2").toDouble();
    l3 = wizardParams.value("l3").toDouble();
    l4 = wizardParams.value("l4").toDouble();
    n = wizardParams.value("n").toInt();
}

//void CAD_electrical_busbarwithtapoffpoints2row::paint(GLWidget *glwidget)
//{
//    QColor color_pen_tmp = getColorPen();
//    QColor color_brush_tmp = getColorBrush();

//    arrayBufVertices.bind();
//    glwidget->shaderProgram->enableAttributeArray(glwidget->shader_vertexLocation);
//    glwidget->shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));

//    if (glwidget->render_solid)
//    {
//        glwidget->setPaintingColor(color_brush_tmp);

//        indexBufFaces.bind();
//        glwidget->glDrawElements(GL_TRIANGLE_STRIP, indexBufFaces.size(), GL_UNSIGNED_SHORT, 0);

//        indexBufFaces.release();
//    }

//    if (glwidget->render_outline)
//    {
//        glwidget->setPaintingColor(color_pen_tmp);
//        glwidget->glLineWidth(1.0);

//        indexBufLines.bind();
//        glwidget->glDrawElements(GL_LINES, indexBufLines.size(), GL_UNSIGNED_SHORT, 0);
//        indexBufLines.release();
//     }                          

//     arrayBufVertices.release();
//}