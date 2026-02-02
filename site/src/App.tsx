//import {useEffect} from 'react'

import { timeEffects } from './effects'
//import { onLaunchEffects } from './onLaunch';
import './App.css'


function App() {
  
  //const [btnState, setBtnState] = useState(true);
  //const [status, setStatus] = useState("pending...");
  //const [colorNum, setColorNum] = useState(0);
  const {currentTime, currentDate, temp} = timeEffects();
  
 

/*
  const disable_traffic_light = async () =>     
  {                                                                         
    const webResult = await fetch("/api/disable");  
    console.log(webResult);
    setStatus("Disabled")
  }
*/




  


  return (
    <>
      
      <h2>Oggi: {currentDate}</h2>
      <h2>Ora: {currentTime}</h2>
      <h2>Temperature: {temp !== null ? `${temp}°C `: 'calculating temperature...'}</h2>
     

  
    </>
  )
}

export default App
