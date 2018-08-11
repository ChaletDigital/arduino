#include <Ethernet.h>
#include <SPI.h>


byte mac[] = {0x00, 0x29, 0xB9, 0xFB, 0xE2, 0x57};
byte ip[] = {192, 168, 25, 36};
byte gateway[] = {192, 168, 25};
byte subnet[] = {255, 255, 255, 0};
int port = 82;

EthernetServer server(port);
int speedSerial = 9600;
int sizeString = 80;
String stringInput = String(sizeString);


unsigned long previousMillis = 0;
long lastDebounceTime = 0;
long debounceDelay = 50;

// VARIAVEIS

// portas de comunicacao serial usadas com GPIOs
int Pin14 = 14; // input estado do portao
// int Pin15 = 15; //
// int Pin16 = 16; //
// int Pin17 = 17; //
// int Pin18 = 18; //
// int Pin19 = 19; //

//pulsadores
int Pin54 = 54; // FILL - A0
int Pin55 = 55; // HIDRO - A1
int Pin56 = 56; // luz hidro - A2
int Pin57 = 57; // lavanderia - A3
int Pin58 = 58; // dicroicas banheiro - A4
int Pin59 = 59; // luz pia - A5
int Pin63 = 63; // welcomeHome RF input
int Pin64 = 64; // input pulsador exaustor Big
int Pin65 = 65; // input pulsador exaustor peq

// leitura atual de cada pino pulsador
int inputPin54State;
int inputPin55State;
int inputPin56State;
int inputPin57State;
int inputPin58State;
int inputPin59State;
int inputPin63State;
int inputPin64State;
int inputPin65State;

// leitura anterior de cada INPUT PIN DOS PULSADORES
int lastInputPin54State = LOW;
int lastInputPin55State = LOW;
int lastInputPin56State = LOW;
int lastInputPin57State = LOW;
int lastInputPin58State = LOW;
int lastInputPin59State = LOW;
int lastInputPin63State = LOW;
int lastInputPin64State = LOW;
int lastInputPin65State = LOW;

// ESTADO ATUAL DOS PINOS DE OUTPUT controlados por pulsadores
int Pin40State = HIGH; // FILL
int Pin29State = HIGH;  // HIDRO
int Pin38State = HIGH; // LUZ HIDRO
int Pin66State = HIGH; // LAVANDERIA
int Pin67State = HIGH; // DICR BANHO
int Pin39State = HIGH; // LUZ PIA
int Pin68State = HIGH; // exautor BIG
int Pin69State = HIGH; // exaustor PEQ


//dimmers
int Pin2 = 2;   // lareira down - verde com papel
int Pin3 = 3;   // lareira up - verde
int Pin4 = 4;   // quarto down - roxo com papel
int Pin5 = 5;   // quarto up - marrom
int Pin6 = 6;   // lustre down - amarelo
int Pin7 = 7;   // lustre up - roxo
int Pin8 = 8;   // dicroica down - cinza
int Pin9 = 9;   // dicroica up - azul
int Pin60 = 60; // chama funcao slowSleep no arduino do quarto

//int Pin61 = 61; // free
int Pin62 = 62; // rele solido II - WORKING LIGHTS

//placa um - reles 1-8
int Pin22 = 22;// rele 01 - fire
int Pin24 = 24;// rele 02 - pulso para controle abrir portao
int Pin26 = 26;// rele 03 - portch
int Pin28 = 28;// rele 04 - flavia hi
int Pin30 = 30;// rele 05 - fundos
int Pin32 = 32;// rele 06 - fred low
int Pin34 = 34;// rele 07 - figueira
int Pin36 = 36;// rele 08 - deck main

//placa dois - reles 9-16
int Pin23 = 23;// rele 09 - laguinho
int Pin25 = 25;// rele 10 - jacaranda
int Pin27 = 27;// rele 11 - gate
int Pin29 = 29;// rele 12 - HIDRO
int Pin31 = 31;// rele 13 - deck fan
int Pin33 = 33;// rele 14 - rampa
int Pin35 = 35;// rele 15 - cascata
int Pin37 = 37;// rele 16 - deck hi

//placa tres - reles 17-24
int Pin38 = 38;// rele 17 - luz hidro
int Pin40 = 40;// rele 18 - FILL
int Pin42 = 42;// rele 19 - holly deck
int Pin44 = 44;// rele 20 - spray
int Pin46 = 46;// rele 21 - shelves
int Pin48 = 48;// rele 22 - desk
int Pin66 = 66;// rele 23 - lavanderia
int Pin67 = 67;// rele 24 - dicroica do banheiro

//placa quatro - 25-32
int Pin39 = 39;// rele 25 -
int Pin41 = 41;// rele 26 -
int Pin43 = 43;// rele 27 -
int Pin45 = 45;// rele 28 -
int Pin47 = 47;// rele 29 -
int Pin49 = 49;// rele 30 -
int Pin68 = 68;// rele 31 -
int Pin69 = 69;// rele 32 -



/**
 * this is the setup
 */
