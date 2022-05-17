#include "QXmppClient.h"
#include "QXmppLogger.h"
#include "QXmppMessage.h"
#include <QCoreApplication>

// classe para o sensor IoT
class Nodo{
   public:
       int id;
       int temperatura;
       Nodo(int id, int temperatura){  // construtor da classe Nodo 
            id = id;
            temperatura = temperatura;
       }
};

Nodo sensor(1,25); // cria um sensor global

/* 
 @brief: Comportamento quando o nodo recebe uma mensagem
 @param: Objeto da classe QXmppMessage
*/
void QXmppClient::messageReceived(const QXmppMessage &message){
    QString from = message.from();                      // origem da mensagem
    QString msg  = message.body();                      // conteúdo da mensagem
    QString temp = QString::number(sensor.temperatura); // valor lido pelo sensor
    QString id   = QString::number(sensor.id);          // ID do sensor
    
    if(from == "wesley@5222.de/gajim.BT104NWF"){ // caso o cliente de origem for Wesley
        if(msg == "temp"){ // caso requisite a temperatura
           sendPacket(QXmppMessage("","wesley@5222.de","Temp: "+temp,""));// responde c/ a temperatura 
        }
        if(msg == "id"){ // caso requisite o ID
            sendPacket(QXmppMessage("","wesley@5222.de","ID: "+id,"")); // responde c/ ID do sensor
        }
    }else{ // caso o cliente de origem nao seja Wesley
        sendPacket(QXmppMessage("",from,"Só converso com o Wesley","")); // responde
    }
}

/*
 @brief: Comportamento quando o nodo recebe um sinal de presenca
 @param: Objeto da classe QXmppPresence
*/
void QXmppClient::presenceReceived(const QXmppPresence & presence){
   sendPacket(QXmppMessage("","wesley@5222.de","Presense signal","")); // envia mensagem p/ Wesley
}

/* @brief: Comportamento quando o nodo recebe um sinal de conexao */
void QXmppClient::connected(){
    sendPacket(QXmppMessage("","wesley@5222.de","Conexão iniciada",""));  // envia mensagem p/ Wesley
}

int main(int argc, char *argv[]){
    QCoreApplication app(argc, argv);
    QXmppClient client;                 // cria um objeto da classe cliente
    sensor.id = 1;                      // atribui um valor a variavel de ID do sensor
    sensor.temperatura = 25;            // atribui um valor a variavel de temperatura do sensor
    client.logger()->setLoggingType(QXmppLogger::StdoutLogging);  // printa as mensagens de log 
    client.connectToServer("iot_sensor@5222.de", "sensor123");    // conecta com o servidor
    return app.exec();
}