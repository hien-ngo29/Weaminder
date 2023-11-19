#include "weaminder.h"

Weaminder::Weaminder(QObject *parent)
    : QObject{parent}
{

}

bool Weaminder::isWifiOn()
{
    QNetworkConfigurationManager manager;

    return manager.isOnline();
}