void setup()
{
	// start ethernet shield webserver
	Serial.begin(speedSerial);
	Ethernet.begin(mac, ip, gateway, subnet);
	server.begin();


/**
 * PINMODE
 */

	//  portas de comunicacao serial como inputs ou outputs
	pinMode(Pin14, INPUT_PULLUP); // estado do portao - aberto ou fechado
//	pinMode(Pin15, INPUT_PULLUP); //
//	pinMode(Pin16, INPUT_PULLUP); //
//	pinMode(Pin17, INPUT_PULLUP); //
//	pinMode(Pin18, INPUT_PULLUP); //
//	pinMode(Pin19, INPUT_PULLUP); //


	// rele solid state
	pinMode(Pin62, OUTPUT); // WORKLIGHTS

	// pinos INPUT dos pulsadores
	pinMode(Pin54, INPUT_PULLUP);// FILL
	pinMode(Pin55, INPUT_PULLUP);// HIDRO
	pinMode(Pin56, INPUT_PULLUP);// LUZ HIDRO
	pinMode(Pin57, INPUT_PULLUP);// LAVANDERIA
	pinMode(Pin58, INPUT_PULLUP);// DICR BANH
	pinMode(Pin59, INPUT_PULLUP);// LUZ PIA
	pinMode(Pin63, INPUT_PULLUP);// RF
	pinMode(Pin64, INPUT_PULLUP);// exaustor BIG
	pinMode(Pin65, INPUT_PULLUP);// exaustor peq

	// dimmers
	pinMode(Pin2, OUTPUT); // lareira down
	pinMode(Pin3, OUTPUT); // lareira up
	pinMode(Pin4, OUTPUT); // quarto down
	pinMode(Pin5, OUTPUT); // quarto up
	pinMode(Pin6, OUTPUT); // lustre down
	pinMode(Pin7, OUTPUT); // lustre up
	pinMode(Pin8, OUTPUT); // hi line down
	pinMode(Pin9, OUTPUT); // hi line up
	pinMode(Pin60, OUTPUT); // output para slowSleep

	pinMode(Pin22, OUTPUT); // rele 01 -fire
	pinMode(Pin24, OUTPUT); // rele 02 - pulso para controle abrir portao
	pinMode(Pin26, OUTPUT); // rele 03 - portch
	pinMode(Pin28, OUTPUT); // rele 04 - flavia hi
	pinMode(Pin30, OUTPUT); // rele 05 - fundos
	pinMode(Pin32, OUTPUT); // rele 06 - fred low
	pinMode(Pin34, OUTPUT); // rele 07 - figueira
	pinMode(Pin36, OUTPUT); // rele 08 - deck main

	pinMode(Pin23, OUTPUT); // rele 09 - laguinho
	pinMode(Pin25, OUTPUT); // rele 10 - jacaranda
	pinMode(Pin27, OUTPUT); // rele 11 - gate
	pinMode(Pin29, OUTPUT); // rele 12 - HIDRO
	pinMode(Pin31, OUTPUT); // rele 13 - deck fan
	pinMode(Pin33, OUTPUT); // rele 14 - rampa
	pinMode(Pin35, OUTPUT); // rele 15 - cascata
	pinMode(Pin37, OUTPUT); // rele 16 - deck hi

	pinMode(Pin38, OUTPUT); // rele 17 - luz hidro
	pinMode(Pin40, OUTPUT); // rele 18 - FILL
	pinMode(Pin42, OUTPUT); // rele 19 - holly deck
	pinMode(Pin44, OUTPUT); // rele 20 - spray
	pinMode(Pin46, OUTPUT); // rele 21 - shelves
	pinMode(Pin48, OUTPUT); // rele 22 - desk
	pinMode(Pin66, OUTPUT); // rele 23 - lavanderia
	pinMode(Pin67, OUTPUT); // rele 24 - dicroica do banheiro

	pinMode(Pin39, OUTPUT); // rele 25 -
	pinMode(Pin41, OUTPUT); // rele 26 -
	pinMode(Pin43, OUTPUT); // rele 27 -
	pinMode(Pin45, OUTPUT); // rele 28 -
	pinMode(Pin47, OUTPUT); // rele 29 -
	pinMode(Pin49, OUTPUT); // rele 30 -
	pinMode(Pin68, OUTPUT); // rele 31 -
	pinMode(Pin69, OUTPUT); // rele 32 -




/**
 * DIGITALWRITE
 */

	// dimmers
	digitalWrite(Pin2, LOW); // lareira down
	digitalWrite(Pin3, LOW); // lareira up
	digitalWrite(Pin4, LOW); // quarto down
	digitalWrite(Pin5, LOW); // quarto up
	digitalWrite(Pin6, LOW); // lustre down
	digitalWrite(Pin7, LOW); // lustre up
	digitalWrite(Pin8, LOW); // hi line down
	digitalWrite(Pin9, LOW); // hi line up
	digitalWrite(Pin60, LOW); // sleepSlow

	//solid state rele
	digitalWrite(Pin62, LOW); // work lights

	//reles mecanicos - necessitam ser inicializados com logica invertida devido aos optoacopladores incorporados na placa de reles
	digitalWrite(Pin22, HIGH);
	digitalWrite(Pin24, HIGH);
	digitalWrite(Pin26, HIGH);
	digitalWrite(Pin28, HIGH);
	digitalWrite(Pin30, HIGH);
	digitalWrite(Pin32, HIGH);
	digitalWrite(Pin34, HIGH);
	digitalWrite(Pin36, HIGH);

	digitalWrite(Pin23, HIGH);
	digitalWrite(Pin25, HIGH);
	digitalWrite(Pin27, HIGH);
	digitalWrite(Pin29, Pin29State);
	digitalWrite(Pin31, HIGH);
	digitalWrite(Pin33, HIGH);
	digitalWrite(Pin35, HIGH);
	digitalWrite(Pin37, HIGH);

	digitalWrite(Pin38, Pin38State);
	digitalWrite(Pin40, Pin40State);
	digitalWrite(Pin42, HIGH);
	digitalWrite(Pin44, HIGH);
	digitalWrite(Pin46, HIGH);
	digitalWrite(Pin48, HIGH);
	digitalWrite(Pin66, Pin66State);
	digitalWrite(Pin67, Pin67State);

	digitalWrite(Pin39, Pin39State);
	digitalWrite(Pin41, HIGH);
	digitalWrite(Pin43, HIGH);
	digitalWrite(Pin45, HIGH);
	digitalWrite(Pin47, HIGH);
	digitalWrite(Pin49, HIGH);
	digitalWrite(Pin68, Pin68State);
	digitalWrite(Pin69, Pin69State);

}





