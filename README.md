# EspnowUARTBridge for ESP8266
<p>Receiver/transmitter based on the espnow protocol.</p>
<p>It uses a serial interface as a data source and can be configured using commands typed in the serial console. </p>
<h1>How it works?</h1>
In short, it takes any information given by the serial port, except commands, and sends it to the indicated receiver or to anything within range
<h1>Features</h1>
<ul>
 <li>Serial commands</li>
 <li>Customizable baud rate</li>
 <li>Customizable receiver MAC address</li>
 <li>Encryption(usign builtin espnow encryption)</li>
</ul>
<h1>Commands list</h1>
<dl>
 <dt>ADDRECV</dt><dd>Allow to specify receiver MAC address. If not specifed , it uses address of last connected device. </dd>
  <dt><b>RESRECV</b></dt><dd>Sets MAC address as FF:FF:FF:FF:FF:FF</dd>
  <dt>SETBR </dt><dd>Allow to specify baud rate</dd>
  <dt>REJECTUNPAIRED </dt><dd>True - Reject messages from unknown peer , False - Accept all connections</dd>
 <dt>RST</dt><dd>Reboot controller</dd>
 <dt>INFO</dt><dd>Print system information</dd>
 <dt>?</dt><dd>Help</dd>
</dl>
<h1>Pre-compiled binaries</h1>
Download pre-compiled binaries for ESP12E/F and ESP-01S
https://github.com/kurson95/EspnowUARTBridge_ESP8266/releases/
 

