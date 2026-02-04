//import {useEffect} from 'react'

import { timeEffects } from './effects'
import './App.css'
import { useState } from 'react';




function App() {
  
  //const [btnState, setBtnState] = useState(true);
  //const [status, setStatus] = useState("pending...");
  //const [colorNum, setColorNum] = useState(0);
  const {currentTime, currentDate, temp} = timeEffects();
  const [mode,setMode] = useState(0);
  const [threshold, setThreshold] = useState(20);
  
 



const handleClick = (num : number) => {
    setMode(num);
    console.log(mode);
};
//React.ChangeEvent<T> is the event type, <T> is the element we are working on, react inherits props like value, checked...
const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {    //e is the event. the after : is the data (event) type
    const value = Number(e.target.value)    //value is inherited by HTMLInputElements which tells react it's a input
    setThreshold(value);
    console.log(e.target.value);
};


  


  return (
    <>
      
      <h2>Oggi: {currentDate}</h2>
      <h2>Ora: {currentTime}</h2>
      <h2>Temperature: {temp !== null ? `${temp} °C `: 'calculating temperature...'}</h2>
      <h2>Mode: </h2>
      <button onClick={() => handleClick(0)}>Off</button>
      <button onClick={() => handleClick(1)}>Man</button>
      <button onClick={() => handleClick(2)}>Auto</button>

    <div style={{
                  display:"block",
                  marginTop:"30px"
                }}>
      <button style={mode === 1? {display:"inline"} : {display:"none"}}>Start/Stop</button>
      <button style={mode ===1? {display:"inline"}: {display:"none"}}>Push</button>

      <span style={{display : mode === 2? "inline" : "none"}}>20</span>
      <input type='range'
             min={20}
             max={100}
             value={threshold}
             onChange={handleChange}
             style={{display : mode === 2? "inline" : "none"}}>
          
      </input>
      <span style={{display : mode === 2? "inline" : "none"}}>100</span>

      <button style={{marginLeft : "20px",
                      display : mode === 2? "inline" : "None",
                      
      }
              
      }>Set threshold</button>

    </div>

  
    </>
  )
}

export default App