// FUNCOES

void acende_patio()
{
	digitalWrite(Pin23, LOW); // laguinho
	delay(2500);
	digitalWrite(Pin33, LOW); // rampa
	delay(2500);
	digitalWrite(Pin36, LOW); // deck main
	delay(2000);
	digitalWrite(Pin42, LOW); // holly deck
	delay(2000);
	digitalWrite(Pin34, LOW); // figueira
	delay(2000);
	digitalWrite(Pin30, LOW); // fundos
	delay(2000);
}


void apaga_patio()
{
	digitalWrite(Pin23, HIGH); // laguinho
	delay(1500);
	digitalWrite(Pin33, HIGH); // rampa
	delay(1000);
	digitalWrite(Pin30, HIGH); // fundos
	delay(1000);
	digitalWrite(Pin42, HIGH); // holly deck
	delay(1000);
	digitalWrite(Pin34, HIGH); // figueira
	delay(1000);
	digitalWrite(Pin36, HIGH); // deck main
}



void acende_dicroics()
{
	digitalWrite(Pin3, HIGH); // lareira
	delay(2800);
	digitalWrite(Pin3, LOW);
	delay(2000);
	digitalWrite(Pin9, HIGH); // hi line
	delay(5000);
	digitalWrite(Pin9, LOW);
	delay(2500);
	digitalWrite(Pin7, HIGH); // lustre
	delay(2700);
	digitalWrite(Pin7, LOW);
	delay(2500);
	digitalWrite(Pin5, HIGH); // quarto
	delay(2500);
	digitalWrite(Pin5, LOW);
}

void apaga_dicroics()
{
	digitalWrite(Pin2, HIGH); // lareira
	delay(5000);
	digitalWrite(Pin2, LOW);
	delay(3000);

	digitalWrite(Pin8, HIGH); // hi line
	delay(6000);
	digitalWrite(Pin8, LOW);
	delay(3000);

	digitalWrite(Pin6, HIGH); // lustre
	delay(5000);
	digitalWrite(Pin6, LOW);
	delay(3000);

	digitalWrite(Pin4, HIGH); // quarto
	delay(5000);
	digitalWrite(Pin4, LOW);
}




