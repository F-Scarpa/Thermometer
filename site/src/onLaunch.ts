import { /*useState,*/ useEffect} from 'react'
import { webSocket } from './utils/websocket';



export const onLaunchEffects = () => 
{
  useEffect(() => {
    webSocket()
  },[]);

}
