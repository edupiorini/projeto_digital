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

  //Correct password? (will only be invoked when using hardware Serial)
  if(!fingerprintSensor.verifyPassword()){
    Serial.println(F("Não foi possível se conectar ao sensor!"));
    while(true);
  }
}

void loop() {
  //Defines menu
  printMenu();

 
  String command = getCommand();
  int i = command.toInt();

  //Makes switching between previous
  switch(i){
    case 1:
      storeFingerprint();
      break;
     case 2:
      checkFingerPrint();
      break;
     case 3:
      printStoredFingerprintsCount();
      break;
     case 4:
      deleteFingerprint();
      break;
     case 5:
      emptyDatabase();
      break;
     default:
      Serial.println(F("Opção Inválida"));
      break;
     
  }

  delay(1000);
}



//Construção do Menu
void printMenu(){
  Serial.println();
  Serial.println(F("\nDigite um dos números do menu abaixo:"));
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

//------------------------------------------------------------------------------------------------------->Fingerprint Store
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

// ----------------------------------------------------------Begin of FP Recognition Loop-----------------------------------------------------
  //Waiting for valid FP
  while(fingerprintSensor.getImage() != FINGERPRINT_OK);

  //Converts to first pattern
  if(fingerprintSensor.image2Tz(1) != FINGERPRINT_OK){
    //bad req
    Serial.println(F("Erro image2Tz 1"));
    return;
  } // passed!

  Serial.println(F("Retire o dedo do sensor"));

  delay(2000);
  //Finger was taken?
  while(fingerprintSensor.getImage() != FINGERPRINT_NOFINGER);

  //new image for quality increasing
  Serial.println(F("Encoste o mesmo dedo no sensor"));


  while(fingerprintSensor.getImage() != FINGERPRINT_OK);
  
  if(fingerprintSensor.image2Tz(2) != FINGERPRINT_OK){
    //bad req
    Serial.println(F("Erro image2Tz 2"));
    return;
  } // passed, then move on
// ----------------------------------------------------------End of FP Recognition Loop-----------------------------------------------------

  //Creation of model
  if(fingerprintSensor.createModel() != FINGERPRINT_OK){
    
    Serial.println(F("Erro createModel"));
    return;
  }

  //Stores model
  if(fingerprintSensor.storeModel(location) != FINGERPRINT_OK){
    //bad req on storeModel
    Serial.println(F("Erro storeModel!"));
    return;
  }

  // Success!
  Serial.println(F("Sucesso!!"));
}

//------------------------------------------------------------------------------------------ >Stored fingerprint check
void checkFingerPrint(){
  Serial.println(F("Encoste o dedo no sensor"));

  while(fingerprintSensor.getImage() != FINGERPRINT_OK);

  if(fingerprintSensor.image2Tz() != FINGERPRINT_OK) {
    Serial.println(F("Erro image2Tz"));
    return;
  }

  if(fingerprintSensor.fingerFastSearch() != FINGERPRINT_OK){
    Serial.println(F("Digital não encontrada!"));
    return;
  }

  //Fingerprint passed tests and will go on
  Serial.print(F("Digital encontrada com confiança de "));
  Serial.print(fingerprintSensor.confidence);
  Serial.print(F(" na posição "));
  Serial.println(fingerprintSensor.fingerID);
  
}

//---------------------------------------------------------------------------------------------> Shows number of fingerprints stored

void printStoredFingerprintsCount() {
  //sensor puts qtty of fingerprints stored in "templateCount"
  fingerprintSensor.getTemplateCount();

  //Shows qtty stored
  Serial.print(F("Digitais cadastradas: "));
  Serial.println(fingerprintSensor.templateCount);
}

//---------------------------------------------------------------------------------------------> Delete fingerprint
void deleteFingerprint() {
  Serial.println(F("Qual a posição para apagar a digital? (1 a 149)"));

  String strLocation = getCommand();
  int location = strLocation.toInt();

  if(location < 1 || location > 149){
    Serial.println(F("Posição inválida!"));
    return;
  }

  //Erases given fingerprint
  if(fingerprintSensor.deleteModel(location) != FINGERPRINT_OK) {
    Serial.println(F("Erro ao apagar digital"));
    
  }else {
    Serial.println(F("Digital apagada com sucesso!!"));
  }
}

//-------------------------------------------------------------------------------------------> Empty Database
void emptyDatabase()
{
  Serial.println(F("Tem certeza? (1 -> Sim  /  2 -> Nao)"));

 
  String strCommand = getCommand();
  int command = strCommand.toInt();

  Serial.println(command);

  if(command == 1 )
  {
    Serial.println(F("Apagando banco de digitais..."));

    if(fingerprintSensor.emptyDatabase() != FINGERPRINT_OK)
    {
      Serial.println(F("Erro ao apagar banco de digitais"));
    }
    else
    {
      Serial.println(F("Banco de digitais apagado com sucesso!!!"));
    }
  }
  else
  {
    Serial.println(F("Cancelado"));
  }
}
