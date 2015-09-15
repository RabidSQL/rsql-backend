#ifndef RABIDSQL_DATABASECONNECTIONMANAGER_H
#define RABIDSQL_DATABASECONNECTIONMANAGER_H

#include "connectionsettings.h"
#include "databaseconnection.h"
#include <map>

namespace RabidSQL {

class DatabaseConnectionManager: virtual public SmartObject
{
    friend class DatabaseConnection;
    static const unsigned int DEFAULT_EXPIRY = 10;

public:

    DatabaseConnectionManager(DatabaseConnection *mainConnection,
                              ConnectionSettings *settings);
    std::string reserveDatabaseConnection(int expiry = 0,
                                          SmartObject *receiver = nullptr);
    void releaseDatabaseConnection(std::string uuid);
    void call(std::string uuid, Variant uid,
              QueryEvent::type event,
              VariantList arguments=VariantList());
    void killQuery(std::string uuid);
    ConnectionSettings::type getType();
    ~DatabaseConnectionManager();

    void disconnected(const VariantList &args);

private:

    void call(DatabaseConnection *connection, Variant uid,
              QueryEvent::type event,
              VariantList arguments=VariantList());
    DatabaseConnection *getDatabaseConnection(std::string uuid);
    DatabaseConnection *reserveDatabaseConnectionObj(
            int timeout = 0, SmartObject *receiver = nullptr);

    ConnectionSettings::type type;
    DatabaseConnection *mainConnection;

    struct ConnectionRecord {
        std::string uuid;
        long expiry = 0;
        SmartObject *receiver = nullptr;
    };

    typedef std::map<DatabaseConnection *, ConnectionRecord> Connections;
    typedef std::map<std::string, DatabaseConnection *> DisconnectingConnections;

    Connections connections;
    DisconnectingConnections disconnectingConnections;
    unsigned int maxConnections;
};

} // namespace RabidSQL

#endif // RABIDSQL_DATABASECONNECTIONMANAGER_H
