#ifndef MANAGER
#define MANAGER

#include "component.h"
#include "tcp/tcpController.h"
#include "models.h"

COMPONENT(home, Manager)
Q_OBJECT
    PROPERTY(home::ModelList*, recipes)
    PROPERTY(home::ModelList*, liquids)
    PROPERTY(home::Session*, session)
public:
    void init();
    void start();
    void tcpStateChanged();
    Q_INVOKABLE quint32 getIngredient(core::Model* liquid, core::Model* recipe);
    Q_INVOKABLE quint64 getLiquidID(core::Model* liquid){return liquid->id();}
    Q_SIGNAL void logged();
    Q_SIGNAL void orderStateChanged(quint32 state);
COMPONENT_END

#endif // MANAGER
