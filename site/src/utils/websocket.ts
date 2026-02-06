  export interface ESP32Data {  //data structure (interface) called ESP32Data
    temperature: number;
  }


  const getWebSocketUrl = (suffix: string) =>
  {
    const l = window.location;
    return ((l.protocol == "https:" ? "wss://":  "ws://") + l.host + l.pathname + suffix);
  }


  export const webSocket = (onDataReceived: (data : ESP32Data) => void) =>    
  {
    const socket = new WebSocket(getWebSocketUrl("ws"));    //create new websocket
    socket.onopen = () =>        //on opened websocket
    {
      socket.send("Hello esp32");       //print on terminal when websocket successfully connects
    }
    socket.onmessage = (event) =>       //when server send data to client
    {
      console.log(event.data);    //event.data is data the server received
      try{
        const esp32Data : ESP32Data = JSON.parse(event.data);  
        console.log(esp32Data.temperature);
        onDataReceived(esp32Data);      //webSocket((data: ESP32Data)=> {setTemp(data.temperature);});  (in effects.ts)
        }
      catch (error) {
        console.error("error parsing JSON:", error);

      }
    }
    socket.onerror = (err) =>     //catch errors on ws
    {
      console.error(err);
    };
    socket.onclose = (event) =>     //ws get closed
    {
      console.log(event);
    }

   return socket
  }