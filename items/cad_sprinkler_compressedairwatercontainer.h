#ifndef CAD_SPRINKLER_COMPRESSEDAIRWATERCONTAINER_H
#define CAD_SPRINKLER_COMPRESSEDAIRWATERCONTAINER_H

#include "caditem.h"

class CAD_sprinkler_compressedAirWaterContainer : public CADitem
{
public:
    CAD_sprinkler_compressedAirWaterContainer();
    virtual ~CAD_sprinkler_compressedAirWaterContainer();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_COMPRESSEDAIRWATERCONTAINER_H