void loop()
{
	//DEBOUNCE

	//Pin54 E Pin40 - FILL
	int reading54 = digitalRead(Pin54);
	if (reading54 != lastInputPin54State)
	{
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay)
	{

		if (reading54 != inputPin54State)
		{
			inputPin54State = reading54;

			if (inputPin54State == LOW)
			{
				Pin40State = !Pin40State;
			}
		}
	}

	digitalWrite(Pin40, Pin40State);
	lastInputPin54State = reading54;



	//Pin55 E Pin29  -  HIDRO
	int reading55 = digitalRead(Pin55);
	if (reading55 != lastInputPin55State)
	{
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay)
	{

		if (reading55 != inputPin55State)
		{
			inputPin55State = reading55;

			if (inputPin55State == LOW)
			{
				Pin29State = !Pin29State;
			}
		}
	}

	digitalWrite(Pin29, Pin29State);
	lastInputPin55State = reading55;




	//Pin56 E Pin38 - LUZ HIDRO
	int reading56 = digitalRead(Pin56);
	if (reading56 != lastInputPin56State)
	{
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay)
	{

		if (reading56 != inputPin56State)
		{
			inputPin56State = reading56;

			if (inputPin56State == LOW)
			{
				Pin38State = !Pin38State;
			}
		}
	}

	digitalWrite(Pin38, Pin38State);
	lastInputPin56State = reading56;




	//Pin57 E Pin66 - LAVANDERIA
	int reading57 = digitalRead(Pin57);
	if (reading57 != lastInputPin57State)
	{
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay)
	{

		if (reading57 != inputPin57State)
		{
			inputPin57State = reading57;

			if (inputPin57State == LOW)
			{
				Pin66State = !Pin66State;
			}
		}
	}

	digitalWrite(Pin66, Pin66State);
	lastInputPin57State = reading57;




	// Pin58 E Pin67 - DICROICAS BANHEIRO
	int reading58 = digitalRead(Pin58);
	if (reading58 != lastInputPin58State)
	{
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay)
	{

		if (reading58 != inputPin58State)
		{
			inputPin58State = reading58;

			if (inputPin58State == LOW)
			{
				Pin67State = !Pin67State;
			}
		}
	}

	//	digitalWrite(Pin67, Pin67State);
	lastInputPin58State = reading58;




	//Pin59 E Pin39 - LUZ PIA
	int reading59 = digitalRead(Pin59);
	if (reading59 != lastInputPin59State)
	{
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay)
	{

		if (reading59 != inputPin59State)
		{
			inputPin59State = reading59;

			if (inputPin59State == LOW)
			{
				Pin39State = !Pin39State;
			}
		}
	}


	//	digitalWrite(Pin39, Pin39State);
	lastInputPin59State = reading59;






	// RF chamando welcomeHome
	// Pin63
	int reading63 = digitalRead(Pin63);
	if (reading63 != lastInputPin63State)
	{
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay)
	{

		if (reading63 != inputPin63State)
		{
			inputPin63State = reading63;
			if (inputPin63State == LOW)
			{
			acende_patio();
			acende_dicroics();
			delay(60000);
			apaga_patio();
			}
		}
	}

	lastInputPin63State = reading63;






	//Pin64 E Pin68 - exaustor Big
	int reading64 = digitalRead(Pin64);
	if (reading64 != lastInputPin64State)
	{
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay)
	{

		if (reading64 != inputPin64State)
		{
			inputPin64State = reading64;

			if (inputPin64State == LOW)
			{
				Pin68State = !Pin68State;
			}
		}
	}


	//	digitalWrite(Pin68, Pin68State);
	lastInputPin64State = reading64;




	// Pin65 E Pin69 - exaustor peq
	int reading65 = digitalRead(Pin65);
	if (reading65 != lastInputPin65State)
	{
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay)
	{

		if (reading65 != inputPin65State)
		{
			inputPin65State = reading65;

			if (inputPin65State == LOW)
			{
				Pin69State = !Pin69State;
			}
		}
	}

	//	digitalWrite(Pin69, Pin69State);
	lastInputPin65State = reading65;






	// CABECALHO

	EthernetClient client = server.available();
	if (client)
	{
		boolean currentLineIsBlank = true;
		stringInput = "";

		if (client.connected())
		{

			client.println("HTTP/1.1 200 OK");
			//client.println("Server:Arduino");
            client.println("Access-Control-Allow-Origin: *");
            client.println("Access-Control-Allow-Headers: access-control-allow-headers, access-control-allow-origin,DNT,X-CustomHeader,Keep-Alive,User-Agent,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type");
            client.println("Access-Control-Request-Method: GET");
            client.println("Connection: Keep-Alive");
			//client.println("Content-Type:text/xml");
			client.println();

			while (client.available())
			{
				char c = client.read();

				if(stringInput.length() < sizeString)
				{
					stringInput.concat(c);

				}

				if (c == '\n' && currentLineIsBlank)
				{

					client.println("<?xml version=\"1.0\" encoding=\"utf-8\"?>");

					client.println("<automation>");




					// 	MONTA

					// Pin14 - estado (aberto ou fechado) do portao

					client.print("<Pin>");
					client.print("<digitalPin>");
					client.print(Pin14);
					client.print("</digitalPin>");
					client.print("<Estado>");
					int estadu = digitalRead(Pin14);
					if(estadu == 0)	{
						client.print("<namePin>*** PORTÃO  ABERTO! ***  </namePin>");
					}
					else {
						client.print("<namePin>portão fechado</namePin>");
					}
					client.print("</Estado>");
					client.print("</Pin>");




					/**
					 * (para os dimmers nao precisa a logica invertida
					 * na leitura do estado usando o ponto de exclamacao
					 * como ha para os reles)
					 */

					// Pin9 - HIGH LINE (cabo dicroicas sala) UP
					client.print("<Pin>");
					client.print("<namePin>HI LINE UP</namePin>");
					client.print("<digitalPin>");
					client.print(Pin9);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(digitalRead(Pin9));
					client.print("</Estado>");
					client.print("<dimerizavel>1</dimerizavel>");
					client.print("</Pin>");


					// Pin8 - HIGH LINE (cabo dicroicas sala) DOWN
					client.print("<Pin>");
					client.print("<namePin>HI LINE down</namePin>");
					client.print("<digitalPin>");
					client.print(Pin8);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(digitalRead(Pin8));
					client.print("</Estado>");
					client.print("<dimerizavel>1</dimerizavel>");
					client.print("</Pin>");


					// P3  LAREIRA UP DIMMER
					client.print("<Pin>");
					client.print("<namePin>LAREIRA UP</namePin>");
					client.print("<digitalPin>");
					client.print(Pin3);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(digitalRead(Pin3));
					client.print("</Estado>");
					client.print("<dimerizavel>1</dimerizavel>");
					client.print("</Pin>");


					// P2 - LAREIRA DOWN DIMMER
					client.print("<Pin>");
					client.print("<namePin>LAREIRA DOWN</namePin>");
					client.print("<digitalPin>");
					client.print(Pin2);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(digitalRead(Pin2));
					client.print("</Estado>");
					client.print("<dimerizavel>1</dimerizavel>");
					client.print("</Pin>");


					// Pin7 - LUSTRE UP DIMMER
					client.print("<Pin>");
					client.print("<namePin>LUSTRE UP</namePin>");
					client.print("<digitalPin>");
					client.print(Pin7);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(digitalRead(Pin7));
					client.print("</Estado>");
					client.print("<dimerizavel>1</dimerizavel>");
					client.print("</Pin>");


					// Pin6 - LUSTRE DOWN DIMMER
					client.print("<Pin>");
					client.print("<namePin>LUSTRE DOWN</namePin>");
					client.print("<digitalPin>");
					client.print(Pin6);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(digitalRead(Pin6));
					client.print("</Estado>");
					client.print("<dimerizavel>1</dimerizavel>");
					client.print("</Pin>");



					// Pin5 - QUARTO UP DIMMER
					client.print("<Pin>");
					client.print("<namePin>QUARTO UP</namePin>");
					client.print("<digitalPin>");
					client.print(Pin5);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(digitalRead(Pin5));
					client.print("</Estado>");
					client.print("<dimerizavel>1</dimerizavel>");
					client.print("</Pin>");


					// Pin4 - QUARTO DOWN DIMMER
					client.print("<Pin>");
					client.print("<namePin>QUARTO DOWN</namePin>");
					client.print("<digitalPin>");
					client.print(Pin4);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(digitalRead(Pin4));
					client.print("</Estado>");
					client.print("<dimerizavel>1</dimerizavel>");
					client.print("</Pin>");


					// Pin60 - CHAMA FUNCAO SLOWSLEEP DIMMER QUARTO
					client.print("<Pin>");
					client.print("<namePin>SLOWSLEEP</namePin>");
					client.print("<digitalPin>");
					client.print(Pin60);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(digitalRead(Pin60));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin62 - RELE SOLIDO - WORK LIGHTS
					client.print("<Pin>");
					client.print("<namePin>WORK LIGHTS</namePin>");
					client.print("<digitalPin>");
					client.print(Pin62);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(digitalRead(Pin62));
					client.print("</Estado>");
					client.print("</Pin>");



//  PLACA UM----------------------------------------------------------------//

					// P22 - FIRE
					// RELE O1
					client.print("<Pin>");
					client.print("<namePin>FIRE</namePin>");
					client.print("<digitalPin>");
					client.print(Pin22);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin22));
					client.print("</Estado>");
					client.print("<requerConfirmacao>1</requerConfirmacao>");
					client.print("</Pin>");

					// Pin24 - RF open gate (abre o portao pelo iPhone)
					// RELE 02 - faz um pulso de 400ms
					client.print("<Pin>");
					client.print("<namePin>PORTAO</namePin>");
					client.print("<digitalPin>");
					client.print(Pin24);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin24));
					client.print("</Estado>");
					client.print("<requerConfirmacao>1</requerConfirmacao>");
					client.print("<pulso>1</pulso>");
					client.print("</Pin>");

					// Pin26 - PORTCH
					// RELE 03
					client.print("<Pin>");
					client.print("<namePin>PORTCH</namePin>");
					client.print("<digitalPin>");
					client.print(Pin26);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin26));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin28 - FLAVIA HI
					// RELE 04
					client.print("<Pin>");
					client.print("<namePin>VERDE FUNDOS</namePin>");
					client.print("<digitalPin>");
					client.print(Pin28);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin28));
					client.print("</Estado>");
					client.print("</Pin>");

					// Pin30 - FUNDOS
					// RELE 05
					client.print("<Pin>");
					client.print("<namePin>FUNDOS</namePin>");
					client.print("<digitalPin>");
					client.print(Pin30);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin30));
					client.print("</Estado>");
					client.print("</Pin>");

					// Pin32 - FRED LOW
					// RELE 06
					client.print("<Pin>");
					client.print("<namePin>FRED LOW</namePin>");
					client.print("<digitalPin>");
					client.print(Pin32);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin32));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin34 - FIGUEIRA
					// RELE 07
					client.print("<Pin>");
					client.print("<namePin>FIGUEIRA</namePin>");
					client.print("<digitalPin>");
					client.print(Pin34);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin34));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin36 - DECK MAIN
					// RELE 08
					client.print("<Pin>");
					client.print("<namePin>DECK MAIN</namePin>");
					client.print("<digitalPin>");
					client.print(Pin36);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin36));
					client.print("</Estado>");
					client.print("</Pin>");


