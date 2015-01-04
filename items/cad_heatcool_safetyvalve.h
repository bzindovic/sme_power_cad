#ifndef CAD_HEATCOOL_SAFETYVALVE_H
#define CAD_HEATCOOL_SAFETYVALVE_H

#include "caditem.h"

class CAD_heatcool_safetyValve : public CADitem
{
public:
    CAD_heatcool_safetyValve();
    virtual ~CAD_heatcool_safetyValve();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_SAFETYVALVE_H