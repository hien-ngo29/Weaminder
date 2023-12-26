#include "weaminder.h"

Weaminder::Weaminder(QObject *parent)
    : QObject{parent}
{

}

bool Weaminder::isConnectionOn()
{
    QNetworkConfigurationManager manager;

    return manager.isOnline();
}
