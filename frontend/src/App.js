import {
  Box,
  Button,
  ButtonGroup,
  Flex,
  HStack,
  IconButton,
  SkeletonText,
  Text,
} from '@chakra-ui/react'

import { FaLocationArrow } from 'react-icons/fa'
import { FiRefreshCw } from 'react-icons/fi'

import {
  useJsApiLoader,
  GoogleMap,
  Marker,
  HeatmapLayer,
} from '@react-google-maps/api'

import { useRef, useState, useEffect } from 'react'

import axios from './Utils/axios_main'
import "./App.css"


const center = { latitude: 44.434951972403574, longitude: 26.047760546207428 }
const libs = ['places', 'visualization'];

function App() {

  const [map, setMap] = useState(/** @type google.maps.Map */(null))
  const [currentMarker, setCurrentMarker] = useState({ latitude: center.latitude, longitude: center.longitude })
  const [markers, setMarkers] = useState([]);
  const [refreshedAt, setrefreshedAt] = useState(Date());

  // const [latitude, setLatitude] = useState(center.lat);
  // const [longitude, setLongitude] = useState(center.lng);


  useEffect(async function () {

    try {
      // const response = await axios.get('/locations', 
      //   {
      //     headers: {
      //       'Content-Type': 'application/json',
      //       'token': auth.token
      //     },
      //     //withCredentials: true
      //   }

      // )

      // setMarkers(response.data.locations);
      setMarkers(markers.slice())
    } catch (err) {
      console.log(err)

      if (!err?.response)
        console.log(err?.response)

    }
  }, []);

  const handleMapclick = (e) => {

    setCurrentMarker({
      latitude: e.latLng.lat(),
      longitude: e.latLng.lng()
    })
  }

  const addMarker = async () => {
    console.log(markers);
    var newMArkers = markers.slice();
    newMArkers.push({
      latitude: currentMarker.latitude,
      longitude: currentMarker.longitude
    })

    setMarkers(newMArkers);
  }

  const refreshPage = () => {
    setMarkers([]);
    setrefreshedAt(Date());
  }

  const { isLoaded } = useJsApiLoader({
    googleMapsApiKey: process.env.REACT_APP_GOOGLE_MAPS_API_KEY,
    libraries: libs,
  })

  if (!isLoaded) {
    return <SkeletonText />
  }

  return (
    <Flex
      position='relative'
      flexDirection='column'
      alignItems='center'
      h='100vh'
      w='100vw'
    >
      <Box position='absolute' left={0} top={0} h='100%' w='100%'>
        {/* Google Map Box */}
        <GoogleMap
          center={{ lat: center.latitude, lng: center.longitude }}
          zoom={16}
          mapContainerStyle={{ width: '100%', height: '100%' }}
          options={{
            zoomControl: false,
            streetViewControl: false,
            mapTypeControl: false,
            fullscreenControl: false,
          }}
          onClick={e => handleMapclick(e)}
          onLoad={map => setMap(map)}

        >
          <HeatmapLayer
            data={markers.map((marker) => (
              new window.google.maps.LatLng(marker.latitude, marker.longitude)
            ))}
            options={{
              radius: 50
            }}
          >
          </HeatmapLayer>
          <Marker position={{ lat: currentMarker.latitude, lng: currentMarker.longitude }} />
          {markers.map((marker, index) => {

            return <Marker key={index} position={{ lat: marker.latitude, lng: marker.longitude }} onClick />

          })}

        </GoogleMap>
      </Box>
      <Box
        p={4}
        borderRadius='lg'
        m={4}
        bgColor='white'
        shadow='base'
        minW='container.md'
        zIndex='1'
      >
        <HStack pacing={2} justifyContent='flex-start'>
          <IconButton
            aria-label='refresh page'
            icon={<FiRefreshCw />}
            isRound
            onClick={refreshPage}
          />
          <Text> Last refreshed at: {refreshedAt}</Text>
        </HStack>
      </Box>
    </Flex>
  )
}

export default App
