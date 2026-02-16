#ifndef MQ9_H_
#define MQ9_H_

#include "BaseMQ.h"

// резистор установленный на плату (кОм)
#define MQ9_RL_BOARD            1.0
// коефициент чистого воздуха из графика
#define MQ9_RO_IN_CLEAR_AIR     9.8

class MQ9 : public BaseMQ {
public: 
    MQ9(uint8 pin);
    MQ9(uint8 pin, uint8 pinHeater);
    unsigned long readLPG();
    unsigned long readMethane();
    unsigned long readCarbonMonoxide();
private:
    // Резистор установленный на плату (кОм)
    virtual float getRL() const override { return MQ9_RL_BOARD; }
    // коефициент чистого воздуха
    virtual float getRoInCleanAir() const override { return (float)MQ9_RO_IN_CLEAR_AIR; }
};

#endif  // MQ9_H_
