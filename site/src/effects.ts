import { useState, useEffect} from 'react'
import { getCurrentDate, getCurrentTime } from './utils/time';
import { webSocket } from './utils/websocket';
import type { ESP32Data } from './utils/websocket';   //typescript use verbatimModuleSyntax, so types must be imported with import type
                                                      //type = custom name for a data structure (like interfaces)

export const timeEffects = () => {
    const [currentTime, setCurrentTime] = useState(getCurrentTime());
    const [currentDate, setCurrentDate] = useState(getCurrentDate());

    const [temp, setTemp] = useState<number | null>(null);                  //when temp isn't defined it is null


  //update every day
  useEffect(() => {
    const interval = setInterval(() => {
      setCurrentDate(getCurrentDate()); 
    }, (1000 * 60 * 60) *24); 
    return () => clearInterval(interval); 
  }, []);


  //update every minute
  useEffect(() => {
  const interval = setInterval(() => {
    setCurrentTime(getCurrentTime());
  }, 1000 * 60); 
  return () => clearInterval(interval); 
}, []);


  useEffect(() => {
    const socket = webSocket((data: ESP32Data) => {
      setTemp(data.temperature);  //this is onDataReceived, set temperature to data.temperature data is the parameter interface 

      });

    return () => {
      socket.close();
    };
  }, []);
    
    
    return {currentTime, currentDate, temp};

}