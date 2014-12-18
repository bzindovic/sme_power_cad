#ifndef CAD_HEATCOOL_EXPANSIONCHAMBER_H
#define CAD_HEATCOOL_EXPANSIONCHAMBER_H

#include "caditem.h"

class CAD_heatcool_expansionChamber : public CADitem
{
public:
    CAD_heatcool_expansionChamber();
    virtual ~CAD_heatcool_expansionChamber();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_EXPANSIONCHAMBER_H
