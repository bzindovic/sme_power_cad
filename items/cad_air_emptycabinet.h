#ifndef CAD_AIR_EMPTYCABINET_H
#define CAD_AIR_EMPTYCABINET_H

#include "caditem.h"

class CAD_air_emptyCabinet : public CADitem
{
public:
    CAD_air_emptyCabinet();
    virtual ~CAD_air_emptyCabinet();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_EMPTYCABINET_H