//  PLACA DOIS----------------------------------------------------------------//

					// P23 - LAGUINHO
					// RELE 09
					client.print("<Pin>");
					client.print("<namePin>LAGUINHO</namePin>");
					client.print("<digitalPin>");
					client.print(Pin23);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin23));
					client.print("</Estado>");
					client.print("</Pin>");

					// Pin25 - JACARANDA
					// RELE 10
					client.print("<Pin>");
					client.print("<namePin>JACARANDA</namePin>");
					client.print("<digitalPin>");
					client.print(Pin25);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin25));
					client.print("</Estado>");
					client.print("</Pin>");

					// Pin27 - GATE (luz)
					// RELE 11
					client.print("<Pin>");
					client.print("<namePin>GATE (luz)</namePin>");
					client.print("<digitalPin>");
					client.print(Pin27);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin27));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin29 - HIDRO
					// RELE 12
					client.print("<Pin>");
					client.print("<namePin>HIDROMASSAGEM</namePin>");
					client.print("<digitalPin>");
					client.print(Pin29);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin29));
					client.print("</Estado>");
					client.print("</Pin>");

					// Pin31 - DECK FAN
					// RELE 13
					client.print("<Pin>");
					client.print("<namePin>DECK FAN</namePin>");
					client.print("<digitalPin>");
					client.print(Pin31);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin31));
					client.print("</Estado>");
					client.print("</Pin>");

					// Pin33 - RAMPA
					// RELE 14
					client.print("<Pin>");
					client.print("<namePin>RAMPA</namePin>");
					client.print("<digitalPin>");
					client.print(Pin33);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin33));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin35 - CASCATA
					// RELE 15
					client.print("<Pin>");
					client.print("<namePin>CASCATA</namePin>");
					client.print("<digitalPin>");
					client.print(Pin35);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin35));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin37 - DECK HI
					// RELE 16
					client.print("<Pin>");
					client.print("<namePin>DECK HI</namePin>");
					client.print("<digitalPin>");
					client.print(Pin37);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin37));
					client.print("</Estado>");
					client.print("</Pin>");


//  PLACA TRES----------------------------------------------------------------//

					// P38 - LUZ HIDRO
					// RELE 17
					client.print("<Pin>");
					client.print("<namePin>LUZ HIDRO</namePin>");
					client.print("<digitalPin>");
					client.print(Pin38);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin38));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin40 - FILL
					// RELE 18
					client.print("<Pin>");
					client.print("<namePin>FILL</namePin>");
					client.print("<digitalPin>");
					client.print(Pin40);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin40));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin42 - HOLLY+DECK
					// RELE 19
					client.print("<Pin>");
					client.print("<namePin>HOLLY+DECK</namePin>");
					client.print("<digitalPin>");
					client.print(Pin42);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin42));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin44 - SPRAY
					// RELE 20
					client.print("<Pin>");
					client.print("<namePin>SPRAY</namePin>");
					client.print("<digitalPin>");
					client.print(Pin44);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin44));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin46 - SHELVES
					// RELE 21
					client.print("<Pin>");
					client.print("<namePin>SHELVES</namePin>");
					client.print("<digitalPin>");
					client.print(Pin46);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin46));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin48 - DESK
					// RELE 22
					client.print("<Pin>");
					client.print("<namePin>DESK</namePin>");
					client.print("<digitalPin>");
					client.print(Pin48);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin48));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin66 - LAVANDERIA
					// RELE 23
					client.print("<Pin>");
					client.print("<namePin>LAVANDERIA</namePin>");
					client.print("<digitalPin>");
					client.print(Pin66);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin66));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin67 - DICR BANH
					// RELE 24
					client.print("<Pin>");
					client.print("<namePin>DICR BANH</namePin>");
					client.print("<digitalPin>");
					client.print(Pin67);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin67));
					client.print("</Estado>");
					client.print("</Pin>");



