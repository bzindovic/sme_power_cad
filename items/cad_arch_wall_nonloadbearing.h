#ifndef CAD_ARCH_WALL_NONLOADBEARING_H
#define CAD_ARCH_WALL_NONLOADBEARING_H

#include "caditem.h"
#include "cad_basic_box.h"

class CAD_arch_wall_nonLoadBearing : public CADitem
{
public:
    CAD_arch_wall_nonLoadBearing();
    virtual ~CAD_arch_wall_nonLoadBearing();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

private:
    qreal b;
    qreal l;
    qreal a;

    CAD_basic_box* basic_box;
};

#endif // CAD_ARCH_WALL_NONLOADBEARING_H