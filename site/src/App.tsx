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
  const [threshold, setThreshold] = useState(35);
  const [toggleState, setToggleState] = useState(false);
  const [currentThreshold, setCurrentThreshold] = useState(35);
  




const handleClick = (num : number) => {
    setMode(num);
    console.log(mode);
};


const handleToggleActuator = (state : number) => {
  if(state === 2)
  {
    setToggleState(!toggleState);
  }
  else if(state === 0)
  {
    setToggleState(false);
  }
  else if(state === 1)
  {
    setToggleState(true);
  }
    //console.log(mode);
};


//React.ChangeEvent<T> is the event type, <T> is the element we are working on, react inherits props like value, checked...
const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {    //e is the event. the after : is the data (event) type (onChange)
    const value = Number(e.target.value)    //value is inherited by HTMLInputElements which tells react it's a input
    setThreshold(value);
    console.log(e.target.value);
};


//post
async function setThresholdCall(newThreshold : number) {
  setCurrentThreshold(newThreshold);
  try {
    const response = await fetch("/api/threshold", 
      {
        method: "POST",
        headers: {
          "Content-Type": "text/plain",
        },
        body: newThreshold.toString(), 
    });

    if (response.ok) {
      await response.text();
      //console.log("ESP32 response:", text);
      console.log(`new threshold is : ${newThreshold}`);
    } else {
      console.error("HTTP error", response.status);
    }
  } catch (err) {
    console.error("Fetch error:", err);
  }
}

//gets
const auto_mode = async () =>     
  {                                                                         
    const webResult = await fetch("/api/auto");  
    console.log(webResult);
  }



const man_mode = async () =>     
  {                                                                         
    const webResult = await fetch("/api/man");  
    console.log(webResult);
  }

const toggle_actuator = async (state : number) =>     
  {                                                                         
    const webResult = await fetch("/api/toggle");  
    handleToggleActuator(state);
    console.log(webResult);
  }

const off_mode = async () =>     
  {                                                                         
    const webResult = await fetch("/api/disable");  
    console.log(webResult);

  }

  


  return (
    <>
      
      <h2>Oggi: {currentDate}</h2>
      <h2>Ora: {currentTime}</h2>
      <h2>Temperature: {temp !== null ? `${temp} °C `: 'calculating temperature...'}</h2>
      <h2>Mode: </h2>
      <button onClick={() => {handleClick(0);
                              off_mode();
      }}>Off</button>

      <button onClick={() => {handleClick(1);
                              man_mode()
      }}>Man</button>

      <button onClick={() => {handleClick(2);
                             auto_mode()
      }}>Auto</button>

    <div style={{
                  display:"block",
                  marginTop:"30px"
                }}>
      <button style={mode === 1? {display:"inline"} : {display:"none"}}
              onClick={() => toggle_actuator(2)}> {toggleState ? "Stop" : "Start"}</button>
      <button style={mode ===1? {display:"inline"}: {display:"none"}}

              onMouseDown={() => toggle_actuator(1)}
              onMouseUp={() => toggle_actuator(0)}>Push</button>

      <span style={{display : mode === 2? "inline" : "none"}}>{threshold}</span>
      <input type='range'
             min={0}
             max={100}
             value={threshold}
             onChange={handleChange}
             style={{display : mode === 2? "inline" : "none"}}>
          
      </input>
      <span style={{display : mode === 2? "inline" : "none"}}>100</span>

      <button style={{marginLeft : "20px",
                      display : mode === 2? "inline" : "None",
                      
      }
             
      }
      onClick={()=> setThresholdCall(threshold)}>Set threshold</button>

      <br></br>
      <h3 style={{display : mode === 2? "block" : "none"}} >Current Threshold is : {currentThreshold}</h3>





    </div>

  
    </>
  )
}

export default App