//  PLACA QUATRO--------------------------------------------------------------//

					// P39 - LUZ DA PIA
					// RELE 25
					client.print("<Pin>");
					client.print("<namePin>LUZ DA PIA</namePin>");
					client.print("<digitalPin>");
					client.print(Pin39);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin39));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin41 -LIVRE
					// RELE 26
					client.print("<Pin>");
					client.print("<namePin>Pin 41</namePin>");
					client.print("<digitalPin>");
					client.print(Pin41);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin41));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin43 - GAS LAREIRA
					// RELE 27
					client.print("<Pin>");
					client.print("<namePin>GAS LAREIRA</namePin>");
					client.print("<digitalPin>");
					client.print(Pin43);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin43));
					client.print("</Estado>");
					client.print("<requerConfirmacao>1</requerConfirmacao>");
					client.print("</Pin>");


					// Pin45 - FAISCA LAREIRA
					// RELE 28
					client.print("<Pin>");
					client.print("<namePin>FAISCA LAREIRA</namePin>");
					client.print("<digitalPin>");
					client.print(Pin45);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin45));
					client.print("</Estado>");
					client.print("<requerConfirmacao>1</requerConfirmacao>");
					client.print("</Pin>");


					// Pin47 - free
					// RELE 29
					client.print("<Pin>");
					client.print("<namePin>Pin 47</namePin>");
					client.print("<digitalPin>");
					client.print(Pin47);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin47));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin49 - FECHO MAGNETICO
					// RELE 30
					client.print("<Pin>");
					client.print("<namePin>FECHO MAGNETICO</namePin>");
					client.print("<digitalPin>");
					client.print(Pin49);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin49));
					client.print("</Estado>");
					client.print("<requerConfirmacao>1</requerConfirmacao>");
					client.print("</Pin>");


					// Pin68 - EXAUSTOR GRANDE
					// RELE 31
					client.print("<Pin>");
					client.print("<namePin>EXAUSTOR BIG</namePin>");
					client.print("<digitalPin>");
					client.print(Pin68);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin68));
					client.print("</Estado>");
					client.print("</Pin>");


					// Pin69 - EXAUSTOR PEQUENO
					// RELE 32
					client.print("<Pin>");
					client.print("<namePin>EXAUSTOR PEQ</namePin>");
					client.print("<digitalPin>");
					client.print(Pin69);
					client.print("</digitalPin>");
					client.print("<Estado>");
					client.print(!digitalRead(Pin69));
					client.print("</Estado>");
					client.print("</Pin>");


