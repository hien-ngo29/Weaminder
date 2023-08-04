#include "weaminder.h"

Weaminder::Weaminder(QObject *parent)
    : QObject{parent}
{

}

bool Weaminder::isWifiOn()
{
    QNetworkConfigurationManager manager;
    QList<QNetworkConfiguration> configs = manager.allConfigurations(QNetworkConfiguration::Active);

    foreach (const QNetworkConfiguration &config, configs) {
        if (config.bearerType() == QNetworkConfiguration::BearerWLAN) {
            return true;
        }
    }

    return false;
}
