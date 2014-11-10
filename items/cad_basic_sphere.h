#ifndef CAD_BASIC_SPHERE_H
#define CAD_BASIC_SPHERE_H

#include "caditem.h"

class CAD_basic_sphere : public CADitem
{
public:
    CAD_basic_sphere();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

//    QVector3D pos;
    qreal radius;
};

#endif // CAD_BASIC_SPHERE_H
