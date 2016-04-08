#include <chrono>
#include <thread>

#include <QtCore/QProcess>

class Server
{
public:
    Server()
    {
        m_server.start("node", QStringList {} << "http_server.js");
        m_server.waitForStarted();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    ~Server()
    {
        m_server.close();
    }

private:
    QProcess m_server;
};

static Server s_httpServer;
