import { useState, useEffect} from 'react'

import './App.css'


function App() {
  
  //const [btnState, setBtnState] = useState(true);
  //const [status, setStatus] = useState("pending...");
  //const [colorNum, setColorNum] = useState(0);

  
  const getWebSocketUrl = (suffix: string) =>
  {
    const l = window.location;
    return ((l.protocol == "https:" ? "wss://":  "ws://") + l.host + l.pathname + suffix);
  }

  //server creates JSON object which will be read from the websocket

  const webSocket = () => 
  {
    const socket = new WebSocket(getWebSocketUrl("ws"));    //create new websocket
    socket.onopen = () =>        //on opened websocket
    {
      socket.send("Hello esp32");
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

/*
  const disable_traffic_light = async () =>     
  {                                                                         
    const webResult = await fetch("/api/disable");  
    console.log(webResult);
    setStatus("Disabled")
  }
*/

const getCurrentDate = () => {
  const now = new Date();
  const day = String(now.getDate()).padStart(2, "0");       // giorno con 2 cifre
  const month = String(now.getMonth() + 1).padStart(2, "0"); // mese (0-indexed!) quindi +1
  const year = now.getFullYear();

  return `${day}/${month}/${year}`;
};
  const [currentDate, setCurrentDate] = useState(getCurrentDate());

const getCurrentTime = () => {
  const now = new Date();
  const hours = String(now.getHours()).padStart(2, "0");   // ore 0-23
  const minutes = String(now.getMinutes()).padStart(2, "0"); // minuti 0-59
  return `${hours}:${minutes}`;
};
const [currentTime, setCurrentTime] = useState(getCurrentTime());




  //update every minute
  useEffect(() => {
    const interval = setInterval(() => {
      setCurrentDate(getCurrentDate()); 
    }, (1000 * 60) *24); 
    return () => clearInterval(interval); 
  }, []);

  useEffect(() => {
    webSocket()
  },[]);


  useEffect(() => {
  const interval = setInterval(() => {
    setCurrentTime(getCurrentTime());
  }, 1000 * 60); 
  return () => clearInterval(interval); 
}, []);


  


  return (
    <>

      
      <h2>Oggi: {currentDate}</h2>
      <h2>Ora: {currentTime}</h2>
     

  
    </>
  )
}

export default App
