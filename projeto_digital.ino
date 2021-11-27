#include <Adafruit_Fingerprint.h> 
#include <SoftwareSerial.h>

//Senha padrão do sensor
//const uint32_t password = 0x0;
//Software Serial para Arduino Uno
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint fingerprintSensor = Adafruit_Fingerprint(&mySerial);


void setup() {
  Serial.begin(9600);

  //Início do sensor
  setupFingerprintSensor();

}

void setupFingerprintSensor() {
  fingerprintSensor.begin(57600);

  //Senha correta?
  if(!fingerprintSensor.verifyPassword()){
    Serial.println(F("Não foi possível se conectar ao sensor!"));
    while(true);
  }
}

void loop() {
  //Gera o menu
  printMenu();

  //Leitura do comando e converte para inteiro
  String command = getCommand();
  int i = command.toInt();

  //Designa opção selecionada no menu para função correspondente
  switch(i){
    case 1:
      storeFingerprint();
      break;
     case 2:
      checkFingerPrint();
      break;
//     case 3;
//      //printStoredFingerprintsCount();
//      break;
//     case 4;
//     // deleteFingerprint();
//      break;
//     case 5;
//     // emptyDatabase();
//      break;
     default:
      Serial.println(F("Opção Inválida"));
      break;
     
  }

  delay(1000);
}



//Construção do Menu
void printMenu(){
  Serial.println();
  Serial.println(F("Digite um dos números do menu abaixo:"));
  Serial.println(F("1 - Cadastrar digital"));
  Serial.println(F("2 - Verificar digital"));
  Serial.println(F("3 - Mostrar quantidade de digitais cadastradas"));
  Serial.println(F("4 - Apagar digital em uma posição"));
  Serial.println(F("5 - Apagar banco de digitais"));
}

String getCommand() {
  while(!Serial.available()) delay(100);
  return Serial.readStringUntil('\n');
}

//Cadastro de digitais
void storeFingerprint(){
  Serial.println(F("Qual a posição para guardar a digital? (1 a 149)"));

  String strLocation = getCommand();
  int location = strLocation.toInt();

  if(location < 1 || location > 149){
    //posição inválida
    Serial.println(F("Posição inválida!"));
    return;
  }

  Serial.println(F("Encoste o dedo no sensor"));

// ----------------------------------------------------------INICIO DE PROCESSO DE RECONHECIMENTO DA DIGITAL-----------------------------------------------------
  //Aguarda imagém válida
  while(fingerprintSensor.getImage() != FINGERPRINT_OK);

  //Converte para o primeiro padrão
  if(fingerprintSensor.image2Tz(1) != FINGERPRINT_OK){
    //deu ruim
    Serial.println(F("Erro image2Tz 1"));
    return;
  } // se passou, então segue a vida

  Serial.println(F("Retire o dedo do sensor"));

  delay(2000);
  //dedo foi retirado?
  while(fingerprintSensor.getImage() != FINGERPRINT_NOFINGER);

  //nova imagem da mesma digital para confirmação
  Serial.println(F("Encoste o mesmo dedo no sensor"));


  while(fingerprintSensor.getImage() != FINGERPRINT_OK);
  
  if(fingerprintSensor.image2Tz(2) != FINGERPRINT_OK){
    //deu ruim
    Serial.println(F("Erro image2Tz 2"));
    return;
  } // se passou, então segue a vida
// ----------------------------------------------------------FIM DE PROCESSO DE RECONHECIMENTO DA DIGITAL-----------------------------------------------------

  //Criação de modelo
  if(fingerprintSensor.createModel() != FINGERPRINT_OK){
    //deu ruim de novo
    Serial.println(F("Erro createModel"));
    return;
  }

  //Guarda o modelo
  if(fingerprintSensor.storeModel(location) != FINGERPRINT_OK){
    //ruim no storeModel
    Serial.println(F("Erro storeModel!"));
    return;
  }

  //Chegamos até última verificação! Success!
  Serial.println(F("Sucesso!!"));
}

// Verificação de digital cadastrada
void checkFingerPrint(){
  Serial.println(F("Encoste o dedo no sensor"));
  
}