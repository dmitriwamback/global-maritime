//
// Created by Dmitri on 2026-09-10.
//

#ifndef GLOBAL_MARITIME_GDELT_H
#define GLOBAL_MARITIME_GDELT_H

#include <QString>
#include <QList>
#include <functional>
#include <QNetworkAccessManager>

struct MaritimeIncident {
    QString vessel;
    QString incident;
    QString flag;

    double latitude;
    double longitude;

    QString desc;
    QString source;
};

enum class MaritimeType {
    Hijacking, Piracy, Collision, Sinking, Capsizing, Grounding, Fire, Explosion, Other
};

class GDELT {
public:
    using Callback = std::function<void(QList<MaritimeIncident>)>;
    static void Search(const QString& query, Callback callback);
private:
    static QNetworkAccessManager* Manager();
};


#endif //GLOBAL_MARITIME_GDELT_H
