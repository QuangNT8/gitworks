#ifndef MASTER_CONTROLLER_H
#define MASTER_CONTROLLER_H
#include "core/component.h"
#include "core/define.h"
#include "core/model.h"
#include <QColor>
#include <QXYSeries>
#include "type.h"
#include "plotline.h"
#include <QUrl>
#include "commandlist.h"
#include "recentlist.h"

COMPONENT(console,Controller)
    Q_OBJECT
    PROPERTY(console::CommandList*, commands)
    PROPERTY(console::RecentList*, recentList)
public:
    void init();
    Q_INVOKABLE void setVariable(quint16 command, QString v);
    Q_INVOKABLE void buttonClicked(quint16 command);
    Q_INVOKABLE QString generateHeaderFile(QUrl folder);
    Q_INVOKABLE void pause(bool pause){pause_ = pause;}
    Q_INVOKABLE CommandList* editModel();
    Q_INVOKABLE void swapModel(CommandList*);
    Q_INVOKABLE bool open(QString fileName);
    Q_INVOKABLE void close();
    void displayPlot(uint8_t* data, uint16_t length);
    void displayOsc(uint8_t* data, uint16_t length);
    Q_INVOKABLE QString convertUrl(QUrl url);
    void appendPlot(uint8_t plot, uint16_t index, int16_t v);
    uint32_t currentIndex(){return currentIndex_;}
    void upadateField(uint16_t cmd, int16_t val);
private:
    void initCharts_();
    void setYAxisRange(uint16_t chart, int32_t min, int32_t max);
    void setXAxisRange(uint16_t chart, int32_t min, int32_t max);

    uint32_t currentIndex_ = 0;
    uint16_t itemcount_=0;
    bool pause_ = false;
COMPONENT_END

#endif // CONTROLLER_H