//------------------------------------------------------------------------------

					client.println("</automation>");
					break;
				}

				if (c == '\n')
				{
					Serial.print(stringInput);
					Serial.println();

					if(stringInput.indexOf("GET") != -1)
					{

//------------------------------------------------------------------------------
						// AQUI O IPHONE COMECA A MANDAR

						//RELE ESTADO SOLIDO

						// Pin62 - RELE SOLIDO - WORKING LIGHTS
						if (stringInput.indexOf("PIN62=ON") != -1)
						{
							digitalWrite(Pin62, HIGH);
						}

						if (stringInput.indexOf("PIN62=OFF") != -1)
						{
							digitalWrite(Pin62, LOW);
						}


//DIMMERS --------------------------------------------------------
						//para os dimmers, ON = HIGH

						// Pin2 - lareira down
						if (stringInput.indexOf("PIN2=ON") != -1)
						{
							digitalWrite(Pin2, HIGH);
						}

						if (stringInput.indexOf("PIN2=OFF") != -1)
						{
							digitalWrite(Pin2, LOW);
						}



						// Pin3 - lareira up
						if (stringInput.indexOf("PIN3=ON") != -1)
						{
							digitalWrite(Pin3, HIGH);
						}

						if (stringInput.indexOf("PIN3=OFF") != -1)
						{
							digitalWrite(Pin3, LOW);
						}



						// Pin4 - quarto down
						if (stringInput.indexOf("PIN4=ON") != -1)
						{
							digitalWrite(Pin4, HIGH);
						}

						if (stringInput.indexOf("PIN4=OFF") != -1)
						{
							digitalWrite(Pin4, LOW);
						}



						// Pin5 - quarto up
						if (stringInput.indexOf("PIN5=ON") != -1)
						{
							digitalWrite(Pin5, HIGH);

						}

						if (stringInput.indexOf("PIN5=OFF") != -1)
						{
							digitalWrite(Pin5, LOW);
						}



						// Pin6 - lustre down
						if (stringInput.indexOf("PIN6=ON") != -1)
						{
							digitalWrite(Pin6, HIGH);

						}

						if (stringInput.indexOf("PIN6=OFF") != -1)
						{
							digitalWrite(Pin6, LOW);
						}



						// Pin7 - lustre up
						if (stringInput.indexOf("PIN7=ON") != -1)
						{
							digitalWrite(Pin7, HIGH);

						}

						if (stringInput.indexOf("PIN7=OFF") != -1)
						{
							digitalWrite(Pin7, LOW);
						}




						// Pin8 - high line down
						if (stringInput.indexOf("PIN8=ON") != -1)
						{
							digitalWrite(Pin8, HIGH);

						}

						if (stringInput.indexOf("PIN8=OFF") != -1)
						{
							digitalWrite(Pin8, LOW);
						}



						// Pin9 - high line up
						if (stringInput.indexOf("PIN9=ON") != -1)
						{
							digitalWrite(Pin9, HIGH);

						}

						if (stringInput.indexOf("PIN9=OFF") != -1)
						{
							digitalWrite(Pin9, LOW);
						}



						// Pin60 - A6 - quarto downSlow
						if (stringInput.indexOf("PIN60=ON") != -1)
						{
							digitalWrite(Pin60, HIGH);

						}

						if (stringInput.indexOf("PIN60=OFF") != -1)
						{
							digitalWrite(Pin60, LOW);
						}




/**
 * para as placas, devido ao optocoupler,
 * a logica de ligar fica  invertida - ON = LOW
 */
//PLACA UM ---------------------------------------------------------------------

						// P22 - FIRE
						// RELE O1
						if (stringInput.indexOf("PIN22=ON") != -1)
						{
							digitalWrite(Pin22, LOW);

						}

						if (stringInput.indexOf("PIN22=OFF") != -1)
						{
							digitalWrite(Pin22, HIGH);
						}



						// Pin24 - RF open gate (abre o portao pelo iPhone)
						// RELE 02 - faz um pulso de 400ms
						if (stringInput.indexOf("PIN24=ON") != -1)
						{
							digitalWrite(Pin24, LOW);
							delay(800);
							digitalWrite(Pin24, HIGH);
						}

						if (stringInput.indexOf("PIN24=OFF") != -1)
						{
							digitalWrite(Pin24, HIGH);
						}



						// Pin26 - PORTCH
						// RELE 03
						if (stringInput.indexOf("PIN26=ON") != -1)
						{
							digitalWrite(Pin26, LOW);
						}

						if (stringInput.indexOf("PIN26=OFF") != -1)
						{
							digitalWrite(Pin26, HIGH);
						}


						// Pin28 - FLAVIA HI
						// RELE 04
						if (stringInput.indexOf("PIN28=ON") != -1)
						{
							digitalWrite(Pin28, LOW);
						}

						if (stringInput.indexOf("PIN28=OFF") != -1)
						{
							digitalWrite(Pin28, HIGH);
						}



						// Pin30 - FUNDOS
						// RELE 05
						if (stringInput.indexOf("PIN30=ON") != -1)
						{
							digitalWrite(Pin30, LOW);
						}

						if (stringInput.indexOf("PIN30=OFF") != -1)
						{
							digitalWrite(Pin30, HIGH);
						}



						// Pin32 - FRED LOW
						// RELE 06
						if (stringInput.indexOf("PIN32=ON") != -1)
						{
							digitalWrite(Pin32, LOW);
						}

						if (stringInput.indexOf("PIN32=OFF") != -1)
						{
							digitalWrite(Pin32, HIGH);
						}



						// Pin34 - FIGUEIRA
						// RELE 07
						if (stringInput.indexOf("PIN34=ON") != -1)
						{
							digitalWrite(Pin34, LOW);
						}

						if (stringInput.indexOf("PIN34=OFF") != -1)
						{
							digitalWrite(Pin34, HIGH);
						}



						// Pin36 - DECK MAIN
						// RELE 08
						if (stringInput.indexOf("PIN36=ON") != -1)
						{
							digitalWrite(Pin36, LOW);
						}

						if (stringInput.indexOf("PIN36=OFF") != -1)
						{
							digitalWrite(Pin36, HIGH);
						}


//PLACA DOIS -------------------------------------------------------------------


						// P23 - LAGUINHO
						// RELE 09
						if (stringInput.indexOf("PIN23=ON") != -1)
						{
							digitalWrite(Pin23, LOW);
						}

						if (stringInput.indexOf("PIN23=OFF") != -1)
						{
							digitalWrite(Pin23, HIGH);
						}



						// Pin25 - JACARANDA
						// RELE 10
						if (stringInput.indexOf("PIN25=ON") != -1)
						{
							digitalWrite(Pin25, LOW);
						}

						if (stringInput.indexOf("PIN25=OFF") != -1)
						{
							digitalWrite(Pin25, HIGH);
						}



						// Pin27 - GATE (luz)
						// RELE 11
						if (stringInput.indexOf("PIN27=ON") != -1)
						{
							digitalWrite(Pin27, LOW);
						}

						if (stringInput.indexOf("PIN27=OFF") != -1)
						{
							digitalWrite(Pin27, HIGH);
						}



						// Pin29 - HIDRO
						// RELE 12
						if (stringInput.indexOf("PIN29=OFF") != -1)
						{
							if (Pin29State == LOW)
							{
								Pin29State = HIGH;
							}

						}

						if (stringInput.indexOf("PIN29=ON") != -1)
						{
							if (Pin29State == HIGH)
							{
								Pin29State = LOW;
							}

						}



						// Pin31 - DECK FAN
						// RELE 13
						if (stringInput.indexOf("PIN31=ON") != -1)
						{
							digitalWrite(Pin31, LOW);
						}

						if (stringInput.indexOf("PIN31=OFF") != -1)
						{
							digitalWrite(Pin31, HIGH);
						}


						// Pin33 - RAMPA
						// RELE 14
						if (stringInput.indexOf("PIN33=ON") != -1)
						{
							digitalWrite(Pin33, LOW);
						}

						if (stringInput.indexOf("PIN33=OFF") != -1)
						{
							digitalWrite(Pin33, HIGH);
						}



						// Pin35 - CASCATA
						// RELE 15
						if (stringInput.indexOf("PIN35=ON") != -1)
						{
							digitalWrite(Pin35, LOW);
						}

						if (stringInput.indexOf("PIN35=OFF") != -1)
						{
							digitalWrite(Pin35, HIGH);
						}



						// Pin37 - DECK HI
						// RELE 16
						if (stringInput.indexOf("PIN37=ON") != -1)
						{
							digitalWrite(Pin37, LOW);
						}

						if (stringInput.indexOf("PIN37=OFF") != -1)
						{
							digitalWrite(Pin37, HIGH);
						}



//  PLACA TRES -----------------------------------------------------------------

						// P38 - LUZ HIDRO
						// RELE 17
						if (stringInput.indexOf("PIN38=OFF") != -1)
						{
							if (Pin38State == LOW)
							{
								Pin38State = HIGH;
							}
						}


						if (stringInput.indexOf("PIN38=ON") != -1)

						{
							if (Pin38State == HIGH)
							{
								Pin38State = LOW;
							}
						}



						// Pin40 - FILL
						// RELE 18
						if (stringInput.indexOf("PIN40=OFF") != -1)
						{
							if (Pin40State == LOW)
							{
								Pin40State = HIGH;
							}
						}

						if (stringInput.indexOf("PIN40=ON") != -1)
						{
							if (Pin40State == HIGH)
							{
								Pin40State = LOW;
							}
						}



						// Pin42 - HOLLY+DECK
						// RELE 19
						if (stringInput.indexOf("PIN42=ON") != -1)
						{
							digitalWrite(Pin42, LOW);
						}

						if (stringInput.indexOf("PIN42=OFF") != -1)
						{
							digitalWrite(Pin42, HIGH);
						}



						// Pin44 - SPRAY
						// RELE 20
						if (stringInput.indexOf("PIN44=ON") != -1)
						{
							digitalWrite(Pin44, LOW);
						}

						if (stringInput.indexOf("PIN44=OFF") != -1)
						{
							digitalWrite(Pin44, HIGH);
						}


						// Pin46 - SHELVES
						// RELE 21
						if (stringInput.indexOf("PIN46=ON") != -1)
						{
							digitalWrite(Pin46, LOW);
						}

						if (stringInput.indexOf("PIN46=OFF") != -1)
						{
							digitalWrite(Pin46, HIGH);
						}


						// Pin48 - DESK
						// RELE 22
						if (stringInput.indexOf("PIN48=ON") != -1)
						{
							digitalWrite(Pin48, LOW);
						}

						if (stringInput.indexOf("PIN48=OFF") != -1)
						{
							digitalWrite(Pin48, HIGH);
						}



						// Pin66 - LAVANDERIA
						// RELE 23
						if (stringInput.indexOf("PIN66=OFF") != -1)
						{
							if (Pin66State == LOW)
							{
								Pin66State = HIGH;
							}

						}
						if (stringInput.indexOf("PIN66=ON") != -1)
						{
							if (Pin66State == HIGH)
							{
								Pin66State = LOW;
							}
						}



						// Pin67 - DICR BANH
						// RELE 24
						if (stringInput.indexOf("PIN67=OFF") != -1)
						{
							if (Pin67State == LOW)
							{
								Pin67State = HIGH;
							}
						}

						if (stringInput.indexOf("PIN67=ON") != -1)
						{
							if (Pin67State == HIGH)
							{
								Pin67State = LOW;
							}
						}



//  PLACA QUATRO ---------------------------------------------------------------

						// P39 - LUZ DA PIA
						// RELE 25
						if (stringInput.indexOf("PIN39=OFF") != -1)
						{

							if (Pin39State == LOW)
							{
								Pin39State = HIGH;
							}
						}

						if (stringInput.indexOf("PIN39=ON") != -1)
						{
							if (Pin39State == HIGH)
							{
								Pin39State = LOW;
							}
						}



						// Pin41 -LIVRE
						// RELE 26 -
						if (stringInput.indexOf("PIN41=ON") != -1)
						{
							digitalWrite(Pin41, LOW);
						}

						if (stringInput.indexOf("PIN41=OFF") != -1)
						{
							digitalWrite(Pin41, HIGH);
						}



						// Pin43 - GAS LAREIRA
						// RELE 27
						if (stringInput.indexOf("PIN43=ON") != -1)
						{
							digitalWrite(Pin43, LOW);
						}

						if (stringInput.indexOf("PIN43=OFF") != -1)
						{
							digitalWrite(Pin43, HIGH);
						}



						// Pin45 - FAISCA LAREIRA
						// RELE 28
						if (stringInput.indexOf("PIN45=ON") != -1)
						{
							digitalWrite(Pin45, LOW);
						}

						if (stringInput.indexOf("PIN45=OFF") != -1)
						{
							digitalWrite(Pin45, HIGH);
						}



						// Pin47 - livre
						// RELE 29
						if (stringInput.indexOf("PIN47=ON") != -1)
						{
							digitalWrite(Pin47, LOW);
						}

						if (stringInput.indexOf("PIN47=OFF") != -1)
						{
							digitalWrite(Pin47, HIGH);
						}


						// Pin49 - FECHO MAGNETICO
						// RELE 30
						if (stringInput.indexOf("PIN49=ON") != -1)
						{
							digitalWrite(Pin49, LOW);
						}

						if (stringInput.indexOf("PIN49=OFF") != -1)
						{
							digitalWrite(Pin49, HIGH);
						}



						// Pin68 - EXAUSTOR GRANDE
						// RELE 31
						if (stringInput.indexOf("PIN68=ON") != -1)
						{
							if (Pin68State == LOW)
							{
								Pin68State = HIGH;
							}
						}

						if (stringInput.indexOf("PIN68=ON") != -1)
						{
							if (Pin68State == HIGH)
							{
								Pin68State = LOW;
							}
						}



						// Pin69 - EXAUSTOR PEQUENO
						// RELE 32
						if (stringInput.indexOf("PIN69=ON") != -1)
						{
							if (Pin69State == LOW)
							{
								Pin69State = HIGH;
							}
						}

						if (stringInput.indexOf("PIN69=ON") != -1)
						{
							if (Pin69State == HIGH)
							{
								Pin69State = LOW;
							}
						}


//----------------------------------------------------------------------------//

					}
					currentLineIsBlank = true;
					stringInput = "";
				}

				else if (c != '\r')
				{
					currentLineIsBlank = false;
				}
			}
		}
		delay(1);
		client.stop();
	}
}
// end
