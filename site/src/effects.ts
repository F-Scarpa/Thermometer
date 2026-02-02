import { useState, useEffect} from 'react'
import { getCurrentDate, getCurrentTime } from './utils/time';

export const timeEffects = () => {
    const [currentTime, setCurrentTime] = useState(getCurrentTime());
    const [currentDate, setCurrentDate] = useState(getCurrentDate());

  //update every day
  useEffect(() => {
    const interval = setInterval(() => {
      setCurrentDate(getCurrentDate()); 
    }, (1000 * 60) *24); 
    return () => clearInterval(interval); 
  }, []);


  //update every minute
  useEffect(() => {
  const interval = setInterval(() => {
    setCurrentTime(getCurrentTime());
  }, 1000 * 60); 
  return () => clearInterval(interval); 
}, []);


    return {currentTime, currentDate};

}