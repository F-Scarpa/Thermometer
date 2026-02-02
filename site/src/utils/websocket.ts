  const getWebSocketUrl = (suffix: string) =>
  {
    const l = window.location;
    return ((l.protocol == "https:" ? "wss://":  "ws://") + l.host + l.pathname + suffix);
  }

  //server creates JSON object which will be read from the websocket

  export const webSocket = () => 
  {
    const socket = new WebSocket(getWebSocketUrl("ws"));    //create new websocket
    socket.onopen = () =>        //on opened websocket
    {
      socket.send("Hello esp32");       //print on terminal when websocket successfully connects
    }
    socket.onmessage = (event) =>       //server send data to client
    {
      console.log(event.data);    //event.data is data the server received
      try{
        //const espColorNum = JSON.parse(event.data);
        //setBtnState(attemptedBtnState.btn_state);       //btn_state is built from server with cJSON
        //console.log(espColorNum.tra_lig_state);           //extract tra_lig_state value from JSON object
        //setColorNum(espColorNum.tra_lig_state);
      }
      finally {
        
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
  }