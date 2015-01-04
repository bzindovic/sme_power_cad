#ifndef CAD_HEATCOOL_PUMP_H
#define CAD_HEATCOOL_PUMP_H

#include "caditem.h"

class CAD_heatcool_pump : public CADitem
{
public:
    CAD_heatcool_pump();
    virtual ~CAD_heatcool_pump();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_PUMP_H