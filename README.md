Questo progetto integra un **PLC programmato in CODESYS** e un **Arduino configurato come slave Modbus RTU**.

L’obiettivo è monitorare la temperatura ambientale tramite un sensore collegato ad Arduino e gestire un allarme sonoro su Arduino e visivo di segnalazione (spia) sul PLC.

---

## Struttura del progetto

Il progetto è composto da due parti principali:

1. **Arduino (Slave Modbus)**

   * Monta un sensore di temperatura ambientale.
   * Integra un buzzer piezo come allarme acustico.
   * Espone i dati tramite protocollo **Modbus RTU** su seriale.

2. **PLC (Master Modbus)**

   * Richiede i dati allo slave (Arduino).
   * Ricostruisce il valore di temperatura in formato **FLOAT (32 bit)** partendo da due holding register.
   * Gestisce la logica di allarme e la visualizzazione grafica.

---

## Funzionamento

Quando la **temperatura supera una soglia preimpostata**,
* Arduino:

  * Attiva il buzzer piezo.
  * Imposta il **coil di allarme** a `1`.<img width="350" height="161" alt="type" src="https://github.com/user-attachments/assets/e1bf1a56-c66b-4008-944a-497c096cc94d" align="right"/>

* Il PLC:

  * Legge il **coil** e lo collega a una variabile BOOL (Allarm). 

  * Usa una struttura `TYPE RealUnion` per ricostruire il **FLOAT** a partire dai due holding register. 
  * Espone in grafica tre elementi: <img width="325" height="500" alt="FB" src="https://github.com/user-attachments/assets/aa388c7c-03ae-49c6-a562-16cee85f4a1c" align="right" />

    * Un **bottone** per disattivare l’allarme (quando la temperatura torna sotto soglia). 
    * Una **spia visiva** che mostra lo stato dell’allarme. 
    * Uno **strumento di misura** che visualizza la temperatura in tempo reale, elaborata tramite `FB_SmoothTemp` per ridurre le fluttuazioni del sensore.
    <br clear="all">


---

## Variabili Modbus

Arduino, come slave, espone tre variabili:

1. **Coil – Allarme temperatura**

   * Tipo: BOOL
   * Stato: `1` se la temperatura ha superato la soglia e quindi l’allarme è attivo, `0` altrimenti.

2. **2 Holding Register**

   * Contengono insieme le due word necessarie per rappresentare la temperatura in formato **FLOAT (32 bit)**'.

---

## Connessione

* **Arduino** = Slave Modbus RTU
* **PLC** = Master Modbus RTU
* **Protocollo** = Modbus RTU su linea seriale

---

## Possibili miglioramenti futuri

* Aggiunta di logging della temperatura su storico.
* Gestione di più sensori in parallelo.
* Interfaccia grafica avanzata con trend di temperatura.
