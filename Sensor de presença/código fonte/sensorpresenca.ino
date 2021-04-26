const int presenca = 7;
int presencaDetectada = LOW;

/*web client*/
#include <SPI.h>
#include <Ethernet.h>

//Definicoes de IP, mascara de rede e gateway
  byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  
  // usando ponte da rede
  IPAddress ip(192,168,15,100);          //Define o endereco IP
  IPAddress gateway(192,168,15,1);     //Define o gateway
  IPAddress subnet(255, 255, 255, 0); //Define a máscara de rede
  
  // usando no cabo de rede ligado ao roteador
  //IPAddress ip(192, 168, 0, 103); /*endereço IP estático para usar se o DHCP falhar ao atribuir*/
  
  char server[] = "dfilitto.com.br";  /*endereço de nome para hidroponia (usando DNS)*/
  EthernetClient client; /*biblioteca do cliente Ethernet (a porta 80 é padrão para HTTP)*/
  
void setup() {
  // put your setup code here, to run once:
/* Inicializa a Serial */
  Serial.begin(9600);

  pinMode(presenca, INPUT);  
}

void loop() {
  // put your main code here, to run repeatedly:
   

  //VERIFICA SE PASSOU ALGUEM
  Serial.println(presencaDetectada);
  //delay(500);
  presencaDetectada = digitalRead(presenca);
  if(presencaDetectada == HIGH){
      enviaWebAgora();      
  } 

}

void enviaWebAgora(){
  String gete = "GET ";  
  String http = " HTTP/1.1";
  String url = "";
    
  url = "GET /arduino/cadastrar.php?status=" + String(presencaDetectada) + " HTTP/1.1";
  Serial.println(url);
  
     
  //delay(5000);

  while (!Serial) {
    ; // aguarde a conexão da porta serial. Necessário apenas para porta USB nativa
  }

  // inicia a conexão Ethernet:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha na configuração do DHCP");
    // tente configurar usando o endereço IP em vez do DHCP:
    Ethernet.begin(mac, ip);
  }
  // dê um segundo ao escudo Ethernet para inicializar:
  delay(1000);
  Serial.println("Conectando...");
  // se você conseguir uma conexão, informe novamente via serial:
  if (client.connect(server, 80)) {
    Serial.println("Conectado");
    // Faça uma solicitação HTTP:
    client.println(url);
    client.println("Host: dfilitto.com.br");
    client.println("Connection: close");
    client.println();
    client.stop();
    Serial.println("Conexão encerrada");
  } else {
    //se você não obteve uma conexão com o servidor:
    Serial.println("Conexão falhou");
  }  
}
